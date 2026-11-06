# Synoptique matériel de la carte

**Statut :** version 1 pour revue (session 10).
**Objet :** décrire chaque bloc fonctionnel de la carte, sa fonction, ses exigences et les composants à choisir. Ce document précède le choix des références et le schéma EasyEDA.
**Convention :** les composants cités sont des **candidats** ; aucune référence n'est retenue tant que sa datasheet et sa disponibilité LCSC n'ont pas été vérifiées.

---

## 1. Vue d'ensemble

```
            DOMAINE COMMANDE (5 V / 3,3 V, GND du contrôleur)                ║  DOMAINE PUISSANCE (12 V, 0 V isolé)
                                                                             ║
 J1 Contrôleur ─► [B1 Entrée 5 V] ─► 5 V ─► [B2 LDO] ─► 3,3 V                ║  J2 12 V ─► [B10 Protection 12 V] ─► +12 V_P
   5 V, GND,           │                       │                             ║                      │
   PWM, STATUS ◄───────┼─────────────── [B3 MCU STM32G071] ◄── [B4 PWM]       ║                      ├─► Contacts K1 / K2 ─► J3 Vérin
                       │                  │   │   │   │                      ║                      │      (pont, B9)
 J4/J5 Fins de course ─┼─► [B5 Entrées FC ×8] ┘   │   │                      ║  J6 5e contact ──────┘ (arrêt matériel, B9)
 J8 Extension ─────────┼─► [B6 Entrées ext. ×4] ──┘   │                      ║
 J7 Cavalier ──────────┼─► [B7 Commande relais] ◄─────┤  bobines K1, K2 ═════╬═══► contacts K1, K2
                       │     pompe de charge,         │                      ║
                       │     watchdog externe         │                      ║
                       │                              ├◄── [B11 Optos] ◄═════╬═══ V12_OK, STOP_HW_FB
                       │                              ├◄── [B12 I_MON isolé] ◄╬═══ shunt / capteur
                       └─► [B13 LED, buzzer]  ◄────────┘                      ║
 J9 SWD, J10 UART ─► MCU                                                      ║
```

Le double trait (║, ═) marque la **barrière d'isolation** (DEC-021). Seuls les relais, les optocoupleurs et le capteur de courant isolé la franchissent.

## 2. Blocs du domaine commande

### B1 — Entrée et protection 5 V

| Élément | Fonction | Exigence / remarque | Candidats |
|---|---|---|---|
| Connecteur J1 | 5 V, GND, PWM, STATUS_OUT depuis / vers le contrôleur | 4 points, détrompé, verrouillable (extérieur, DEC-004) | À choisir avec la mécanique (PO-15) |
| Protection contre l'inversion | Empêcher la destruction si 5 V et GND sont inversés | Chute de tension faible : les bobines de relais 5 V ont une tension minimale de fonctionnement | MOSFET P en « diode idéale » (chute de quelques dizaines de mV) ou diode Schottky (0,3 à 0,5 V, à confronter à la tension d'enclenchement des relais) |
| Fusible réarmable | Limiter le courant en cas de court-circuit aval | Calibre au-dessus du bilan (§5), sous 500 mA (DEC-032) | Fusible CTP CMS |
| Diode TVS | Écrêter les surtensions transitoires (CEM, ESD sur câble) | Tension de veille ≥ 5,5 V | TVS unidirectionnelle 5 V, boîtier SMA ou SOD-123 |
| Filtrage | Bloquer le bruit vers et depuis le contrôleur | Ferrite + condensateurs | Ferrite CMS, céramiques X7R |

### B2 — Régulation 3,3 V

| Élément | Exigence | Candidats |
|---|---|---|
| Régulateur linéaire 5 V → 3,3 V | Courant ≥ 150 mA (MCU, buffers, optos côté sortie, ADC). Chute compatible avec 5 V − protections. Stable avec céramiques. | AMS1117-3.3 (chute ≈ 1 V, courant de repos élevé), régulateurs à faible chute de la classe 200 à 300 mA (ME6211, XC6206, AP2112) |
| Découplage | Selon datasheet du régulateur | — |

Un régulateur linéaire suffit : la puissance dissipée reste faible (≈ 1,7 V × courant 3,3 V).

### B3 — Microcontrôleur

| Élément | Détail | Source |
|---|---|---|
| STM32G071CBT6, LQFP48 | LCSC C432212 | DEC-036 |
| Découplage VDD/VDDA | 100 nF + 4,7 µF au plus près | DS12232 Rev 5, figure 13 |
| VREF+ | 100 nF (le 1 µF n'est requis qu'avec VREFBUF, non utilisé) | DS12232 Rev 5, figure 13, note 2 |
| VBAT | Relié au 3,3 V, 100 nF | — |
| NRST | 100 nF au plus près. Niveau > VIH(NRST) garanti à la mise sous tension | DS12232 Rev 5, figure 23 |
| Quartz HSE | Fréquence à choisir (4 à 48 MHz), 2 condensateurs de charge calculés selon CL | DEC-060 |
| J9 SWD | 3V3, SWDIO, SWCLK, NRST, GND | — |
| J10 UART2 | TX, RX, GND (atelier) ; USB-C J12 + CH340C U13 sur USART1 (programmation, moniteur série) — v1.1 | — |
| Points de test | 3,3 V, 5 V, GND, signaux critiques | X08 |

