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
| DEC-001 | 2026-11-06 | Carte de repli de la carte STM32 V3, au cas où celle-ci n'aboutit pas ou ne fonctionne pas. Conçue **manuellement par Julien**, Claude en appui (relecture, documentation, firmware). | Validée | Julien : « faire de façon manuelle, avec toi en aide » |
| DEC-002 | 2026-11-06 | **Même cahier des charges dans l'ensemble** que la carte STM32. Objectif : remplacer le STM32 par un ESP32 et simplifier si possible. | Validée | Julien |
| DEC-003 | 2026-11-06 | **Aucune défaillance simple ne doit provoquer l'action** (reprise de STM32 DEC-020, sans dérogation). | Validée | Julien : « ça doit être le cas pour cette carte » |
| DEC-004 | 2026-11-06 | Niveau retenu : **carte jumelle**. Tous les blocs de sécurité de la carte STM32 sont conservés (pont DPDT, relecture, pompe de charge, watchdog externe, cavalier, opto 12 V, protection 12 V). Seules disparaissent les fonctions fournies par la carte de dev. | Validée | Julien : « OK ». Option « carte minimale » écartée car contraire à DEC-003 |
| DEC-005 | 2026-11-06 | **Clarté et simplicité pour Julien** : documents courts, une étape à la fois, explications explicites. | Validée | Julien : « il faut que ce soit clair et simple pour moi » |
| DEC-008 | 2026-11-06 | Usage du POC : **chez le client, en extérieur**. Les exigences d'environnement (STM32 DEC-004, DEC-040) s'appliquent. | Validée | Julien |
| DEC-013 | 2026-11-06 | Dépôt séparé `JulienBEBL/PCB-PWM-ESP32-SECURIS2-V3`, alimenté par les informations du dépôt STM32 (copie figée dans `references/stm32_v3/`). | Validée | Julien |
| DEC-014 | 2026-11-06 | **Délai** : carte reçue et testée fin novembre 2026. Conception (schéma + PCB) en 2 semaines maximum. | Validée | Julien. Date de commande au plus tard : PO-11 |

## Microcontrôleur et firmware

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-006 | 2026-11-06 | MCU = **carte de développement ESP32 enfichée** sur barrettes femelles, amovible. Pas de module ESP32 soudé. Remplace STM32 DEC-035 et DEC-036. | Validée | Julien : « ESP32 rajouté manuellement, qui peut s'enlever, avec les broches ». Modèle : PO-01 |
| DEC-007 | 2026-11-06 | **Toutes les communications radio coupées** : Wi-Fi, Bluetooth, et toute autre liaison sans fil. | Validée | Julien. Mise en œuvre et preuve : PO-05 |
| DEC-011 | 2026-11-06 | Autocontrôle au démarrage réalisé par l'ESP32, si faisable. | À confirmer | Julien. Contenu de l'autocontrôle : PO-12 |

## Matériel et fabrication

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-009 | 2026-11-06 | PCB **4 couches** accepté (empilement STM32 DEC-070 repris). | Validée | Julien |
| DEC-010 | 2026-11-06 | **Entrées d'extension capteurs et buzzer conservés.** | Validée | Julien |
| DEC-012 | 2026-11-06 | **Assemblage par JLCPCB au maximum.** Julien ne soude que ce que JLCPCB ne peut pas monter. | Validée | Julien. Barrettes femelles : PO-02 |
