// carte_pwm_esp32.ino — Firmware de mise en service v0
//
// ROLE DE CETTE VERSION
//   Verifier la carte et le cablage, SANS JAMAIS COMMANDER LE VERIN.
//   - K1_CMD et K2_CMD restent a 0 en permanence ; la pompe de charge est arretee.
//     Le verin reste donc en position sure (tige rentree) des que le 12 V est present.
//   - Mesure du PWM, lecture des fins de course, des relectures et des tensions.
//   - Autocontrole au demarrage (proposition PO-12), puis surveillance permanente.
//   - Affichage : LED RGB, ligne STATUS, moniteur serie (115200 bauds).
//   L'etat ACTION (sortie de tige) sera ajoute quand la FSM sera specifiee (STM32 PO-05).
//
// RADIO (DEC-007)
//   Aucune bibliotheque radio n'est incluse (pas de WiFi.h, BLE, Bluetooth).
//   La radio n'est jamais initialisee.
//
// CIBLE
//   Carte ESP32-S3-DevKitC-1-N8, coeur Arduino ESP32 version 3.x.
//   Reglages Arduino IDE : voir firmware/README.md.

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "config.h"

// ===========================================================================
// Etats de la carte (v0 : pas d'etat ACTION)
// ===========================================================================
enum Etat { DEMARRAGE, AUTOCONTROLE, SURE, DEFAUT };
static Etat etat = DEMARRAGE;
static const char* NOM_ETAT[] = { "DEMARRAGE", "AUTOCONTROLE", "SURE", "DEFAUT" };
static String motifDefaut = "";

// ===========================================================================
// Mesure du PWM par interruption
// ===========================================================================
static volatile int64_t pwmFrontMontant = 0;   // instant du dernier front montant (us)
static volatile int64_t pwmPeriodeUs    = 0;   // ecart entre deux fronts montants
static volatile int64_t pwmLargeurUs    = 0;   // duree de l'etat haut
static volatile int64_t pwmDernierFin   = 0;   // instant de la derniere impulsion complete

void IRAM_ATTR isrPwm() {
  int64_t t = esp_timer_get_time();
  if (digitalRead(PIN_PWM_IN)) {
    if (pwmFrontMontant != 0) pwmPeriodeUs = t - pwmFrontMontant;
    pwmFrontMontant = t;
  } else if (pwmFrontMontant != 0) {
    pwmLargeurUs  = t - pwmFrontMontant;
    pwmDernierFin = t;
  }
}

struct MesurePwm { bool valide; int32_t largeurUs; int32_t periodeUs; };

MesurePwm lirePwm() {
  noInterrupts();
  int64_t largeur = pwmLargeurUs, periode = pwmPeriodeUs, fin = pwmDernierFin;
  interrupts();
  MesurePwm m = { false, (int32_t)largeur, (int32_t)periode };
  bool recent = (fin != 0) && (esp_timer_get_time() - fin) < (int64_t)PWM_TIMEOUT_MS * 1000;
  m.valide = recent
          && largeur >= PWM_LARGEUR_MIN_US && largeur <= PWM_LARGEUR_MAX_US
          && periode >= PWM_PERIODE_MIN_US && periode <= PWM_PERIODE_MAX_US;
  return m;
}

// ===========================================================================
// Lecture des entrees
// ===========================================================================
// Fin de course cable en inverseur : rappel au 3,3 V, commun a la masse.
//   Contact au repos  : NO ouvert (HAUT), NC ferme (BAS).
//   Contact actionne  : NO ferme (BAS),  NC ouvert (HAUT).
//   NO == NC          : incoherent (fil coupe, court-circuit, contact defectueux).
enum EtatFc { FC_REPOS, FC_ACTIONNE, FC_INCOHERENT };
static const char* NOM_FC[] = { "repos", "ACTIONNE", "INCOHERENT" };

EtatFc lireFc(int pinNo, int pinNc) {
  bool no = digitalRead(pinNo), nc = digitalRead(pinNc);
  if (no == nc) return FC_INCOHERENT;
  return no ? FC_REPOS : FC_ACTIONNE;
}

struct Entrees {
  EtatFc r1, r2, s1, s2;
  bool v12Present, k1Excite, k2Excite, cavalierEnPlace, contactArretFerme, ext2;
  int  v5mV, ext1mV;
};

