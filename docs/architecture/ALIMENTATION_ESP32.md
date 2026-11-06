# Architecture d'alimentation avec la carte de dev

**Statut :** proposition v1 de Claude, **à valider par Julien** (PO-07).

**Sources consultées le 2026-11-06 :**

- [D1] Espressif, *ESP32-S3-DevKitC-1 v1.1 User Guide* : trois modes d'alimentation **mutuellement exclusifs** (USB, broche 5V, broche 3V3).
- [D3] Espressif, schéma *SCH_ESP32-S3-DevKitC-1_V1.1_20221130* : régulateur U2 SGM2212-3.3, deux diodes Schottky D1 et D7 (1N5819HW-7-F), pont USB-série CP2102N. Lecture d'un extrait texte : **la position exacte de D1 et D7 n'est pas confirmée** (PO-18).
- [D4] SG Micro, fiche produit SGM2212 : 800 mA, entrée 2,7 à 20 V, −40 à +125 °C, limitation de courant et arrêt thermique.

## 1. Problème

[D1] interdit d'alimenter la carte de dev par l'USB et par la broche 5V en même temps. Or Julien doit pouvoir brancher l'USB (programmation, moniteur série) alors que le 5 V externe est présent. Sans précaution, deux risques :

- le 5 V externe remonte dans le port USB du PC ;
- le 5 V de l'USB remonte dans la carte et le contrôleur externe.

## 2. Proposition

```
J1 5 V ─ F1 ─ D1 (TVS) ─ U15 LM66100 ─┬─ +5V carte : relais, LED RGB, 74AHCT1G125, LED5…
                                       │
                                       └─ U16 LM66100 ─ broche 5V carte de dev
                                                          │
                       USB carte de dev ─ Schottky D1/D7 ─┤ (interne carte de dev, PO-18)
                                                          │
                                                 SGM2212 (interne) ─ broche 3V3 ─ +3V3 carte
```

| N° | Choix | Raison |
|---|---|---|
| A1 | **U15** (LM66100, repris de STM32 DEC-067) sur l'entrée J1. | Bloque tout retour vers le contrôleur externe. Composant déjà validé |
| A2 | **U16** (LM66100) entre le +5V de la carte et la broche 5V de la carte de dev, sens +5V → carte de dev. | Le 5 V de l'USB ne peut pas alimenter la carte. U16 était la diode USB de la carte STM32 : même référence, autre emplacement |
| A3 | Pas de régulateur 3,3 V sur la carte. Le **+3V3 de la carte vient de la broche 3V3** de la carte de dev (SGM2212, 800 mA [D4]). U2 (ME6211) supprimé. | Un seul domaine 3,3 V : pas de conflit entre deux régulateurs, pas de courant injecté dans un ESP32 éteint |
| A4 | LED5 « 3,3 V présent » branchée sur ce +3V3. | Elle montre que la carte de dev est en place et alimentée |
| A5 | `V5_MON` mesure le +5V de la carte (après U15). | Autocontrôle de l'alimentation (DEC-011) |

## 3. Comportement par cas

| Cas | Carte de dev | +5V carte (relais) | Effet |
|---|---|---|---|
| 5 V externe seul | Alimentée par sa broche 5V | Oui | Fonctionnement normal |
| USB seul (bureau) | Alimentée par l'USB | **Non** : U16 bloque | Programmation et essais firmware. **Relais impossibles à exciter** : aucune bobine alimentée |
| 5 V externe + USB | Alimentée par les deux | Oui | U16 donne ≈ 5 V, l'USB arrive après la Schottky interne : la Schottky est bloquée, aucun courant vers le PC. **Sous réserve de PO-18** |
| Carte de dev retirée | — | Oui | Pas de +3V3 : pompe de charge et watchdog sans alimentation, relais au repos. À confirmer au schéma (PO-20) |

## 4. Écart avec la carte STM32

STM32 DEC-066 : la carte était alimentable par l'USB. Ici, l'USB n'alimente que la carte de dev. **Le vérin ne peut pas bouger quand seul l'USB est branché.** C'est un gain de sécurité pour les essais au bureau.

## 5. Points à vérifier

| N° | Point |
|---|---|
| PO-18 | Les Schottky D1 / D7 de la carte de dev sont-elles en série entre chaque VBUS USB et le réseau 5V de la broche ? Vérification simple au multimètre (voir §6) |
| PO-19 | 74AHCT1G125 non alimenté (cas USB seul) avec une entrée pilotée par l'ESP32 : courant d'entrée à vérifier sur la datasheet TI. La résistance série et le rappel à la masse limitent le courant |
| PO-20 | Vérifier au schéma que la perte du +3V3 bloque bien les deux chaînes de relais (pompe de charge, watchdog) |
| — | Bilan de consommation sur le +3V3 et le +5V : à faire au schéma (budget 5 V : 500 mA, STM32 DEC-032) |

## 6. Vérification au multimètre (PO-18)

Sur une carte de dev **non alimentée**, multimètre en mode diode :

1. Pointe rouge sur le VBUS d'un port USB (broche 1 du connecteur, ou point de test), pointe noire sur la broche **5V** du connecteur J1 de la carte de dev.
2. Attendu : **≈ 0,2 à 0,4 V** (Schottky passante).
3. Inverser les pointes. Attendu : **pas de conduction** (OL).
4. Refaire avec le second port USB.

Si les deux ports donnent ce résultat, PO-18 est clos : le 5 V externe ne peut pas remonter dans le PC.
