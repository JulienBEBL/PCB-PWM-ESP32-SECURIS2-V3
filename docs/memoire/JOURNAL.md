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
