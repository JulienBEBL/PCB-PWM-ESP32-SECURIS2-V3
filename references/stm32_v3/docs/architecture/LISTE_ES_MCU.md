# Liste des éléments raccordés au MCU

**Statut :** version 1 pour revue par Julien (session 06). Affectation des broches : `PLAN_BROCHAGE_MCU.md`.
**MCU :** STM32G071CBT6, LQFP48, 44 GPIO (DEC-036).

Chaque ligne décrit un signal entre le MCU et son environnement. La colonne « Besoin MCU » indique le périphérique interne requis : elle contraint l'affectation des broches.

## 1. Signaux retenus

### Entrées

| Repère | Signal | Élément raccordé | Besoin MCU | Conditionnement sur la carte | Source |
|---|---|---|---|---|---|
| E01 | `PWM_IN` | Consigne PWM du contrôleur externe | Canal 1 ou 2 d'un timer (mode « PWM input ») | Protection ESD, résistance série, rappel à la masse, buffer à trigger de Schmitt compatible 3,3 / 5 V | DEC-030 |
| E02 | `FC_R1_NO` | Fin de course « rentré » n° 1, sortie NO | GPIO, interruption | Rappel au 3,3 V, filtre RC, protection ESD | DEC-037 |
| E03 | `FC_R1_NC` | Fin de course « rentré » n° 1, sortie NC | GPIO, interruption | Idem | DEC-037 |
| E04 | `FC_R2_NO` | Fin de course « rentré » n° 2, sortie NO | GPIO, interruption | Idem | DEC-037 |
| E05 | `FC_R2_NC` | Fin de course « rentré » n° 2, sortie NC | GPIO, interruption | Idem | DEC-037 |
| E06 | `FC_S1_NO` | Fin de course « sorti » n° 1, sortie NO | GPIO, interruption | Idem | DEC-037 |
| E07 | `FC_S1_NC` | Fin de course « sorti » n° 1, sortie NC | GPIO, interruption | Idem | DEC-037 |
| E08 | `FC_S2_NO` | Fin de course « sorti » n° 2, sortie NO | GPIO, interruption | Idem | DEC-037 |
| E09 | `FC_S2_NC` | Fin de course « sorti » n° 2, sortie NC | GPIO, interruption | Idem | DEC-037 |
| E10 | `EXT_IN1` | Capteur d'extension n° 1 | GPIO **et** entrée ADC (type de capteur inconnu) | Protection, rappel configurable par résistance non montée | PO-07 |
| E11 | `EXT_IN2` | Capteur d'extension n° 2 | GPIO et ADC | Idem | PO-07 |
| E12 | `EXT_IN3` | Capteur d'extension n° 3 | GPIO et ADC | Idem | PO-07 |
| E13 | `EXT_IN4` | Capteur d'extension n° 4 | GPIO et ADC | Idem | PO-07 |
| E14 | `V12_OK` | Présence du 12 V | GPIO | Optocoupleur (LED côté 12 V, transistor côté 3,3 V) | DEC-024 |
| E15 | `K1_FB` | Relecture du relais K1 (2e contact) | GPIO | Rappel au 3,3 V, filtre RC | DEC-023 |
| E16 | `K2_FB` | Relecture du relais K2 (2e contact) | GPIO | Idem | DEC-023 |
| E17 | `ARM_FB` | Relecture du cavalier d'armement | GPIO | À définir avec le schéma de la chaîne K2 | DEC-025 |
| E18 | `STOP_HW_FB` | Relecture de l'arrêt matériel en position d'action | GPIO | Optocoupleur (contact côté 12 V) | DEC-029, optionnel |

### Sorties

| Repère | Signal | Élément raccordé | Besoin MCU | Conditionnement sur la carte | Source |
|---|---|---|---|---|---|
| S01 | `K1_CMD` | Bobine du relais K1 | GPIO | Transistor, rappel à la masse (relais au repos pendant le reset), diode de roue libre | DEC-022 |
| S02 | `K2_CMD` | Bobine du relais K2 | GPIO | Idem, en série avec le cavalier | DEC-022, DEC-025 |
| S03 | `CP_DRV` | Pompe de charge de validation de K1 | Sortie de timer (signal carré permanent) | Condensateurs, diodes, transistor de validation | `ARCHITECTURE_COMMANDE_VERIN.md` §5 |
| S04 | `WDT_KICK` | Watchdog externe de validation de K2 | GPIO | Circuit superviseur externe | Idem |
| S05 | `LED1` | LED d'état n° 1 | GPIO | Résistance série | DEC-033 |
| S06 | `LED2` | LED d'état n° 2 | GPIO | Idem | DEC-033 |
| S07 | `LED3` | LED d'état n° 3 | GPIO | Idem | DEC-033 |
| S08 | `LED4` | LED d'état n° 4 | GPIO | Idem | DEC-033, PO-12 (3 ou 4) |
| S09 | `BUZZER` | Buzzer | Sortie de timer (buzzer passif) ou GPIO (buzzer actif) | Transistor | DEC-033, PO-12 |

