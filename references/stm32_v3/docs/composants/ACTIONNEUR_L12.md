# Actionneur — série Actuonix L12-S

**Statut :** version à choisir (PO-18). La version initiale L12-30-50-12-S est inadaptée (PO-01).

## 1. Données d'entrée

| Donnée | Valeur | Source |
|---|---|---|
| Course utile | 3 mm ± 1 mm | DEC-016 |
| Effort extérieur | ≈ 30 N, vers la sortie de tige (estimation) | DEC-017 |
| Position sûre | Tige rentrée au maximum | DEC-012 |
| Tension | 12 V fourni par le système | DEC-019 |
| Durée de course | 2 à 3 s maximum | DEC-005 |
| Endurance | ≈ 200 cycles | DEC-005 |

## 2. Caractéristiques de la série (datasheet constructeur, copie 2016)

Source : fiche technique Actuonix L12, https://static.chipdip.ru/lib/429/DOC039429161.pdf. **À recontrôler sur la dernière révision publiée par Actuonix.**

| Réduction | Point de puissance max | Point de rendement max | Vitesse à vide | Effort max | Réversibilité statique |
|---|---|---|---|---|---|
| 50:1 | 17 N à 14 mm/s | 10 N à 19 mm/s | 25 mm/s | 22 N | 12 N |
| 100:1 | 31 N à 7 mm/s | 17 N à 10 mm/s | 13 mm/s | 42 N | 22 N |
| 210:1 | 62 N à 3,2 mm/s | 36 N à 4,5 mm/s | 6,5 mm/s | 80 N | 45 N |

Communs à la série 12 V, option -S : tension max 13,5 V ; courant de blocage 185 mA ; facteur de marche max 20 % (période non définie, PO-10) ; −10 à +50 °C ; fins de course internes coupant le moteur à 0,5 mm de chaque extrémité, non réglables, avec diodes internes permettant de repartir en sens inverse ; fuite max 8 µA. Courses disponibles : 10, 30, 50, 100 mm.

Câblage -S : fil rouge = V+, fil noir = V−. Rouge au + : la tige sort. Polarité inversée : la tige rentre.

## 3. Deux conditions de tenue de la position sûre

1. **Rentrer contre la charge :** effort max > 30 N.
2. **Rester rentré sans moteur :** effort de réversibilité > 30 N.

La condition 2 s'applique **même avec le 12 V présent** : en position rentrée, la fin de course interne coupe le moteur. Seule la réversibilité retient alors la charge. Si elle est insuffisante, la tige sort, la fin de course interne se referme, le moteur rentre la tige, et le cycle se répète.

| Réduction | Condition 1 (30 N) | Condition 2 (30 N) | Verdict |
|---|---|---|---|
| 50:1 | 22 N : **non** | 12 N : **non** | Inadapté |
| 100:1 | 42 N : oui, marge 1,4 | 22 N : **non** | Inadapté |
| 210:1 | 80 N : oui, marge 2,7 | 45 N : oui, marge 1,5 | **Seul candidat** |

Les marges sont calculées sur une charge estimée (PO-20).

## 4. Durée de course avec 210:1

La vitesse d'un moteur CC diminue quand l'effort augmente. À 30 N, l'effort est inférieur au point de rendement max (36 N à 4,5 mm/s) : la vitesse est donc supérieure à 4,5 mm/s.

- Sortie, charge favorable : vitesse ≤ 6,5 mm/s (vitesse à vide), soit ≥ 0,46 s pour 3 mm.
- Rentrée, contre 30 N : vitesse > 4,5 mm/s, soit < 0,67 s pour 3 mm.

Les deux sens respectent l'exigence de 2 à 3 s.

## 5. Choix de la course

| Course | Avantage | Inconvénient |
|---|---|---|
| **10 mm** (proposée) | Dépassement maximal limité si l'arrêt à 3 mm échoue : la fin de course interne coupe à 9,5 mm | Longueur fermée différente du 30 mm : intégration mécanique à revoir |
| 30 mm | Pas de changement mécanique si l'implantation actuelle est prévue pour ce modèle | Dépassement possible jusqu'à 29,5 mm |
| Course personnalisée | Fin de course interne à la course utile | Disponibilité et délai à vérifier auprès d'Actuonix |

**[Reco]** L12-10-210-12-S. L'existence de cette référence est confirmée chez un revendeur (RobotShop), ses caractéristiques sont à vérifier sur la datasheet constructeur.

## 6. Écarts entre sources

| Paramètre | Datasheet constructeur 2016 | Fiche revendeur | Retenu |
|---|---|---|---|
| L12-30-50-12-S, réversibilité | 12 N | 43 N (RobotShop) | 12 N |
| L12-30-50-12-S, point de puissance | 17 N à 14 mm/s | 12 N à 11 mm/s (RobotShop) | À vérifier |
| L12-10-210-12-S, point de puissance | 62 N à 3,2 mm/s | 45 N à 2,5 mm/s (RobotShop) | À vérifier |
| L12-10-210-12-S, réversibilité | 45 N | 150 N (RobotShop) | 45 N (le plus défavorable) |
| Courant de blocage 12 V | 185 mA | 200 mA (RobotShop) | 200 mA pour le dimensionnement |

Règle appliquée : en cas d'écart, retenir la valeur la plus défavorable jusqu'à confirmation par la datasheet constructeur à jour.
