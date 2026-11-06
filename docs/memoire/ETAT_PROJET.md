# État du projet

**Dernière mise à jour :** 2026-11-06 (session 02)

## Phase en cours

| Phase | Statut |
|---|---|
| 0. Cadrage et reprise du dépôt STM32 | **Terminée** (`docs/REPRISE_STM32.md`) |
| 1. Choix de la carte de dev ESP32 | **Terminée** : ESP32-S3-DevKitC-1-N8 (DEC-015) |
| 2. Liste des E/S et brochage ESP32 | **v2** (DEC-020) : `docs/architecture/BROCHAGE_ESP32.md` |
| 3. Schéma (Julien, à la main) | À faire |
| 4. PCB 4 couches (Julien, à la main) | À faire |
| 5. Commande JLCPCB | À faire, **au plus tard ~14 nov** (PO-11) |
| 6. Firmware Arduino ESP32 | À faire, en parallèle de la fabrication |
| 7. Essais sur banc | À faire, fin novembre |

## Synthèse

- Même fonction et même CdC que la carte STM32 (DEC-002). Aucune défaillance simple ne provoque l'action (DEC-003).
- Carte jumelle : tous les blocs de sécurité STM32 conservés (DEC-004).
- MCU : ESP32-S3-DevKitC-1-**N8** enfichée, amovible (DEC-006, DEC-015). 27 signaux, aucune fonction de sécurité sur une broche sensible. LED RGB adressable (DEC-019).
- Mesure isolée du courant supprimée (DEC-016).
- Radio coupée : Wi-Fi et Bluetooth jamais initialisés (DEC-007).
- Usage chez le client, en extérieur (DEC-008). PCB 4 couches (DEC-009). Assemblage JLCPCB au maximum (DEC-012).
- Câblage externe identique à la carte STM32 : mêmes borniers, même brochage. Les deux cartes sont interchangeables côté câblage (objectif, à tenir au schéma).

## Planning (DEC-014)

| Dates | Tâche | Qui |
|---|---|---|
| 7–9 nov | Carte de dev, liste des E/S, brochage, architecture | Claude propose, Julien valide |
| 9–11 nov | Schéma | Julien, relecture Claude |
| 12–13 nov | Placement et routage | Julien, relecture Claude |
| **14 nov** | **Commande JLCPCB, transport express** | Julien |
| 14–25 nov | Firmware sur carte de dev nue + breadboard | Claude, tests Julien |
| 25–30 nov | Réception, essais | Julien |

## Points bloquants

| N° | Sujet |
|---|---|
| PO-16 | Référence de la LED RGB |
| PO-07 | Architecture d'alimentation (USB et 5 V exclusifs) |
| STM32 PO-05 | Règles de la FSM (pour le firmware, pas pour le schéma) |

## Prochaine étape

1. Julien : PO-15 (entrée maintenance).
2. Claude : architecture d'alimentation avec la carte de dev (PO-07).
3. Claude : liste des modifications du schéma STM32 pour Julien (blocs à retirer, signaux à rebrancher sur l'ESP32).
