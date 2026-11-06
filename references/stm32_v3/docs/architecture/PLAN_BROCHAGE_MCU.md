# Plan de brochage du STM32G071CBT6 (LQFP48)

**Statut :** version 2 (session 10). Remplace la v1 : 7 signaux déplacés suite à la DS12232 Rev 5 (règle R9, §5).
**Sources :** base officielle ST « STM32_open_pin_data », fichier `STM32G071C(6-8-B)Tx.xml` ; datasheet DS12232 **Rev 5 (juin 2025)** ; feuille d'errata ES0418 Rev 6.
**Données d'entrée :** `LISTE_ES_MCU.md` v1 validée, X01 à X08 retenus (DEC-039), quartz HSE monté (DEC-060).

## 1. Règles d'affectation

| N° | Règle | Raison |
|---|---|---|
| R1 | SWD (PA13, PA14), USART1 du bootloader (PA9, PA10), NRST (PF2) et quartz HSE (PF0, PF1) à leurs broches fixes | Imposé par le MCU |
| R2 | Entrées d'extension et mesures analogiques sur des broches ADC | Type de capteur inconnu (PO-07) |
| R3 | Les 8 entrées de fin de course sur 8 numéros de broche différents | Une ligne d'interruption EXTI par numéro de broche, tous ports confondus |
| R4 | Les deux contacts redondants d'une même extrémité sur deux ports différents | Une erreur d'accès à un port ne fausse pas les deux lectures redondantes |
| R5 | `K1_CMD` et `K2_CMD` sur deux ports différents | Une écriture erronée sur un registre de port ne commande pas les deux relais à la fois |
| R6 | Aucune commande de relais sur une broche de l'interface USB-C (UCPD) | §4, point 1 |
| R7 | Un timer distinct par fonction temporelle : PWM (TIM3), pompe de charge (TIM17), buzzer (TIM16) | Pas de dépendance de configuration entre fonctions |
| R8 | USART2 (PA2, PA3) affecté à J10 (liaison atelier) ; USART1 (PA9, PA10) au pont USB U13 (v2.1, session 13) | Programmation et moniteur série par USB, seconde liaison série sur J10 |
| **R9** | **Aucune sortie critique ni sortie vers l'extérieur sur une broche que le bootloader ROM peut configurer** | DS12232 Rev 5 §3.5 : le bootloader sort certaines GPIO de leur état haute impédance. Il s'exécute notamment au premier démarrage d'un MCU neuf (flash vide). Voir §5 |

Broches que le bootloader peut configurer (DS12232 Rev 5 §3.5) : PA9 / PA10, PA2 / PA3 (USART) ; PB6 / PB7, PB10 / PB11 (I2C) ; PA4 à PA7, PB12 à PB15 (SPI). PC10 / PC11 n'existent pas en LQFP48.

## 2. Affectation par fonction

| Repère | Signal | Broche | N° | Fonction MCU utilisée | Remarque |
|---|---|---|---|---|---|
| E01 | `PWM_IN` | PC6 | 30 | TIM3_CH1, mode « PWM input » | |
| E02 | `FC_R1_NO` | PB3 | 42 | GPIO, EXTI3 | Rentré n° 1, port B |
| E03 | `FC_R1_NC` | PB4 | 43 | GPIO, EXTI4 | |
| E04 | `FC_R2_NO` | PC13 | 1 | GPIO, EXTI13 | Rentré n° 2, port C (R4). Domaine de sauvegarde : entrée seulement |
| E05 | `FC_R2_NC` | PC14 | 2 | GPIO, EXTI14 | Domaine de sauvegarde : entrée seulement |
| E06 | `FC_S1_NO` | PB5 | 44 | GPIO, EXTI5 | Sorti n° 1, port B |
| E07 | `FC_S1_NC` | PB6 | 45 | GPIO, EXTI6 | Broche bootloader : entrée, sans risque |
| E08 | `FC_S2_NO` | PC15 | 3 | GPIO, EXTI15 | Sorti n° 2, port C (R4). Domaine de sauvegarde |
| E09 | `FC_S2_NC` | PC7 | 31 | GPIO, EXTI7 | |
| E10 | `EXT_IN1` | PA4 | 15 | GPIO / ADC1_IN4 | **TT_a : 3,6 V max.** Broche DAC : jamais en sortie (errata 2.3.1) |
| E11 | `EXT_IN2` | PA5 | 16 | GPIO / ADC1_IN5 | **TT_a : 3,6 V max.** Idem |
| E12 | `EXT_IN3` | PA6 | 17 | GPIO / ADC1_IN6 | |
| E13 | `EXT_IN4` | PA7 | 18 | GPIO / ADC1_IN7 | |
| E14 | `V12_OK` | **PB11** | 23 | GPIO | v1 : PA11. Broche bootloader : entrée, sans risque |
| E15 | `K1_FB` | PB7 | 46 | GPIO | Scrutation (EXTI7 prise par E09) |
| E16 | `K2_FB` | **PB1** | 20 | GPIO | DEC-078 (routage). **Même port que K1_FB (PB7) : PO-42** |
| E17 | `ARM_FB` | PA1 | 12 | GPIO | |
| E18 | `STOP_HW_FB` | **PB14** | 26 | GPIO | v1 : PA12. Broche bootloader : entrée, sans risque |
| S01 | `K1_CMD` | **PA15** | 37 | GPIO sortie | DEC-078 (routage). Port A (R5) |
| S02 | `K2_CMD` | **PB2** | 21 | GPIO sortie | DEC-078 (routage). Port B (R5) |
| S03 | `CP_DRV` | PB9 | 48 | TIM17_CH1 | Pompe de charge. Voir §4, point 6 |
| S04 | `WDT_KICK` | **PA12** | 34 | GPIO sortie | v1 : PB14 (broche bootloader SPI MISO). Sans remappage |
| S05 | `LED1` | PA8 | 28 | GPIO sortie | Broche CC de UCPD1 |
| S06 | `LED2` | PB15 | 27 | GPIO sortie | Broche CC de UCPD1 et broche bootloader : sortie non critique tolérée (LED) |
| S07 | `LED3` | PD0 | 38 | GPIO sortie | Broche CC de UCPD2 |
| S08 | `LED4` | PD2 | 40 | GPIO sortie | Broche CC de UCPD2 |
| S09 | `BUZZER` | PB8 | 47 | TIM16_CH1 | Voir §4, point 6 |
| C01 | `UART_TX` | PA9 | 29 | USART1_TX | Bootloader ROM |
| C02 | `UART_RX` | PA10 | 32 | USART1_RX | Bootloader ROM |
| C03 | `SWDIO` | PA13 | 35 | SYS_SWDIO | |
| C04 | `SWCLK` | PA14-BOOT0 | 36 | SYS_SWCLK | Porte aussi BOOT0 |
| Y01 | `OSC_IN` | PF0 | 8 | RCC_OSC_IN | Quartz HSE monté (DEC-060) |
| Y02 | `OSC_OUT` | PF1 | 9 | RCC_OSC_OUT | |

