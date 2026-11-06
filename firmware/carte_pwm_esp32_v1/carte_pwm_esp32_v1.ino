// carte_pwm_esp32_v1.ino — Firmware v1 : machine d'etats a 4 etats
//
// Specification : docs/architecture/FSM_V1.md (proposition, a valider : PO-24).
// Cycle toujours dans le meme sens : REPOS -> PRET -> ACTION -> APRES -> REPOS.
// Etats techniques hors cycle : DEMARRAGE (autocontrole) et DEFAUT (verrouille).
//
// RAPPEL MATERIEL
//   Relais au repos (K1 = K2 = 0) : la tige RENTRE (position sure).
//   K2 seul : bornes au meme potentiel, arret.   K1 + K2 : la tige SORT.
//   K1 n'est tenu que si la pompe de charge tourne ; K2 que si le watchdog externe
//   est rafraichi et si le cavalier d'armement est en place.
//
// RADIO (DEC-007) : aucune bibliotheque radio incluse, radio jamais initialisee.
// CIBLE : ESP32-S3-DevKitC-1-N8, coeur Arduino ESP32 3.x (firmware/README.md).

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "config.h"

// ===========================================================================
// Etats
// ===========================================================================
enum Etat { DEMARRAGE, REPOS, PRET, ACTION, APRES, DEFAUT };
static const char* NOM_ETAT[] = { "DEMARRAGE", "1-REPOS", "2-PRET", "3-ACTION", "4-APRES", "DEFAUT" };
static Etat     etat = DEMARRAGE;
static uint32_t debutEtat = 0;          // millis() a l'entree dans l'etat
static String   motifDefaut = "";
static bool     enButee = false;        // ACTION : fin de course sorti atteinte

// ===========================================================================
// Mesure du PWM par interruption
// ===========================================================================
static volatile int64_t pwmFrontMontant = 0, pwmPeriodeUs = 0, pwmLargeurUs = 0, pwmDernierFin = 0;

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

enum Bande { B_INVALIDE, B_REPOS, B_PRET, B_ACTION, B_ZONE_MORTE };
static const char* NOM_BANDE[] = { "INVALIDE", "repos", "pret", "ACTION", "zone morte" };

struct MesurePwm { Bande bande; int32_t largeurUs; int32_t periodeUs; };

MesurePwm lirePwm() {
  noInterrupts();
  int64_t largeur = pwmLargeurUs, periode = pwmPeriodeUs, fin = pwmDernierFin;
  interrupts();
  MesurePwm m = { B_INVALIDE, (int32_t)largeur, (int32_t)periode };
  bool recent = (fin != 0) && (esp_timer_get_time() - fin) < (int64_t)PWM_TIMEOUT_MS * 1000;
  bool valide = recent
             && largeur >= PWM_LARGEUR_MIN_US && largeur <= PWM_LARGEUR_MAX_US
             && periode >= PWM_PERIODE_MIN_US && periode <= PWM_PERIODE_MAX_US;
  if (!valide) return m;
  if      (largeur >= BANDE_REPOS_MIN_US - BANDE_TOLERANCE_US && largeur <= BANDE_REPOS_MAX_US)   m.bande = B_REPOS;
  else if (largeur >= BANDE_PRET_MIN_US                       && largeur <= BANDE_PRET_MAX_US)    m.bande = B_PRET;
  else if (largeur >= BANDE_ACTION_MIN_US && largeur <= BANDE_ACTION_MAX_US + BANDE_TOLERANCE_US) m.bande = B_ACTION;
  else m.bande = B_ZONE_MORTE;
  return m;
}

// Duree pendant laquelle la bande est restee la meme sans interruption.
static Bande    bandePrec = B_INVALIDE;
static uint32_t debutBande = 0;
uint32_t dureeBande(Bande b) {
  if (b != bandePrec) { bandePrec = b; debutBande = millis(); }
  return millis() - debutBande;
}

// ===========================================================================
// Entrees
// ===========================================================================
// Fin de course : repos = NO haut / NC bas ; actionne = NO bas / NC haut ; NO == NC : incoherent.
enum EtatFc { FC_REPOS, FC_ACTIONNE, FC_INCOHERENT };
static const char* NOM_FC[] = { "repos", "ACT", "INCOH" };

EtatFc lireFc(int pinNo, int pinNc) {
  bool no = digitalRead(pinNo), nc = digitalRead(pinNc);
  if (no == nc) return FC_INCOHERENT;
  return no ? FC_REPOS : FC_ACTIONNE;
}

