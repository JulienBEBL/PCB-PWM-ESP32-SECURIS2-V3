# Liste des E/S et brochage ESP32

**Statut :** v1 **validée** (DEC-017). Ajustement en attente sur les broches sensibles (PO-14).
**Carte de dev :** ESP32-S3-DevKitC-1-**N8** (DEC-015). Module ESP32-S3-WROOM-1-N8 : flash 8 Mo Quad SPI, pas de PSRAM, VDD_SPI 3,3 V.

**Sources vérifiées le 2026-11-06 :**

- [D1] Espressif, *ESP32-S3-DevKitC-1 v1.1 User Guide* (docs.espressif.com) : brochage J1 / J3, variantes, alimentation, LED RGB.
- [D2] Espressif, *ESP32-S3 Series Datasheet v2.2* : tableau 2-1 (état au reset), tableau 2-2 (impulsions à la mise sous tension), tableau 3-1 (broches de démarrage), tableau 2-14 (broches flash / PSRAM).

## 1. Règles appliquées

| N° | Règle | Raison | Source |
|---|---|---|---|
| R1 | Les 4 sorties de sécurité (`K1_CMD`, `K2_CMD`, `CP_DRV`, `WDT_KICK`) sont **actives à l'état haut**, avec rappel externe à la masse. | Un ESP32 en reset, non alimenté ou absent (carte retirée) laisse les relais au repos. | DEC-003 |
| R2 | Aucune sortie sur une broche qui subit une **impulsion haute** au démarrage : GPIO18, 19, 20. | Une impulsion haute pourrait exciter un relais. Une impulsion basse (GPIO1 à 17) est sans effet sur une sortie déjà tirée à la masse (R1). | [D2] tab. 2-2 |
| R3 | `K1_CMD` et `K2_CMD` sur des **registres GPIO différents** (GPIO0–31 et GPIO32–48). Idem `K1_FB` / `K2_FB`, et chaque commande séparée de son signal de validation. | Un défaut d'écriture dans un registre ne commande pas les deux relais. Reprise de la règle STM32 (PO-42 STM32). | Règle de conception |
| R4 | Broches de démarrage : GPIO0 et GPIO3 **non utilisées en sortie**. GPIO45 et GPIO46 utilisées seulement pour des LED vers la masse. | GPIO45 doit être à 0 au reset (flash 3,3 V). Une LED vers la masse maintient ce 0. Un rappel au 3,3 V sur GPIO45 empêcherait le démarrage. | [D2] tab. 3-1, 3-4 |
| R5 | Non utilisées : GPIO43 / 44 (UART0 vers le pont USB, moniteur série), GPIO19 / 20 (port USB natif), GPIO0 (bouton BOOT), GPIO38 (LED RGB en v1.1). | Fonctions occupées sur la carte de dev. | [D1] |
| R6 | GPIO48 pilote la LED RGB sur les cartes **v1.0**. Seule une LED d'état y est affectée. | La carte livrée peut être v1.0 ou v1.1. | [D1] |
| R7 | Entrées analogiques sur l'**ADC1** (GPIO1 à 10). | L'ADC2 est partagé avec la radio. La radio est coupée (DEC-007), mais l'ADC1 évite toute dépendance. | [D2] tab. 2-8 |

## 2. Brochage

30 broches utilisées sur 30 disponibles après application des règles. **Aucune réserve** (voir §4).

### Sorties

| Repère | Signal | GPIO | Registre | Rôle | Remarque |
|---|---|---|---|---|---|
| S01 | `K1_CMD` | 21 | 0–31 | Bobine K1 | Aucune impulsion au démarrage |
| S02 | `K2_CMD` | 47 | 32–48 | Bobine K2 (en série avec le cavalier) | Aucune impulsion au démarrage |
| S03 | `CP_DRV` | 37 | 32–48 | Pompe de charge, validation de K1 | Signal carré LEDC. Registre différent de `K1_CMD` |
| S04 | `WDT_KICK` | 14 | 0–31 | Watchdog externe, validation de K2 | Registre différent de `K2_CMD`. Impulsion basse au démarrage sans effet |
| S05 | `LED1` | 45 | 32–48 | LED d'état 1 | Broche de démarrage : LED vers la masse obligatoire (R4) |
| S06 | `LED2` | 46 | 32–48 | LED d'état 2 | Idem |
| S07 | `LED3` | 36 | 32–48 | LED d'état 3 | |
| S08 | `LED4` | 48 | 32–48 | LED d'état 4 | LED RGB en parallèle sur carte v1.0 (R6) |
| S09 | `BUZZER` | 13 | 0–31 | Buzzer | Signal LEDC |
| S10 | `STATUS` | 35 | 32–48 | Sortie prêt / défaut vers le contrôleur (STM32 X02) | |