Entrees lireEntrees() {
  Entrees e;
  e.r1 = lireFc(PIN_FC_R1_NO, PIN_FC_R1_NC);
  e.r2 = lireFc(PIN_FC_R2_NO, PIN_FC_R2_NC);
  e.s1 = lireFc(PIN_FC_S1_NO, PIN_FC_S1_NC);
  e.s2 = lireFc(PIN_FC_S2_NO, PIN_FC_S2_NC);
  e.v12Present        = !digitalRead(PIN_V12_OK);
  e.k1Excite          = !digitalRead(PIN_K1_FB);
  e.k2Excite          = !digitalRead(PIN_K2_FB);
  e.cavalierEnPlace   =  digitalRead(PIN_ARM_FB);
  e.contactArretFerme = !digitalRead(PIN_STOP_HW_FB);
  e.ext2              =  digitalRead(PIN_EXT_IN2);
  e.v5mV   = analogReadMilliVolts(PIN_V5_MON) * V5_FACTEUR;
  e.ext1mV = analogReadMilliVolts(PIN_EXT_IN1);
  return e;
}

// ===========================================================================
// Sorties
// ===========================================================================
void sortiesSures() {
  digitalWrite(PIN_K1_CMD, LOW);
  digitalWrite(PIN_K2_CMD, LOW);
  ledcWrite(PIN_CP_DRV, 0);          // pompe de charge arretee : K1 impossible
}

void led(uint8_t r, uint8_t g, uint8_t b) { rgbLedWrite(PIN_RGB_DATA, r, g, b); }

// Bip non bloquant : la boucle continue de tourner (watchdogs rafraichis).
static uint32_t finBip = 0;
void bip(uint16_t dureeMs) {
  ledcWrite(PIN_BUZZER, 128);
  finBip = millis() + dureeMs;
}
void gererBuzzer() {
  if (finBip && (int32_t)(millis() - finBip) >= 0) { ledcWrite(PIN_BUZZER, 0); finBip = 0; }
}

// Front descendant sur WDI toutes les WDT_PERIODE_MS, seulement si la boucle tourne.
void rafraichirWatchdogExterne() {
  static uint32_t dernier = 0;
  uint32_t t = millis();
  if (t - dernier >= WDT_PERIODE_MS) {
    dernier = t;
    digitalWrite(PIN_WDT_KICK, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_WDT_KICK, LOW);   // front descendant
  }
}

// ===========================================================================
// Controles (autocontrole au demarrage et surveillance permanente)
// ===========================================================================
// Renvoie "" si tout va bien, sinon le motif du defaut.
String controler(const Entrees& e) {
  if (e.v5mV < V5_MIN_MV || e.v5mV > V5_MAX_MV) return "Tension +5V hors plage : " + String(e.v5mV) + " mV";
  // v0 : les relais ne sont jamais commandes. Une relecture "excite" est donc un defaut
  // (contact soude, commande en court-circuit, cablage).
  if (e.k1Excite) return "Relecture K1 : relais vu excite alors qu'il n'est pas commande";
  if (e.k2Excite) return "Relecture K2 : relais vu excite alors qu'il n'est pas commande";
  if (e.r1 == FC_INCOHERENT) return "Fin de course rentre 1 incoherente (NO = NC)";
  if (e.r2 == FC_INCOHERENT) return "Fin de course rentre 2 incoherente (NO = NC)";
  if (e.s1 == FC_INCOHERENT) return "Fin de course sorti 1 incoherente (NO = NC)";
  if (e.s2 == FC_INCOHERENT) return "Fin de course sorti 2 incoherente (NO = NC)";
  bool rentre = (e.r1 == FC_ACTIONNE) || (e.r2 == FC_ACTIONNE);
  bool sorti  = (e.s1 == FC_ACTIONNE) || (e.s2 == FC_ACTIONNE);
  if (rentre && sorti) return "Fins de course rentre et sorti actionnees en meme temps";
  return "";
}

