# Netlist cible — carte ESP32

**Phase :** 2 (`CONSIGNES_EASYEDA_CLAUDE_CODE.md`). **Jalon :** G2 **validé** (DEC-027).
**Version :** v1, 2026-11-06. Dérivée de la netlist STM32 v1.3 (`references/stm32_v3/hardware/NETLIST_CIBLE.md`). Les blocs repris sont copiés à l'identique, sauf les lignes marquées **Modifié**.
**Composants :** `hardware/BOM_CIBLE.md`. **Brochage :** `docs/architecture/BROCHAGE_ESP32.md` v3. **Alimentation :** `docs/architecture/ALIMENTATION_ESP32.md`.

## Pages du schéma

Projet vierge (DEC-028) : toutes les pages sont saisies. La dernière colonne indique l'écart avec la carte STM32, pour la relecture.

| Page | Nom dans EasyEDA | Contenu | Écart avec la carte STM32 |
|---|---|---|---|
| P1 | 01_ALIM_5V_3V3 | J1, J13, U15, U16, LED5, `V5_MON` | **Modifiée** : U2 et C103 à C105 supprimés, U16 déplacée depuis la page USB |
| P2 | 02_ESP32 | Carte de dev ESP32 (barrettes J20, J21) | **Remplacée** : STM32, quartz, boutons, SWD, J10 supprimés |
| P3 | 03_ENTREE_PWM | Entrée PWM | Inchangée |
| — | 04_USB_PC | — | **Supprimée** |
| P3B | 05_FINS_DE_COURSE | J4, J5, J14, J15 | Inchangée |
| P3C | 06_EXTENSION | J8, 2 voies | **Modifiée** : voies 3 et 4, J11 (maintenance), révision supprimés |
| P4 | 07_RELAIS_SECURITES | K1, K2, pompe, watchdog, J7 | **Modifiée** : R411 vers `ESP_EN`, R413 = 15 kΩ |
| P4B | 08_IHM_STATUS | LED RGB, buzzer, STATUS | **Modifiée** : 4 LED → LED RGB + adaptateur |
| P5 | 09_PUISSANCE_12V | 12 V, pont du vérin, optos | **Modifiée** : R504 supprimé |
| — | 10_MESURE_COURANT | — | **Supprimée** |

Chaque page porte un texte explicatif en bleu (CONSIGNES, règle C2).

## 0. Conventions

| Sujet | Règle |
|---|---|
| Domaine commande | `GND`, `+5V`, `+5V_ESP`, `+3V3` |
| Domaine puissance (isolé) | `GND_P`, `+12V_IN`, `+12V_F`, `+12V_P` |
| Interdit | Tout composant reliant un net du domaine commande à un net du domaine puissance, sauf K1, K2, U6, U7 (contrôle V4) |
| Nets ESP32 | Noms de `BROCHAGE_ESP32.md` v3 ; `STATUS` y est nommé `STATUS_OUT` ici, comme sur la carte STM32 |
| Nets côté connecteur | Suffixe `X` pour le nœud avant la résistance série |
| Non montés | **NM** (empreinte posée, composant non assemblé) |

---

