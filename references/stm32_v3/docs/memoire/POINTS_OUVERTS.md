# Points ouverts

Un point ouvert bloque ou conditionne une décision. Un point clos reste listé avec sa résolution.

**Criticité :** B = bloquant pour l'architecture, C = bloquant pour le schéma, M = à traiter avant firmware ou essais.

## Ouverts

| N° | Ouvert le | Point | Criticité | Impact | Responsable |
|---|---|---|---|---|---|
| PO-01 | 2026-11-02 | **Le L12-30-50-12-S ne convient pas.** Avec 30 N vers la sortie (DEC-017) : effort max 22 N < 30 N, le vérin ne peut pas rentrer contre la charge ; réversibilité 12 N < 30 N, la charge sort la tige dès que le moteur n'est plus alimenté. | **B** | Faisabilité de la fonction de sécurité | Julien (via PO-18) |
| PO-03 | 2026-11-02 | Plage de tension du 12 V fourni par le système (min / max, batterie ou alimentation régulée). Tension max du vérin : 13,5 V. Session 03 : alimentation externe encore inconnue. **[Reco]** Concevoir la protection côté 12 V pour une source non maîtrisée (inversion, surtension, coupure au-delà de 13,5 V) en attendant. | **B** | Protection surtension côté puissance | Julien |
| PO-04 | 2026-11-02 | Niveau logique du PWM : 3,3 V ou 5 V. | C | Étage d'entrée. Non bloquant si l'entrée est conçue compatible 3,3 / 5 V | Julien (vérification) |
| PO-05 | 2026-11-02 | Règles de la FSM : liste des états, seuils PWM (ON / OFF, hystérésis), temporisations, séquences, conditions de transition, délai max entre perte PWM et retour sûr, séquence d'armement logiciel au démarrage. | **B** pour firmware | Spécification logicielle, exigences du CdCF | Julien décrit (annoncé), Claude formalise |
| PO-07 | 2026-11-02 | Entrées capteurs d'extension : nombre, type (contact sec, numérique, analogique), tension. | C | Réservation d'E/S et connecteur | Julien |
| PO-09 | 2026-11-02 | Courant réellement disponible sur le 5 V du contrôleur externe (hypothèse 500 mA). | C | Bilan de puissance, choix des relais | Julien |
| PO-10 | 2026-11-02 | Facteur de marche 20 % du vérin : la datasheet ne définit pas la période de référence. | M | Temps de repos minimal imposé par le firmware | Julien / Actuonix |
| PO-11 | 2026-11-02 | Réglage mécanique : les fins de course externes « rentré » doivent être actionnées quand le vérin est arrêté sur sa fin de course interne (0,5 mm de l'extrémité). | M | Détection fiable de la position sûre | Julien (méca) |
| PO-12 | 2026-11-02 | Nombre de LED (3 ou 4), signification de chaque état. Type de buzzer (actif / passif), niveau sonore. | M | Allocation E/S, BOM | Julien |
| PO-17 | 2026-11-02 | Plage −10 / +50 °C : température ambiante extérieure ou température interne du boîtier ? | C | Choix et derating des composants | Julien |
| PO-18 | 2026-11-02 | **Choix de la version du vérin.** Proposition Claude : **L12-10-210-12-S** (course 10 mm, réduction 210:1). Voir `docs/composants/ACTIONNEUR_L12.md`. Julien fait des essais avec le vérin et une mécanique sommaire (mercredi suivant le 2026-11-02). | **B** | Faisabilité, intégration mécanique (longueur fermée différente) | Julien |
| PO-19 | 2026-11-02 | **Intégration de l'arrêt matériel** (principe validé, DEC-029). Un contact inverseur a un seul commun : il ne peut pas servir à la fois côté 12 V et côté commande sans rompre l'isolation (DEC-021). Il faut donc un 5e contact dédié. À définir : position de déclenchement (après les contacts « sorti » lus par le MCU, avant 4 mm), relecture par opto (oui / non). Pouvoir de coupure : jugé suffisant par Julien (DEC-038). | **B** | Schéma puissance, mécanique | Julien |
| PO-20 | 2026-11-02 | Effort de 30 N estimé : à mesurer ou à borner (valeur max). La marge de tenue du L12-x-210 est de 45 N / 30 N = 1,5 sur la base de l'estimation. | C | Choix de la réduction | Julien |
| PO-22 | 2026-11-02 | Butée mécanique externe en position d'action : prévue ? (Question de PO-21 restée sans réponse.) | C | Comportement en cas d'échec des arrêts électriques | Julien |
| PO-23 | 2026-11-02 | Courant minimal de commutation des fins de course lus à sec (contacts argent ou dorés). Il fixe le courant de rappel des entrées. Nécessite la référence ou la datasheet des microrupteurs. | C | Fiabilité de lecture, valeur des résistances de rappel | Julien |
| PO-27 | 2026-11-03 | Capteur de courant isolé pour X05 (`I_MON`) : options A / B / C dans `SYNOPTIQUE_MATERIEL.md` §B12, recommandation A, plage (vérin ≤ 200 mA, blocage 185 à 200 mA), précision utile. | C | Schéma puissance, BOM | Claude propose, Julien valide |
| PO-29 | 2026-11-03 | Sélection des références : **traitée en session 12** (`hardware/BOM_CIBLE.md` v1, codes LCSC vérifiés en ligne). Restent : codes des barrettes J7, J8, J10, J11, du shunt R504 et de R502. **Attente du jalon G1 (Julien).** | C | Schéma, BOM | Julien (G1) |
| PO-30 | 2026-11-03 | Revue du synoptique matériel v1 (`docs/architecture/SYNOPTIQUE_MATERIEL.md`) et du brochage v2. | C | Schéma | Julien |
| PO-38 | 2026-11-03 | Défauts de l'API. Phase 3 : export de netlist bloquant confirmé ; `createNetLabel`, `lib_Symbol.getDocumentSource` sans réponse ; erreurs passagères `cmdKey`, `create failed!` ; `create` parfois sans réponse ; **lot 3 : après un `create` sans réponse, toute `modify` échoue (`cmdKey`)** jusqu'à intervention dans l'éditeur. Contournements : consignes S6, S14, S15. Restent : `importChanges()`, API PCB, DRC. **Phase 4 :** `importChanges()` attend le clic « Apply » de Julien ; contour de carte par polyligne seulement (`pcb_PrimitiveLine` refusé sur la couche 11) ; `pcb_Drc.check` donne le détail ; 198 déplacements PCB sans échec ; `getPrimitivesBBox` donne l'encombrement réel. **Import PCB :** après une modification de page, `importChanges` + « Apply » sort de la carte les composants de cette page (nouveaux identifiants) : replacer depuis le plan. Suppressions de textes du schéma parfois rétablies au rechargement.  | **C** | Bloque la saisie en cours | Julien (relance de l'éditeur), Claude Code |
| PO-41 | 2026-11-04 | Fiches à vis des borniers J1 à J7 : codes LCSC et compatibilité avec les embases Kangnex à confirmer avant commande. | M | Câblage | Claude propose, Julien commande |
| PO-42 | 2026-11-05 | DEC-078 met `K2_FB` (PB1) sur le même port que `K1_FB` (PB7), alors que le plan de brochage les séparait (un défaut de registre ne doit pas fausser les deux relectures). Piste : échanger `K2_FB` et `ARM_FB` (PA1, broche 12, voisine). À trancher avec Julien avant de finir le routage. | C | Sûreté de la relecture des relais | Julien |

## Clos

| N° | Clos le | Point | Résolution |
|---|---|---|---|
| PO-02 | 2026-11-02 | Sens de l'effort de 30 N | Vers la sortie de tige (DEC-017) |
| PO-13 | 2026-11-02 | Licence Apache 2.0 sur le dépôt | Conservée, open source accepté (DEC-053) |
| PO-06 | 2026-11-02 | Mode de lecture des fins de course | Antivalence NO + NC, 8 entrées (DEC-037). Référence et pouvoir de coupure DC des contacts : à fournir (rattaché à PO-19) |
| PO-08 | 2026-11-02 | Choix du microcontrôleur | STM32G071CBT6 LQFP48 (DEC-036) |
| PO-28 | 2026-11-03 | Révision de la datasheet | DS12232 Rev 5 fournie et ajoutée. Vérifications refaites ; errata ES0418 Rev 6 analysée. Conséquence : brochage v2 (R9) |
| PO-26 | 2026-11-03 | Montage du quartz HSE | Monté (DEC-060) |
| PO-14 | 2026-11-03 | Mesure du courant du vérin souhaitée ? | Oui, retenue via X05 (DEC-039). Dimensionnement : PO-27 |
| PO-24 | 2026-11-03 | Revue de la liste des E/S | Pas d'ajout, X01 à X08 retenus (DEC-039) |
| PO-25 | 2026-11-03 | Vérification du plan de brochage (UCPD, remappage, domaine de sauvegarde, état au reset, tolérance 5 V) | Vérifié sur DS12232 et le cœur STM32duino (`PLAN_BROCHAGE_MCU.md` §4). Nouvelle contrainte : PA4 / PA5 tolérantes 3,6 V seulement |
| PO-31 | 2026-11-03 | Méthode de travail EasyEDA Pro avec Claude | Pilotage par Claude Code via l'API, jalons de validation par Julien (DEC-061) |
| PO-21 | 2026-11-02 | Origine des 3 mm utiles | Depuis la butée « rentré » (DEC-016). Question de la butée mécanique reportée en PO-22 |
| PO-34 | 2026-11-03 | Compatibilité du skill `easyeda-api` et de l'extension Run API Gateway avec EasyEDA Pro V3.2.209 Full Online ; défaut « attributs fournisseur perdus » | Compatible sur le périmètre de la phase 0 (T1 à T6 OK). Défaut des attributs non reproduit ; ERC en totaux seulement. Non testé : suivi par PO-38. `hardware/rapports/RAPPORT_PHASE_0.md` |
| PO-35 | 2026-11-03 | Précision de la coupure en surtension 12 V (LM74502) | Option (a) acceptée par Julien : DEC-065 |
| PO-37 | 2026-11-03 | Validation conjointe des jalons G1 et G2 | Validés par Julien : DEC-064 |
| PO-15 | 2026-11-02 | Dimensions de la carte, type et position des connecteurs, fixation | 100 × 80 mm, 4 trous M3, connecteurs sur deux bords opposés (DEC-068) |
| PO-33 | 2026-11-03 | Distances d'isolement entre domaines, fente de fraisage | ≥ 2 mm entre cuivres, pas de fente ; relais limités à 1,1 mm par leur empreinte (DEC-069) |
| PO-32 | 2026-11-03 | Nombre de couches du PCB | 4 couches, S / GND / Alim / S (DEC-070) |
| PO-39 | 2026-11-03 | Points de test | TP1 à TP7 placés (DEC-074) |
| PO-40 | 2026-11-04 | Avertissement ERC du schéma | « Component attributes does not match the Supplier Part » sur J7 (C8447, fiche de bibliothèque incomplète). J7 remplacé par C8377 : ERC 0 erreur, 0 avertissement |
| PO-16 | 2026-11-03 | Liaison de diagnostic vers un PC | USB-C + CH340C sur USART1, J10 sur USART2 (DEC-066) |

| PO-36 | 2026-11-04 | Brochages à lire sur symboles (relais, buzzer, ESD, SRV05-4, optos) | Relais confirmés sur la datasheet Omron ; autres lus sur les images des symboles LCSC (rapport lot 3 §7 ter) |