// ===========================================================================
// Initialisation
// ===========================================================================
void setup() {
  // 1. Sorties de securite a 0 AVANT de les passer en sortie.
  digitalWrite(PIN_K1_CMD, LOW);   pinMode(PIN_K1_CMD, OUTPUT);
  digitalWrite(PIN_K2_CMD, LOW);   pinMode(PIN_K2_CMD, OUTPUT);
  digitalWrite(PIN_WDT_KICK, LOW); pinMode(PIN_WDT_KICK, OUTPUT);
  digitalWrite(PIN_STATUS_OUT, LOW); pinMode(PIN_STATUS_OUT, OUTPUT);  // carte non prete
  ledcAttach(PIN_CP_DRV, CP_FREQ_HZ, CP_RESOLUTION_BITS);
  ledcWrite(PIN_CP_DRV, 0);
  ledcAttach(PIN_BUZZER, BUZ_FREQ_HZ, 8);
  ledcWrite(PIN_BUZZER, 0);

  // 2. Entrees (rappels physiques sur la carte, pas de rappel interne).
  const int entrees[] = { PIN_PWM_IN, PIN_FC_R1_NO, PIN_FC_R1_NC, PIN_FC_R2_NO, PIN_FC_R2_NC,
                          PIN_FC_S1_NO, PIN_FC_S1_NC, PIN_FC_S2_NO, PIN_FC_S2_NC, PIN_EXT_IN2,
                          PIN_V12_OK, PIN_K1_FB, PIN_K2_FB, PIN_ARM_FB, PIN_STOP_HW_FB };
  for (int p : entrees) pinMode(p, INPUT);
  analogSetPinAttenuation(PIN_V5_MON, ADC_11db);
  analogSetPinAttenuation(PIN_EXT_IN1, ADC_11db);
  attachInterrupt(digitalPinToInterrupt(PIN_PWM_IN), isrPwm, CHANGE);

  // 3. Watchdog interne : reset de l'ESP32 si la boucle se bloque.
  esp_task_wdt_config_t cfg = { .timeout_ms = TWDT_TIMEOUT_MS, .idle_core_mask = 0, .trigger_panic = true };
  if (esp_task_wdt_reconfigure(&cfg) != ESP_OK) esp_task_wdt_init(&cfg);
  esp_task_wdt_add(NULL);

  Serial.begin(115200);
  led(0, 0, 40);   // bleu : demarrage
  bip(100);
  Serial.println();
  Serial.println("=== Carte PWM ESP32 - firmware de mise en service v0 ===");
  Serial.println("Verin JAMAIS commande dans cette version. Radio non initialisee.");
  etat = AUTOCONTROLE;
}

// ===========================================================================
// Boucle principale
// ===========================================================================
void loop() {
  esp_task_wdt_reset();
  rafraichirWatchdogExterne();
  gererBuzzer();
  sortiesSures();                    // v0 : relais toujours au repos

  Entrees e = lireEntrees();
  MesurePwm pwm = lirePwm();
  String motif = controler(e);

  switch (etat) {
    case AUTOCONTROLE:
      if (millis() < 500) break;     // laisse les filtres RC et le 12 V s'etablir
      if (motif.length()) { motifDefaut = motif; etat = DEFAUT; bip(400); }
      else { etat = SURE; bip(50); }
      break;
    case SURE:
      if (motif.length()) { motifDefaut = motif; etat = DEFAUT; bip(400); }
      break;
    case DEFAUT:                     // verrouille jusqu'au prochain reset
    default:
      break;
  }

  // Affichage
  bool pret = (etat == SURE);
  digitalWrite(PIN_STATUS_OUT, pret ? HIGH : LOW);
  bool clignote = (millis() / 250) % 2;
  if (etat == DEFAUT)         led(clignote ? 60 : 0, 0, 0);       // rouge clignotant
  else if (etat != SURE)      led(0, 0, 40);                      // bleu
  else if (!e.v12Present)     led(40, 30, 0);                     // jaune : 12 V absent
  else if (!pwm.valide)       led(40, 0, 40);                     // violet : PWM absent ou invalide
  else                        led(0, 40, 0);                      // vert : tout est present

  static uint32_t dernierLog = 0;
  if (millis() - dernierLog >= 500) {
    dernierLog = millis();
    Serial.printf("[%s] PWM %s largeur=%d us periode=%d us | 12V=%s +5V=%d mV | "
                  "R1=%s R2=%s S1=%s S2=%s | K1=%s K2=%s cavalier=%s arret=%s | EXT1=%d mV EXT2=%d\n",
                  NOM_ETAT[etat], pwm.valide ? "OK" : "INVALIDE", pwm.largeurUs, pwm.periodeUs,
                  e.v12Present ? "oui" : "non", e.v5mV,
                  NOM_FC[e.r1], NOM_FC[e.r2], NOM_FC[e.s1], NOM_FC[e.s2],
                  e.k1Excite ? "EXCITE" : "repos", e.k2Excite ? "EXCITE" : "repos",
                  e.cavalierEnPlace ? "oui" : "non", e.contactArretFerme ? "ferme" : "ouvert",
                  e.ext1mV, e.ext2);
    if (etat == DEFAUT) Serial.println("  DEFAUT : " + motifDefaut);
  }
}
