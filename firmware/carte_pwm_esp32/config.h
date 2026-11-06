// config.h — Brochage et constantes de la carte PWM ESP32
// Source du brochage : docs/architecture/BROCHAGE_ESP32.md v3 (DEC-023).
// Carte : ESP32-S3-DevKitC-1-N8 uniquement (DEC-015).
#pragma once

// ---------------------------------------------------------------------------
// Sorties de securite : actives a l'etat haut, rappel a la masse sur la carte
// ---------------------------------------------------------------------------
#define PIN_K1_CMD      21   // Bobine K1 (avec la pompe de charge)
#define PIN_K2_CMD      47   // Bobine K2 (avec le cavalier et le watchdog)
#define PIN_CP_DRV      37   // Pompe de charge, validation de K1
#define PIN_WDT_KICK    14   // Watchdog externe TPS3430, validation de K2

// ---------------------------------------------------------------------------
// Autres sorties
// ---------------------------------------------------------------------------
#define PIN_RGB_DATA     6   // LED RGB WS2812B (via 74AHCT1G125)
#define PIN_BUZZER      13   // Buzzer passif
#define PIN_STATUS_OUT  35   // HAUT = Q2 passant = ligne STATUS basse = carte prete

// ---------------------------------------------------------------------------
// Entrees
// ---------------------------------------------------------------------------
#define PIN_PWM_IN       1
#define PIN_FC_R1_NO     2
#define PIN_FC_R1_NC     9
#define PIN_FC_R2_NO    10
#define PIN_FC_R2_NC    11
#define PIN_FC_S1_NO    15
#define PIN_FC_S1_NC    16
#define PIN_FC_S2_NO    17
#define PIN_FC_S2_NC    36
#define PIN_EXT_IN1      4   // Analogique (ADC1_CH3)
#define PIN_EXT_IN2      5   // Numerique
#define PIN_V5_MON       8   // +5V / 2 (ADC1_CH7)
#define PIN_V12_OK      39   // BAS = 12 V present
#define PIN_K1_FB       40   // BAS = contact de relecture ferme = K1 excite
#define PIN_K2_FB       12   // BAS = K2 excite
#define PIN_ARM_FB      41   // HAUT = cavalier d'armement en place
#define PIN_STOP_HW_FB  42   // BAS = 5e contact ferme et 12 V present

// ---------------------------------------------------------------------------
// Constantes materielles (sources citees)
// ---------------------------------------------------------------------------
// Watchdog TPS3430 : un front descendant toutes les 36 ms +/- 5 ms
// (NETLIST_CIBLE.md P4, datasheet TI tableau 8-3, fenetre 25,9 a 46,8 ms).
#define WDT_PERIODE_MS        36
// Pompe de charge : carre 10 kHz (NETLIST_CIBLE.md P4).
#define CP_FREQ_HZ         10000
#define CP_RESOLUTION_BITS     8
// Buzzer MLT-8530 : 2,7 kHz (BOM STM32 LS1).
#define BUZ_FREQ_HZ         2700
// Pont diviseur V5_MON : R102 = R103 = 10 kOhm -> facteur 2.
#define V5_FACTEUR             2

// ---------------------------------------------------------------------------
// Seuils A DEFINIR avec la FSM (STM32 PO-05). Valeurs provisoires, prudentes.
// ---------------------------------------------------------------------------
#define PWM_LARGEUR_MIN_US   900   // [A DEFINIR] en dessous : impulsion invalide
#define PWM_LARGEUR_MAX_US  2100   // [A DEFINIR] au-dessus : impulsion invalide
#define PWM_PERIODE_MIN_US  10000  // [A DEFINIR] 100 Hz max
#define PWM_PERIODE_MAX_US  40000  // [A DEFINIR] 25 Hz min
#define PWM_TIMEOUT_MS        100  // [A DEFINIR] sans impulsion valide : PWM perdu
#define V5_MIN_MV            4500  // [A DEFINIR] seuil bas du +5V
#define V5_MAX_MV            5500  // LM66100 : VIN max 5,5 V en fonctionnement

// Watchdog interne de l'ESP32 : reset si la boucle principale se bloque.
#define TWDT_TIMEOUT_MS       300