### Fonctions complémentaires X01 à X05 (retenues, DEC-039)

| Repère | Signal | Broche | N° | Fonction | Remarque |
|---|---|---|---|---|---|
| X01 | `V5_MON` | PB0 | 19 | ADC1_IN8 | Mesure du 5 V d'entrée |
| X02 | `STATUS_OUT` | **PA11** | 33 | GPIO sortie | v1 : PB11 (broche bootloader I2C). Sortie vers le contrôleur externe (R9) |
| X03 | `MAINT_IN` | **PB10** | 22 | GPIO | v1 : PB12. Entrée, broche bootloader sans risque |
| X04 | `BOARD_ID` | **PB12** | 24 | ADC1_IN16 | v1 : PB2. Entrée analogique sur pont diviseur |
| X05 | `I_MON` | **PA0** | 11 | ADC1_IN0 | Courant du vérin, capteur isolé (PO-27). DEC-078 |

X06 (température interne), X07 (VREFINT) et X08 (points de test) n'utilisent pas de broche.

### Réserves

| Usage | Broche | N° | Remarque |
|---|---|---|---|
| USART2 (R8) | PA2, PA3 | 13, 14 | Affecté à J10 en v2.1 (`UART2_TX`, `UART2_RX`). Broches bootloader : sortie TX vers un connecteur atelier, tolérée (R9) |
| Libre, entrée uniquement (R9) | PB13 | 25 | Broche bootloader SPI |
| Libre, dernier recours | PD1, PD3 | 39, 41 | Broches DBCC de UCPD2 : leur niveau commande une résistance de rappel sur PD0 / PD2 au reset |

## 3. Affectation par broche physique

| N° | Broche | Signal | N° | Broche | Signal |
|---|---|---|---|---|---|
| 1 | PC13 | `FC_R2_NO` | 25 | PB13 | Libre (entrée) |
| 2 | PC14 | `FC_R2_NC` | 26 | PB14 | `STOP_HW_FB` |
| 3 | PC15 | `FC_S2_NO` | 27 | PB15 | `LED2` |
| 4 | VBAT | 3,3 V | 28 | PA8 | `LED1` |
| 5 | VREF+ | 3,3 V, 100 nF | 29 | PA9 | `UART_TX` |
| 6 | VDD/VDDA | 3,3 V, 100 nF + 4,7 µF | 30 | PC6 | `PWM_IN` |
| 7 | VSS/VSSA | GND | 31 | PC7 | `FC_S2_NC` |
| 8 | PF0-OSC_IN | Quartz | 32 | PA10 | `UART_RX` |
| 9 | PF1-OSC_OUT | Quartz | 33 | PA11 | `STATUS_OUT` |
| 10 | PF2-NRST | NRST, 100 nF | 34 | PA12 | `WDT_KICK` |
| 11 | PA0 | `I_MON` | 35 | PA13 | `SWDIO` |
| 12 | PA1 | `ARM_FB` | 36 | PA14-BOOT0 | `SWCLK` |
| 13 | PA2 | `UART2_TX` | 37 | PA15 | `K1_CMD` |
| 14 | PA3 | `UART2_RX` | 38 | PD0 | `LED3` |
| 15 | PA4 | `EXT_IN1` | 39 | PD1 | Libre (dernier recours) |
| 16 | PA5 | `EXT_IN2` | 40 | PD2 | `LED4` |
| 17 | PA6 | `EXT_IN3` | 41 | PD3 | Libre (dernier recours) |
| 18 | PA7 | `EXT_IN4` | 42 | PB3 | `FC_R1_NO` |
| 19 | PB0 | `V5_MON` | 43 | PB4 | `FC_R1_NC` |
| 20 | PB1 | `K2_FB` | 44 | PB5 | `FC_S1_NO` |
| 21 | PB2 | `K2_CMD` | 45 | PB6 | `FC_S1_NC` |
| 22 | PB10 | `MAINT_IN` | 46 | PB7 | `K1_FB` |
| 23 | PB11 | `V12_OK` | 47 | PB8 | `BUZZER` |
| 24 | PB12 | `BOARD_ID` | 48 | PB9 | `CP_DRV` |

