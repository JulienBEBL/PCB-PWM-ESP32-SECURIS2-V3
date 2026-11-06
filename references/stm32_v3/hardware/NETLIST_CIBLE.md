# Netlist cible — PCB_PWM_SECURISE_V3

**Phase :** 2 (`CONSIGNES_EASYEDA_CLAUDE_CODE.md` §3). **Jalon :** G2, validation de Julien requise avant la saisie.
**Version :** v1.3, session 13 (2026-11-04) : borniers enfichables J1 à J7, barrettes Dupont J13 à J15, D17, LED5, R426 (DEC-071, DEC-073, DEC-074) ; même brochage que v1.2. v1.2 (2026-11-03) : diodes idéales LM66100 (U15, U16) à la place de D15 / D16 (DEC-067) ; brochage des relais K1 / K2 lu sur le symbole LCSC (PO-36). v1.1 : USB (page P6), boutons, J10 sur USART2. v1 : session 12, G1 et G2 validés (DEC-064).
**Composants :** `hardware/BOM_CIBLE.md`. **Nets du MCU :** `hardware/brochage_stm32g071cbt6.csv` (brochage v2).

## Pages du schéma (renommées le 2026-11-04)

| Nom dans ce document | Nom dans EasyEDA | Contenu |
|---|---|---|
| P1 | 01_ALIM_5V_3V3 | Alimentation 5 V / 3,3 V, J1, J13, LED5 |
| P2 | 02_MCU | STM32, quartz, RESET / BOOT, SWD, UART2 |
| P3 | 03_ENTREE_PWM | Entrée PWM |
| P6 | 04_USB_PC | USB-C, CH340C |
| P3B | 05_FINS_DE_COURSE | J4, J5, J14, J15 |
| P3C | 06_EXTENSION_MAINTENANCE | J8, J11, révision |
| P4 | 07_RELAIS_SECURITES | K1, K2, pompe, watchdog, J7 |
| P4B | 08_IHM_STATUS | LED, buzzer, STATUS |
| P5 | 09_PUISSANCE_12V | 12 V, pont du vérin, optos |
| P5B | 10_MESURE_COURANT | Mesure isolée du courant |

Chaque page porte un texte explicatif en bleu.

## 0. Conventions

| Sujet | Règle |
|---|---|
| Domaine commande | `GND`, `+5V`, `+3V3` |
| Domaine puissance (isolé) | `GND_P`, `+12V_IN`, `+12V_F`, `+12V_P`, `+5V_ISO` |
| Interdit | Tout composant reliant un net du domaine commande à un net du domaine puissance, sauf K1, K2, U6, U7, U9 (contrôle V4) |
| Nets MCU | Noms du CSV, inchangés (S7) |
| Nets côté connecteur | Suffixe `X` pour le nœud situé avant la résistance série (exemple : `FCX_R1_NO` côté connecteur, `FC_R1_NO` côté MCU) |
| Broches | Numéro physique quand il est connu et vérifié. `fonction` quand le numéro doit être lu sur le symbole LCSC (AV) |
| Valeurs | Code LCSC dans `BOM_CIBLE.md` §3 ; `A_DEFINIR` = décision en attente (PO cité) |
| Non montés | Marqués **NM** (empreinte posée, composant non assemblé) |

---