## P1 — Alimentation commande

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J1 | Bornier WJ15EDGRC-3.81-04P | 1 / 2 / 3 / 4 | `+5V_IN` / `GND` / `PWM_EXT` / `STATUS_EXT` |
| F1 | BSMD1206-050-30V | 1 / 2 | `+5V_IN` / `+5V_F` |
| D1 | SMAJ5.0A | K / A | `+5V_F` / `GND` |
| U15 | LM66100DCKR (diode idéale, J1) | 1 VIN / 2 GND / 3 CE# / 4 NC / 5 ST / 6 VOUT | `+5V_F` / `GND` / `+5V` / non connecté / `GND` / `+5V` |
| C107 | 1 µF | 1 / 2 | `+5V_F` / `GND` |
| C101 | 10 µF 0805 | 1 / 2 | `+5V` / `GND` |
| C102 | 100 nF | 1 / 2 | `+5V` / `GND` |
| U16 | LM66100DCKR (diode idéale, vers la carte de dev) | 1 VIN / 2 GND / 3 CE# / 4 NC / 5 ST / 6 VOUT | `+5V` / `GND` / `+5V_ESP` / non connecté / `GND` / `+5V_ESP` — **Modifié** : entrée sur `+5V` au lieu de `VBUS` |
| C602 | 1 µF | 1 / 2 | `+5V` / `GND` (entrée U16, repère STM32 conservé) |
| C108 | 1 µF | 1 / 2 | `+5V_ESP` / `GND` (sortie U16) — **Ajouté** |
| J13 | Barrette 1×4 2,54 (Dupont, parallèle de J1) | 1 / 2 / 3 / 4 | `+5V_IN` / `GND` / `PWM_EXT` / `STATUS_EXT` |
| R426 | 680 Ω | 1 / 2 | `+3V3` / `LED5_A` |
| LED5 | NCD0603C1 (3,3 V présent) | A / K | `LED5_A` / `GND` |
| R102 | 10 kΩ | 1 / 2 | `+5V` / `V5_MON` |
| R103 | 10 kΩ | 1 / 2 | `V5_MON` / `GND` |
| C106 | 100 nF | 1 / 2 | `V5_MON` / `GND` |
| C105 | 100 nF | 1 / 2 | `+3V3` / `GND` (découplage du `+3V3` reçu de la carte de dev, au plus près de J20) |
| TP101, TP102, TP103 | Points de test | — | `+5V`, `+3V3`, `GND` |

Supprimés : U2 (ME6211), C103, C104. Le `+3V3` vient de la broche 3V3 de la carte de dev (`ALIMENTATION_ESP32.md` A3).

