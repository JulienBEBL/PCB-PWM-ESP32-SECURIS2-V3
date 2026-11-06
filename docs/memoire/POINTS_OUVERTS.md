# Points ouverts

Un point ouvert bloque ou conditionne une décision. Un point clos reste listé avec sa résolution.

**Criticité :** B = bloque l'architecture, C = bloque le schéma ou la commande, M = à traiter avant firmware ou essais.

## Ouverts propres à la carte ESP32

| N° | Ouvert le | Point | Criticité | Responsable |
|---|---|---|---|---|
| PO-01 | 2026-11-06 | **Modèle de carte de dev.** Besoin ≈ 27 signaux (liste STM32 E01–E17, S01–S09). L'ESP32 classique (DevKitC) offre environ 24 GPIO utilisables, dont plusieurs de démarrage : trop juste. **[Reco]** ESP32-S3-DevKitC-1, variante **N8** (sans PSRAM octale, qui occupe GPIO 35 à 37). À confirmer sur la documentation Espressif avant l'allocation. | **B** | Claude propose, Julien valide |
| PO-02 | 2026-11-06 | Barrettes femelles : référence LCSC, montage traversant par JLCPCB, contacts tulipe pour l'extérieur. Approvisionnement de la carte de dev elle-même (LCSC ou autre) et délai. | C | Claude propose, Julien commande |
| PO-03 | 2026-11-06 | Tenue de la carte de dev en extérieur : vibrations, oxydation des contacts, CEM. Maintien mécanique (vis, collier, entretoise). La carte de dev n'est pas conçue pour STM32 DEC-040 / DEC-041. | C | Julien (méca), Claude (analyse) |
| PO-04 | 2026-11-06 | **États des broches de l'ESP32 au reset et au démarrage** (broches de configuration, impulsions au boot, rappels internes). Aucune commande de relais, de pompe de charge ou de watchdog sur une broche qui bouge. À vérifier sur la datasheet ESP32-S3 avant l'allocation. | **B** | Claude |
| PO-05 | 2026-11-06 | Coupure radio (DEC-007) : le matériel radio reste présent sur la carte de dev, la coupure est logicielle (radio jamais initialisée). Faut-il une preuve (mesure, revue de code) ? Le client accepte-t-il une radio présente mais inactive ? | M | Julien (client) |
| PO-07 | 2026-11-06 | Alimentation : 5 V externe vers la broche 5 V de la carte de dev. Conflit possible si l'USB est branché en même temps (retour de courant). 3,3 V des circuits de la carte : régulateur de la carte de dev ou régulateur propre (U2 STM32). Budget 500 mA (STM32 DEC-032). | C | Claude propose, Julien valide |
| PO-08 | 2026-11-06 | Mesure isolée du courant du vérin (I_MON, U8 / U9 / U10) : diagnostic, pas sécurité. Garder (carte jumelle) ou supprimer pour simplifier ? | C | Julien |
| PO-09 | 2026-11-06 | Liste des E/S ESP32 : reprise de STM32 E01–E18, S01–S09, X01–X08. Liaison série J10 utile ou non (l'USB de la carte de dev sert au moniteur série). | C | Claude propose, Julien valide |
| PO-10 | 2026-11-06 | Dimensions de la carte (STM32 : 120 × 80 mm) avec la carte de dev enfichée. | C | Julien |
| PO-11 | 2026-11-06 | **Date limite de commande JLCPCB** pour une réception fin novembre. Estimation : 5 à 7 jours de fabrication 4 couches + assemblage, 3 à 5 jours de transport express. Commande au plus tard le **14 novembre** environ. **[Hypothèse]** Délais à confirmer sur le devis JLCPCB. | C | Julien |
| PO-12 | 2026-11-06 | Contenu de l'autocontrôle au démarrage (DEC-011) : tensions, relecture relais au repos, cohérence NO / NC des fins de course, présence 12 V, cavalier, PWM. | M | Claude propose, Julien valide |

## Ouverts communs aux deux cartes (suivis dans le dépôt STM32)

Ces points ont la même réponse pour les deux cartes. Ils sont suivis dans le dépôt STM32 pour éviter deux sources divergentes.

| N° STM32 | Point | Criticité |
|---|---|---|
| PO-03 | Plage de tension du 12 V | B |
| PO-04 | Niveau du PWM (3,3 / 5 V) — non bloquant, étage d'entrée compatible | C |
| PO-05 | Règles de la FSM | B pour firmware |
| PO-07 | Type des capteurs d'extension | C |
| PO-12 | Nombre et sens des LED, type de buzzer | M |
| PO-18 | Version du vérin | B |
| PO-19 | Intégration de l'arrêt matériel | B |
| PO-23 | Courant minimal des fins de course | C |
| PO-27 | Capteur de courant isolé (lié à PO-08 ici) | C |

## Clos

| N° | Clos le | Point | Résolution |
|---|---|---|---|
| PO-06 | 2026-11-06 | Niveau du PWM | Doublon de STM32 PO-04, suivi là-bas. Non bloquant |
