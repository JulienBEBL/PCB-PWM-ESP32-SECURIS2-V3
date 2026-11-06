# Consignes — schéma et PCB sous EasyEDA Pro par Claude Code (carte ESP32)

**Statut :** v1 ESP32 (2026-11-06), dérivée des consignes STM32 v1.2. Le §0 est propre à la carte ESP32 et **prime** sur la suite en cas de conflit.
**Lecteur :** Claude Code, à chaque session de schéma ou de PCB, après `CLAUDE.md` et la mémoire projet.
**Installation :** `hardware/INSTALLATION_EASYEDA_API.md`.

---

## 0. Spécificités de la carte ESP32

### 0.1 Méthode (DEC-025)

Claude Code saisit le schéma et le PCB par l'API. Julien contrôle et valide à chaque jalon. **Exigence de Julien : schéma, placement et routage très simples et clairs** (DEC-005, DEC-025).

| Étape | Action | Qui |
|---|---|---|
| **E0** | Sur le **PC personnel** de Julien : installation (`INSTALLATION_EASYEDA_API.md`), puis **nouveau projet vide** `PCB_PWM_ESP32_V3` (un schéma, un PCB). Noter son uuid ici (règle S1). Pas de copie du projet STM32 (DEC-028) | Julien |
| E1 | Phase 0 (§9) sur le nouveau projet | Claude Code |
| E2 | Schéma : saisie complète, page par page, depuis `NETLIST_CIBLE.md` (ordre : P1, P2, P3, P3B, P3C, P4, P4B, P5) | Claude Code, jalon G3 |
| E3 | Placement | Claude Code, jalon G4 |
| E4 | Routage, DRC | Claude Code, jalon G5 |
| E5 | Exports Gerber, BOM, CPL | Claude Code, Julien commande |

Les jalons G1 (BOM) et G2 (netlist) sont validés (DEC-027).

**Le projet STM32 ne doit jamais être modifié depuis ce dépôt.** Uuid du projet STM32, interdit en écriture : `b4788f45338c4e59bfcfdff2832a8063`. Il n'est normalement pas présent sur le PC personnel ; s'il apparaît dans l'espace de travail, ne pas l'ouvrir.

### 0.2 Règles de clarté

| N° | Règle |
|---|---|
| C1 | Une fonction par page. Titre de page explicite. Un texte bleu de 2 à 4 lignes en haut de page : rôle du bloc, signaux d'entrée et de sortie, décision de référence |
| C2 | Flux de gauche à droite : entrées à gauche, sorties à droite, alimentations en haut, masses en bas |
| C3 | Liaisons entre pages par étiquettes de net uniquement, noms identiques à `NETLIST_CIBLE.md`. Pas de fil long qui traverse la page |
| C4 | Composants d'une même voie alignés (par exemple : rappel, série, filtre, ESD d'une fin de course sur une ligne) |
| C5 | Carte de dev : symbole en deux barrettes J20 / J21 dessinées dans l'ordre physique des broches, nom du signal ESP32 écrit à côté de chaque broche |
| C6 | Placement PCB : blocs regroupés comme sur le schéma ; composants d'une même voie côte à côte, dans le même ordre |
| C7 | Routage : pistes droites, angles à 45°, le moins de vias possible, pas de piste sous la barrière d'isolation. Règles de largeur et d'écart : DEC-076 et DEC-077 de la carte STM32 |
| C8 | Sérigraphie : nom de chaque connecteur et de chaque broche externe ; contour de la carte de dev avec **« ESP32-S3-DevKitC-1-N8 uniquement »**, repère de la broche 1 et côté USB |
| C9 | Le port USB de la carte de dev enfichée doit rester accessible depuis un bord de la carte (programmation sans démontage) |
| C10 | Sous la carte de dev : composants CMS bas seulement, aucun connecteur ni composant traversant |

### 0.3 Sources de vérité (remplace le §2)

| Rang | Document |
|---|---|
| 1 | `docs/memoire/DECISIONS.md` (et décisions STM32 héritées, `docs/REPRISE_STM32.md`) |
| 2 | `docs/architecture/BROCHAGE_ESP32.md` v3 |
| 3 | `docs/architecture/ALIMENTATION_ESP32.md` |
| 4 | `references/stm32_v3/docs/architecture/ARCHITECTURE_COMMANDE_VERIN.md` |
| 5 | `hardware/BOM_CIBLE.md` et `hardware/bom/BOM_CIBLE.csv` |
| 6 | `hardware/NETLIST_CIBLE.md` |

### 0.4 Adaptations des règles de la suite