U15 et U16 : montage « blocage permanent du courant inverse » (CE# relié à VOUT, datasheet TI SLVSEZ8A §8.3.2). U16 empêche l'USB de la carte de dev d'alimenter `+5V`. `V5_MON` = `+5V` / 2.

---

## P2 — Carte de dev ESP32-S3-DevKitC-1-N8

La carte de dev s'enfiche sur deux barrettes femelles 1×22 au pas de 2,54 mm. Brochage des rangées J1 et J3 de la carte de dev : guide Espressif v1.1. **Écart entre rangées : 25,40 mm [Hypothèse]**, relevé sur le plan Espressif `DXF_ESP32-S3-DevKitC-1_V1_20210312CB` sans cote nommée : à mesurer sur la carte reçue avant le placement (PO-21).

### J20 — rangée J1 de la carte de dev

| Broche | Carte de dev | Net |
|---|---|---|
| 1 | 3V3 | `+3V3` |
| 2 | 3V3 | `+3V3` |
| 3 | RST (EN) | `ESP_EN` |
| 4 | GPIO4 | `EXT_IN1` |
| 5 | GPIO5 | `EXT_IN2` |
| 6 | GPIO6 | `RGB_DATA` |
| 7 | GPIO7 | non connecté (réserve) |
| 8 | GPIO15 | `FC_S1_NO` |
| 9 | GPIO16 | `FC_S1_NC` |
| 10 | GPIO17 | `FC_S2_NO` |
| 11 | GPIO18 | non connecté (sensible) |
| 12 | GPIO8 | `V5_MON` |
| 13 | GPIO3 | non connecté (sensible) |
| 14 | GPIO46 | non connecté (sensible) |
| 15 | GPIO9 | `FC_R1_NC` |
| 16 | GPIO10 | `FC_R2_NO` |
| 17 | GPIO11 | `FC_R2_NC` |
| 18 | GPIO12 | `K2_FB` |
| 19 | GPIO13 | `BUZZER` |
| 20 | GPIO14 | `WDT_KICK` |
| 21 | 5V | `+5V_ESP` |
| 22 | G | `GND` |

### J21 — rangée J3 de la carte de dev

| Broche | Carte de dev | Net |
|---|---|---|
| 1 | G | `GND` |
| 2 | TX (GPIO43) | non connecté (moniteur série USB) |
| 3 | RX (GPIO44) | non connecté (moniteur série USB) |
| 4 | GPIO1 | `PWM_IN` |
| 5 | GPIO2 | `FC_R1_NO` |
| 6 | GPIO42 | `STOP_HW_FB` |
| 7 | GPIO41 | `ARM_FB` |
| 8 | GPIO40 | `K1_FB` |
| 9 | GPIO39 | `V12_OK` |
| 10 | GPIO38 | non connecté (LED RGB de la carte de dev) |
| 11 | GPIO37 | `CP_DRV` |
| 12 | GPIO36 | `FC_S2_NC` |
| 13 | GPIO35 | `STATUS_OUT` |
| 14 | GPIO0 | non connecté (BOOT) |
| 15 | GPIO45 | non connecté (sensible) |
| 16 | GPIO48 | non connecté (sensible) |
| 17 | GPIO47 | `K2_CMD` |
| 18 | GPIO21 | `K1_CMD` |
| 19 | GPIO20 | non connecté (USB natif) |
| 20 | GPIO19 | non connecté (USB natif) |
| 21 | G | `GND` |
| 22 | G | `GND` |

`ESP_EN` ne porte que l'option R411 (NM). Le RC de reset est sur la carte de dev.

---

## P3 — Entrées

### PWM

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| D9 | ESD5Z5V0 | K / A | `PWM_EXT` / `GND` |
| R301 | 100 kΩ | 1 / 2 | `PWM_EXT` / `GND` (entrée débranchée = niveau bas) |
| R302 | 1 kΩ | 1 / 2 | `PWM_EXT` / `PWM_F` |
| C301 | 1 nF | 1 / 2 | `PWM_F` / `GND` (τ = 1 µs) |
| U3 | SN74LVC1G17DBVR | 1 NC | non connecté |
| | | 2 A | `PWM_F` |
| | | 3 GND | `GND` |
| | | 4 Y | `PWM_BUF` |
| | | 5 VCC | `+3V3` |
| C302 | 100 nF | 1 / 2 | `+3V3` / `GND` |
| R303 | 33 Ω | 1 / 2 | `PWM_BUF` / `PWM_IN` (→ GPIO1) |

### Fins de course (8 voies)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J4 (rentré) | Bornier WJ15EDGRC-3.81-06P | 1 | `GND` (COM contact R1) |
| | | 2 / 3 | `FCX_R1_NO` / `FCX_R1_NC` |
| | | 4 | `GND` (COM contact R2) |
| | | 5 / 6 | `FCX_R2_NO` / `FCX_R2_NC` |
| J5 (sorti) | Bornier WJ15EDGRC-3.81-07P | 1 | `GND` (COM contact S1) |
| | | 2 / 3 | `FCX_S1_NO` / `FCX_S1_NC` |
| | | 4 | `GND` (COM contact S2) |
| | | 5 / 6 | `FCX_S2_NO` / `FCX_S2_NC` |
| | | 7 | non connecté (détrompage) |
| J14 / J15 | Barrettes 1×6 / 1×7 2,54 (Dupont) | 1 à 6 / 1 à 7 | Mêmes nets que J4 / J5, broche par broche (J15.7 non connecté) |
| U11 | SRV05-4 | 1 I/O1 / 3 I/O2 / 4 I/O3 / 6 I/O4 | `FCX_R1_NO` / `FCX_R1_NC` / `FCX_R2_NO` / `FCX_R2_NC` |
| | | 2 GND / 5 VCC | `GND` / `+3V3` |
| U12 | SRV05-4 | 1 / 3 / 4 / 6 | `FCX_S1_NO` / `FCX_S1_NC` / `FCX_S2_NO` / `FCX_S2_NC` |
| | | 2 / 5 | `GND` / `+3V3` |

Par voie (i = R1_NO, R1_NC, R2_NO, R2_NC, S1_NO, S1_NC, S2_NO, S2_NC) :

| Repères | Valeur | Broches → nets |
|---|---|---|
| R311 à R318 | 1 kΩ, `A_DEFINIR` (PO-23) | `FCX_i` → `+3V3` (rappel, courant de mouillage 3,3 mA) |
| R321 à R328 | 1 kΩ | `FCX_i` → `FC_i` |
| C311 à C318 | 10 nF | `FC_i` → `GND` (τ = 10 µs) |

Brochage SRV05-4 (SOT-23-6 : 1 I/O1, 2 GND, 3 I/O2, 4 I/O3, 5 VCC, 6 I/O4) : à confirmer sur le symbole LCSC C85364 (AV).

### Extension (2 voies, DEC-021) — **Modifié**

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J8 | Barrette 1×4 2,54 | 1 / 2 / 3 / 4 | `+3V3_EXT` / `EXTX_1` / `EXTX_2` / `GND` |
| R331 | 0 Ω | 1 / 2 | `+3V3` / `+3V3_EXT` |
| D11, D12 | ESD5Z5V0 | K / A | `EXTX_1`, `EXTX_2` / `GND` |
| R332, R333 | 4,7 kΩ | 1 / 2 | `EXTX_n` / `EXT_INn` |
| C331, C332 | 10 nF | 1 / 2 | `EXT_INn` / `GND` |
| R336, R337 | 10 kΩ **NM** | 1 / 2 | `EXTX_n` / `+3V3` (rappel haut optionnel) |
| R340, R341 | 10 kΩ **NM** | 1 / 2 | `EXTX_n` / `GND` (rappel bas optionnel) |

`EXT_IN1` : analogique (GPIO4, ADC1_CH3). `EXT_IN2` : numérique (GPIO5). **Les signaux d'extension ne doivent pas dépasser 3,3 V** : l'ESP32 n'est pas tolérant 5 V. R332 / R333 limitent le courant en cas d'erreur. Supprimés : voies 3 et 4 (D13, D14, R334, R335, C333, C334, R338, R339, R342, R343), J11, R351 à R354, C351.

---

## P4 — Commande des relais et IHM

### Chaîne K1 (pompe de charge)

Brochage G6K-2F-Y (symbole LCSC C47190, image relue) : bobine 1 (+) – 8 (−), **bobine polarisée** (datasheet Omron K106 : « Check carefully the coil polarity »). Pôle bas : COM 3, NC 2, NO 4. Pôle haut : COM 6, NC 7, NO 5. Pôle bas côté commande (rangée 1-2-3-4, avec la bobine +), pôle haut côté puissance. **Confirmé** sur le dessin « G6K-2F-Y Terminal Arrangement/Internal Connections (TOP VIEW) » de la datasheet Omron K106 (session 13).

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| K1 | G6K-2F-Y DC5 | 1 bobine (+) | `+5V` |
| | | 8 bobine (−) | `K1_COIL_N` |
| | | pôle B (commande) : 3 COM / 4 NO / 2 NC | `GND` / `K1_FBX` / non connecté |
| | | pôle A (puissance) : 6 COM / 5 NO / 7 NC | `VERIN_A` / `NO_FEED` / `GND_P` (→ P5) — **Modifié** (R504 supprimé) |
| D4 | 1N4148W | K / A | `+5V` / `K1_COIL_N` |
| Q3 | AO3400A (validation pompe) | 1 G / 2 S / 3 D | `CP_OUT` / `K1_MID` / `K1_COIL_N` |
| Q4 | AO3400A (K1_CMD) | 1 G / 2 S / 3 D | `K1_G` / `GND` / `K1_MID` |
| R401 | 100 Ω | 1 / 2 | `K1_CMD` / `K1_G` |
| R402 | 100 kΩ | 1 / 2 | `K1_G` / `GND` |
| C401 | 100 nF | 1 / 2 | `CP_DRV` / `CP_A` |
| D7 | BAT54S | 1 (anode D1) | `GND` |
| | | 3 (cathode D1 / anode D2) | `CP_A` |
| | | 2 (cathode D2) | `CP_OUT` |
| C402 | 100 nF | 1 / 2 | `CP_OUT` / `GND` |
| R403 | 100 kΩ | 1 / 2 | `CP_OUT` / `GND` (purge, τ = 10 ms) |
| R404 | 10 kΩ | 1 / 2 | `K1_FBX` / `+3V3` |
| R405 | 1 kΩ | 1 / 2 | `K1_FBX` / `K1_FB` |
| C403 | 10 nF | 1 / 2 | `K1_FB` / `GND` |

Pompe : `CP_DRV` carré 0 / 3,3 V à 10 kHz (périphérique LEDC de l'ESP32). `CP_OUT` ≈ 3,3 V − 2 × VF(BAT54S) ≈ 2,7 V, au-dessus de VGS(th) max de Q3 (1,45 V) ; RDS(on) de Q3 spécifié à 2,5 V. Signal figé (haut ou bas) → `CP_OUT` décroît avec τ = 10 ms → Q3 bloqué → K1 retombe. Carte de dev retirée : pas de `CP_DRV`, K1 au repos (PO-20).

### Chaîne K2 (cavalier + watchdog externe)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J7 | Bornier WJ2EDGRC-5.08-03P-14-00A, C8377 (armement : pont de fil 1-3 dans la fiche) | 1 / 2 / 3 | `+5V` / non connecté / `K2_ARM` |
| K2 | G6K-2F-Y DC5 | 1 bobine (+) | `K2_ARM` |
| | | 8 bobine (−) | `K2_COIL_N` |
| | | pôle B (commande) : 3 COM / 4 NO / 2 NC | `GND` / `K2_FBX` / non connecté |
| | | pôle A (puissance) : 6 COM / 5 NO / 7 NC | `VERIN_B` / `GND_P` / `+12V_P` (→ P5) — **Modifié** (R504 supprimé) |
| D5 | 1N4148W | K / A | `K2_ARM` / `K2_COIL_N` |
| Q5 | AO3400A (validation watchdog) | 1 G / 2 S / 3 D | `WD_OK` / `K2_MID` / `K2_COIL_N` |
| Q6 | AO3400A (K2_CMD) | 1 G / 2 S / 3 D | `K2_G` / `GND` / `K2_MID` |
| R406 | 100 Ω | 1 / 2 | `K2_CMD` / `K2_G` |
| R407 | 100 kΩ | 1 / 2 | `K2_G` / `GND` |
| U4 | TPS3430WDRCR | 1 VDD1 / 10 VDD2 | `+3V3` |
| | | 2 CWD | non connecté (temporisation usine) |
| | | 3 SET0 / 6 SET1 | `GND` / `GND` |
| | | 4 CRST | non connecté (tRST 200 ms) |
| | | 5 GND, pastille | `GND` |
| | | 7 WDI | `WDT_KICK` |
| | | 8 WDO | `WD_OK` |
| | | 9 NC | non connecté |
| C404 | 100 nF | 1 / 2 | `+3V3` / `GND` |
| R408 | 100 kΩ | 1 / 2 | `WDT_KICK` / `GND` (WDI défini pendant le reset du MCU) |
| R409 | 10 kΩ | 1 / 2 | `WD_OK` / `+3V3` (rappel de WDO) |
| R411 | 0 Ω **NM** | 1 / 2 | `WD_OK` / `ESP_EN` (option : défaut watchdog → reset de l'ESP32) |
| R412 | 10 kΩ | 1 / 2 | `K2_ARM` / `ARM_FB` |
| R413 | **15 kΩ** | 1 / 2 | `ARM_FB` / `GND` (5,0 V → 3,0 V ; 5,5 V → 3,3 V). **Modifié** : 20 kΩ donnait 3,33 V à 5,0 V, au-delà de 3,3 V ; l'ESP32 n'est pas tolérant 5 V |
| C405 | 10 nF | 1 / 2 | `ARM_FB` / `GND` |
| R414 | 10 kΩ | 1 / 2 | `K2_FBX` / `+3V3` |
| R415 | 1 kΩ | 1 / 2 | `K2_FBX` / `K2_FB` |
| C406 | 10 nF | 1 / 2 | `K2_FB` / `GND` |

Watchdog (CWD non connecté, SET0 = SET1 = 0, datasheet TI tableau 8-3) : fenêtre fermée tWDL 19,1 à 25,9 ms, ouverture tWDU 46,8 à 63,3 ms. **Le firmware doit produire un front descendant sur `WDT_KICK` toutes les 36 ms ± 5 ms.** Hors fenêtre : `WD_OK` bas pendant 200 ms → Q5 bloqué → K2 retombe. Carte de dev retirée : plus de `+3V3`, `WD_OK` bas, K2 au repos (PO-20).

Relecture : contact pôle B fermé (relais excité) → `K1_FB` / `K2_FB` bas. Cavalier présent → `ARM_FB` haut.

### STATUS_OUT (X02)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| Q2 | AO3400A | 1 G / 2 S / 3 D | `Q2_G` / `GND` / `STATUS_D` |
| R416 | 100 Ω | 1 / 2 | `STATUS_OUT` / `Q2_G` |
| R417 | 100 kΩ | 1 / 2 | `Q2_G` / `GND` |
| R418 | 100 Ω | 1 / 2 | `STATUS_D` / `STATUS_EXT` |
| D10 | ESD5Z5V0 | K / A | `STATUS_EXT` / `GND` |

Sortie à drain ouvert : rappel haut fourni par le contrôleur externe. ESP32 arrêté ou retiré → Q2 bloqué → ligne haute. **Convention proposée : ligne basse = carte prête ; ligne haute = non prête ou défaut** (sûre par défaut). À valider avec la FSM (PO-05).

### LED RGB et buzzer — **Modifié**

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| R420 | 100 kΩ | 1 / 2 | `RGB_DATA` / `GND` (entrée jamais flottante si l'ESP32 est retiré ou en reset) |
| U17 | SN74AHCT1G125DBVR | 1 OE / 2 A / 3 GND / 4 Y / 5 VCC | `GND` / `RGB_DATA` / `GND` / `RGB_Y` / `+5V` |
| C420 | 100 nF | 1 / 2 | `+5V` / `GND` (au plus près de U17) |
| R421 | 330 Ω | 1 / 2 | `RGB_Y` / `RGB_DIN` |
| LED1 | WS2812B-B/W | VDD / VSS / DIN / DOUT | `+5V` / `GND` / `RGB_DIN` / non connecté (broches à lire sur le symbole LCSC : AV) |
| C421 | 100 nF | 1 / 2 | `+5V` / `GND` (au plus près de LED1) |
| LS1 | MLT-8530 | + / − | `+3V3` / `BUZ_N` (polarité à confirmer sur le symbole : AV) |
| D6 | 1N4148W | K / A | `+3V3` / `BUZ_N` |
| Q7 | AO3400A | 1 G / 2 S / 3 D | `BUZ_G` / `GND` / `BUZ_N` |
| R424 | 100 Ω | 1 / 2 | `BUZZER` / `BUZ_G` |
| R425 | 100 kΩ | 1 / 2 | `BUZ_G` / `GND` |
| C407 | 10 µF 0805 | 1 / 2 | `+3V3` / `GND` (au plus près de LS1) |

U17 : brochage et seuils vérifiés sur la datasheet TI SCLS378P. OE actif bas, relié à la masse. Entrée sans diode vers VCC (VI max 7 V indépendant de VCC) : sans risque dans le cas « USB seul » où `+5V` est absent (PO-19). R421 : 330 Ω **[Reco]**, résistance série courante sur `DIN` (300 à 500 Ω), à confirmer. Supprimés : LED2 à LED4 et R420 à R423 de la carte STM32 (repères R420, R421 réattribués).

---

## P5 — Puissance 12 V (B9, B10, B11, B12)

### Entrée et protection

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J2 | Bornier WJ2EDGRC-5.08-02P | 1 / 2 | `+12V_IN` / `GND_P` |
| F2 | BSMD1206-050-30V | 1 / 2 | `+12V_IN` / `+12V_F` |
| D2 | SMAJ15A | K / A | `+12V_F` / `GND_P` |
| U5 | LM74502DDFR | 1 EN/UVLO | `LM_EN` |
| | | 2 GND | `GND_P` |
| | | 3 NC | non connecté |
| | | 4 VCAP | `LM_VCAP` |
| | | 5 VS | `+12V_F` |
| | | 6 GATE | `LM_GATE` |
| | | 7 OV | `LM_OV` |
| | | 8 SRC | `LM_SRC` |
| R501 | 0 Ω | 1 / 2 | `+12V_F` / `LM_EN` (toujours actif) |
| C501 | 220 nF 25 V | 1 / 2 | `LM_VCAP` / `+12V_F` (VCAP vers VS) |
| C502 | 100 nF | 1 / 2 | `+12V_F` / `GND_P` |
| Q8 | AO3400A | 1 G / 2 S / 3 D | `LM_GATE` / `LM_SRC` / `+12V_F` |
| Q9 | AO3400A | 1 G / 2 S / 3 D | `LM_GATE` / `LM_SRC` / `+12V_P` |
| D8 | BZT52C10 | K / A | `LM_GATE` / `LM_SRC` |
| R502 | 95,3 kΩ (STM32 DEC-065) | 1 / 2 | `+12V_F` / `LM_OV` |
| R503 | 10 kΩ | 1 / 2 | `LM_OV` / `GND_P` |
| C503 | 10 µF 0805 25 V | 1 / 2 | `+12V_P` / `GND_P` |
| C504 | 100 nF | 1 / 2 | `+12V_P` / `GND_P` |

Q8 et Q9 en source commune (dos à dos), selon la datasheet TI LM74502.

### Pont du vérin et arrêt matériel

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| K1 pôle A | — | COM / NC / NO | `VERIN_A` / `GND_P` / `NO_FEED` |
| K2 pôle A | — | COM / NC / NO | `VERIN_B` / `+12V_P` / `GND_P` |
| J6 | Bornier WJ15EDGRC-3.81-03P | 1 | `+12V_P` |
| | | 2 | non connecté (détrompage) |
| | | 3 | `NO_FEED` |
| D17 | SMAJ15A | K / A | `NO_FEED` / `GND_P` (ESD du câble du 5e contact) |
| J3 | Bornier WJ15EDGRC-3.81-02P | 1 | `VERIN_A` (fil rouge, V+) |
| | | 2 | `VERIN_B` (fil noir, V−) |
| C505 | 100 nF 50 V | 1 / 2 | `VERIN_A` / `VERIN_B` |
| D3 | SMAJ15CA | 1 / 2 | `VERIN_A` / `VERIN_B` |

**Modifié** : sans mesure de courant (DEC-016), le shunt R504 est supprimé ; le NC de K1 et le NO de K2 vont directement à `GND_P`.

Contrôle (V6) : relais au repos → `VERIN_A` = `GND_P` ≈ 0 V, `VERIN_B` = `+12V_P` → rentrée. K1 et K2 excités, 5e contact fermé → `VERIN_A` = `+12V_P`, `VERIN_B` = `GND_P` → sortie. Le 5e contact (NC externe entre J6.1 et J6.3) ouvert coupe la sortie seule.

### Optocoupleurs

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| U6 (V12_OK) | EL357N | 1 A / 2 K | `U6_A` / `GND_P` |
| | | 4 C / 3 E | `V12_OK` / `GND` |
| R505 | 4,7 kΩ | 1 / 2 | `+12V_P` / `U6_A` (≈ 2,3 mA à 12 V) |
| R506 | 10 kΩ | 1 / 2 | `V12_OK` / `+3V3` |
| C506 | 10 nF | 1 / 2 | `V12_OK` / `GND` |
| U7 (STOP_HW_FB) | EL357N | 1 A / 2 K | `U7_A` / `GND_P` |
| | | 4 C / 3 E | `STOP_HW_FB` / `GND` |
| R507 | 4,7 kΩ | 1 / 2 | `NO_FEED` / `U7_A` |
| R508 | 10 kΩ | 1 / 2 | `STOP_HW_FB` / `+3V3` |
| C507 | 10 nF | 1 / 2 | `STOP_HW_FB` / `GND` |

Logique : 12 V présent → `V12_OK` bas. 5e contact fermé (hors butée) et 12 V présent → `STOP_HW_FB` bas. Brochage EL357N (1 A, 2 K, 3 E, 4 C) : à confirmer sur le symbole LCSC C29981 (AV).

## 6. Points d'arrêt pour la saisie

| N° | Point | Effet sur la saisie |
|---|---|---|
| PO-21 | Écart entre les rangées de la carte de dev (25,40 mm supposé) | Bloque le placement, pas le schéma |
| PO-02 | Barrettes femelles 1×22 : référence et compatibilité avec les broches carrées de la carte de dev | Code saisi AV |
| STM32 PO-23 | Valeur des rappels des fins de course | R311 à R318 saisies 1 kΩ, comme sur la carte STM32 |
| — | Codes LCSC AV (U17, LED1, J20, J21) | Confirmés au devis JLCPCB |