### B4 — Entrée PWM

| Élément | Fonction | Exigence | Candidats |
|---|---|---|---|
| Résistance série + diode ESD | Protection | Niveau ESD visé : IEC 61000-4-2 niveau 4 (DEC-040) | Diode ESD bas niveau de capacité |
| Rappel à la masse | Entrée débranchée = aucune impulsion | — | Résistance |
| Buffer à trigger de Schmitt | Mise en forme, compatibilité 3,3 / 5 V (PO-04) | Alimenté en 3,3 V, entrée tolérante 5 V, seuils compatibles avec une sortie 3,3 V | 74LVC1G17 (seuils à vérifier dans sa datasheet) |
| Filtre RC (optionnel) | Réjection des parasites courts | Constante de temps ≪ 1000 µs | — |

### B5 — Entrées fin de course (×8)

Chaque contact inverseur : COM à la masse commande, NO et NC vers deux entrées (DEC-037).

| Élément | Fonction | Exigence | Candidats |
|---|---|---|---|
| Rappel au 3,3 V | Niveau haut contact ouvert | Courant ≥ courant minimal de commutation des contacts (PO-23) | Résistance, valeur à fixer après PO-23 |
| Résistance série + RC | Filtrage, limitation de courant ESD | Rebonds mécaniques filtrés en matériel et logiciel | — |
| Protection ESD | Câbles de 15 à 30 cm vers l'extérieur de la carte | Niveau 4 IEC 61000-4-2 | Réseau de diodes TVS multicanal |
| Connecteurs J4, J5 | 2 contacts × 3 fils par extrémité | Détrompés | PO-15 |

### B6 — Entrées d'extension (×4)

| Élément | Exigence | Remarque |
|---|---|---|
| Protection et limitation | Tension sur PA4 / PA5 ≤ 3,6 V (TT_a) ; ≤ 3,3 V recommandé | Écrêtage vers 3,3 V ou diviseur |
| Rappel configurable | Résistance de rappel haut ou bas, non montée par défaut | Type de capteur inconnu (PO-07) |
| Compatibilité ADC | Impédance de source compatible avec le temps d'échantillonnage | Condensateur au pied de l'entrée |
| Connecteur J8 | 4 entrées + 3,3 V + GND | — |

### B7 — Commande des relais

Architecture : `ARCHITECTURE_COMMANDE_VERIN.md` §5.

| Chaîne | Éléments en série sur la bobine | Composants |
|---|---|---|
| K1 | MOSFET N commandé par `K1_CMD` + interrupteur validé par la pompe de charge (`CP_DRV`) | MOSFET N à seuil logique (classe AO3400, à vérifier à VGS = 3,3 V), pompe de charge à condensateurs et diodes |
| K2 | Cavalier J7 + MOSFET N commandé par `K2_CMD` + interrupteur validé par le watchdog externe (`WDT_KICK`) | Watchdog externe à fenêtre (classe TPS3430 / TPS3431, à vérifier) |
| Commun | Rappel à la masse sur chaque grille ; diode de roue libre sur chaque bobine | — |
| Relecture `ARM_FB` | Mesure du nœud entre cavalier et MOSFET, via un diviseur vers 3,3 V | Cavalier présent et bobine au repos → niveau haut |

**[Reco]** Watchdog à fenêtre plutôt que simple : un firmware bloqué dans une boucle qui rafraîchit trop vite est aussi détecté.

### B13 — Interface opérateur

| Élément | Exigence | Remarque |
|---|---|---|
| 4 LED | Visibles boîtier fermé ou par conduit de lumière (mécanique) | Courant ≤ quelques mA par LED |
| Buzzer | Actif ou passif (PO-12) | Commandé par transistor, alimenté en 5 V |
| `STATUS_OUT` (X02) | Sortie vers le contrôleur, niveau compatible 3,3 / 5 V (PO-04) | Sortie à drain ouvert + résistance série + ESD |
| `MAINT_IN` (X03) | Entrée de mode maintenance | Cavalier ou bouton |
| `V5_MON` (X01) | Diviseur 5 V → ADC | — |
| `BOARD_ID` (X04) | Pont diviseur fixe par révision | Résistances 1 % |

## 3. Blocs du domaine puissance (12 V isolé)

### B10 — Entrée et protection 12 V (DEC-034)

