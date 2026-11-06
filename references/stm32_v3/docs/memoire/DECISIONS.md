# Registre des décisions

Une décision n'est inscrite ici que si Julien l'a exprimée ou validée. Une décision révisée est marquée « Remplacée par DEC-xxx », jamais supprimée.

| Statut | Signification |
|---|---|
| Validée | Exprimée ou confirmée par Julien |
| À confirmer | Validée sur le principe, un détail reste ouvert (renvoi PO) |
| Remplacée | Annulée par une décision ultérieure |

## Besoin et contexte

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-001 | 2026-11-02 | Fonction principale : lire en continu un signal PWM de façon fiable et sûre, et piloter le vérin selon des règles définies (FSM avec états et temporisations). | Validée | Julien. Règles de la FSM : PO-05 |
| DEC-002 | 2026-11-02 | Usage professionnel (secteur privé), non grand public. 1 à 2 cartes (prototypes), 10 cartes maximum chez JLCPCB. Pas d'industrialisation. | Validée | Julien |
| DEC-003 | 2026-11-02 | Outil de CAO : EasyEDA Pro. Fabrication et assemblage : JLCPCB. | Validée | Julien |
| DEC-004 | 2026-11-02 | Environnement : extérieur, installation fixe, −10 à +50 °C. Boîtier de carte réalisé par Julien (mécanique), étanche et blindable. | Validée | Julien. Ambiante ou interne boîtier : PO-17 |
| DEC-005 | 2026-11-02 | Endurance : environ 200 cycles complets sur la durée de vie. Durée d'une course : 2 à 3 s maximum. | Validée | Julien |

