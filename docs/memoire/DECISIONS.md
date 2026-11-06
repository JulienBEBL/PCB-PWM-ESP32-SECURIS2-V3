# Registre des décisions

Une décision n'est inscrite ici que si Julien l'a exprimée ou validée. Une décision révisée est marquée « Remplacée par DEC-xxx », jamais supprimée.

Les décisions du dépôt STM32 s'appliquent ici sauf écart listé dans `docs/REPRISE_STM32.md` (DEC-002).

| Statut | Signification |
|---|---|
| Validée | Exprimée ou confirmée par Julien |
| À confirmer | Validée sur le principe, un détail reste ouvert (renvoi PO) |
| Remplacée | Annulée par une décision ultérieure |

## Cadre du projet

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-001 | 2026-11-06 | Carte de repli de la carte STM32 V3, au cas où celle-ci n'aboutit pas ou ne fonctionne pas. Conçue **manuellement par Julien**, Claude en appui (relecture, documentation, firmware). | Remplacée en partie par DEC-025 (méthode de conception) | Julien : « faire de façon manuelle, avec toi en aide » |
| DEC-002 | 2026-11-06 | **Même cahier des charges dans l'ensemble** que la carte STM32. Objectif : remplacer le STM32 par un ESP32 et simplifier si possible. | Validée | Julien |
| DEC-003 | 2026-11-06 | **Aucune défaillance simple ne doit provoquer l'action** (reprise de STM32 DEC-020, sans dérogation). | Validée | Julien : « ça doit être le cas pour cette carte » |
| DEC-004 | 2026-11-06 | Niveau retenu : **carte jumelle**. Tous les blocs de sécurité de la carte STM32 sont conservés (pont DPDT, relecture, pompe de charge, watchdog externe, cavalier, opto 12 V, protection 12 V). Seules disparaissent les fonctions fournies par la carte de dev. | Validée | Julien : « OK ». Option « carte minimale » écartée car contraire à DEC-003 |
| DEC-005 | 2026-11-06 | **Clarté et simplicité pour Julien** : documents courts, une étape à la fois, explications explicites. | Validée | Julien : « il faut que ce soit clair et simple pour moi » |
| DEC-008 | 2026-11-06 | Usage du POC : **chez le client, en extérieur**. Les exigences d'environnement (STM32 DEC-004, DEC-040) s'appliquent. | Validée | Julien |
| DEC-025 | 2026-11-06 | **Schéma et PCB saisis par Claude Code**, qui pilote EasyEDA Pro par l'API (comme la carte STM32). Julien contrôle et valide à chaque jalon. **Schéma, placement et routage très simples et clairs.** Remplace la partie « conçue manuellement par Julien » de DEC-001. | Validée | Julien. Consignes : `hardware/CONSIGNES_EASYEDA_CLAUDE_CODE.md` §0 |
| DEC-027 | 2026-11-06 | **Jalons G1 (BOM cible) et G2 (netlist cible) validés.** Claude travaille en autonomie jusqu'au prochain jalon. | Validée | Julien : « je valide tout, travaille en autonomie » |
| DEC-028 | 2026-11-06 | Schéma et PCB dans un **nouveau projet EasyEDA vierge** `PCB_PWM_ESP32_V3`, sur le **PC personnel** de Julien. Pas de copie du projet STM32. | Validée | Julien. Remplace l'étape E0 « copie » proposée |
| DEC-029 | 2026-11-06 | Carte de dev ESP32-S3-DevKitC-1-N8 commandée par Julien en parallèle. | Validée | Julien. Réception : PO-18, PO-21 |
| DEC-013 | 2026-11-06 | Dépôt séparé `JulienBEBL/PCB-PWM-ESP32-SECURIS2-V3`, alimenté par les informations du dépôt STM32 (copie figée dans `references/stm32_v3/`). | Validée | Julien |
| DEC-014 | 2026-11-06 | **Délai** : carte reçue et testée fin novembre 2026. Conception (schéma + PCB) en 2 semaines maximum. | Validée | Julien. Date de commande au plus tard : PO-11 |

