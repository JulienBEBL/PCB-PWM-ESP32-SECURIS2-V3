# État du projet

**Dernière mise à jour :** 2026-11-06 (session 02, fin)

## Phase en cours

| Phase | Statut |
|---|---|
| 0. Cadrage, reprise STM32 | Terminée |
| 1. Carte de dev | Terminée (DEC-015) |
| 2. Brochage | Terminé, v3 (DEC-023) |
| 3. Alimentation | Terminée (DEC-026) |
| 4. BOM cible et netlist cible | **v1 rédigées, jalons G1 et G2 à valider par Julien** : `hardware/BOM_CIBLE.md`, `hardware/NETLIST_CIBLE.md` |
| 5. Schéma (Claude Code + API, contrôle Julien) | À faire, jalon G3. Prérequis : étape E0 (copie du projet STM32 par Julien) |
| 6. PCB 4 couches | À faire, jalons G4 et G5 |
| 7. Commande JLCPCB | **Au plus tard ~14 nov** (PO-11) |
| 8. Firmware Arduino ESP32 | À faire, en parallèle de la fabrication |
| 9. Essais | Fin novembre |

## Synthèse

- Même fonction et même CdC que la carte STM32 (DEC-002). Aucune défaillance simple ne provoque l'action (DEC-003).
- Carte jumelle : tous les blocs de sécurité STM32 conservés (DEC-004).
- MCU : ESP32-S3-DevKitC-1-**N8** enfichée, amovible (DEC-006, DEC-015). 24 signaux, aucune broche sensible utilisée, réserve GPIO7 (DEC-023). LED RGB WS2812B + adaptateur de niveau (DEC-024). 2 extensions, pas d'entrée maintenance.
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
| G1, G2 | Validation de la BOM et de la netlist cibles par Julien |
| E0 | Copie du projet EasyEDA STM32 par Julien, installation du pont |
| PO-22 | Délai de la saisie par l'API |
| PO-21 | Écart entre les rangées de la carte de dev (bloque le placement) |
| STM32 PO-05 | Règles de la FSM (firmware) |

## Prochaine étape

1. Julien : valider G1 et G2 ; commander la carte de dev N8 (PO-18, PO-21 se mesurent dessus).
2. Julien : étape E0 (copie du projet `PCB_PWM_SECURISE_V3` en `PCB_PWM_ESP32_V3`), lancement du pont, puis Claude Code dans ce dépôt.
3. Claude Code : phase 0 sur la copie, puis saisie du schéma (G3).