| Élément | Fonction | Exigence | Candidats |
|---|---|---|---|
| Connecteur J2 | +12 V, 0 V système | Détrompé | PO-15 |
| Fusible | Protection du câblage | Calibre au-dessus de 200 mA (courant de blocage du vérin) | Fusible CMS ou CTP |
| Protection contre l'inversion + coupure en surtension | Source non maîtrisée ; vérin limité à 13,5 V | Coupure franche au-delà d'un seuil < 13,5 V | Contrôleur de protection à MOSFET externes avec seuil de surtension réglable (classe LM74502, à vérifier) |
| TVS | Transitoires sur le câble | Tension de veille au-dessus de la tension nominale maximale | TVS 15 V, à ajuster selon PO-03 |

### B9 — Pont de relais et vérin

| Élément | Exigence | Candidats |
|---|---|---|
| K1, K2 | DPDT, bobine 5 V ; contacts ≥ 1 A sous 30 V DC (vérin ≤ 0,2 A sous 12 V) ; isolement bobine-contacts ≥ 1 kV ; endurance électrique ≫ 200 cycles (DEC-005) ; 2e contact pour la relecture | Relais de signal DPDT 5 V (classe Panasonic TQ2-5V, Omron G6K-2, à vérifier) |
| Câblage | `ARCHITECTURE_COMMANDE_VERIN.md` §2 | — |
| J6, 5e contact | NC en série entre +12 V_P et le contact NO de K1 (DEC-029) | Microrupteur RS (DEC-038) |
| Suppression des parasites du moteur | CEM : bruit de balais | Condensateurs céramiques aux bornes du vérin, TVS bidirectionnelle |
| Connecteur J3 | Vérin, 2 fils | Détrompé |

### B11 — Optocoupleurs

| Signal | Côté 12 V | Côté commande | Candidats |
|---|---|---|---|
| `V12_OK` | LED alimentée par le +12 V_P protégé, résistance série | Phototransistor, rappel au 3,3 V | Optocoupleur à phototransistor (classe PC817 / EL357N) |
| `STOP_HW_FB` | LED alimentée à travers le 5e contact | Idem | Idem |

### B12 — Mesure de courant isolée (X05, PO-27)

| Option | Principe | Avantages | Inconvénients |
|---|---|---|---|
| A | Shunt côté 12 V + amplificateur isolé, alimenté côté 12 V par un petit régulateur | Précision, linéarité, bonne résolution à 200 mA | Régulateur côté 12 V, plus de composants |
| B | Capteur à effet Hall isolé intégré | Pas d'alimentation côté 12 V | Les calibres usuels (ampères) donnent une faible résolution à 200 mA |
| C | Comparateur côté 12 V + optocoupleur | Simple, robuste | Information tout ou rien (seuil de blocage) seulement |

**[Reco]** Option A. L'alimentation côté isolé est disponible localement (+12 V_P), aucun convertisseur isolé n'est nécessaire.

## 4. Implantation et isolation

| Règle | Détail |
|---|---|
| Séparation des domaines | Zone 12 V regroupée sur un bord de la carte ; aucune piste ni plan ne traverse la barrière, hors composants d'isolation |
| Distances d'isolement | À définir selon la tension de service (12 V) et la tenue souhaitée. Fente de fraisage sous les relais et optocoupleurs si nécessaire |
| Connecteurs | Regroupés par domaine sur un même bord (CEM, DEC-040) |
| Couches | 4 couches recommandées : plan de masse continu par domaine |

## 5. Bilan de courant 5 V (préliminaire)

| Consommateur | Courant | Base |
|---|---|---|
| Bobine K1 | ≈ 28 mA | Hypothèse : relais 140 mW sous 5 V, à confirmer au choix |
| Bobine K2 | ≈ 28 mA | Idem |
| MCU à 64 MHz | ≤ 7,0 mA (max à 85 °C, périphériques désactivés) | DS12232 Rev 5, tableau 25 |
| Périphériques MCU et logique 3,3 V | < 15 mA | Hypothèse |
| LED (4 × 2 à 5 mA) | ≤ 20 mA | Hypothèse |
| Buzzer | 20 à 30 mA | Hypothèse, selon référence |
| Rappels des fins de course (8) | Selon PO-23 | À calculer |
| Watchdog, buffers, optos côté sortie | Quelques mA | — |
| **Total estimé** | **≈ 130 à 160 mA**, hors rappels | Marge confortable sur 500 mA (DEC-032) |

## 6. Points à trancher avant le choix des références

| N° | Sujet |
|---|---|
| PO-03 | Plage du 12 V : seuils de protection |
| PO-12 | LED, type de buzzer |
| PO-15 | Dimensions, connecteurs |
| PO-23 | Courant minimal des fins de course |
| PO-27 | Option de mesure de courant (A, B ou C) |
| PO-29 | Valeurs du quartz, régulateur, relais, watchdog, protections : sélection et vérification LCSC (prochaine étape) |
