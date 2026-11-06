# CAHIER DES CHARGES FONCTIONNEL
## Carte électronique de traitement et d'interface

**Version :** document de travail – extraction des exigences génériques relatives à la carte électronique  
**Date de révision :** 02/11/2026

---

## Table des révisions

| Réviseur | État | Remarques |
|---|---|---|
| — | Version de travail | Réorganisation des seules exigences génériques relatives à la carte électronique |

## Table des matières

1. [Introduction](#1-introduction)
2. [Périmètre de la prestation](#2-périmètre-de-la-prestation)
3. [Références et principes généraux](#3-références-et-principes-généraux)
4. [Description de la carte électronique](#4-description-de-la-carte-électronique)
5. [Analyse fonctionnelle](#5-analyse-fonctionnelle)
6. [Classification des exigences](#6-classification-des-exigences)
7. [Fonctions contraintes](#7-fonctions-contraintes)
8. [Cycle générique de fonctionnement](#8-cycle-générique-de-fonctionnement)
9. [Points ouverts et prérequis](#9-points-ouverts-et-prérequis)
10. [Attendus](#10-attendus)
11. [Conclusion](#11-conclusion)

---

# 1. Introduction

## 1.1 Préambule

Ce document est un Cahier des Charges Fonctionnel (CdCF). Son objectif est de définir les besoins, les fonctions attendues et les contraintes applicables à une carte électronique, sans figer prématurément les solutions techniques.

Conformément à la démarche de l'analyse fonctionnelle, le document décrit le besoin à satisfaire et non la manière détaillée de le satisfaire. Le choix définitif des composants et des architectures sera précisé dans le futur Cahier des Charges Technique (CdCT).

## 1.2 Contexte et enjeux

Le besoin porte sur la conception ou l'évolution d'une carte électronique assurant l'acquisition, le traitement et l'échange de signaux avec des éléments externes.

La conception devra notamment prendre en compte :

- l'adéquation des entrées et sorties aux signaux attendus ;
- la compatibilité électrique des interfaces ;
- la stabilité du fonctionnement logiciel et matériel ;
- la maîtrise des états de fonctionnement et des situations de défaut ;
- les contraintes d'alimentation, d'encombrement et d'environnement ;
- la possibilité de tester et de maintenir la carte.

## 1.3 Principes de fonctionnement et de sécurité

L'architecture devra respecter les principes généraux suivants :

1. Une anomalie matérielle ou logicielle ne doit pas conduire à un état de sortie non maîtrisé.
2. Au démarrage, pendant une réinitialisation ou en cas de défaut, les entrées et sorties doivent adopter des états définis et documentés.
3. Le logiciel doit être surveillé par un mécanisme de watchdog et prévoir un comportement de repli en cas d'anomalie.
4. Les fonctions de protection matérielles et logicielles doivent être cohérentes et vérifiables lors des essais.

# 2. Périmètre de la prestation

| Domaine | Sous-systèmes / fonctions couvertes |
|---|---|
| **Conception électronique** | Carte électronique (PCB), alimentation, protections électriques, conditionnement des signaux et connectique |
| **Firmware** | Logiciel embarqué, gestion générique des états, watchdog, gestion des défauts et interfaces d'entrée/sortie |
| **Interfaces** | Entrées et sorties numériques et, si nécessaire, analogiques ou de type PWM ; niveaux électriques et protocoles à définir |
| **Validation** | Tests électriques et fonctionnels sur banc, vérification des entrées/sorties, essais de robustesse et d'endurance |
| **Documentation** | Schémas, nomenclature (BOM), fichiers de fabrication, procédures de programmation et procédures de test |

## 2.1 Hors périmètre

- Définition détaillée des équipements externes raccordés à la carte, sauf leurs caractéristiques d'interface.
- Conception mécanique globale de l'équipement d'accueil.
- Définition des fonctions métier réalisées par les équipements externes.

# 3. Références et principes généraux

La conception devra s'appuyer sur les exigences réglementaires et normatives applicables au produit final et à son environnement d'utilisation. Les références précises seront déterminées lors de la définition technique.

Les domaines à considérer comprennent notamment :

- la compatibilité électromagnétique (CEM) ;
- l'immunité aux décharges électrostatiques (ESD) ;
- la tenue aux vibrations et aux chocs, selon les conditions d'utilisation ;
- la sécurité électrique et la protection contre les erreurs de raccordement ;
- la tolérance aux défauts et le retour à un état défini.

**Principes structurants retenus :**

- **Conception robuste :** dimensionnement des composants et des interfaces en fonction des contraintes spécifiées.
- **Tolérance aux défauts :** détection des anomalies pertinentes et comportement documenté en cas de défaut.
- **Conception fail-safe :** les états des sorties en situation anormale doivent être définis et maîtrisés.
- **Maintenabilité :** accès aux points de test et possibilité de diagnostiquer les principales fonctions de la carte.

# 4. Description de la carte électronique

La carte électronique a pour finalité d'assurer le traitement de signaux, la gestion de ses entrées/sorties et la communication avec les éléments externes prévus dans la spécification d'interface.

Elle devra notamment permettre :

- la gestion et la distribution de son alimentation ;
- l'acquisition de signaux d'entrée ;
- le traitement des informations par un microcontrôleur ;
- la génération ou la transmission de signaux de sortie selon les interfaces définies ;
- la gestion des états de fonctionnement et des défauts ;
- la signalisation de l'état général de la carte ;
- le retour à un état défini en cas d'anomalie.

La carte pourra notamment comprendre :

- un circuit imprimé (PCB) ;
- un microcontrôleur, l'ESP32 étant une référence envisagée dans le document source et non une obligation de conception ;
- le firmware associé ;
- les fonctions d'alimentation et de protection ;
- les connecteurs d'alimentation, de programmation et d'entrées/sorties ;
- les indicateurs d'état ;
- les points de test nécessaires à la validation et à la maintenance.

# 5. Analyse fonctionnelle

## 5.1 Objectif de l'analyse fonctionnelle

L'analyse fonctionnelle identifie les fonctions de service de la carte électronique ainsi que les contraintes imposées par ses interfaces et son environnement.

## 5.2 Fonction principale

| N° fonction | Description |
|---|---|
| **FP1** | Assurer l'acquisition, le traitement et l'échange de signaux conformément aux interfaces et aux états de fonctionnement spécifiés. |

## 5.3 Éléments du milieu extérieur

| Élément extérieur | N° fonction | Description |
|---|---|---|
| Opérateur / Mainteneur | FC100 | Permettre une utilisation, une lecture d'état et une maintenance compréhensibles |
| Alimentation | FC200 | Assurer une alimentation compatible et stable |
| Interfaces externes | FC300 | Assurer la compatibilité électrique et fonctionnelle des entrées/sorties |
| Carte électronique / Firmware | FC400 | Assurer le traitement, la gestion des états et le fonctionnement logiciel |
| Sécurité électrique et fonctionnelle | FC500 | Maîtriser les états de la carte et son comportement en cas de défaut |
| Environnement d'utilisation | FC600 | Résister aux contraintes électriques, électromagnétiques et mécaniques spécifiées |
| Maintenance / Essais | FC700 | Permettre les tests, contrôles et interventions |

# 6. Classification des exigences

## 6.1 Classes de flexibilité

- **F0 :** exigence impérative, non négociable, liée à la sécurité ou au fonctionnement essentiel.
- **F1 :** exigence importante avec une faible marge d'adaptation.
- **F2 :** exigence souhaitable pouvant être ajustée.
- **F3 :** exigence de confort ou d'optimisation.

# 7. Fonctions contraintes

## 7.1 FC 100 : Utilisation et maintenance

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC101** | Visualisation de l'état général de la carte | Indicateur d'état lisible | — | F1 | Visible à 2 m, selon l'intégration | Exploitation |
| **FC102** | Raccordement de la carte sans erreur de branchement | Détrompage et repérage des connecteurs | — | F0 | Erreur de raccordement réduite au minimum par conception | Sécurité / Maintenabilité |
| **FC103** | Mise en service de la carte | Durée de mise en œuvre | min | F2 | À définir selon l'application | Exploitation |
| **FC104** | Autocontrôle au démarrage | Vérification de l'alimentation, du microcontrôleur et des interfaces pertinentes | — | F1 | Automatisé si possible | Fiabilité |

## 7.2 FC 200 : Alimentation

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC201** | Compatibilité de l'alimentation avec la carte | Tension et plage d'entrée | V | F0 | À définir dans la spécification électrique | Intégration |
| **FC202** | Protection contre les défauts d'alimentation | Protections adaptées aux risques identifiés | — | F0 | À définir lors de la conception | Robustesse |
| **FC203** | Stabilité des tensions internes | Tolérances des rails d'alimentation | % / V | F1 | Selon les composants retenus | Fiabilité |

## 7.3 FC 300 : Interfaces et performances

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC301** | Compatibilité des entrées/sorties avec les équipements externes | Niveaux électriques, type de signal et brochage | — | F0 | À définir dans le document d'interface | Intégration |
| **FC302** | Fiabilité du traitement des signaux | Absence de dérive fonctionnelle lors des essais définis | cycles | F1 | À définir selon le profil d'utilisation | Fiabilité |
| **FC303** | Immunité aux perturbations externes | Maintien d'un fonctionnement maîtrisé en présence de perturbations spécifiées | — | F0 | Plan d'essais CEM à définir | Robustesse |
| **FC304** | Résistance aux perturbations internes | Détection des anomalies internes pertinentes et réaction documentée | — | F1 | À définir selon l'architecture | Fiabilité |
| **FC305** | Masse de la carte et de ses composants associés | Masse | g | F1 | À définir | Intégration |

## 7.4 FC 400 : Carte électronique et firmware

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC401** | Gestion des interfaces de commande et de communication | Types de signaux, niveaux logiques et protocoles documentés | — | F0 | À définir selon les équipements externes | Interface |
| **FC402** | Gestion structurée des états de fonctionnement | États et transitions définis dans le firmware | — | F0 | Spécification logicielle documentée | Architecture |
| **FC403** | Surveillance du fonctionnement logiciel | Watchdog et détection des blocages | — | F0 | Réaction définie et vérifiable | Fiabilité |
| **FC404** | Gestion des entrées de configuration et de diagnostic | Entrées logiques protégées et états électriques définis | — | F1 | Selon le schéma électrique | Exploitation |
| **FC405** | Retour à un état de sortie défini en cas d'anomalie | Comportement matériel et logiciel documenté | — | F0 | Réaction définie pour chaque défaut identifié | Tolérance aux défauts |
| **FC406** | Maîtrise des états des sorties lors de la mise sous tension et de la réinitialisation | États électriques au démarrage et au redémarrage | — | F0 | Aucun état transitoire non maîtrisé | Robustesse |
| **FC407** | Acquisition et remontée des informations de diagnostic | Retour d'information sur les états utiles de la carte | — | F2 | À définir selon les besoins | Maintenance |
| **FC408** | Encombrement de la carte | Dimensions hors tout | mm | F1 | À définir | Intégration |
| **FC409** | Détection des défauts et disponibilité d'entrées d'extension | Gestion des anomalies pertinentes et réservation de 1 à 2 entrées libres si nécessaire | — | F2 | À définir lors de l'allocation des E/S | Évolutivité |
| **FC410** | Mise hors service logicielle ou matérielle de la carte | Fonction et conditions de désactivation documentées | — | F1 | À définir selon l'application | Exploitation |
| **FC411** | Limitation du volume et de la masse de la carte | Dimensions et masse de l'ensemble électronique | mm / g | F1 | À définir selon les contraintes d'intégration | Intégration |

## 7.5 FC 500 : Sécurité électrique et fonctionnelle

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC501** | Mise en œuvre de protections matérielles et logicielles cohérentes | Analyse des protections et de leur indépendance lorsque nécessaire | — | F0 | À justifier dans le dossier de conception | Robustesse |
| **FC502** | Limitation des causes communes de défaillance | Analyse des dépendances d'alimentation, de signaux et de traitement | — | F1 | Selon l'analyse de risques | Tolérance aux défauts |
| **FC503** | Définition des états électriques par défaut | États des entrées/sorties documentés au démarrage et en défaut | — | F0 | Obligatoire | Sécurité |
| **FC504** | Retour à un état maîtrisé sur perte d'alimentation ou anomalie | Comportement par défaut | — | F0 | Défini et testé | Fail-safe |
| **FC505** | Absence de transition intempestive des sorties en fonctionnement normal | Analyse des défauts et essais fonctionnels | — | F0 | Démontrée par les essais prévus | Fiabilité |
| **FC506** | Protection des entrées/sorties contre les erreurs électriques prévisibles | Protections adaptées aux niveaux et usages définis | — | F1 | À préciser dans le schéma électrique | Robustesse |

## 7.6 FC 600 : Environnement d'utilisation

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC601** | Tenue aux vibrations de l'environnement d'utilisation | Niveau vibratoire | — | F2 | À définir et mesurer pendant les essais | Fiabilité |
| **FC602** | Tenue aux chocs et à la manutention | Niveau de choc | g | F2 | À définir selon l'intégration | Robustesse |
| **FC603** | Compatibilité électromagnétique (CEM) | Immunité et émissions | — | F1 | Plan de validation à définir | Compatibilité |
| **FC604** | Immunité aux décharges électrostatiques (ESD) | Niveau d'immunité | — | F1 | Validation selon l'environnement spécifié | Robustesse |
| **FC605** | Compatibilité de l'alimentation avec l'installation d'accueil | Tension et caractéristiques d'entrée | V | F0 | À définir | Intégration |

## 7.7 FC 700 : Maintenance et essais

| N° | Description | Critère | Unité | Classe | Limite | Justification |
|---|---|---|---|---|---|---|
| **FC701** | Tests fonctionnels sur banc | Tensions, états des entrées/sorties, communications et temporisations pertinentes | — | F0 | Fonctions définies couvertes par les essais | Validation |
| **FC702** | Vérification de robustesse et d'endurance | Répétition des cycles de test représentatifs | cycles | F2 | Profil à définir | Fiabilité |
| **FC703** | Accès aux points de test sur la carte | Points de mesure accessibles et repérés | — | F2 | À définir dans le PCB | Maintenance |
| **FC704** | Procédure d'intervention sur la carte | Procédure de mise hors tension, de raccordement et de démontage | — | F1 | Fournie | Maintenance |
| **FC705** | Interface utilisateur compréhensible | Repérage, boutons et voyants explicites lorsque présents | — | F1 | À définir selon l'application | Ergonomie |
| **FC706** | Maîtrise des énergies résiduelles avant intervention | Procédure et vérification de mise hors énergie | — | F1 | À définir selon l'architecture d'alimentation | Sécurité électrique |

# 8. Cycle générique de fonctionnement

Le fonctionnement général de la carte devra être documenté dans la spécification logicielle. À titre de structure fonctionnelle, les phases suivantes sont à prévoir :

1. **Mise sous tension :** établissement des alimentations et maintien des sorties dans leurs états électriques définis.
2. **Initialisation :** configuration du microcontrôleur, des périphériques et des interfaces.
3. **Autocontrôle :** vérification des fonctions internes et acquisition des informations de diagnostic disponibles.
4. **Fonctionnement nominal :** acquisition, traitement et échange des signaux conformément à la spécification d'interface.
5. **Gestion d'un défaut :** signalisation de l'anomalie et application du comportement de repli défini.
6. **Mise hors tension :** arrêt maîtrisé et prise en compte des éventuelles énergies résiduelles.

Les temporisations, seuils électriques, protocoles et conditions de transition seront définis dans le CdCT après allocation des entrées/sorties et choix des composants.

# 9. Points ouverts et prérequis

Les éléments suivants conditionnent le passage en conception détaillée :

| N° | Point ouvert | Impact |
|---|---|---|
| **PO1** | Définition de la tension et de la plage d'alimentation | Choix des protections et des régulateurs |
| **PO2** | Liste des entrées/sorties et caractéristiques électriques | Schéma, choix des composants et brochage |
| **PO3** | Choix du microcontrôleur | Architecture matérielle, firmware et ressources disponibles |
| **PO4** | Définition des protocoles de communication | Choix des interfaces et validation des échanges |
| **PO5** | Dimensions et contraintes d'intégration de la carte | Placement, routage et connectique |
| **PO6** | Environnement électrique et électromagnétique | Choix des protections et plan d'essais CEM/ESD |
| **PO7** | Besoins de diagnostic et d'entrées d'extension | Allocation des E/S et points de test |
| **PO8** | Profil d'utilisation et exigences d'endurance | Définition des essais de validation |

Le planning de conception, de fabrication, de réception et de tests sera établi après gel des spécifications d'interface et des contraintes d'intégration.

# 10. Attendus

Les livrables attendus sont :

- une carte électronique (PCB) répondant aux exigences fonctionnelles et électriques définies ;
- le firmware associé et ses paramètres de configuration ;
- les schémas électroniques et les fichiers de conception nécessaires à la fabrication ;
- la nomenclature des composants (BOM) ;
- les fichiers de fabrication et d'assemblage ;
- les procédures de programmation, de test et de maintenance ;
- un rapport de validation présentant les résultats des essais fonctionnels et électriques.

# 11. Conclusion

Le présent Cahier des Charges Fonctionnel formalise les besoins, les fonctions de service et les contraintes génériques applicables à une carte électronique de traitement et d'interface.

Il définit les attentes relatives à l'alimentation, aux entrées/sorties, au microcontrôleur, au firmware, à la gestion des défauts, à la robustesse électrique et environnementale, ainsi qu'aux moyens de test et de maintenance.

La conception détaillée pourra débuter lorsque les points ouverts relatifs aux interfaces, à l'alimentation, à l'intégration et à l'environnement auront été définis.

---

*Document de travail générique : les exigences propres à une application particulière doivent être complétées et validées dans le CdCF et le CdCT du projet concerné.*