## 4. Points propres au STM32G071 (vérifiés sur DS12232 Rev 5 et ES0418 Rev 6)

| N° | Point | Constat | Traitement |
|---|---|---|---|
| 1 | Interface USB-C (UCPD), fonction « dead battery » | DS12232 Rev 5, tableau 12, note 4 : au reset, une résistance de rappel à la masse peut être présente sur PA8, PB15, PD0, PD2 selon le niveau de PA9, PA10, PD1, PD3. Elle se désactive par le bit UCPDx_STROBE de SYSCFG_CFGR1. Le cœur STM32duino le fait à l'initialisation (`hw_config_init()`). ES0418 2.15.3 : valeur de cette résistance mal ajustée, sans contournement. | Uniquement des LED sur ces broches (R6). Effet limité au démarrage. |
| 2 | Remappage PA11 / PA12 | Tableau 12, note 5 : PA11 / PA12 par défaut, remappage par SYSCFG_CFGR1. | Pas de remappage. |
| 3 | Domaine de sauvegarde (PC13 à PC15) | Tableau 12, note 1 : 3 mA max, ≤ 2 MHz, pas de source de courant. Note 2 : fonction dépendante des registres RTC, non réinitialisés par un reset système. ES0418 2.2.8 : les transitions sur PC13 perturbent le LSE (non utilisé). | Entrées seulement. Le firmware vérifie la configuration RTC. |
| 4 | État au reset | Tableau 11 : toutes les E/S en entrée analogique ; tableau 12, note 6 : rappel haut sur PA13, rappel bas sur PA14. | Rappels externes à la masse sur `K1_CMD` et `K2_CMD`. |
| 5 | Tolérance 5 V | Toutes les broches affectées sont FT, sauf PA4 et PA5 (TT_a, 3,6 V). ES0418 2.3.1 : les broches DAC (PA4, PA5) sont inutilisables en sortie. | Étage d'entrée de `EXT_IN1` / `EXT_IN2` limité à 3,3 V. PA4 / PA5 jamais en sortie. |
| 6 | TIM16 / TIM17 sur silicium révision B | ES0418 2.8.6 : TIM16 et TIM17 cadencés à tort par SYSCLK, révision B uniquement. | Le firmware lit la révision du silicium au démarrage (DBG_IDCODE) et vérifie la fréquence de `CP_DRV` et `BUZZER` à la première mise en service. |
| 7 | NRST à la mise sous tension | DS12232 Rev 5, figure 23, note 2 : le niveau de NRST doit dépasser VIH(NRST) à la mise sous tension, sinon le MCU reste en reset. | Tout circuit externe relié à NRST (watchdog, superviseur) doit respecter cette condition. |

## 5. Modifications v1 → v2

| Signal | v1 | v2 | Motif |
|---|---|---|---|
| `K1_CMD` | PB13 | PB2 | R9 : PB13 est configurable par le bootloader (SPI2) |
| `WDT_KICK` | PB14 | PA12 | R9 : PB14 peut devenir la sortie MISO du bootloader SPI |
| `STATUS_OUT` | PB11 | PA11 | R9 : sortie vers l'extérieur, PB11 configurable par le bootloader (I2C2) |
| `V12_OK` | PA11 | PB11 | Libère PA11 ; entrée sans risque sur broche bootloader |
| `STOP_HW_FB` | PA12 | PB14 | Libère PA12 ; entrée sans risque sur broche bootloader |
| `MAINT_IN` | PB12 | PB10 | Libère PB12 pour `BOARD_ID` |
| `BOARD_ID` | PB2 | PB12 | Libère PB2 pour `K1_CMD` ; PB12 reste une entrée ADC |

R3, R4, R5 et R6 restent respectées.

## 6. Bilan

| Catégorie | Broches |
|---|---|
| GPIO du boîtier (PF2-NRST incluse) | 44 |
| NRST (PF2) | 1 |
| Signaux retenus | 33 |
| Fonctions complémentaires X01 à X05 | 5 |
| Réserve USART2 | 2 |
| Libres | 3 (PB13 en entrée seulement, PD1, PD3) |
