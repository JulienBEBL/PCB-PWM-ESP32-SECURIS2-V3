# État du projet

**Dernière mise à jour :** 2026-11-06 (session 02, fin)

## Phase en cours

| Phase | Statut |
|---|---|
| 0. Cadrage, reprise STM32 | Terminée |
| 1. Carte de dev | Terminée (DEC-015) |
| 2. Brochage | Terminé, v3 (DEC-023) |
| 3. Alimentation | Terminée (DEC-026) |
| 4. BOM cible et netlist cible | **Validées** (G1, G2, DEC-027) |
| 5. Schéma (Claude Code + API, contrôle Julien) | À faire, jalon G3. Prérequis : étape E0 sur le PC personnel (installation + projet vierge, DEC-028) |
| 6. PCB 4 couches | À faire, jalons G4 et G5 |
| 7. Commande JLCPCB | **Au plus tard ~14 nov** (PO-11) |
| 8. Firmware Arduino ESP32 | **v0** (mise en service, sans commande du vérin) et **v1** (FSM 4 états, DEC-030) écrites. Non compilées ici. FSM à valider (PO-24) |
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
| E0 | Installation EasyEDA + API sur le PC personnel, projet vierge `PCB_PWM_ESP32_V3` |
| PO-24 | Validation de la FSM v1 |
| PO-22 | Délai de la saisie par l'API |
| PO-21 | Écart entre les rangées de la carte de dev (bloque le placement) |

## Prochaine étape

1. Julien : étape E0 sur le PC personnel (`hardware/INSTALLATION_EASYEDA_API.md`), puis Claude Code dans un clone de ce dépôt.
2. Claude Code : phase 0, puis saisie du schéma (G3).
3. Julien, à réception de la carte de dev : test au multimètre (PO-18), mesure de l'écart des rangées (PO-21), premier téléversement du firmware v0.
4. Julien : valider la FSM v1 (PO-24), en particulier le maintien en position sortie et les bandes PWM.