## Microcontrôleur et firmware

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-006 | 2026-11-06 | MCU = **carte de développement ESP32 enfichée** sur barrettes femelles, amovible. Pas de module ESP32 soudé. Remplace STM32 DEC-035 et DEC-036. | Validée | Julien : « ESP32 rajouté manuellement, qui peut s'enlever, avec les broches ». Modèle : PO-01 |
| DEC-007 | 2026-11-06 | **Toutes les communications radio coupées** : Wi-Fi, Bluetooth, et toute autre liaison sans fil. | Validée | Julien. Mise en œuvre et preuve : PO-05 |
| DEC-015 | 2026-11-06 | Carte de dev : **ESP32-S3-DevKitC-1-N8** (module WROOM-1-N8, flash Quad SPI 8 Mo, sans PSRAM). La variante N8 est imposée par le brochage. | Validée | Julien : « PO-01 validé ». Clôt PO-01. `docs/architecture/BROCHAGE_ESP32.md` A2 |
| DEC-011 | 2026-11-06 | Autocontrôle au démarrage réalisé par l'ESP32, si faisable. | À confirmer | Julien. Contenu de l'autocontrôle : PO-12 |

## Matériel et fabrication

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-009 | 2026-11-06 | PCB **4 couches** accepté (empilement STM32 DEC-070 repris). | Validée | Julien |
| DEC-010 | 2026-11-06 | **Entrées d'extension capteurs et buzzer conservés.** | Validée | Julien |
| DEC-026 | 2026-11-06 | **Alimentation** selon `ALIMENTATION_ESP32.md` : U15 sur J1, U16 vers la broche 5V de la carte de dev, +3V3 pris sur la carte de dev, U2 supprimé. | Validée | Julien : « ok pour l'alimentation, si tout est OK et compatible ». Clôt PO-07. Réserve : PO-18 |
| DEC-016 | 2026-11-06 | **Mesure isolée du courant du vérin supprimée** (STM32 X05, U8 / U9 / U10 et composants associés). | Validée | Julien : « PO-08 supprime ». Clôt PO-08. Diagnostic seulement, sans effet sur la sécurité |
| DEC-017 | 2026-11-06 | **Brochage v1 validé** (`docs/architecture/BROCHAGE_ESP32.md`). **X04** (identification de révision) et **J10** (série USART2) supprimés. | Validée | Julien : « OK pour 1 2 3 ». Clôt PO-13. Usage des broches sensibles : PO-14 |
| DEC-018 | 2026-11-06 | Principe : **ne pas utiliser les broches sensibles** (modifiées au démarrage) **s'il y a de la marge**. | Validée | Julien. Pas de marge en v1 : arbitrage PO-14 |
| DEC-019 | 2026-11-06 | **1 LED RGB adressable** (type WS2812B / SK6812, 1 broche) remplace les 4 LED d'état. LED5 « 3,3 V présent » conservée. | Validée | Julien : « OUI ». Référence et niveau logique : PO-16 |
| DEC-020 | 2026-11-06 | Broches sensibles : utilisées seulement pour des fonctions sans lien avec la sécurité (`RGB_DATA` GPIO46, `MAINT_IN` GPIO3). `FC_S2_NC` déplacée de GPIO18 à GPIO36. Brochage v2. | Remplacée par DEC-023 | Découle de DEC-018 et DEC-019. Clôt PO-14 |
| DEC-021 | 2026-11-06 | Entrées d'extension réductibles à **2** (1 numérique, 1 analogique) si des broches manquent. | Validée | Julien : « si besoin de pin, passe à seulement 2 extensions ». Application : PO-17 |
| DEC-022 | 2026-11-06 | **Entrée de mode maintenance supprimée** (STM32 X03). Les essais se font avec un générateur PWM. | Validée | Julien : « à supprimer ». Clôt PO-15 |
| DEC-023 | 2026-11-06 | **Brochage v3** : 2 extensions (`EXT_IN1` analogique GPIO4, `EXT_IN2` numérique GPIO5), `RGB_DATA` sur GPIO6. **Aucune broche sensible utilisée.** Réserve : GPIO7. | Validée | Julien : « OUI ». Applique DEC-018 et DEC-021. Clôt PO-17. Remplace la répartition de DEC-020 |
| DEC-024 | 2026-11-06 | LED RGB WS2812B pilotée via un adaptateur de niveau **74AHCT1G125** alimenté en 5 V, résistance série sur `DIN`, rappel à la masse sur l'entrée. | Validée | Julien : « si compatible avec l'ensemble OK ». Compatibilité : `BROCHAGE_ESP32.md` §5. Clôt PO-16 sauf codes LCSC (AV) |
| DEC-012 | 2026-11-06 | **Assemblage par JLCPCB au maximum.** Julien ne soude que ce que JLCPCB ne peut pas monter. | Validée | Julien. Barrettes femelles : PO-02 |