## Actionneur

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-010 | 2026-11-02 | Actionneur imposé : Actuonix **L12-30-50-12-S** (course 30 mm, réduction 50:1, 12 V, fins de course internes). Il ne change pas. | Remplacée par DEC-018 | Julien. Incohérent avec l'effort de 30 N (PO-01) |
| DEC-011 | 2026-11-02 | Tension de commande du vérin : 12 V DC. L'hypothèse 24 V est abandonnée. | Validée | Julien. Complétée par DEC-019 |
| DEC-012 | 2026-11-02 | **Position sûre = tige rentrée au maximum.** | Validée | Julien |
| DEC-013 | 2026-11-02 | Aucune position intermédiaire dans la FSM : deux positions (rentré / sorti). | Validée | Julien |
| DEC-014 | 2026-11-02 | Le vérin est installé dans un boîtier mécanique protégé. | Validée | Julien |
| DEC-015 | 2026-11-02 | Vitesse non régulée : elle varie naturellement avec la charge (moteur CC). Pas de variation de vitesse commandée. | Validée | Julien |
| DEC-016 | 2026-11-02 | **Course utile physique : environ 3 mm ± 1 mm, mesurée depuis la butée « rentré ».** | Validée | Julien. Origine précisée en session 03 (clôt PO-21) |
| DEC-017 | 2026-11-02 | **Effort extérieur : environ 30 N, orienté vers la sortie de tige** (vers la position d'action). Valeur estimée. | Validée (estimation) | Julien. Mesure à confirmer : PO-20 |
| DEC-018 | 2026-11-02 | Le vérin peut être remplacé par une autre version si nécessaire. Le L12-30-50-12-S n'est plus imposé. | Validée | Julien. Choix de la version : PO-18 |
| DEC-019 | 2026-11-02 | Le 12 V est fourni directement par le système. Aucun convertisseur sur la carte. | Validée | Julien. Plage min / max : PO-03 |

## Sécurité et architecture de puissance

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-020 | 2026-11-02 | Exigence de sécurité : aucune défaillance simple ne doit provoquer le mouvement vers la position d'action. En cas de problème, retour en position sûre. | Validée | Julien : « très gros danger » en cas d'action intempestive |
| DEC-021 | 2026-11-02 | Isolation galvanique impérative entre le domaine commande (5 V + PWM) et le domaine puissance (12 V vérin). Les masses ne sont **pas** communes. | Validée | Julien |
| DEC-022 | 2026-11-02 | Commande du vérin par **2 relais DPDT** câblés pour que l'état de repos (relais non excités) commande le retour en position sûre. L'action exige les 2 relais excités. | Validée | Voir `docs/architecture/ARCHITECTURE_COMMANDE_VERIN.md` |
| DEC-023 | 2026-11-02 | Le 2e contact de chaque relais DPDT est relu par le microcontrôleur (diagnostic d'état du relais). | Validée | Diagnostic uniquement : un relais standard ne garantit pas la recopie en cas de contact soudé |
| DEC-024 | 2026-11-02 | Détection de présence du 12 V par optocoupleur. | Validée | Julien |
| DEC-025 | 2026-11-02 | Armement par **cavalier (jumper) physique** : sans cavalier, aucune action électrique vers la position d'action n'est possible côté vérin. | Validée | Julien. Implantation : cavalier en série avec la bobine du relais d'action (K2) |
| DEC-026 | 2026-11-02 | Trois niveaux de barrière : (1) logiciel (validation PWM et FSM), (2) électrique (cavalier), (3) mécanique (blocage). | Validée | Julien. Le blocage mécanique est hors périmètre carte |
| DEC-027 | 2026-11-02 | Mouvement automatique vers la position sûre dès la présence du 12 V, avant le démarrage du microcontrôleur : accepté. | Validée | Julien |
| DEC-028 | 2026-11-02 | 4 contacts de fin de course externes minimum : 2 par extrémité (redondance). Chaque contact dispose d'une sortie NO et d'une sortie NC (inverseur). Câbles de 15 à 20 cm visés, 30 cm maximum. | Validée | Julien. Mode de lecture : PO-06 |
| DEC-029 | 2026-11-02 | Arrêt matériel en position d'action retenu sur le principe (contact dédié dans l'alimentation « sortie », côté 12 V). | Validée (principe) | Julien : « bonne proposition ». Intégration : PO-19 |

## Interfaces commande

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-030 | 2026-11-02 | Entrée PWM : impulsion 1000 à 2000 µs, environ 50 Hz (non figé). Émise par un contrôleur externe. Câble 20 cm maximum. | Validée | Julien. Niveau logique 3,3 / 5 V : PO-04 |
| DEC-031 | 2026-11-02 | Alimentation carte : 5 V DC externe. Le GND est celui du contrôleur qui génère le PWM (masse commune 5 V / PWM). | Validée | Julien |
| DEC-032 | 2026-11-02 | Budget de courant 5 V : 500 mA disponibles (hypothèse de travail retenue par Julien). | À confirmer | Julien : « par du principe qu'on a que 500 mA ». PO-09 |
| DEC-033 | 2026-11-02 | Sorties secondaires : 3 ou 4 LED d'état, un buzzer (probable). Entrées capteurs d'extension à prévoir (à définir). | À confirmer | PO-07, PO-12 |
| DEC-034 | 2026-11-02 | Protection côté 12 V conçue pour une source non maîtrisée tant que l'alimentation n'est pas connue : inversion de polarité, surtension, coupure au-delà de 13,5 V (tension max du vérin). | Validée | Proposition Claude validée par Julien. PO-03 reste ouvert pour la plage réelle |
| DEC-035 | 2026-11-02 | MCU soudé directement sur le PCB (pas de module ni de carte de développement embarquée). Critère : intégration simple, pas de composant lourd à mettre en œuvre. | Validée | Julien |
| DEC-036 | 2026-11-02 | **Microcontrôleur : STM32G071CBT6 (LQFP48), LCSC C432212.** | Validée | Julien, sous condition d'absence de problème physique, d'assemblage, d'appro, de débogage et de fabrication. Conditions vérifiées : `CHOIX_MCU.md` §12. Clôt PO-08 |
| DEC-037 | 2026-11-02 | Fins de course lus en antivalence : sorties NO **et** NC de chaque contact, soit 8 entrées. | Validée | Julien. Clôt PO-06 |
| DEC-038 | 2026-11-02 | Contacts de fin de course : microrupteurs classiques de petite taille (distributeur RS). Pouvoir de coupure suffisant pour 12 V / 500 mA selon Julien (vérin : ≤ 200 mA). | Validée | Julien. Référence exacte non fournie ; courant minimal de commutation : PO-23 |
| DEC-039 | 2026-11-03 | Liste des E/S v1 validée sans ajout « pour le moment ». Fonctions complémentaires X01 à X08 retenues : mesure 5 V, sortie d'état vers le contrôleur, entrée de mode maintenance, identification de révision, mesure isolée du courant du vérin, température interne, surveillance 3,3 V, points de test. | Validée | Julien. Clôt PO-24. X05 implique un capteur de courant isolé (PO-14 → à dimensionner) |

## Matériel

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-060 | 2026-11-03 | **Quartz HSE monté** sur PF0 / PF1. Précision de mesure du PWM indépendante de l'oscillateur interne (HSI16 : jusqu'à −2,85 % / +2,05 % sur la plage du projet). | Validée | Julien. Clôt PO-26. Fréquence et référence du quartz à choisir au schéma |
| DEC-065 | 2026-11-03 | Précision du seuil de surtension du LM74502 acceptée : coupure entre 12,27 et 14,04 V (13,16 V typique) avec R502 = 95,3 kΩ / R503 = 10 kΩ. Pas de comparateur ajouté. | Validée | Julien : « PO 35 pas de problème » (option a). Clôt PO-35. DEC-034 (coupure au-delà de 13,5 V) garantie en typique, pas dans le pire cas |
| DEC-066 | 2026-11-03 | **Liaison PC par USB-C** (J12) et pont USB-série CH340C (U13) sur USART1 : programmation par le bootloader ROM et moniteur série, « comme une Arduino ». Carte alimentable par l'USB, OU d'alimentation par 2 Schottky (D15 remplace Q1 + R101). J10 sur USART2. Protection ESD USBLC6-2. SWD J9 conservé en recours. | Validée | Julien : « ok pour toutes tes conceptions », « au plus simple, au plus robuste ». Clôt PO-16. Boutons BOOT / RESET à la place du reset automatique : à confirmer (rapport lot 2, question 1) |
| DEC-067 | 2026-11-03 | **Diodes idéales LM66100** (U15 sur J1, U16 sur l'USB) à la place des Schottky : `+5V` ≈ 5,0 V. Montage « blocage permanent du courant inverse » (CE# relié à VOUT), pas le montage « OU » à CE croisés (coupure si entrées égales). | Validée | Julien : « je veux du 5 V, si plus simple et moins de bug, mets une diode idéale ». Datasheet TI SLVSEZ8A §8.3.2 et §9.2.1 |
| DEC-068 | 2026-11-04 | **PCB 100 × 80 mm**, rectangulaire. **4 trous M3** non métallisés (Ø 3,2 mm) aux coins, centrés à 4 mm des bords, zone libre de Ø 7 mm. **Connecteurs sur deux bords opposés** : commande (J12 USB-C, J1, J4, J5, J8) sur un bord, puissance (J2, J6, J3) sur le bord opposé. | Validée | Julien, choix proposés en session 13. Clôt PO-15. Types de connecteurs : ceux de la BOM v1.2 (JST XH / PH, barrettes 2,54 mm) |
| DEC-069 | 2026-11-04 | **Isolement fonctionnel** entre domaines commande (`GND`) et puissance (`GND_P`) : **≥ 2 mm** entre cuivres des deux domaines, sur toutes les couches, hors empreintes des composants de barrière. **Pas de fente** de fraisage. Exception acceptée : empreinte des relais K1, K2 (1,1 mm entre la broche 8, bobine, et la broche 7, contact ; 1 500 VAC bobine / contacts, isolation de base 125 VAC selon Omron K106). | Validée | Julien, choix proposés en session 13. Clôt PO-33. Tenue en impulsion ≈ 2,5 kV pour 2 mm : **[Hypothèse]** IEC 60664-1 tableau F.2, non vérifiée sur la norme |
| DEC-070 | 2026-11-04 | **PCB 4 couches**, empilement **L1 signaux + composants / L2 plans de masse / L3 alimentations / L4 signaux**. L2 : `GND` sous la zone commande, `GND_P` sous la zone puissance, écart ≥ 2 mm (DEC-069). L3 : `+3V3` côté commande, `+12V_P` côté puissance, quelques pistes. L1 et L4 : remplissage de masse du domaine, cousu à L2 par vias. Couches internes en type « signal » avec zones de cuivre (pas de plans négatifs). Empilement GND / S / S / GND **écarté** pour ce projet. | Validée | Julien : 4 couches ; il a demandé si GND en L1 et L4 avec signaux internes est meilleur, « si oui fais ça, si non garde l'option recommandée ». Analyse : `RAPPORT_PHASE_4_PLACEMENT_V0.md` §10. Clôt PO-32. Épaisseurs JLCPCB : à vérifier au routage |
| DEC-071 | 2026-11-04 | **Borniers à vis enfichables** pour les connecteurs externes (Kangnex, angle droit, ouverture vers le bord) : J1 4 pts, J4 6 pts, J5 7 pts, J6 3 pts, J3 2 pts au pas de 3,81 mm ; J2 2 pts au pas de 5,08 mm. Aucun couple (pas, nombre de points) en double : pas d'inversion de fiche possible. **Barrettes Dupont 2,54 mm en parallèle** des connecteurs de commande seulement : J13 (J1), J14 (J4), J15 (J5). J8 reste en barrette 2,54 mm. | Validée | Julien : « bornes enfichables ou connecteurs à vis » + « alternative en 2,54 mm » ; choix « Commande seulement » pour le Dupont |
| DEC-072 | 2026-11-04 | **Carte 120 × 80 mm** (remplace la taille de DEC-068 ; trous M3 aux coins et bords opposés inchangés). | Validée | Julien : les borniers ne tiennent pas sur 100 mm |
| DEC-073 | 2026-11-04 | **Armement J7** : bornier enfichable 3 pts au pas de 5,08 mm ; armé = pont de fil entre 1 et 3 serré dans la fiche ; point 2 non câblé (aucune autre fiche de la carte n'y entre). Permet un interrupteur à clé déporté. Capuchon JP1 supprimé. | Validée | Julien : « un plus gros connecteur, plus solide » ; choix « Bornier 2 pts + pont » (3 pts retenus pour le détrompage) |
| DEC-074 | 2026-11-04 | Ajouts de la revue de conception : D17 (TVS SMAJ15A sur `NO_FEED`, J6), LED5 + R426 (« 3,3 V présent », sans MCU), 7 points de test TP1 à TP7 (`+5V`, `+3V3`, `GND`, `PWM_IN`, `I_MON`, `+12V_P`, `GND_P`). | Validée | Julien : « si besoin faire les modifs nécessaires ». `REVUE_CONCEPTION_V1.md` M3 à M5 |
| DEC-075 | 2026-11-04 | **Sérigraphie** : aucun texte à moins de 0,15 mm d'une pastille ni sur un corps de composant ; repères 1,0 mm (0,9 mm en zone dense), trait 0,15 mm ; noms de signaux par broche sur J8, J9, J10, J13 à J15 ; cartouche, avertissement 12 V, frontière commande / puissance. C205 et D1 sans repère imprimé (plan de repérage). | Validée | Julien : « vérifier que le marquage ne soit pas sur les composants… rendre la carte pro » |
| DEC-076 | 2026-11-04 | **Routage hybride** : liaisons critiques tracées par script (plans de masse coupés avec bande d'isolement de 2 mm, chemin 12 V et vérin, quartz, USB, découplage du MCU), autorouteur EasyEDA pour les signaux lents restants. **Règles** : signaux 0,25 mm, écart 0,2 mm ; alimentations 3,3 / 5 V 0,5 mm ; chemin 12 V et vérin 1,0 mm ; vias 0,6 / 0,3 mm ; aucun cuivre dans la bande d'isolement ; L2 masse (GND / GND_P), L3 +3V3 / +12V_P, masse sur L1 et L4 cousue. | Validée | Julien, choix proposés en session 13 (marge sur les minimums JLCPCB 4 couches : 0,09 mm) |
| DEC-077 | 2026-11-04 | **Règles plus fines localement** : 3 zones à règle propre (autour de U1 + U4, de U16, de U2) avec piste 0,2 mm et écart 0,15 mm ; règles DEC-076 inchangées ailleurs. Une règle d'écart par net ne marche pas (le DRC applique l'écart le plus strict des deux nets). | Validée | Julien : « Règles plus fines localement » |
| DEC-078 | 2026-11-05 | **Échange de broches du MCU** pour le routage : `K1_CMD` ↔ `K2_CMD` (PA15 / PB2, broches 37 / 21) ; `I_MON` ↔ `K2_FB` (PA0 ADC1_IN0 / PB1, broches 11 / 20). Schéma (page 02_MCU) et PCB mis à jour, ERC propre. R5 respectée. | Validée | Julien : « Oui, échange ». **Effet non vu à la proposition : `K2_FB` passe sur le port B comme `K1_FB` (PO-42)** |

## Outils et méthode de conception

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-061 | 2026-11-03 | Schéma et PCB saisis par Claude Code qui pilote EasyEDA Pro par l'API. Séquence : schéma → retouche et validation par Julien → routage → retouche et validation par Julien. Julien peut retoucher ou faire lui-même avant chaque validation. | Validée | Julien. Clôt PO-31. Jalons détaillés : `hardware/CONSIGNES_EASYEDA_CLAUDE_CODE.md` §3 |
| DEC-062 | 2026-11-03 | Assemblage complet par JLCPCB (PCBA). Soudure interne limitée au minimum : gros connecteurs éventuels (type XT60 / XT90). | Validée | Julien |
| DEC-063 | 2026-11-03 | EasyEDA Pro mis à jour en **V3.2.209**, client desktop, mode Full Online conservé. Skill `easyeda-api` et extension Run API Gateway installés sur le poste de Julien. | Validée (mode délégué à Claude) | Julien : « local ou cloud, ce qui convient le mieux ». L'extension v1.0.6 cible EasyEDA ~3.2.0 : la V2.2.47 était incompatible. Compatibilité à confirmer : PO-34 |
| DEC-064 | 2026-11-03 | Jalons **G1** (`hardware/BOM_CIBLE.md` v1) et **G2** (`hardware/NETLIST_CIBLE.md` v1) validés. La saisie du schéma (phase 3) peut commencer. | Validée | Julien : « je valide G1 et G2 pour l'instant ». Clôt PO-37. Les lignes AV / AD de la BOM restent soumises à leurs PO |

## CEM / IEM

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-040 | 2026-11-02 | Référentiel CEM de conception : immunité EN 61000-6-2 (environnement industriel), ESD IEC 61000-4-2 niveau 4 (±8 kV contact, ±15 kV air). | Validée | Proposition Claude validée implicitement par Julien (« niveau le plus élevé possible ») |
| DEC-041 | 2026-11-02 | Objectif de conception IEM : s'inspirer des essais MIL-STD-461G CS114, CS116, RS103, RS105, **sans revendication de conformité**. | Validée | Julien : « OUI TRES BIEN » |
| DEC-042 | 2026-11-02 | Pas d'essai en laboratoire CEM. Essais réalisés par le client. | Validée | Julien |
| DEC-043 | 2026-11-02 | Blindage complémentaire possible côté boîtier mécanique. | Validée | Julien |

## Firmware et organisation

| N° | Date | Décision | Statut | Source / remarque |
|---|---|---|---|---|
| DEC-050 | 2026-11-02 | Langage : C++ avec le framework Arduino (Arduino IDE 2 ou PlatformIO). Outils gratuits, programmation depuis un PC. MicroPython écarté pour une fonction de sécurité. | Validée | Julien connaît Python et Arduino |
| DEC-051 | 2026-11-02 | Répartition : Claude écrit le firmware, Julien teste et fait les retours. | Validée | Julien |
| DEC-052 | 2026-11-02 | Toute la mémoire du projet (décisions, échanges, documents, données, code) est versionnée dans le dépôt GitHub privé `JulienBEBL/PCB_PWM_SECURISE_V3`. Claude gère commit, push et pull. | Validée | Julien |
| DEC-053 | 2026-11-02 | Licence Apache 2.0 du dépôt conservée (open source accepté). | Validée | Julien. Clôt PO-13 |
