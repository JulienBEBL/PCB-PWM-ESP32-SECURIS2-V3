# Journal

Historique des échanges, session par session. Les décisions sont dans `DECISIONS.md`, les questions dans `POINTS_OUVERTS.md`.

## Session 01 — 2026-11-06 (chat claude.ai)

**Demande de Julien.** Créer, en parallèle de la carte STM32 conçue par IA, une carte de repli basée sur un ESP32, conçue à la main avec l'aide de Claude. Même CdC, aussi simple que possible.

**Échanges.**

- Claude a proposé un périmètre réduit (POC de banc). Julien a précisé : POC utilisé chez le client, en extérieur ; entrées d'extension et buzzer conservés ; PCB 4 couches accepté ; ESP32 enfiché et amovible, pas soudé.
- Claude a signalé trois incohérences : simplicité contre usage terrain ; MCU enfiché contre STM32 DEC-035 ; délai (commande vers le 14 novembre pour une réception fin novembre).
- Claude a proposé deux niveaux : carte jumelle ou carte minimale. La carte minimale ne respecte pas « aucune défaillance simple ». Julien a retenu la **carte jumelle**, avec l'exigence que tout reste clair et simple pour lui.
- Julien demande la coupure de toutes les communications radio.
- « RPI » dans un message précédent désignait l'ESP32 : l'autocontrôle est fait par l'ESP32 lui-même.
- Claude a recommandé l'ESP32-S3-DevKitC-1-N8 (assez de broches ; l'ESP32 classique est trop juste). Non validé.

**Travaux.**

- Création de la structure du dépôt : `CLAUDE.md`, mémoire, `docs/REPRISE_STM32.md`.
- Copie figée du dépôt STM32 (commit `e73ff39`) dans `references/stm32_v3/`.
- Décisions DEC-001 à DEC-014. Points ouverts PO-01 à PO-12.

**Suite.** Validation de la carte de dev par Julien, puis liste des E/S et brochage.

## Session 02 — 2026-11-06 (chat claude.ai)

**Décisions de Julien.** ESP32-S3-DevKitC-1-N8 validée (DEC-015). Mesure isolée du courant supprimée (DEC-016).

**Travaux.**

- Vérification sur la documentation Espressif : guide DevKitC-1 v1.1 (brochage J1 / J3, variantes, alimentation) et datasheet ESP32-S3 v2.2 (état au reset, impulsions au démarrage, broches de démarrage, broches flash). Clôt PO-04.
- Brochage v1 : `docs/architecture/BROCHAGE_ESP32.md`. 30 broches, aucune réserve. Sorties de sécurité actives à l'état haut, hors broches à impulsion haute, commandes et validations sur registres GPIO différents.
- Constats nouveaux : la LED RGB est sur GPIO48 en v1.0 et GPIO38 en v1.1 ; les entrées USB et 5 V de la carte de dev sont exclusives (PO-07) ; le brochage n'est valable que pour la variante N8.
- Propositions en attente (PO-13) : suppression de X04 et de J10.

**Suite de la session 02.** Julien valide le brochage v1, la suppression de X04 et de J10 (DEC-017). Il pose le principe de ne pas utiliser les broches sensibles s'il y a de la marge (DEC-018). Pas de marge en v1 : arbitrage ouvert (PO-14).

**Suite de la session 02.** Pour libérer les broches sensibles, Julien valide une LED RGB adressable à la place des 4 LED (DEC-019). Brochage v2 : seules `RGB_DATA` (GPIO46) et `MAINT_IN` (GPIO3) restent sur des broches sensibles ; `FC_S2_NC` passe sur GPIO36 (DEC-020). Réserve : GPIO18, 45, 48. Maintien de `MAINT_IN` sans réponse (PO-15).

**Suite de la session 02.** Julien accepte 2 extensions au lieu de 4 si besoin (DEC-021). Question sur l'entrée maintenance : Claude recommande de la supprimer (PO-15). WS2812B : seuil d'entrée 0,7 × VDD incompatible avec une sortie 3,3 V, adaptateur de niveau recommandé (PO-16). Proposition : aucune broche sensible utilisée (PO-17).

**Suite de la session 02.** Julien supprime l'entrée maintenance (DEC-022), valide 2 extensions et la LED RGB sur GPIO6 : brochage v3 sans aucune broche sensible (DEC-023). Adaptateur 74AHCT1G125 validé sous réserve de compatibilité, vérifiée dans `BROCHAGE_ESP32.md` §5 (DEC-024).

**Suite de la session 02.** Proposition d'alimentation v1 (`ALIMENTATION_ESP32.md`) : U15 sur J1, U16 réaffectée vers la broche 5V de la carte de dev, +3V3 pris sur la carte de dev (SGM2212, 800 mA), U2 supprimé. Effet : l'USB seul n'alimente pas les relais. Schéma Espressif lu en extrait texte : position des Schottky D1 / D7 non confirmée (PO-18, test au multimètre).

## Session 02 (suite) — 2026-11-06

**Décisions de Julien.** Alimentation validée (DEC-026). Schéma et PCB saisis par Claude Code via l'API EasyEDA, contrôlés par Julien, très simples et clairs (DEC-025).

**Travaux.**

- `hardware/NETLIST_CIBLE.md` v1 et `hardware/BOM_CIBLE.md` v1 (+ CSV), dérivées de la carte STM32 : blocs repris à l'identique, modifications marquées.
- Modifications de fond relevées en dérivant : R413 passe de 20 à 15 kΩ (`ARM_FB` dépassait 3,3 V, l'ESP32 n'est pas tolérant 5 V) ; R504 supprimé avec la mesure de courant ; signaux d'extension limités à 3,3 V.
- `hardware/CONSIGNES_EASYEDA_CLAUDE_CODE.md` : §0 propre à l'ESP32 (copie du projet STM32 comme base, règles de clarté C1 à C10, interdiction d'écrire dans le projet STM32).
- Scripts de routage STM32 non copiés : ils contiennent l'uuid du projet STM32 en dur.
- PO-07, PO-19, PO-20 clos. Nouveaux : PO-21 (écart des rangées), PO-22 (délai de la saisie par l'API).

## Session 02 (fin) — 2026-11-06

**Décisions de Julien.** G1 et G2 validés, travail en autonomie (DEC-027). Nouveau projet EasyEDA vierge sur le PC personnel, pas de copie du projet STM32 (DEC-028). Carte de dev commandée (DEC-029).

**Travaux.**

- Consignes, guide d'installation et netlist adaptés au projet vierge sur le PC personnel.
- Firmware v0 de mise en service : mesure PWM, fins de course, relectures, autocontrôle, watchdogs, LED RGB, STATUS, journal série. Aucune commande du vérin. Radio jamais initialisée. Vérification de syntaxe seulement (pas de chaîne ESP32 dans l'environnement).
- PO-09 clos. Nouveau : PO-23 (Julien annonce « travailler sur la partie PCB » : rôle à préciser).