| Règle | Adaptation |
|---|---|
| S1 | Uuid du projet `PCB_PWM_ESP32_V3` : **à renseigner à l'étape E0**. Tout uuid égal à celui du projet STM32 : arrêt immédiat |
| S7 | Noms des nets ESP32 : `BROCHAGE_ESP32.md` v3 et `NETLIST_CIBLE.md` P2 |
| V4 | Composants autorisés à toucher les deux domaines : K1, K2, U6, U7 seulement (U9 supprimé) |
| V7 | Remplacé : chaque broche de J20 / J21 conforme à `NETLIST_CIBLE.md` P2 ; aucune liaison sur les broches sensibles ; rappel à la masse sur `K1_CMD`, `K2_CMD`, `RGB_DATA` ; aucun net à plus de 3,3 V vers une broche ESP32 |
| Placement | Pas de placement STM32 à reprendre (projet vierge). Le placement STM32 (`references/stm32_v3/`, rapport non copié) peut servir d'exemple pour la zone 12 V : relire le dépôt STM32 si besoin |
| §8 | Nommage : `AAAA-MM-JJ_PCB_PWM_ESP32_V3_G<n>.epro2` |
| Outils | Les scripts de routage du dépôt STM32 (`hardware/outils/routage/`) contiennent l'uuid du projet STM32 en dur : **ne pas les exécuter tels quels**. Les recopier en changeant l'uuid |

---

## Suite : consignes héritées de la carte STM32 (v1.2)

## 1. Chaîne d'outils

