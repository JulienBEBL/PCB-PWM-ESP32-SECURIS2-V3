# Firmware

Code Arduino pour la carte de dev ESP32-S3-DevKitC-1-N8.

| Dossier | Version | Usage |
|---|---|---|
| `carte_pwm_esp32/` | v0, mise en service | Vérifier la carte. **Ne commande jamais le vérin** |
| `carte_pwm_esp32_v1/` | v1, FSM 4 états | Fonctionnement normal. Spécification : `docs/architecture/FSM_V1.md` (à valider, PO-24) |

Les deux versions partagent le même brochage et les mêmes réglages. **Charger d'abord la v0**, valider la carte, puis la v1.

## Version v0 — mise en service (`carte_pwm_esp32/`)

**Le vérin n'est jamais commandé** : `K1_CMD` et `K2_CMD` restent à 0, la pompe de charge est arrêtée. Cette version sert à vérifier la carte de dev, puis la carte complète.

| Fonction | v0 |
|---|---|
| Mesure du PWM (largeur, période, perte) | Oui |
| Lecture des 4 fins de course NO + NC, détection d'incohérence | Oui |
| Relecture K1, K2, cavalier, 5e contact, présence 12 V, +5V | Oui |
| Autocontrôle au démarrage puis surveillance (proposition PO-12) | Oui |
| Watchdog externe TPS3430 rafraîchi (36 ms) et watchdog interne (300 ms) | Oui |
| LED RGB, buzzer, ligne STATUS, moniteur série | Oui |
| Commande du vérin (état ACTION) | **Non**, attend la FSM (STM32 PO-05) |
| Radio | Jamais initialisée : aucune bibliothèque radio incluse (DEC-007) |

**v0 et v1 non compilées dans cet environnement** : seule une vérification de syntaxe a été faite. Premier test à faire par Julien.

## Installation (une fois)

1. Arduino IDE 2.
2. Préférences → URL de cartes supplémentaires : `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Gestionnaire de cartes : installer **esp32 by Espressif Systems**, version **3.x**.

## Réglages de la carte

| Réglage | Valeur |
|---|---|
| Carte | ESP32S3 Dev Module |
| Port | Celui du port USB marqué **UART** sur la carte de dev |
| USB CDC On Boot | Disabled (le moniteur série passe par le port UART) |
| Flash Size | 8MB |
| PSRAM | Disabled (variante N8, sans PSRAM) |
| Moniteur série | 115200 bauds |

Si la compilation signale `rgbLedWrite` inconnu, le cœur installé est en 2.x : remplacer par `neopixelWrite` ou installer la 3.x.

## Codes de la LED RGB (provisoires, à figer avec la FSM)

| Couleur | Signification |
|---|---|
| Bleu fixe | Démarrage, autocontrôle en cours |
| Rouge clignotant | Défaut verrouillé jusqu'au reset. Motif affiché sur le moniteur série |
| Jaune | Carte sûre, 12 V absent |
| Violet | Carte sûre, 12 V présent, PWM absent ou invalide |
| Vert | Carte sûre, 12 V et PWM valides |

Ligne STATUS : basse = carte prête (état SÛRE), haute = non prête ou défaut.

## Ce qu'on observe sur la carte de dev seule

Sans la carte complète, les entrées sont en l'air. Le firmware passe en **DÉFAUT** (fins de course incohérentes, +5V hors plage). C'est le comportement attendu. Ce test valide la compilation, le téléversement, la LED RGB de GPIO6 (si câblée) et le moniteur série.

Mesure du PWM sur la carte de dev seule : relier la sortie d'un testeur de servo (signal 3,3 V, masse commune) à GPIO1. **Ne pas injecter 5 V directement sur une broche de l'ESP32.**

## Version v1 — FSM 4 états (`carte_pwm_esp32_v1/`)

Cycle REPOS → PRÊT → ACTION → APRÈS → REPOS, toujours dans ce sens. États techniques : DÉMARRAGE, DÉFAUT (verrouillé jusqu'au reset). Détail : `docs/architecture/FSM_V1.md`.

| Couleur | État |
|---|---|
| Bleu fixe | DÉMARRAGE |
| Vert | 1 REPOS |
| Orange | 2 PRÊT |
| Bleu clignotant + bips | 3 ACTION |
| Cyan | 4 APRÈS (retour) |
| Rouge clignotant | DÉFAUT |

Les seuils et temporisations sont dans `carte_pwm_esp32_v1/config.h`.

**Premier essai avec le vérin** : vérin à vide, sans charge, blocage mécanique en place. Vérifier d'abord que l'état PRÊT ne produit aucun mouvement.
