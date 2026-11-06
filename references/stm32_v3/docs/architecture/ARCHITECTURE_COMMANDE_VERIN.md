# Architecture de commande du vérin

**Statut :** principe validé (DEC-022 à DEC-027). Détail du schéma à réaliser.

## 1. Domaines d'isolation (DEC-021)

| Domaine | Référence de masse | Contenu |
|---|---|---|
| Commande | GND du contrôleur externe (commun au 5 V et au PWM) | Microcontrôleur, entrée PWM, fins de course, LED, buzzer, bobines des relais, côté sortie des optocoupleurs |
| Puissance | 0 V du 12 V système | Contacts des relais, vérin, côté LED de l'optocoupleur de présence 12 V |

Franchissements de la barrière d'isolation : relais (bobine / contacts) et optocoupleurs uniquement. Aucune piste, plan ou composant ne relie les deux masses.

**Hypothèse :** les fins de course externes sont lues à sec, côté commande. Elles ne doivent avoir aucune liaison électrique avec le vérin ou son boîtier côté 12 V.

## 2. Pont de 2 relais DPDT

Affectation : borne A = fil **rouge** (V+), borne B = fil **noir** (V−). Rouge au + : sortie de tige.

| Relais | COM (contact 1) | NC (repos) | NO (travail) | Contact 2 |
|---|---|---|---|---|
| K1 | Borne A (rouge) | 0 V | +12 V | Relecture d'état côté commande |
| K2 | Borne B (noir) | +12 V | 0 V | Relecture d'état côté commande |

| K1 | K2 | Rouge | Noir | Effet |
|---|---|---|---|---|
| repos | repos | 0 V | +12 V | **Rentrée de tige → position sûre.** Arrêt par fin de course interne. |
| travail | repos | +12 V | +12 V | Arrêt (bornes au même potentiel, freinage) |
| repos | travail | 0 V | 0 V | Arrêt (bornes au même potentiel, freinage) |
| travail | travail | +12 V | 0 V | **Sortie de tige → position d'action** |

## 3. Propriétés

- Court-circuit du 12 V impossible : chaque COM est relié soit au +12 V, soit au 0 V, jamais aux deux.
- L'action exige deux relais excités.
- Retour en position sûre sans aucune électronique active : il suffit que les deux bobines soient non alimentées.
- Isolation native par les relais.

## 4. Comportement en défaut

| Défaillance | Résultat |
|---|---|
| MCU en reset, planté ou non alimenté | Bobines non alimentées → retour en position sûre (si 12 V présent) |
| Perte du 5 V | Retour en position sûre (si 12 V présent) |
| Perte du 12 V | Aucun mouvement commandé. La tige reste en place seulement si la réversibilité du vérin dépasse l'effort extérieur (30 N vers la sortie, DEC-017). Voir `docs/composants/ACTIONNEUR_L12.md`, section 3. |
| Commande d'un relais en court-circuit ou contact soudé | Arrêt du vérin, jamais d'action. Retour sûr perdu, détecté par relecture (DEC-023). |
| Fil du vérin coupé | Aucun mouvement. Détecté par les fins de course externes et une temporisation de course. |
| Cavalier d'armement absent | Action électriquement impossible. Retour sûr conservé. |

## 5. Chaîne de commande des bobines (proposition, à valider au schéma)

| Bobine | Éléments en série | Rôle |
|---|---|---|
| K1 | Interrupteur commandé par le MCU + interrupteur validé par une pompe de charge (signal alternatif permanent du MCU) | Un MCU bloqué, sortie figée à 1 ou à 0, ne maintient pas K1 |
| K2 | Cavalier d'armement (DEC-025) + interrupteur commandé par le MCU + interrupteur validé par un watchdog externe | Barrière physique et surveillance indépendante du logiciel |

Les deux bobines sont donc tenues par deux principes indépendants. Une sortie MCU figée, un transistor en court-circuit ou un blocage logiciel ne suffisent pas à obtenir l'action.

Compléments à intégrer :

- résistances de rappel garantissant les bobines non alimentées pendant le reset et la programmation ;
- diode de roue libre sur chaque bobine ;
- relecture de l'état du cavalier par le MCU ;
- optocoupleur de présence 12 V (DEC-024) ;
- suppression des parasites du moteur à balais côté puissance (CEM) ;
- protection du 12 V : inversion de polarité, surtension (vérin limité à 13,5 V), fusible.

## 6. Arrêt matériel en position d'action (principe validé DEC-029, intégration PO-19)

La course utile (3 mm, DEC-016) est plus courte que la course du vérin. Sans arrêt matériel, seul le firmware arrête la sortie à 3 mm. Un blocage du firmware est couvert (pompe de charge et watchdog externe font retomber les relais, donc retour sûr). Une erreur de logique avec un firmware actif ne l'est pas : la tige sort jusqu'à la fin de course interne du vérin.

Option : un contact NC de fin de course « sortie » dédié, en série entre le +12 V et le contact NO de K1.

| Propriété | Effet |
|---|---|
| Sortie au-delà de la position d'action | Coupée en matériel, indépendamment du firmware |
| Rentrée (retour sûr) | Non affectée : elle passe par le contact NC de K1, pas par ce contact |
| Diode | Inutile : les chemins de sortie et de rentrée sont distincts |
| Contrepartie | Ce contact est dans le domaine 12 V. Il s'ajoute aux 4 fins de course lus à sec, il n'en fait pas partie : un contact inverseur n'a qu'un commun et ne peut pas desservir les deux domaines sans rompre l'isolation. |
| Position | Après les contacts « sorti » lus par le MCU (le firmware arrête en premier), avant 4 mm (limite haute de DEC-016) |
| Diagnostic | État lisible par le MCU via un optocoupleur, si souhaité |

## 7. Limites acceptées ou à statuer

- Sans 12 V, le retour en position sûre est physiquement impossible (DEC-027 accepte le mouvement à la mise sous tension, pas l'absence de 12 V).
- Un relais standard ne garantit pas que le contact de relecture recopie un contact soudé. Seul un relais à contacts liés le garantirait.
- Une erreur de logique du firmware qui commande l'action alors que le PWM ne la demande pas n'est arrêtée que par le cavalier et le blocage mécanique (DEC-026).