## P1 — Alimentation commande (B1, B2, X01)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J1 | Bornier WJ15EDGRC-3.81-04P | 1 | `+5V_IN` |
| | | 2 | `GND` |
| | | 3 | `PWM_EXT` (→ P3) |
| | | 4 | `STATUS_EXT` (→ P4) |
| F1 | BSMD1206-050-30V | 1 / 2 | `+5V_IN` / `+5V_F` |
| D1 | SMAJ5.0A | K / A | `+5V_F` / `GND` |
| U15 | LM66100DCKR (diode idéale, J1) | 1 VIN / 2 GND / 3 CE# / 4 NC / 5 ST / 6 VOUT | `+5V_F` / `GND` / `+5V` / non connecté / `GND` / `+5V` |
| C107 | 1 µF | 1 / 2 | `+5V_F` / `GND` (entrée U15) |
| C101 | 10 µF 0805 | 1 / 2 | `+5V` / `GND` |
| C102 | 100 nF | 1 / 2 | `+5V` / `GND` |
| U2 | ME6211C33M5G-N | 1 VIN | `+5V` |
| | | 2 VSS | `GND` |
| | | 3 CE | `+5V` |
| | | 4 NC | non connecté |
| | | 5 VOUT | `+3V3` |
| C103 | 1 µF | 1 / 2 | `+5V` / `GND` |
| C104 | 4,7 µF | 1 / 2 | `+3V3` / `GND` |
| C105 | 100 nF | 1 / 2 | `+3V3` / `GND` |
| J13 | Barrette 1×4 2,54 (Dupont, parallèle de J1) | 1 / 2 / 3 / 4 | `+5V_IN` / `GND` / `PWM_EXT` / `STATUS_EXT` |
| R426 | 680 Ω | 1 / 2 | `+3V3` / `LED5_A` |
| LED5 | NCD0603C1 (3,3 V présent) | A / K | `LED5_A` / `GND` |
| R102 | 10 kΩ | 1 / 2 | `+5V` / `V5_MON` |
| R103 | 10 kΩ | 1 / 2 | `V5_MON` / `GND` |
| C106 | 100 nF | 1 / 2 | `V5_MON` / `GND` |
| TP101, TP102, TP103 | Points de test | — | `+5V`, `+3V3`, `GND` |

