# Liste des E/S et brochage ESP32

**Statut :** **v2** (2026-11-06). Remplace la v1 validée (DEC-017) : LED RGB adressable à la place des 4 LED (DEC-019), broches sensibles réservées aux fonctions sans lien avec la sécurité (DEC-020).
**Carte de dev :** ESP32-S3-DevKitC-1-**N8** (DEC-015). Module ESP32-S3-WROOM-1-N8 : flash 8 Mo Quad SPI, pas de PSRAM, VDD_SPI 3,3 V.

**Sources vérifiées le 2026-11-06 :**

- [D1] Espressif, *ESP32-S3-DevKitC-1 v1.1 User Guide* (docs.espressif.com) : brochage J1 / J3, variantes, alimentation, LED RGB.
- [D2] Espressif, *ESP32-S3 Series Datasheet v2.2* : tableau 2-1 (état au reset), tableau 2-2 (impulsions à la mise sous tension), tableau 3-1 (broches de démarrage), tableau 2-14 (broches flash / PSRAM).

## 1. Règles appliquées

| N° | Règle | Raison | Source |
|---|---|---|---|
| R1 | Les 4 sorties de sécurité (`K1_CMD`, `K2_CMD`, `CP_DRV`, `WDT_KICK`) sont **actives à l'état haut**, avec rappel externe à la masse. | Un ESP32 en reset, non alimenté ou absent (carte retirée) laisse les relais au repos. | DEC-003 |
| R2 | **Broches sensibles** : broches de démarrage (GPIO0, 3, 45, 46), broches à impulsion haute au démarrage (GPIO18, 19, 20), LED RGB de la carte de dev (GPIO38 en v1.1, GPIO48 en v1.0). Elles ne portent **aucune fonction de sécurité**. | DEC-018, DEC-020 | [D1], [D2] tab. 2-2, 3-1 |
| R3 | Les impulsions **basses** au démarrage (GPIO1 à 17) sont admises, y compris pour les sorties de sécurité. | Sans effet sur une sortie déjà tirée à la masse (R1). | [D2] tab. 2-2 |
| R4 | `K1_CMD` et `K2_CMD` sur des **registres GPIO différents** (GPIO0–31 et GPIO32–48). Idem `K1_FB` / `K2_FB`. Chaque commande est séparée de son signal de validation. | Un défaut d'écriture dans un registre ne commande pas les deux relais. | Règle de conception |
| R5 | Non utilisées : GPIO43 / 44 (UART0 vers le pont USB, moniteur série) et GPIO19 / 20 (port USB natif). | Fonctions occupées sur la carte de dev. | [D1] |
| R6 | Entrées analogiques sur l'**ADC1** (GPIO1 à 10). | L'ADC2 est partagé avec la radio. La radio est coupée (DEC-007), mais l'ADC1 évite toute dépendance. | [D2] tab. 2-8 |

## 2. Brochage

27 signaux. 25 sur des broches non sensibles, 2 sur des broches sensibles (fonctions sans lien avec la sécurité).

### Sorties

| Repère | Signal | GPIO | Registre | Rôle | Remarque |
|---|---|---|---|---|---|
| S01 | `K1_CMD` | 21 | 0–31 | Bobine K1 | Aucune impulsion au démarrage |
| S02 | `K2_CMD` | 47 | 32–48 | Bobine K2 (en série avec le cavalier) | Aucune impulsion au démarrage |
| S03 | `CP_DRV` | 37 | 32–48 | Pompe de charge, validation de K1 | Signal carré LEDC. Registre différent de `K1_CMD` |
| S04 | `WDT_KICK` | 14 | 0–31 | Watchdog externe, validation de K2 | Registre différent de `K2_CMD`. Impulsion basse au démarrage sans effet (R3) |
| S05 | `RGB_DATA` | **46** | 32–48 | LED RGB adressable (DEC-019) | **Broche sensible.** Rien ne doit tirer GPIO46 vers le haut au reset |
| S09 | `BUZZER` | 13 | 0–31 | Buzzer | Signal LEDC |
| S10 | `STATUS` | 35 | 32–48 | Sortie prêt / défaut vers le contrôleur (STM32 X02) | |

S06 à S08 (LED2 à LED4) : supprimées (DEC-019). Repères non réutilisés.

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
| E09 | `FC_S2_NC` | **36** | 32–48 | Fin de course sorti 2, NC | Déplacée de GPIO18 (v1) |
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
| X03 | `MAINT_IN` | **3** | 0–31 | Entrée de mode maintenance | **Broche sensible** (source JTAG), sans effet avec les eFuses d'usine. Entrée seulement. Maintien à confirmer (PO-15) |

GPIO39 à 42 sont les broches JTAG. Avec la configuration d'usine, le JTAG passe par l'USB : ces broches sont des GPIO ordinaires. Elles ne servent ici qu'en entrée.

### Broches libres

| GPIO | Raison | Usage possible |
|---|---|---|
| 18, 45, 48 | Sensibles (R2) | Réserve, fonctions sans lien avec la sécurité seulement. GPIO45 : jamais de rappel au 3,3 V (la flash ne démarre pas si GPIO45 = 1 au reset) |
| 0, 38 | Bouton BOOT, LED RGB de la carte de dev (v1.1) | Aucun |
| 19, 20, 43, 44 | USB natif, UART0 (R5) | Aucun |

## 3. Écarts avec la liste STM32

| Élément STM32 | Statut | Raison |
|---|---|---|
| S05 à S08 (4 LED d'état) | Remplacés par 1 LED RGB adressable | DEC-019 |
| X05 `I_MON` (mesure de courant) | Supprimé | DEC-016 |
| X04 Identification de révision | Supprimé | DEC-017 |
| J10 liaison série USART2 | Supprimé | DEC-017 |
| X06 température, X07 surveillance 3,3 V | Non repris | Capteurs internes du STM32. Sans objet pour la sécurité |
| C01–C04 (UART, SWD), Y01–Y02 (quartz) | Supprimés | Fournis par la carte de dev |
| LED5 « 3,3 V présent » (câblée en dur) | Repris | Indique l'alimentation même si l'ESP32 ou la LED RGB est en panne |

## 4. Points d'attention

| N° | Point | Conséquence |
|---|---|---|
| A1 | Réserve : 3 broches, toutes sensibles (GPIO18, 45, 48). | Un ajout lié à la sécurité imposerait de supprimer un signal |
| A2 | Le câblage dépend de la variante **N8**. Sur N8R8, GPIO35 à 37 sont pris par la PSRAM ; sur les variantes 1,8 V, GPIO47 / 48 passent en 1,8 V. | Commander exactement **ESP32-S3-DevKitC-1-N8**. Le noter en sérigraphie |
| A3 | Retirer la carte de dev laisse toutes les entrées et sorties en l'air. | R1 garantit les relais au repos. Les rappels externes fixent les autres niveaux |
| A4 | Alimentations : [D1] indique que les entrées USB et 5 V sont **mutuellement exclusives**. | À traiter dans l'architecture d'alimentation (PO-07) |
| A5 | LED RGB : niveau haut d'entrée attendu (≈ 0,7 × VDD) peut dépasser 3,3 V si elle est alimentée en 5 V. Plage de température à vérifier. | Référence et adaptation de niveau : PO-16 |
