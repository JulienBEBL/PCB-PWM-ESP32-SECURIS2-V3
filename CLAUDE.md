# CLAUDE.md — Reprise du projet

Lu automatiquement par Claude Code. Toute session Claude (Claude Code ou chat claude.ai) qui reprend le projet lit ce fichier en premier.

## 1. Projet en une phrase

Carte de repli de la carte STM32 V3 (`JulienBEBL/PCB_PWM_SECURISE_V3`) : même fonction, même cahier des charges, mais le STM32 soudé est remplacé par une **carte de développement ESP32 enfichée et amovible**. Schéma et PCB saisis par Claude Code via l'API EasyEDA, contrôlés par Julien (DEC-025).

Fonction : lire une consigne PWM (1000–2000 µs, ~50 Hz), exécuter une machine d'états, piloter un vérin Actuonix L12 (12 V) par 2 relais DPDT, avec retour garanti en position sûre (tige rentrée). **Aucune défaillance simple ne doit provoquer l'action.**

## 2. Ordre de lecture

1. `docs/memoire/ETAT_PROJET.md` — où en est le projet, prochaine étape, planning.
2. `docs/memoire/DECISIONS.md` — décisions validées par Julien.
3. `docs/memoire/POINTS_OUVERTS.md` — questions en attente.
4. `docs/memoire/JOURNAL.md` — historique, session par session.
5. `docs/REPRISE_STM32.md` — ce qui est repris, adapté ou supprimé de la carte STM32.
6. Selon le sujet : `references/stm32_v3/` (copie figée du dépôt STM32).

## 3. Rôles

- **Julien** : décide, contrôle et valide schéma et PCB à chaque jalon, commande chez JLCPCB, teste.
- **Claude** : propose, rédige la documentation, écrit le firmware, gère ce dépôt (commit, push).
- **Claude Code** : saisit le schéma et le PCB en pilotant EasyEDA Pro par l'API, selon `hardware/CONSIGNES_EASYEDA_CLAUDE_CODE.md`. **Schéma, placement et routage très simples et clairs.** Ne modifie jamais le projet EasyEDA de la carte STM32.

## 4. Règles de travail

- **Rester clair et simple pour Julien** (DEC-005) : une étape à la fois, documents courts, tableaux, pas de jargon inutile.
- Distinguer **[Exprimé]**, **[Décidé]**, **[Hypothèse]**, **[Reco]**.
- Ne jamais transformer une hypothèse en donnée validée. Aucune valeur sans source.
- Signaler toute incohérence, en particulier avec les décisions héritées du dépôt STM32.
- Délai court (DEC-014) : prioriser ce qui bloque la commande JLCPCB.
- Style : français technique, phrases courtes, voix active.

## 5. Gestion du dépôt

- Branche `main`.
- En fin de session : mettre à jour `ETAT_PROJET.md`, `DECISIONS.md`, `POINTS_OUVERTS.md`, ajouter une entrée au `JOURNAL.md`, puis commit et push.
- Commits en français, préfixés : `docs:`, `hw:`, `fw:`, `test:`, `chore:`.
- Numérotation propre à ce dépôt : `DEC-xxx`, `PO-xx`. Une référence au dépôt STM32 s'écrit `STM32 DEC-xxx` ou `STM32 PO-xx`.
- Un numéro n'est jamais réutilisé ; un élément clos est marqué clos, pas supprimé.
- `references/stm32_v3/` est une copie figée (commit `e73ff39` du dépôt STM32, 2026-11-06). Ne pas la modifier. Pour un état plus récent, consulter le dépôt STM32.

## 6. Arborescence

| Dossier | Contenu |
|---|---|
| `docs/memoire/` | État, décisions, points ouverts, journal |
| `docs/architecture/` | Architecture ESP32, liste des E/S, brochage |
| `docs/REPRISE_STM32.md` | Héritage de la carte STM32 |
| `hardware/` | Projet EasyEDA, exports, Gerber, BOM, CPL |
| `firmware/` | Code Arduino ESP32 |
| `tests/` | Procédures et rapports d'essais |
| `references/` | CdC générique et copie figée du dépôt STM32 (non modifiables) |