| Élément | Version | Remarque |
|---|---|---|
| EasyEDA Pro, client desktop | V3.2.209 (2026-10-21), mode Full Online | DEC-063. Full Online obligatoire : en Full Offline, l'extension n'est pas chargée |
| Extension Run API Gateway | v1.0.6, cible EasyEDA ~3.2.0 | Options « Allow interactive with external » et « Show at header menu » cochées (libellés V3.2.209 ; la doc de l'extension cite « Allow External Interaction » et « Show in Top Menu ») |
| Skill `easyeda-api` | v1.1.64 ou plus récent | `%USERPROFILE%\.claude\skills\easyeda-api` |
| Pont (bridge) | Node.js, 127.0.0.1, port 49620 à 49629 | **Aucune authentification** : lancé uniquement pendant les sessions |
| Freerouting | v2.1.0 + Java 21 (v2.2+ exige Java 25) | Phase 5 uniquement, si retenu |

Le retour d'expérience publié par JLCPCB porte sur la V3.2.149 en mode local. **Phase 0 (session 13) :** compatibilité vérifiée en V3.2.209 Full Online sur le périmètre testé (PO-34 clos). Restent à qualifier au premier usage : PO-38. Rapport : `hardware/rapports/RAPPORT_PHASE_0.md`.

## 2. Sources de vérité

Ordre de priorité. En cas de contradiction entre deux sources : **arrêt et signalement à Julien**, jamais d'arbitrage silencieux.

| Rang | Document | Usage |
|---|---|---|
| 1 | `docs/memoire/DECISIONS.md` | Décisions validées |
| 2 | `docs/architecture/PLAN_BROCHAGE_MCU.md` v2 et `hardware/brochage_stm32g071cbt6.csv` | Nets du MCU, broche par broche |
| 3 | `docs/architecture/ARCHITECTURE_COMMANDE_VERIN.md` | Pont de relais, chaînes de bobines |
| 4 | `docs/architecture/SYNOPTIQUE_MATERIEL.md` | Blocs B1 à B13, candidats de composants |
| 5 | `hardware/BOM_CIBLE.md` (phase 1) et `hardware/bom/BOM_CIBLE.csv` | Références retenues, après validation G1. **v1 rédigée en session 12, G1 en attente** |
| 6 | `hardware/NETLIST_CIBLE.md` (phase 2) | Connexions cibles, après validation G2. **v1 rédigée en session 12, G2 en attente** |
| 7 | `hardware/PLACEMENT_CIBLE.md` (phase 4) | Règles de placement composant par composant, après G3 |
| — | `docs/memoire/POINTS_OUVERTS.md` | Ce qui n'est pas encore décidé |

## 3. Séquence et jalons

Séquence imposée par Julien (DEC-061) : schéma → retouche et validation par Julien → routage → retouche et validation par Julien. Les phases 1 et 2 sont des prérequis ajoutés par Claude **[Reco]**.

| Phase | Contenu | Livrable | Jalon (Julien) |
|---|---|---|---|
| 0 | Mise en service : pont, fenêtre EDA, projet actif, test lecture seule, test d'écriture sur page d'essai | Rapport de test (§9) | — |
| 1 | Sélection des références (PO-29) : LCSC, Basic / Extended, stock, prix, paramètres vérifiés sur datasheet | `hardware/BOM_CIBLE.md` | **G1** |
| 2 | Netlist cible : chaque broche de chaque composant → net, avec justification et source | `hardware/NETLIST_CIBLE.md` | **G2** |
| 3 | Saisie du schéma, page par page ; relecture et comparaison automatique à la netlist cible | Projet EasyEDA, PDF, rapport | **G3** |
| 4 | Placement selon les règles de zone (§7) | Projet EasyEDA, captures, rapport | **G4** |
| 5 | Routage (§7), DRC | Projet EasyEDA, rapport | **G5** |
| 6 | Exports de fabrication (Gerber, BOM, CPL) | `hardware/fabrication/` | Julien commande |

Règles :

- Claude Code **s'arrête à chaque jalon** et attend la validation explicite de Julien. Julien peut retoucher ou faire lui-même avant de valider.
- Après une retouche de Julien, Claude Code relit le projet et met à jour la netlist cible avant de reprendre.
- Aucune valeur n'est inventée. Une valeur non décidée est saisie `A_DEFINIR` et listée dans le rapport avec le PO correspondant.

### Pages du schéma et dépendances

| Page | Blocs | Saisissable dès G2 si… |
|---|---|---|
| P1 Alimentation commande | B1, B2 | PO-29 traité |
| P2 Microcontrôleur | B3, J9 SWD, J10 UART2, SW1 RESET, SW2 BOOT | Saisie (lots 1 et 2) |
| P3 Entrées | B4 PWM, B5 fins de course, B6 extension | PO-23 (valeur des rappels), PO-07 (configuration extension) |
| P4 Commande relais et IHM | B7, B13 | PO-12 (LED, buzzer), PO-29 (watchdog) |
| P5 Puissance 12 V | B9, B10, B11, B12 | PO-03 (seuils), PO-19 (5e contact), PO-27 (I_MON) ; PO-18 sans effet attendu sur le schéma |
| P6 USB vers PC | J12 USB-C, U13 CH340C, U14 ESD, D16 | Saisie (lot 2) |

Ordre de saisie recommandé : P1, P2, puis P3 et P4, P5 en dernier.

## 4. Règles de saisie

| N° | Règle | Raison |
|---|---|---|
| S1 | **Avant chaque lot d'écriture, vérifier que le projet actif a l'uuid `b4788f45338c4e59bfcfdff2832a8063`** (`PCB_PWM_SECURISE_V3`). Le nom ne suffit pas. Sinon : arrêt. | Les projets clients sont dans le même espace de travail. Un projet modifié en V3 ne s'ouvre plus à jour en V2.2. Un projet local homonyme peut exister (incident Full Offline, phase 0). |
| S2 | Ne jamais appeler `openProject` sans enregistrement préalable : il peut perdre les modifications non enregistrées. Ne jamais supprimer un projet, un document ou une page sans accord de Julien. | Skill `easyeda-api`, SKILL.md |
| S3 | Composants : uniquement des composants de la bibliothèque LCSC avec empreinte, résolus par leur code LCSC (`lib_Device`), à partir de `BOM_CIBLE.md` validée. | Assemblage JLCPCB (DEC-062) |
| S4 | Après tout placement ou modification, réécrire **en un seul appel** tous les attributs fournisseur (code LCSC, MPN, fabricant, nom). Puis relire et vérifier. | Défaut constaté en V3.2.149 : attributs perdus après placement API ou après une modification partielle. Non reproduit en V3.2.209 (phase 0, un seul composant) : règle conservée par prudence, coût nul. |
| S5 | Lire la position réelle des broches avant de tracer un fil. | Les fils doivent aboutir exactement sur les broches |
| S6 | Connexions par fils courts et étiquettes de net. **Un seul nom de net par fil.** Broches inutilisées : drapeau « non connecté » explicite. Étiquette = nom du fil : `sch_PrimitiveWire.create(line, net)` crée un attribut `Name` visible. Ne pas utiliser `createNetLabel` (EDA v4, sans réponse en V3.2.209). | Lisibilité, vérification ; erreur « Wire has multiple net names » |
| S7 | Noms des nets du MCU : ceux du CSV, sans modification. | Traçabilité schéma ↔ firmware |
| S8 | Nets d'alimentation **[Reco]**, à valider en G2 : `+5V`, `+3V3`, `GND` (domaine commande) ; `+12V_P`, `GND_P` (domaine puissance isolé). | DEC-021 : les deux masses ne doivent jamais porter le même nom |
| S9 | Repères : ceux du synoptique (J1 à J10, K1, K2…). | Cohérence documentaire |
| S10 | Code envoyé à EasyEDA : une seule ligne, sans commentaire, `await` sur chaque promesse, résultat renvoyé par `return`. Valeurs d'énumération par leur nom, jamais par leur valeur numérique. Les énumérations `ESYS_*` ne sont pas toutes exposées : omettre un paramètre optionnel plutôt que passer sa valeur. | Contraintes du skill ; constat phase 3 |
| S11 | Lots courts : chaque appel doit finir en moins de 30 s. Enregistrer le document après chaque lot. | Délai maximal par appel du pont |
| S12 | Identifier projets, pages et documents par leur uuid. Comparer les noms sans tenir compte de la casse. | Casse incohérente selon la méthode : `getSchematicPageInfo` et `getAllBoardsInfo` renvoient `essai`, `p1` ; `getAllSchematicPagesInfo` renvoie `ESSAI`, `P1` (phase 0) |
| S13 | Vérifier le document actif (`dmt_SelectControl.getCurrentDocumentInfo()`) avant chaque lot. Ne jamais supprimer la page active : activer une autre page d'abord. | Les API SCH agissent sur le document actif |
| S14 | Placement : passer le nom du sous-ensemble (`lib_Device.get(uuid, lib).subPartNames[0]`) à `create`. Rotation : par `modify` (convention opposée à celle de `create`), puis relecture des broches. | Sans sous-ensemble, `create` peut rester sans réponse ; rotations de signe opposé (phase 3) |
| S15 | Erreur API passagère (`cmdKey`, `create failed!`) : vérifier l'état en lecture, puis **une seule** nouvelle tentative. Sans réponse après 30 s : vérifier l'état, ne pas relancer la même méthode. | R7 du brief de phase 0 ; constats phase 3 |

## 5. Vérification du schéma

La réussite d'un appel API ne prouve pas que le schéma est correct. La vérification se fait par relecture.

| N° | Contrôle | Méthode | Critère d'acceptation |
|---|---|---|---|
| V1 | Netlist | Reconstruite par relecture des composants, broches, fils et étiquettes (l'export de netlist peut bloquer) ; comparaison à `NETLIST_CIBLE.md` | 0 broche manquante, 0 connexion en trop, 0 net inattendu |
| V2 | Attributs fournisseur | Relecture de chaque composant | Code LCSC = `BOM_CIBLE.md`, aucun avertissement de non-concordance |
| V3 | ERC | `sch_Drc.check` : totaux par catégorie seulement (`[{type, count}]`) ; le mode strict échoue dès un avertissement | Julien colle le journal ERC ; 0 erreur (mode non strict), avertissements justifiés |
| V4 | Barrière d'isolation | Script : composants dont les broches touchent `GND`/`+5V`/`+3V3` **et** `GND_P`/`+12V_P` | Uniquement K1, K2, optocoupleurs, capteur de courant isolé |
| V5 | Commande des relais | Revue | Rappel à la masse sur `K1_CMD` et `K2_CMD` ; diode de roue libre sur chaque bobine ; cavalier J7 en série avec la chaîne de K2 ; pompe de charge sur K1, watchdog externe sur K2 (`ARCHITECTURE_COMMANDE_VERIN.md` §5) |
| V6 | Pont du vérin | Revue | Conforme au tableau de `ARCHITECTURE_COMMANDE_VERIN.md` §2 ; 5e contact (J6) en série entre `+12V_P` et le NO de K1 (DEC-029) |
| V7 | MCU | Revue | Découplage et VREF+ selon DS12232 Rev 5 fig. 13 ; NRST 100 nF et niveau > VIH(NRST) à la mise sous tension ; PA4 / PA5 limitées à 3,3 V |
| V8 | Valeurs ouvertes | Liste | Chaque `A_DEFINIR` rattaché à un PO |

## 6. Rapport de phase

Un rapport par phase dans `hardware/rapports/RAPPORT_PHASE_<n>.md` : ce qui a été fait, résultats des contrôles, écarts, valeurs `A_DEFINIR`, questions pour Julien. Le rapport distingue **[Exprimé]**, **[Décidé]**, **[Hypothèse]** et **[Reco]** (`CLAUDE.md` §4).

## 7. Placement et routage

| Sujet | Règle | Statut |
|---|---|---|
| Couches | 4 couches : L1 signaux et composants, L2 `GND` / `GND_P` (écart ≥ 2 mm), L3 `+3V3` / `+12V_P` et pistes, L4 signaux. Remplissage de masse cousu sur L1 et L4. Couches internes de type signal | DEC-070 |
| Séparation des domaines | Zone 12 V regroupée sur un bord ; aucune piste ni plan ne traverse la barrière, hors composants d'isolation | Synoptique §4 |
| Distances d'isolement | ≥ 2 mm entre cuivres des domaines commande et puissance, toutes couches ; pas de fente ; exception : empreinte des relais (1,1 mm) | DEC-069 |
| Connecteurs | Regroupés par domaine sur un même bord ; protections (TVS, ESD, filtres) au plus près des connecteurs | DEC-040 |
| Règles de fabrication | Prises dans les capacités JLCPCB en vigueur, vérifiées au moment du routage | Aucune valeur de mémoire |
| Routage automatique | Autorisé seulement hors zones critiques. L'API n'a pas d'autorouteur : Freerouting en externe (export DSN, import SES) | — |
| Zones critiques | Barrière d'isolation, pont de relais et vérin, entrées protégées, quartz, découplage MCU, plans de masse. Routage par Julien ou par Claude sur règles explicites validées | **[Reco]** |
| DRC | L'API ne renvoie que des totaux. Julien colle le journal DRC | 0 erreur |

Claude Code ne voit pas le canevas. Revue visuelle, 3D, encombrement et thermique : Julien, sur captures ou dans l'éditeur.

## 8. Versionnement

- À chaque jalon validé : enregistrement local du projet (Save As > Save Project to Local, `.epro2`) dans `hardware/easyeda/`, PDF du schéma dans `hardware/schemas_pdf/`, commit `hw:`.
- Nommage : `AAAA-MM-JJ_PCB_PWM_SECURISE_V3_G<n>.epro2`.
- Mise à jour de la mémoire projet en fin de session (`CLAUDE.md` §5).

## 9. Phase 0 — test de mise en service

| N° | Test | Critère |
|---|---|---|
| T1 | `GET /health` sur le port du pont | `service = easyeda-bridge`, `edaConnected = true` |
| T2 | `GET /eda-windows` | 1 fenêtre ; si plusieurs, demander à Julien |
| T3 | `eda.dmt_Project.getCurrentProjectInfo()` | Projet d'uuid `b4788f45338c4e59bfcfdff2832a8063` |
| T4 | Liste des schémas, pages et PCB du projet | Cohérente avec ce que Julien a créé |
| T5 | Sur une page `ESSAI` : placement de C432212, relecture des broches et des attributs fournisseur, suppression du composant, suppression de la page | Broches lues ; attributs conformes (sinon : défaut S4 confirmé en V3.2.209) |
| T6 | Lancement de l'ERC par l'API | Totaux renvoyés |

Le rapport de phase 0 conclut sur PO-34 (compatibilité V3.2.209 en mode Full Online).

**Réalisée en session 13 :** T1 à T6 OK, PO-34 clos. Rapport : `hardware/rapports/RAPPORT_PHASE_0.md`.

## 10. Défauts connus de l'API

Constatés en V3.2.149 par JLCPCB. Statut en V3.2.209 Full Online : phase 0 (session 13). Les défauts non testés sont suivis par PO-38.

| Symptôme | V3.2.209 | Contournement |
|---|---|---|
| Attributs fournisseur perdus après placement ou modification partielle | Infirmé (un composant) | S4, conservée par prudence |
| « Wire has multiple net names » | Non testé | S6 |
| Export de netlist bloqué | **Confirmé** (`sch_Netlist.getNetlist`, phase 3) | V1 : netlist reconstruite par relecture |
| ERC / DRC : totaux seulement | Confirmé pour l'ERC ; DRC non testé | Journal collé par Julien |
| `importChanges()` (schéma → PCB) attend un clic « Apply » | Non testé | Julien clique |
| Pont arrêté par manque de mémoire quand il tourne en tâche de fond de Claude Code | Sans objet | Pont lancé dans une fenêtre PowerShell séparée |
| Extension : arrêt des tentatives de connexion après 5 échecs | Non testé | Menu API Gateway > Reconnect |
| `createNetLabel`, `lib_Symbol.getDocumentSource` sans réponse | Constaté (phase 3) | S6 ; ne pas utiliser |
| Erreurs passagères `cmdKey`, `create failed!` | Constaté (phase 3) | S15 |

## Sources

- Skill `easyeda-api` : https://github.com/easyeda/easyeda-api-skill
- Extension Run API Gateway : https://jlc-ext.com/item/oshwhub/run-api-gateway
- Retour d'expérience JLCPCB (V3.2.149) : https://jlcpcb.com/blog/claude-code-pcb-design
- Notes de version EasyEDA Pro V3 : https://pro.easyeda.com/page/update-record
