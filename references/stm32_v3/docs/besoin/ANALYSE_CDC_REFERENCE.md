# Analyse du CdCF de référence

**Document analysé :** `references/cdc_carte_electronique_generique.md` (version de travail du 02/11/2026).
**Statut du document :** référence méthodologique, non applicable par défaut au projet.

## 1. Verdict

Le document est un **gabarit** : 40 exigences (16 F0, 17 F1, 7 F2, 0 F3), deux valeurs chiffrées seulement, toutes deux conditionnelles. Sa valeur tient à sa structure et à sa liste de thèmes. Ses exigences ne sont pas vérifiables en l'état (ni niveau, ni méthode de vérification).

## 2. À réutiliser

- Séparation CdCF / CdCT.
- Principes de sécurité en tête de document.
- Périmètre et hors périmètre.
- Familles d'exigences numérotées par centaines.
- Cycle de fonctionnement comme base de la FSM.
- Tableau des points ouverts avec impact.
- Liste des livrables.

## 3. Faiblesses identifiées

| Faiblesse | Exemple | Correction retenue pour notre CdCF |
|---|---|---|
| Éléments du milieu extérieur incohérents | « Carte / Firmware » et « Sécurité » listés comme éléments extérieurs | Familles thématiques assumées, graphe des interacteurs séparé |
| Doublons | FC201 / FC605 ; FC305 / FC408 / FC411 ; 5 exigences sur l'état des sorties | Une exigence par besoin |
| Exigences non atomiques | FC409, FC411, FC104, FC701 | Découpage |
| Classes de flexibilité mêlant importance et négociabilité | F0 « liée à la sécurité » | Priorité (impératif / souhaitable) distincte de la flexibilité du niveau (NF EN 16271) |
| F0 sans limite | 16 cas « à définir » | F0 interdit sans limite chiffrée ou renvoi à un PO |
| Limites non mesurables | « réduite au minimum », « si possible », « pertinentes » | Listes fermées ou renvoi PO |
| Unités incohérentes ou absentes | FC302, FC601, FC602, FC604 | Unités et conditions d'essai complètes |
| Solutions dans le CdCF | Watchdog, points de test, ESP32, FSM | Rubrique « contraintes de conception imposées » |
| Thèmes absents | Thermique, consommation, performances temporelles, mise à jour firmware, cybersécurité, fabrication (IPC, RoHS), identification, modes dégradé / maintenance, reprise après défaut | Familles ajoutées |
| Traçabilité absente | Document source non identifié ; pas de méthode de vérification | Colonnes Source, Méthode (I/A/E/D), Critère d'acceptation |
| Arrêt maîtrisé incompatible avec une perte d'alimentation non commandée | §8 phase 6 vs FC504 | État sûr garanti passivement (réalisé ici par les relais au repos) |

## 4. Format d'exigence retenu pour notre CdCF

| N° | Description | Critère | Niveau / limite | Unité | Priorité | Flexibilité | Source | Vérification (I/A/E/D) | Critère d'acceptation |
|---|---|---|---|---|---|---|---|---|---|

I = inspection, A = analyse, E = essai, D = démonstration.