### Communication et programmation

| Repère | Signal | Élément raccordé | Besoin MCU | Remarque |
|---|---|---|---|---|
| C01 | `UART_TX` | Connecteur de diagnostic | USART1 TX (PA9) | Broches du bootloader ROM : à conserver |
| C02 | `UART_RX` | Connecteur de diagnostic | USART1 RX (PA10) | Idem |
| C03 | `SWDIO` | Connecteur SWD | PA13 (fixe) | — |
| C04 | `SWCLK` | Connecteur SWD | PA14 (fixe, partagée avec BOOT0) | — |

### Broches système (hors GPIO fonctionnelles)

| Repère | Broche | Usage |
|---|---|---|
| Y01 | `PF0-OSC_IN` | Quartz HSE (empreinte prévue, montage facultatif) |
| Y02 | `PF1-OSC_OUT` | Idem |
| P01 | `NRST` | Reset : condensateur, connecteur SWD, sortie éventuelle d'un superviseur |
| P02 | `VDD/VDDA`, `VSS/VSSA` | Alimentation 3,3 V et découplage |
| P03 | `VREF+` | Référence ADC, reliée au 3,3 V avec découplage |
| P04 | `VBAT` | Reliée au 3,3 V |

## 2. Bilan

| Catégorie | Nombre de GPIO |
|---|---|
| Entrées (E01–E18) | 18 |
| Sorties (S01–S09) | 9 |
| Communication et débogage (C01–C04) | 4 |
| Quartz (Y01–Y02) | 2 |
| **Total** | **33** |
| Disponibles (LQFP48) | 44, dont PF2 utilisée en NRST, soit 43 |
| **Réserve** | **10** (correction session 07 : PF2 sert de NRST) |

## 3. Éléments proposés, non retenus (à valider par Julien)

| Repère | Proposition | Intérêt | Coût en GPIO |
|---|---|---|---|
| X01 | Mesure de la tension 5 V d'entrée (diviseur vers l'ADC) | Détecter une alimentation faible ou instable avant le reset, journaliser le défaut | 1 (ADC) |
| X02 | Sortie d'état vers le contrôleur externe (prêt / défaut) | Le contrôleur sait si la carte est opérationnelle sans lire l'UART | 1 |
| X03 | Entrée de mode maintenance (cavalier ou bouton) | Mode banc : mouvements manuels, essais, sans dépendre du PWM | 1 |
| X04 | Identification de révision de carte (1 broche ADC sur pont diviseur, modifié session 07) | Le firmware vérifie qu'il tourne sur la bonne révision | 1 (ADC) |
| X05 | Mesure du courant du vérin, isolée | Détection de blocage, de vérin débranché ou de court-circuit (PO-14) | 1 (ADC ou numérique) |
| X06 | Température de la carte | Déjà disponible : capteur interne du MCU, lu par l'ADC | 0 |
| X07 | Surveillance du 3,3 V | Déjà disponible : référence interne VREFINT | 0 |
| X08 | Points de test sur chaque signal critique | Mesure sur banc, diagnostic | 0 |

## 4. Points d'attention relevés en construisant la liste

| Point | Constat | Action |
|---|---|---|
| Courant minimal des contacts | Les fins de course sont lus « à sec » à quelques mA. Les contacts en argent des microrupteurs classiques peuvent devenir peu fiables à faible courant (oxydation, absence d'arc de nettoyage). | Lire le courant minimal de commutation dans la datasheet des contacts. Choisir un courant de rappel compatible, ou des contacts dorés. PO-23 |
| Câblage des fins de course | Lecture NO + NC d'un même contact : 3 fils par contact (COM, NO, NC), soit 12 fils pour 4 contacts, plus le 5e contact d'arrêt matériel côté 12 V. | Connecteurs à dimensionner au schéma |
| Entrées d'extension | Type inconnu (PO-07). Les broches réservées doivent accepter un usage numérique et analogique. | Affectation sur des broches ADC |