### Entrées

| Repère | Signal | GPIO | Registre | Rôle | Remarque |
|---|---|---|---|---|---|
| E01 | `PWM_IN` | 1 | 0–31 | Consigne PWM | Mesure par interruption. Horloge : quartz 40 MHz de la carte de dev |
| E02 | `FC_R1_NO` | 2 | 0–31 | Fin de course rentré 1, NO | |
| E03 | `FC_R1_NC` | 9 | 0–31 | Fin de course rentré 1, NC | |
| E04 | `FC_R2_NO` | 10 | 0–31 | Fin de course rentré 2, NO | |
| E05 | `FC_R2_NC` | 11 | 0–31 | Fin de course rentré 2, NC | |
| E06 | `FC_S1_NO` | 15 | 0–31 | Fin de course sorti 1, NO | |
| E07 | `FC_S1_NC` | 16 | 0–31 | Fin de course sorti 1, NC | |
| E08 | `FC_S2_NO` | 17 | 0–31 | Fin de course sorti 2, NO | |
| E09 | `FC_S2_NC` | 18 | 0–31 | Fin de course sorti 2, NC | Impulsion haute au démarrage : la résistance série du filtre limite le courant si le contact est fermé |
| E10 | `EXT_IN1` | 4 | 0–31 | Extension 1 | ADC1_CH3 |
| E11 | `EXT_IN2` | 5 | 0–31 | Extension 2 | ADC1_CH4 |
| E12 | `EXT_IN3` | 6 | 0–31 | Extension 3 | ADC1_CH5 |
| E13 | `EXT_IN4` | 7 | 0–31 | Extension 4 | ADC1_CH6 |
| E14 | `V12_OK` | 39 | 32–48 | Présence 12 V (opto) | |
| E15 | `K1_FB` | 40 | 32–48 | Relecture K1 | Registre différent de `K2_FB` et de `K1_CMD` |
| E16 | `K2_FB` | 12 | 0–31 | Relecture K2 | Registre différent de `K1_FB` et de `K2_CMD` |
| E17 | `ARM_FB` | 41 | 32–48 | Relecture du cavalier | |
| E18 | `STOP_HW_FB` | 42 | 32–48 | Relecture de l'arrêt matériel (opto) | |
| X01 | `V5_MON` | 8 | 0–31 | Mesure du 5 V d'entrée (pont diviseur) | ADC1_CH7. Sert à l'autocontrôle (DEC-011) |
| X03 | `MAINT_IN` | 3 | 0–31 | Entrée de mode maintenance | Broche de démarrage (source JTAG), sans effet avec les eFuses d'usine. Entrée seulement (R4) |

GPIO39 à 42 sont les broches JTAG. Avec la configuration d'usine, le JTAG passe par l'USB : ces broches sont des GPIO ordinaires. Elles ne servent ici qu'en entrée.

## 3. Écarts avec la liste STM32

| Élément STM32 | Statut | Raison |
|---|---|---|
| X05 `I_MON` (mesure de courant) | Supprimé | DEC-016 |
| X04 Identification de révision | Supprimé (DEC-017) | Une seule révision de carte |
| J10 liaison série USART2 | Supprimé (DEC-017) | Le port USB-UART de la carte de dev sert au moniteur série |
| X06 température, X07 surveillance 3,3 V | Non repris | Capteurs internes du STM32. L'ESP32-S3 a un capteur de température interne, peu précis. Sans objet pour la sécurité |
| C01–C04 (UART, SWD), Y01–Y02 (quartz) | Supprimés | Fournis par la carte de dev |

## 4. Points d'attention

| N° | Point | Conséquence |
|---|---|---|
| A1 | **Aucune broche de réserve.** Les seules broches libres sont GPIO19 / 20 (USB natif, impulsions hautes), GPIO38 (LED RGB) et GPIO0 (BOOT). | Un ajout d'E/S impose de supprimer un signal. Candidats à réduire si besoin : LED 4 → 3 (STM32 PO-12), `MAINT_IN` |
| A2 | Le câblage dépend de la variante **N8**. Sur N8R8, GPIO35 à 37 sont pris par la PSRAM ; sur les variantes 1,8 V, GPIO47 / 48 passent en 1,8 V. | Commander exactement **ESP32-S3-DevKitC-1-N8**. Le noter en sérigraphie |
| A3 | Retirer la carte de dev laisse toutes les entrées et sorties en l'air. | R1 garantit les relais au repos. Les rappels externes fixent les autres niveaux |
| A4 | Alimentations : [D1] indique que les entrées USB et 5 V sont **mutuellement exclusives**. | Branchement USB pendant que le 5 V externe est présent : risque de conflit. À traiter dans l'architecture d'alimentation (PO-07) |