Notes : U15 et U16 en « blocage permanent du courant inverse » (CE# relié à VOUT, datasheet TI SLVSEZ8A §8.3.2) : chacun conduit quand son entrée dépasse `+5V` et bloque tout retour. Le montage « OU » à CE croisés (§9.2.1) est écarté : avec deux entrées égales, les deux puces se coupent. Inversion de J1 : U15 bloque (VIN jusqu'à −6 V). Chute ≈ 16 mV à 200 mA (79 mΩ) : `+5V` ≈ 5,0 V. `V5_MON` = `+5V` / 2. Limite : VIN ≤ 6 V absolu.

---

## P2 — Microcontrôleur (B3, J9, J10)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| U1 | STM32G071CBT6 | 1 à 48 | Selon `brochage_stm32g071cbt6.csv`, sauf alimentations ci-dessous |
| | | 4 VBAT | `+3V3` |
| | | 5 VREF+ | `+3V3` |
| | | 6 VDD/VDDA | `+3V3` |
| | | 7 VSS/VSSA | `GND` |
| | | 10 PF2-NRST | `NRST` |
| | | 13 PA2, 14 PA3 | `UART2_TX`, `UART2_RX` (→ R203, R204, J10) |
| | | 25 PB13, 39 PD1, 41 PD3 | non connecté (drapeau explicite) |
| C201 | 100 nF | 1 / 2 | `+3V3` / `GND` (au plus près de la broche 6) |
| C202 | 4,7 µF | 1 / 2 | `+3V3` / `GND` (broche 6) |
| C203 | 100 nF | 1 / 2 | `+3V3` / `GND` (broche 5, VREF+) |
| C204 | 100 nF | 1 / 2 | `+3V3` / `GND` (broche 4, VBAT) |
| C205 | 100 nF | 1 / 2 | `NRST` / `GND` (au plus près de la broche 10) |
| Y1 | X322512MSB4SI 12 MHz | 1 | `OSC_IN` |
| | | 3 | `OSC_OUT` |
| | | 2, 4 | `GND` |
| C206 | 33 pF C0G | 1 / 2 | `OSC_IN` / `GND` |
| C207 | 33 pF C0G | 1 / 2 | `OSC_OUT` / `GND` |
| J9 | PZ254V-11-05P (SWD) | 1 | `+3V3` |
| | | 2 | `SWDIO` |
| | | 3 | `SWCLK` |
| | | 4 | `NRST` |
| | | 5 | `GND` |
| J10 | PZ254V-11-03P (UART2, atelier) | 1 / 2 / 3 | `UART2X_TX` / `UART2X_RX` / `GND` |
| R203 | 1 kΩ | 1 / 2 | `UART2_TX` / `UART2X_TX` |
| R204 | 1 kΩ | 1 / 2 | `UART2_RX` / `UART2X_RX` |
| SW1 | TS-1187A (RESET) | contacts A / B | `NRST` / `GND` |
| SW2 | TS-1187A (BOOT) | contacts A / B | `BOOT_SW` / `+3V3` |
| R201 | 1 kΩ | 1 / 2 | `BOOT_SW` / `SWCLK` (limite le courant si la sonde SWD tire SWCLK au niveau bas) |
| R202 | 10 kΩ | 1 / 2 | `SWCLK` / `GND` (BOOT0 = 0 au reset : démarrage sur la flash) |

Programmation (`hardware/PROCEDURE_PROGRAMMATION.md`) : BOOT0 = PA14, lue au reset seulement si l'octet d'option `nBOOT_SEL` = 0 (réglage unique au premier flash). SW2 maintenu + appui sur SW1 = entrée en bootloader. J9 (SWD) reste le recours.

Brochage du quartz 3225-4P (1 et 3 = quartz, 2 et 4 = boîtier) : à confirmer sur le symbole LCSC C9002 (AV).

---

## P3 — Entrées (B4, B5, B6, X03, X04)

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
| R303 | 33 Ω | 1 / 2 | `PWM_BUF` / `PWM_IN` |

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

### Extension (4 voies, PO-07)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J8 | Barrette 1×6 | 1 | `+3V3_EXT` |
| | | 2 / 3 / 4 / 5 | `EXTX_1` / `EXTX_2` / `EXTX_3` / `EXTX_4` |
| | | 6 | `GND` |
| R331 | 0 Ω, `A_DEFINIR` (PO-07) | 1 / 2 | `+3V3` / `+3V3_EXT` |

Par voie n = 1 à 4 :

| Repères | Valeur | Broches → nets |
|---|---|---|
| D10+n (D11 à D14) | ESD5Z5V0 | K `EXTX_n`, A `GND` |
| R332 à R335 | 4,7 kΩ | `EXTX_n` → `EXT_INn` (limite à ≈ 0,3 mA le courant d'injection d'un signal 5 V : PA4 / PA5 sont TT_a) |
| C331 à C334 | 10 nF | `EXT_INn` → `GND` |
| R336 à R339 | 10 kΩ **NM** | `EXTX_n` → `+3V3` (rappel haut optionnel) |
| R340 à R343 | 10 kΩ **NM** | `EXTX_n` → `GND` (rappel bas optionnel) |

### Mode maintenance (X03) et révision de carte (X04)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J11 | Barrette 1×2 | 1 / 2 | `MAINTX` / `GND` |
| R351 | 10 kΩ | 1 / 2 | `MAINTX` / `+3V3` |
| R352 | 1 kΩ | 1 / 2 | `MAINTX` / `MAINT_IN` |
| R353 | 10 kΩ | 1 / 2 | `+3V3` / `BOARD_ID` |
| R354 | 10 kΩ (révision A → 1,65 V) | 1 / 2 | `BOARD_ID` / `GND` |
| C351 | 100 nF | 1 / 2 | `BOARD_ID` / `GND` |

---

## P4 — Commande des relais et IHM (B7, B13, X02)

### Chaîne K1 (pompe de charge)

Brochage G6K-2F-Y (symbole LCSC C47190, image relue) : bobine 1 (+) – 8 (−), **bobine polarisée** (datasheet Omron K106 : « Check carefully the coil polarity »). Pôle bas : COM 3, NC 2, NO 4. Pôle haut : COM 6, NC 7, NO 5. Pôle bas côté commande (rangée 1-2-3-4, avec la bobine +), pôle haut côté puissance. **Confirmé** sur le dessin « G6K-2F-Y Terminal Arrangement/Internal Connections (TOP VIEW) » de la datasheet Omron K106 (session 13).

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| K1 | G6K-2F-Y DC5 | 1 bobine (+) | `+5V` |
| | | 8 bobine (−) | `K1_COIL_N` |
| | | pôle B (commande) : 3 COM / 4 NO / 2 NC | `GND` / `K1_FBX` / non connecté |
| | | pôle A (puissance) : 6 COM / 5 NO / 7 NC | `VERIN_A` / `NO_FEED` / `MOT_RET` (→ P5) |
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

Pompe : `CP_DRV` carré 0 / 3,3 V à 10 kHz (TIM17). `CP_OUT` ≈ 3,3 V − 2 × VF(BAT54S) ≈ 2,7 V, au-dessus de VGS(th) max de Q3 (1,45 V) ; RDS(on) de Q3 spécifié à 2,5 V. Signal figé (haut ou bas) → `CP_OUT` décroît avec τ = 10 ms → Q3 bloqué → K1 retombe.

### Chaîne K2 (cavalier + watchdog externe)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J7 | Bornier WJ2EDGRC-5.08-03P-14-00A, C8377 (armement : pont de fil 1-3 dans la fiche) | 1 / 2 / 3 | `+5V` / non connecté / `K2_ARM` |
| K2 | G6K-2F-Y DC5 | 1 bobine (+) | `K2_ARM` |
| | | 8 bobine (−) | `K2_COIL_N` |
| | | pôle B (commande) : 3 COM / 4 NO / 2 NC | `GND` / `K2_FBX` / non connecté |
| | | pôle A (puissance) : 6 COM / 5 NO / 7 NC | `VERIN_B` / `MOT_RET` / `+12V_P` (→ P5) |
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
| R411 | 0 Ω **NM** | 1 / 2 | `WD_OK` / `NRST` (option : défaut watchdog → reset MCU) |
| R412 | 10 kΩ | 1 / 2 | `K2_ARM` / `ARM_FB` |
| R413 | 20 kΩ | 1 / 2 | `ARM_FB` / `GND` (5 V → 3,33 V) |
| C405 | 10 nF | 1 / 2 | `ARM_FB` / `GND` |
| R414 | 10 kΩ | 1 / 2 | `K2_FBX` / `+3V3` |
| R415 | 1 kΩ | 1 / 2 | `K2_FBX` / `K2_FB` |
| C406 | 10 nF | 1 / 2 | `K2_FB` / `GND` |

Watchdog (CWD non connecté, SET0 = SET1 = 0, datasheet TI tableau 8-3) : fenêtre fermée tWDL 19,1 à 25,9 ms, ouverture tWDU 46,8 à 63,3 ms. **Le firmware doit produire un front descendant sur `WDT_KICK` toutes les 36 ms ± 5 ms.** Hors fenêtre : `WD_OK` bas pendant 200 ms → Q5 bloqué → K2 retombe.

Relecture : contact pôle B fermé (relais excité) → `K1_FB` / `K2_FB` bas. Cavalier présent → `ARM_FB` haut.

### STATUS_OUT (X02)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| Q2 | AO3400A | 1 G / 2 S / 3 D | `Q2_G` / `GND` / `STATUS_D` |
| R416 | 100 Ω | 1 / 2 | `STATUS_OUT` / `Q2_G` |
| R417 | 100 kΩ | 1 / 2 | `Q2_G` / `GND` |
| R418 | 100 Ω | 1 / 2 | `STATUS_D` / `STATUS_EXT` |
| D10 | ESD5Z5V0 | K / A | `STATUS_EXT` / `GND` |

Sortie à drain ouvert : rappel haut fourni par le contrôleur externe. MCU arrêté → Q2 bloqué → ligne haute. **Convention proposée : ligne basse = carte prête ; ligne haute = non prête ou défaut** (sûre par défaut). À valider avec la FSM (PO-05).

### LED et buzzer

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| R420 / LED1 | 680 Ω / NCD0603C1 | R : `LED1` → `LED1_A` ; LED : A `LED1_A`, K `GND` | — |
| R421 / LED2 | 680 Ω / C72038 | `LED2` → `LED2_A` ; A `LED2_A`, K `GND` | — |
| R422 / LED3 | 680 Ω / KT-0603R | `LED3` → `LED3_A` ; A `LED3_A`, K `GND` | — |
| R423 / LED4 | 680 Ω / KT-0603R | `LED4` → `LED4_A` ; A `LED4_A`, K `GND` | — |
| LS1 | MLT-8530 | + / − | `+3V3` / `BUZ_N` (polarité à confirmer sur le symbole : AV) |
| D6 | 1N4148W | K / A | `+3V3` / `BUZ_N` |
| Q7 | AO3400A | 1 G / 2 S / 3 D | `BUZ_G` / `GND` / `BUZ_N` |
| R424 | 100 Ω | 1 / 2 | `BUZZER` / `BUZ_G` |
| R425 | 100 kΩ | 1 / 2 | `BUZ_G` / `GND` |
| C407 | 10 µF 0805 | 1 / 2 | `+3V3` / `GND` (au plus près de LS1) |

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
| R501 | 0 Ω | 1 / 2 | `+12V_F` / `LM_EN` (toujours actif ; empreinte réservée pour PO-35) |
| C501 | 220 nF 25 V | 1 / 2 | `LM_VCAP` / `+12V_F` (VCAP vers VS) |
| C502 | 100 nF | 1 / 2 | `+12V_F` / `GND_P` |
| Q8 | AO3400A | 1 G / 2 S / 3 D | `LM_GATE` / `LM_SRC` / `+12V_F` |
| Q9 | AO3400A | 1 G / 2 S / 3 D | `LM_GATE` / `LM_SRC` / `+12V_P` |
| D8 | BZT52C10 | K / A | `LM_GATE` / `LM_SRC` |
| R502 | 95,3 kΩ, `A_DEFINIR` (PO-35) | 1 / 2 | `+12V_F` / `LM_OV` |
| R503 | 10 kΩ | 1 / 2 | `LM_OV` / `GND_P` |
| C503 | 10 µF 0805 25 V | 1 / 2 | `+12V_P` / `GND_P` |
| C504 | 100 nF | 1 / 2 | `+12V_P` / `GND_P` |

Q8 et Q9 en source commune (dos à dos), selon la datasheet TI LM74502.

### Pont du vérin et arrêt matériel

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| K1 pôle A | — | COM / NC / NO | `VERIN_A` / `MOT_RET` / `NO_FEED` |
| K2 pôle A | — | COM / NC / NO | `VERIN_B` / `+12V_P` / `MOT_RET` |
| J6 | Bornier WJ15EDGRC-3.81-03P | 1 | `+12V_P` |
| | | 2 | non connecté (détrompage) |
| | | 3 | `NO_FEED` |
| D17 | SMAJ15A | K / A | `NO_FEED` / `GND_P` (ESD du câble du 5e contact) |
| J3 | Bornier WJ15EDGRC-3.81-02P | 1 | `VERIN_A` (fil rouge, V+) |
| | | 2 | `VERIN_B` (fil noir, V−) |
| C505 | 100 nF 50 V | 1 / 2 | `VERIN_A` / `VERIN_B` |
| D3 | SMAJ15CA | 1 / 2 | `VERIN_A` / `VERIN_B` |
| R504 | 1 Ω, 1 %, 1206, `A_DEFINIR` (PO-27) | 1 / 2 | `MOT_RET` / `GND_P` (prises Kelvin vers U9) |

Contrôle (V6) : relais au repos → `VERIN_A` = `MOT_RET` ≈ 0 V, `VERIN_B` = `+12V_P` → rentrée. K1 et K2 excités, 5e contact fermé → `VERIN_A` = `+12V_P`, `VERIN_B` = `MOT_RET` → sortie. Le 5e contact (NC externe entre J6.1 et J6.3) ouvert coupe la sortie seule.

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

### Mesure de courant isolée (X05, option A de PO-27)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| U8 | HT7550-1 | 1 GND / 2 VIN / 3 VOUT | `GND_P` / `+12V_P` / `+5V_ISO` |
| C508 | 1 µF 50 V | 1 / 2 | `+12V_P` / `GND_P` |
| C509 | 10 µF 0805 | 1 / 2 | `+5V_ISO` / `GND_P` |
| U9 | AMC1301DWVR | 1 VDD1 | `+5V_ISO` |
| | | 2 INP | `AMC_INP` |
| | | 3 INN | `AMC_INN` |
| | | 4 GND1 | `GND_P` |
| | | 5 GND2 | `GND` |
| | | 6 OUTN | `AMC_OUTN` |
| | | 7 OUTP | `AMC_OUTP` |
| | | 8 VDD2 | `+3V3` |
| C510 / C511 | 100 nF / 1 µF | 1 / 2 | `+5V_ISO` / `GND_P` (VDD1) |
| C512 / C513 | 100 nF / 1 µF | 1 / 2 | `+3V3` / `GND` (VDD2) |
| R509 | 10 Ω | 1 / 2 | `MOT_RET` (prise Kelvin sur R504) / `AMC_INP` |
| R510 | 10 Ω | 1 / 2 | `GND_P` (prise Kelvin sur R504) / `AMC_INN` |
| C514 | 10 nF | 1 / 2 | `AMC_INP` / `AMC_INN` |
| U10 | TLV9001IDBVR | 1 OUT / 2 V− / 3 IN+ / 4 IN− / 5 V+ | `IMON_OUT` / `GND` / `DIFF_P` / `DIFF_N` / `+3V3` |
| C515 | 100 nF | 1 / 2 | `+3V3` / `GND` |
| R511 | 10 kΩ | 1 / 2 | `AMC_OUTP` / `DIFF_P` |
| R512 | 10 kΩ | 1 / 2 | `DIFF_P` / `GND` |
| R513 | 10 kΩ | 1 / 2 | `AMC_OUTN` / `DIFF_N` |
| R514 | 10 kΩ | 1 / 2 | `DIFF_N` / `IMON_OUT` |
| R515 | 100 Ω | 1 / 2 | `IMON_OUT` / `I_MON` |
| C516 | 10 nF | 1 / 2 | `I_MON` / `GND` |

Échelle : 200 mA × 1 Ω × 8,2 = 1,64 V en sortie (gain 1 de l'amplificateur différentiel). Brochages AMC1301 (DWV), TLV9001 (DBV), HT7550-1 (SOT-89) : à confirmer sur les symboles LCSC (AV). Valeurs du filtre d'entrée (R509, R510, C514) : à confirmer avec la section « Application » de la datasheet AMC1301 (AV).

---

## P6 — USB vers PC (programmation, moniteur série)

| Repère | Valeur / réf. | Broche | Net |
|---|---|---|---|
| J12 | TYPE-C-31-M-12 (USB-C 16P) | VBUS (×2) | `VBUS` |
| | | GND (×2), blindage | `GND` |
| | | CC1 / CC2 | `CC1` / `CC2` |
| | | D+ (×2) / D− (×2) | `USB_DP` / `USB_DN` |
| | | SBU1, SBU2 | non connecté |
| R601 | 5,1 kΩ | 1 / 2 | `CC1` / `GND` (puits USB-C, 5 V par défaut) |
| R602 | 5,1 kΩ | 1 / 2 | `CC2` / `GND` |
| U14 | USBLC6-2SC6 | I/O1 (1, 6) / I/O2 (3, 4) | `USB_DP` / `USB_DN` |
| | | VBUS (5) / GND (2) | `VBUS` / `GND` |
| U16 | LM66100DCKR (diode idéale, USB) | 1 VIN / 2 GND / 3 CE# / 4 NC / 5 ST / 6 VOUT | `VBUS` / `GND` / `+5V` / non connecté / `GND` / `+5V` |
| C602 | 1 µF | 1 / 2 | `VBUS` / `GND` (entrée U16) |
| U13 | CH340C | VCC, V3 | `+3V3` (alimentation 3,3 V : V3 reliée à VCC) |
| | | GND | `GND` |
| | | UD+ / UD− | `USB_DP` / `USB_DN` |
| | | TXD / RXD | `UART_RX` / `UART_TX` (croisement vers PA10 / PA9) |
| | | CTS#, DSR#, RI#, DCD#, DTR#, RTS#, R232, NC | non connecté |
| C601 | 100 nF | 1 / 2 | `+3V3` / `GND` (au plus près de U13) |

Notes : U13 est alimenté par `+3V3` de la carte, jamais par `VBUS` : pas d'alimentation fantôme du MCU par ses broches UART, niveaux 3,3 V. Pas de reset automatique (DTR / RTS non câblés) : le téléversement série de STM32duino ne pilote aucune séquence de reset (`stm32CubeProg.sh`), et l'ouverture du moniteur série ne réinitialise pas la carte.

---

## 6. Points d'arrêt pour la saisie

| N° | Point | Effet sur la saisie |
|---|---|---|
| PO-35 | Précision de la coupure en surtension 12 V | Clos (DEC-065) : R502 = 95,3 kΩ, code LCSC AV |
| PO-36 | Brochage du relais G6K-2F-Y | Lu sur le symbole LCSC (v1.2) ; confirmation sur la datasheet Omron demandée à Julien |
| PO-27 | Option A de mesure de courant | Page P5, sous-bloc I_MON saisi tel quel, R504 `A_DEFINIR` |
| PO-23 | Valeur des rappels des fins de course | R311 à R318 saisies 1 kΩ `A_DEFINIR` |
| PO-12 | LED, buzzer | Références saisies, couleurs à confirmer |
| PO-15 | Série des connecteurs | Références saisies, à confirmer avec la mécanique |
| — | Codes des barrettes et du shunt | Résolus en v1.2 : J7, J11 C492401 ; J8 C492405 ; J10 C492402 ; R504 C17928 (1 Ω ±1 %, 250 mW) ; R502 C23267 (95,3 kΩ ±1 %) |