struct Entrees {
  EtatFc r1, r2, s1, s2;
  bool rentre;            // R1 ET R2 actionnees
  bool sorti;             // S1 OU S2 actionnee
  bool v12Present, k1Excite, k2Excite, cavalierEnPlace, contactArretFerme, ext2;
  int  v5mV, ext1mV;
};

Entrees lireEntrees() {
  Entrees e;
  e.r1 = lireFc(PIN_FC_R1_NO, PIN_FC_R1_NC);
  e.r2 = lireFc(PIN_FC_R2_NO, PIN_FC_R2_NC);
  e.s1 = lireFc(PIN_FC_S1_NO, PIN_FC_S1_NC);
  e.s2 = lireFc(PIN_FC_S2_NO, PIN_FC_S2_NC);
  e.rentre = (e.r1 == FC_ACTIONNE) && (e.r2 == FC_ACTIONNE);
  e.sorti  = (e.s1 == FC_ACTIONNE) || (e.s2 == FC_ACTIONNE);
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
static bool cmdK1 = false, cmdK2 = false;
static uint32_t changementK1 = 0, changementK2 = 0;

// Seule fonction qui touche aux relais. Ordre impose : K2 avant K1 a l'excitation,
// K1 avant K2 a la retombee (jamais de sortie de tige pendant une transition).
void commanderRelais(bool k1, bool k2) {
  if (!k2) k1 = false;                       // K1 sans K2 : interdit
  if (!k1 && cmdK1) { digitalWrite(PIN_K1_CMD, LOW); ledcWrite(PIN_CP_DRV, 0); cmdK1 = false; changementK1 = millis(); }
  if (k2 != cmdK2)  { digitalWrite(PIN_K2_CMD, k2 ? HIGH : LOW); cmdK2 = k2; changementK2 = millis(); }
  if (k1 && !cmdK1) {
    ledcWrite(PIN_CP_DRV, 1 << (CP_RESOLUTION_BITS - 1));   // pompe : carre 50 %
    digitalWrite(PIN_K1_CMD, HIGH); cmdK1 = true; changementK1 = millis();
  }
}

void led(uint8_t r, uint8_t g, uint8_t b) { rgbLedWrite(PIN_RGB_DATA, r, g, b); }

static uint32_t finBip = 0;
void bip(uint16_t dureeMs) { ledcWrite(PIN_BUZZER, 128); finBip = millis() + dureeMs; }
void gererBuzzer() {
  if (finBip && (int32_t)(millis() - finBip) >= 0) { ledcWrite(PIN_BUZZER, 0); finBip = 0; }
}

void rafraichirWatchdogExterne() {
  static uint32_t dernier = 0;
  if (millis() - dernier >= WDT_PERIODE_MS) {
    dernier = millis();
    digitalWrite(PIN_WDT_KICK, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_WDT_KICK, LOW);         // front descendant
  }
}

// ===========================================================================
// Controles permanents. Renvoie "" si tout va bien, sinon le motif du defaut.
// ===========================================================================
String controler(const Entrees& e) {
  if (e.v5mV < V5_MIN_MV || e.v5mV > V5_MAX_MV) return "Tension +5V hors plage : " + String(e.v5mV) + " mV";
  if (e.r1 == FC_INCOHERENT) return "Fin de course rentre 1 incoherente (NO = NC)";
  if (e.r2 == FC_INCOHERENT) return "Fin de course rentre 2 incoherente (NO = NC)";
  if (e.s1 == FC_INCOHERENT) return "Fin de course sorti 1 incoherente (NO = NC)";
  if (e.s2 == FC_INCOHERENT) return "Fin de course sorti 2 incoherente (NO = NC)";
  bool unRentre = (e.r1 == FC_ACTIONNE) || (e.r2 == FC_ACTIONNE);
  if (unRentre && e.sorti) return "Fins de course rentre et sorti actionnees en meme temps";
  // Relecture des relais (apres le temps de basculement).
  if (millis() - changementK1 > T_RELAIS_MS && e.k1Excite != cmdK1)
    return String("Relecture K1 incoherente : commande ") + (cmdK1 ? "1" : "0") + ", relu " + (e.k1Excite ? "1" : "0");
  // K2 : sans cavalier, la bobine n'est pas alimentee ; ACTION n'est jamais atteint sans cavalier.
  if (millis() - changementK2 > T_RELAIS_MS && e.k2Excite != cmdK2)
    return String("Relecture K2 incoherente : commande ") + (cmdK2 ? "1" : "0") + ", relu " + (e.k2Excite ? "1" : "0");
  return "";
}

// Conditions pour entrer et rester en PRET (FSM_V1.md §4).
bool conditionsPret(const Entrees& e) {
  return e.v12Present && e.cavalierEnPlace && e.rentre && !e.sorti;
}

// ===========================================================================
// Changement d'etat
// ===========================================================================
void allerA(Etat nouveau, const String& motif = "") {
  if (nouveau == etat) return;
  Serial.printf(">>> %s -> %s %s\n", NOM_ETAT[etat], NOM_ETAT[nouveau], motif.c_str());
  etat = nouveau;
  debutEtat = millis();
  enButee = false;
  switch (etat) {
    case PRET:   bip(80);  break;
    case ACTION: bip(300); break;
    case DEFAUT: bip(800); break;
    default: break;
  }
}

void passerEnDefaut(const String& motif) {
  commanderRelais(false, false);
  if (etat == DEFAUT) return;                // premier motif conserve
  motifDefaut = motif;
  allerA(DEFAUT, motif);
}

// ===========================================================================
// Initialisation
// ===========================================================================
void setup() {
  // Sorties de securite a 0 AVANT de les passer en sortie.
  digitalWrite(PIN_K1_CMD, LOW);     pinMode(PIN_K1_CMD, OUTPUT);
  digitalWrite(PIN_K2_CMD, LOW);     pinMode(PIN_K2_CMD, OUTPUT);
  digitalWrite(PIN_WDT_KICK, LOW);   pinMode(PIN_WDT_KICK, OUTPUT);
  digitalWrite(PIN_STATUS_OUT, LOW); pinMode(PIN_STATUS_OUT, OUTPUT);   // carte non prete
  ledcAttach(PIN_CP_DRV, CP_FREQ_HZ, CP_RESOLUTION_BITS);  ledcWrite(PIN_CP_DRV, 0);
  ledcAttach(PIN_BUZZER, BUZ_FREQ_HZ, 8);                  ledcWrite(PIN_BUZZER, 0);

  const int entrees[] = { PIN_PWM_IN, PIN_FC_R1_NO, PIN_FC_R1_NC, PIN_FC_R2_NO, PIN_FC_R2_NC,
                          PIN_FC_S1_NO, PIN_FC_S1_NC, PIN_FC_S2_NO, PIN_FC_S2_NC, PIN_EXT_IN2,
                          PIN_V12_OK, PIN_K1_FB, PIN_K2_FB, PIN_ARM_FB, PIN_STOP_HW_FB };
  for (int p : entrees) pinMode(p, INPUT);        // rappels physiques sur la carte
  analogSetPinAttenuation(PIN_V5_MON, ADC_11db);
  analogSetPinAttenuation(PIN_EXT_IN1, ADC_11db);
  attachInterrupt(digitalPinToInterrupt(PIN_PWM_IN), isrPwm, CHANGE);

  esp_task_wdt_config_t cfg = { .timeout_ms = TWDT_TIMEOUT_MS, .idle_core_mask = 0, .trigger_panic = true };
  if (esp_task_wdt_reconfigure(&cfg) != ESP_OK) esp_task_wdt_init(&cfg);
  esp_task_wdt_add(NULL);

  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Carte PWM ESP32 - firmware v1 (FSM 4 etats) ===");
  led(0, 0, 40);
  bip(100);
  debutEtat = millis();
}

// ===========================================================================
// Boucle principale
// ===========================================================================
void loop() {
  esp_task_wdt_reset();
  rafraichirWatchdogExterne();
  gererBuzzer();

  Entrees e = lireEntrees();
  MesurePwm pwm = lirePwm();
  uint32_t tBande = dureeBande(pwm.bande);
  uint32_t tEtat  = millis() - debutEtat;

  // 1. Defauts graves : verrouillage.
  if (etat != DEFAUT && !(etat == DEMARRAGE && tEtat < 500)) {
    String motif = controler(e);
    if (motif.length()) passerEnDefaut(motif);
  }

  // 2. Desaccord durable R1 / R2 en position stable (REPOS, PRET).
  static uint32_t debutDesaccord = 0;
  bool desaccord = (etat == REPOS || etat == PRET) && (e.r1 != e.r2);
  if (!desaccord) debutDesaccord = millis();
  else if (millis() - debutDesaccord > T_DESACCORD_FC_MS) passerEnDefaut("Desaccord durable entre R1 et R2");

  // 3. Rentree trop longue : 12 V present, relais au repos, tige pas rentree depuis trop longtemps.
  static uint32_t debutNonRentre = 0;
  bool doitRentrer = (etat == REPOS || etat == APRES) && e.v12Present && !e.rentre;
  if (!doitRentrer) debutNonRentre = millis();
  else if (millis() - debutNonRentre > T_COURSE_MAX_MS)
    passerEnDefaut("Rentree trop longue : pas de fin de course rentre apres " + String(T_COURSE_MAX_MS) + " ms");

  // 4. Machine d'etats.
  switch (etat) {
    case DEMARRAGE:
      commanderRelais(false, false);
      if (tEtat >= 500) allerA(REPOS, "(autocontrole OK)");
      break;

    case REPOS:                                   // relais au repos : la tige rentre si 12 V
      commanderRelais(false, false);
      if (pwm.bande == B_PRET && tBande >= T_CONFIRM_PRET_MS && conditionsPret(e))
        allerA(PRET, "(PWM pret confirme)");
      break;

    case PRET:                                    // aucun mouvement
      commanderRelais(false, false);
      if (!conditionsPret(e) || pwm.bande == B_REPOS || pwm.bande == B_INVALIDE)
        allerA(APRES, "(abandon)");
      else if (pwm.bande == B_ACTION && tBande >= T_CONFIRM_ACTION_MS)
        allerA(ACTION, "(PWM action confirme)");
      break;

    case ACTION:
      if (pwm.bande != B_ACTION || !e.v12Present || !e.cavalierEnPlace) {
        commanderRelais(false, false);
        allerA(APRES, "(fin de demande d'action)");
      } else if (!e.contactArretFerme) {
        passerEnDefaut("Arret materiel atteint pendant l'action (5e contact ouvert)");
      } else if (enButee && !e.sorti) {
        commanderRelais(false, false);            // position sortie perdue : retour sur
        allerA(APRES, "(fin de course sorti perdue pendant le maintien)");
      } else if (e.sorti || enButee) {
        enButee = true;
        commanderRelais(false, true);             // arrive en butee : arret, position tenue
      } else if (tEtat > T_COURSE_MAX_MS) {
        passerEnDefaut("Sortie trop longue : pas de fin de course sorti apres " + String(T_COURSE_MAX_MS) + " ms");
      } else {
        commanderRelais(true, true);              // sortie de tige
      }
      break;

    case APRES:                                   // relais au repos : la tige rentre
      commanderRelais(false, false);
      if (e.rentre && pwm.bande == B_REPOS)
        allerA(REPOS, "(tige rentree, PWM repos)");
      break;

    case DEFAUT:                                  // verrouille jusqu'au reset
    default:
      commanderRelais(false, false);
      break;
  }

  // 5. Affichage.
  bool clignote = (millis() / 250) % 2;
  static bool clignotePrec = false;
  if (etat == ACTION && clignote && !clignotePrec) bip(30);    // un bip par clignotement
  clignotePrec = clignote;
  digitalWrite(PIN_STATUS_OUT, (etat == REPOS || etat == PRET) ? HIGH : LOW);   // HAUT = ligne basse = prete
  switch (etat) {
    case DEMARRAGE: led(0, 0, 40); break;
    case REPOS:     led(0, 40, 0); break;                       // vert
    case PRET:      led(50, 20, 0); break;                      // orange
    case ACTION:    led(0, 0, clignote ? 60 : 0);               // bleu clignotant + bip court
                    break;
    case APRES:     led(0, 30, 30); break;                      // cyan
    default:        led(clignote ? 60 : 0, 0, 0); break;        // rouge clignotant
  }

  static uint32_t dernierLog = 0;
  if (millis() - dernierLog >= 500) {
    dernierLog = millis();
    Serial.printf("[%s] PWM %s %d us | 12V=%s +5V=%d mV cav=%s | R1=%s R2=%s S1=%s S2=%s arret=%s | "
                  "K1 cmd=%d lu=%d K2 cmd=%d lu=%d | EXT1=%d mV EXT2=%d\n",
                  NOM_ETAT[etat], NOM_BANDE[pwm.bande], pwm.largeurUs,
                  e.v12Present ? "oui" : "non", e.v5mV, e.cavalierEnPlace ? "oui" : "non",
                  NOM_FC[e.r1], NOM_FC[e.r2], NOM_FC[e.s1], NOM_FC[e.s2], e.contactArretFerme ? "ferme" : "OUVERT",
                  cmdK1, e.k1Excite, cmdK2, e.k2Excite, e.ext1mV, e.ext2);
    if (etat == DEFAUT) Serial.println("  DEFAUT : " + motifDefaut);
  }
}
