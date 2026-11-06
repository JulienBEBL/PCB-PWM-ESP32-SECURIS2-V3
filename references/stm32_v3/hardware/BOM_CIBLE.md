# BOM cible — PCB_PWM_SECURISE_V3

**Phase :** 1 (`CONSIGNES_EASYEDA_CLAUDE_CODE.md` §3). **Jalon :** G1, validation de Julien requise avant toute saisie.
**Version :** v1.3, session 13 (2026-11-04) : borniers à vis enfichables + barrettes Dupont (DEC-071), armement J7 (DEC-073), D17, LED5, R426 (DEC-074). v1.2 (2026-11-03) : diodes idéales LM66100 (DEC-067) ; codes de J7, J8, J11, R502, R504 résolus. v1.1 : USB, boutons, J10 ; Q1 supprimé. v1 : session 12, G1 validé (DEC-064).
**Fichier machine :** `hardware/bom/BOM_CIBLE.csv` (mêmes données, une ligne par repère).
**Connexions :** `hardware/NETLIST_CIBLE.md`.

## 0. Méthode et statut des données

- Chaque code LCSC a été **vérifié en ligne le 2026-11-03** sur la page produit JLCPCB ou LCSC : référence fabricant, boîtier, catégorie Basic / Extended, paramètres principaux. Les stocks sont des relevés ponctuels.
- **Basic** : pas de frais de chargement JLCPCB. **Extended** : frais fixes par référence (montant au devis).
- Statut de chaque ligne : **V** = vérifié ; **AV** = à vérifier (code ou paramètre non confirmé) ; **AD** = à définir (décision en attente, PO cité).
- Aucune valeur n'est inventée : toute valeur calculée renvoie à son calcul (§3).

## 1. Circuits intégrés et semi-conducteurs

| Repère | Fonction | Référence fabricant | Fabricant | LCSC | Cat. | Boîtier | Paramètres vérifiés | Statut |
|---|---|---|---|---|---|---|---|---|
| U1 | MCU | STM32G071CBT6 | ST | C432212 | Ext. | LQFP-48 7×7 | Cortex-M0+ 64 MHz, 128 Ko, 1,7–3,6 V, 44 E/S | V (DEC-036) |
| U2 | Régulateur 5 V → 3,3 V | ME6211C33M5G-N | Microne | C82942 | Ext. | SOT-23-5 | 3,3 V fixe, 500 mA, chute 120 mV à 100 mA, Vin ≤ 6 V | V |
| U3 | Buffer trigger de Schmitt (PWM) | SN74LVC1G17DBVR | TI | C7836 | Ext. | SOT-23-5 | 1,65–5,5 V | V ; seuils VT+ / VT− à 3,3 V : AV (datasheet TI) |
| U4 | Watchdog externe à fenêtre (chaîne K2) | TPS3430WDRCR | TI | C2870545 | Ext. | VSON-10 3×3, pastille | 1,6–6,5 V, WDO drain ouvert actif bas | V |
| U5 | Protection 12 V (inversion + surtension) | LM74502DDFR | TI | C3236215 | Ext. | SOT-23-8 (DDF) | 3,2–65 V, OV réglable, MOSFET N dos à dos | V |
| U6, U7 | Optocoupleurs (V12_OK, STOP_HW_FB) | EL357N(C)(TA)-G | Everlight | C29981 | Ext. | SOP-4 | 3,75 kVeff, CTR 200–400 % | V |
| U8 | Régulateur côté isolé 12 V → 5 V | HT7550-1 | Holtek | C16106 | Ext. | SOT-89-3 | Vin ≤ 30 V, 5 V, 150 mA | V |
| U9 | Amplificateur isolé (I_MON) | AMC1301DWVR | TI | C190273 | Ext. | SOIC-8 large (DWV) | ±250 mV, gain 8,2, sortie différentielle, 5 kVeff | V ; option A de PO-27 : AD |
| U10 | Amplificateur différentiel (I_MON) | TLV9001IDBVR | TI | C398363 | Ext. | SOT-23-5 | 1,8–5,5 V, rail à rail E/S | V ; idem PO-27 |
| U11, U12 | Protection ESD 4 voies (fins de course) | SRV05-4-P-T7 | ProTek | C85364 | Ext. | SOT-23-6 | Vrwm 5 V, 3,5 pF | V |
| ~~Q1~~ | ~~Protection inversion 5 V~~ | ~~AO3401A~~ | — | — | — | — | **Supprimé en v1.1** : remplacé par D15 (OU d'alimentation et inversion en un seul composant) | — |
| Q2 à Q9 | MOSFET N : Q2 STATUS, Q3 / Q4 chaîne K1, Q5 / Q6 chaîne K2, Q7 buzzer, Q8 / Q9 protection 12 V | AO3400A | AOS | C20917 | AV | SOT-23 | 30 V, 5,7 A, 48 mΩ à 2,5 V, VGS(th) 1,45 V | V ; catégorie AV |
| D1 | TVS 5 V | SMAJ5.0A | MDD | C113952 | Ext. | SMA | Vrwm 5 V, Vc 9,2 V, 400 W | V ; voir §4 point 3 |
| D2 | TVS 12 V | SMAJ15A | BORN | C152121 | Ext. | SMA | Vrwm 15 V, Vc 24,4 V, 400 W | V |
| D3 | TVS bidirectionnelle aux bornes du vérin | SMAJ15CA | BORN | C151939 | Ext. | SMA | Vrwm 15 V, bidirectionnelle | V |
| D4, D5, D6 | Roue libre (K1, K2, buzzer) | 1N4148W | Semtech | C81598 | Basic | SOD-123 | 75 V, 150 mA, 4 ns | V |
| D7 | Pompe de charge | BAT54S,215 | Nexperia | C47546 | Ext. | SOT-23 | 2 Schottky en série, 30 V | V |
| D8 | Écrêtage VGS (LM74502) | BZT52C10 | JSCJ | C2103 | AV | SOD-123 | Zener 10 V | Code relevé en recherche : AV |
| D17 | TVS 15 V sur `NO_FEED` (J6) | SMAJ15A | BORN | C152121 | Ext. | SMA | Vrwm 15 V | V (DEC-074) |
| D9 à D14 | ESD 1 voie (PWM, STATUS, EXT×4) | ESD5Z5V0 | Semtech | C85708 | Ext. | SOD-523 | Vrwm 5 V, Vbr 6,2 V, 80 pF | V |
| Y1 | Quartz HSE 12 MHz | X322512MSB4SI | YXC | C9002 | Basic | SMD3225-4P | 12 MHz, CL 20 pF, ±10 ppm, ±20 ppm stabilité | V ; marge d'oscillation AV (§4 point 1) |
| U15, U16 | Diodes idéales 5 V (J1, USB) | LM66100DCKR | TI | C2869734 | Ext. | SC-70-6 | 1,5–5,5 V, 1,5 A, 79 mΩ, −6 V en inverse, 6 V max absolu (datasheet SLVSEZ8A) | V |
| ~~D15, D16~~ | ~~B5819W~~ | — | — | — | — | — | **Supprimés en v1.2** : chute de 0,3 à 0,45 V refusée (« je veux du 5 V ») | — |
| U13 | Pont USB-série (USART1) | CH340C | WCH | C84681 | Ext. | SOP-16 | Horloge interne, sans quartz ; 3,3 V ou 5 V | V ; stock AV |
| U14 | Protection ESD USB | USBLC6-2SC6 | ST | C7519 | Ext. | SOT-23-6 | 2 lignes + VBUS | V ; stock AV |

## 2. Électromécanique, connectique, divers

| Repère | Fonction | Référence fabricant | LCSC | Cat. | Paramètres vérifiés | Statut |
|---|---|---|---|---|---|---|
| K1, K2 | Relais DPDT 5 V | Omron G6K-2F-Y-TR DC5 | C47190 | Ext. | Bobine 237 Ω, 21,1 mA, ≈ 100 mW ; enclenchement ≤ 80 % Un ; 1 A / 30 V DC, 60 V DC max ; 1 500 Veff bobine-contacts, 1 000 Veff entre pôles ; 100 000 manœuvres électriques ; −40 à +70 °C ; CMS 10 × 6,5 mm ; 62 676 en stock | V ; brochage AV (§4 point 2) |
| LS1 | Buzzer passif électromagnétique | MLT-8530 | C94599 | Ext. | 2,5–4,5 V, 95 mA, 2,7 kHz, 80 dB, −20 à +70 °C | V ; choix AD (PO-12) |
| F1 | Fusible réarmable 5 V | BSMD1206-050-30V | C883126 | Ext. | Ihold 500 mA, Itrip 1 A, 30 V | V |
| F2 | Fusible réarmable 12 V | BSMD1206-050-30V | C883126 | Ext. | Idem | V |
| LED1 | LED vert-jaune | NCD0603C1 | C84264 | Ext. | Vf 1,6–2,6 V | V ; couleurs AD (PO-12) |
| LED2 | LED jaune | 19-213/Y2C-CQ2R2L/3T(CY) | C72038 | Ext. | Vf 1,7–2,3 V | V ; idem |
| LED5 | LED « 3,3 V présent » (câblée en dur) | NCD0603C1 | C84264 | Ext. | Vf 1,6–2,6 V ; R426 680 Ω (C23228) | V (DEC-074) |
| LED3, LED4 | LED rouge | KT-0603R | C2286 | Basic | Vf 1,8–2,4 V | V ; idem |
| J1 | Contrôleur : 5 V, GND, PWM, STATUS | KANGNEX WJ15EDGRC-3.81-04P-14-00A | C7245 | Ext. | Bornier enfichable 4 pts, 3,81 mm, angle droit. Fiche à vis à commander à part | V (DEC-071) |
| J2 | Entrée 12 V | KANGNEX WJ2EDGRC-5.08-02P-14-00A | C3697 | Ext. | 2 pts, **5,08 mm** (seul bornier 5,08 à 2 points : détrompage avec J3) | V (DEC-071) |
| J3 | Vérin | KANGNEX WJ15EDGRC-3.81-02P-14-00A | C8387 | Ext. | 2 pts, 3,81 mm | V (DEC-071) |
| J4 | Fins de course « rentré » | KANGNEX WJ15EDGRC-3.81-06P-14-00A | C7246 | Ext. | 6 pts, 3,81 mm | V (DEC-071) |
| J5 | Fins de course « sorti » | KANGNEX WJ15EDGRC-3.81-07P-14-00A | C24525 | Ext. | **7 pts** (point 7 non câblé : détrompage avec J4) | V (DEC-071) |
| J6 | 5e contact (arrêt matériel) | KANGNEX WJ15EDGRC-3.81-03P-14-00A | C8406 | Ext. | 3 pts, 3,81 mm (point central non câblé) | V (DEC-071) |
| J7 | Armement (bobine de K2) | KANGNEX WJ2EDGRC-5.08-03P-14-00A | C8377 | Ext. | 3 pts, 5,08 mm ; armé = pont de fil entre 1 et 3 dans la fiche ; point 2 non câblé. (C8447 écarté : fiche de bibliothèque sans fabricant ni code fournisseur, avertissement ERC) | V (DEC-073) |
| ~~JP1~~ | ~~Capuchon de cavalier~~ | — | — | — | Supprimé en v1.3 (DEC-073) | — |
| J8 | Extension capteurs | XFCN PZ254V-11-06P | C492405 | Ext. | 1×6, 2,54 mm | V ; PO-07 |
| J9 | SWD | XFCN PZ254V-11-05P | C492404 | Ext. | 1×5, 2,54 mm, traversant | V |
| J10 | UART2 (atelier) | XFCN PZ254V-11-03P | C492402 | Ext. | 1×3, 2,54 mm, même famille que J9 | V ; stock AV |
| J12 | USB-C (PC) | HRO TYPE-C-31-M-12 | C165948 | Ext. | 16 broches, CMS, USB 2.0 | V ; stock AV |
| SW1, SW2 | Boutons RESET, BOOT | XKB TS-1187A-B-A-B | C318884 | Basic | CMS 5,1 × 5,1 mm | V |
| J13, J14, J15 | Barrettes Dupont en parallèle de J1, J4, J5 (banc) | XFCN PZ254V-11-04P / 06P / 07P | C492403 / C492405 / C492406 | Ext. | 2,54 mm, broche 1 du même côté que celle du bornier | V (DEC-071) |
| J11 | Mode maintenance (X03) | XFCN PZ254V-11-02P | C492401 | Ext. | 1×2, 2,54 mm | V |

**Fiches à commander à part (non assemblées par JLCPCB) :** une fiche à vis de la même série et du même nombre de points pour J1 à J7 (Kangnex 15EDGK-3,81 et 2EDGK-5,08). Codes LCSC à relever à la commande. **[Hypothèse]** compatibilité fiche / embase dans la même série Kangnex, à vérifier sur la datasheet.

**Règle de détrompage appliquée :** aucun couple (série, nombre de points) n'apparaît deux fois parmi les connecteurs de câblage externe (J1 à J6, J8). Elle évite trois inversions dangereuses : vérin branché sur l'entrée 12 V (J2 / J3), faisceaux « rentré » et « sorti » permutés (J4 / J5), 5e contact branché à la place du vérin (J3 / J6).

## 3. Passifs

Tous en 0603 sauf mention. Résistances : ±1 %, 100 mW. Condensateurs : X7R / X5R / C0G selon la ligne.

| Valeur | Type | LCSC | Cat. | Usage | Statut |
|---|---|---|---|---|---|
| 0 Ω | Résistance | C21189 | Basic | Options (R501, R411 non monté) | V |
| 10 Ω | Résistance | C22859 | Basic | Filtre d'entrée AMC1301 | V |
| 33 Ω | Résistance | C23140 | Basic | Sortie buffer PWM | V |
| 100 Ω | Résistance | C22775 | Basic | Séries de grille, sorties | V |
| 680 Ω | Résistance | C23228 | Basic | LED (≈ 1,9 mA, §4 point 6) | V |
| 1 kΩ | Résistance | C21190 | Basic | Séries d'entrée, rappels des fins de course | V ; rappels AD (PO-23) |
| 4,7 kΩ | Résistance | C23162 | Basic | Séries EXT, LED des optocoupleurs | V |
| 5,1 kΩ | Résistance | C23186 | Basic | CC1, CC2 de l'USB-C (R601, R602) | V |
| 10 kΩ | Résistance | C25804 | Basic | Rappels, diviseurs, ampli différentiel | V |
| 20 kΩ | Résistance | C4184 | Basic | Diviseur ARM_FB | V |
| 95,3 kΩ ±1 % | Résistance | C23267 | Ext. | Seuil OV du LM74502 (R502, DEC-065) | V |
| 100 kΩ | Résistance | C25803 | Basic | Rappels de grille, purge pompe | V |
| 1 Ω ±1 %, 1206, 250 mW | Shunt | C17928 | Basic | Mesure du courant du vérin (R504). Attention : C25334 (même série) vaut 100 mΩ | V ; PO-27 |
| 33 pF C0G 50 V | Condensateur | C1663 | Basic | Charge du quartz | V ; valeur AV (§4 point 1) |
| 1 nF X7R 50 V | Condensateur | C1588 | Basic | Filtre PWM | V |
| 10 nF X7R 50 V | Condensateur | C57112 | Basic | Filtres d'entrée | V |
| 100 nF X7R 50 V | Condensateur | C14663 | Basic | Découplage, pompe de charge | V |
| 220 nF X7R 25 V | Condensateur | C21120 | Basic | VCAP du LM74502 | V |
| 1 µF X5R 50 V | Condensateur | C15849 | Basic | Régulateurs, AMC1301 | V |
| 4,7 µF X5R 16 V | Condensateur | C19666 | Basic | MCU, sortie 3,3 V | V |
| 10 µF X5R 25 V, 0805 | Condensateur | C15850 | Basic | Réservoirs 5 V, 12 V, 5 V isolé, buzzer | V |

## 4. Calculs et points à vérifier

| N° | Sujet | Détail | Statut |
|---|---|---|---|
| 1 | Quartz | CL = 20 pF (C9002). Condensateurs : C = 2 × (CL − Cparasite). Avec Cparasite ≈ 3 à 5 pF : 30 à 34 pF → **33 pF** retenu. La marge de gain de l'oscillateur HSE du STM32G0 doit être vérifiée selon AN2867 (ESR et C0 du quartz, gm du G0). Si la marge est insuffisante, choisir un quartz à CL plus faible. | AV |
| 2 | Relais G6K-2F-Y | Brochage 1 à 8 et polarité éventuelle de la bobine non extraits de la datasheet : à lire sur le symbole LCSC C47190 et la datasheet Omron (Cat. K106). Le pôle B (relecture) est côté commande, le pôle A côté puissance : isolement entre pôles 1 000 Veff (datasheet Omron). | AV, PO-36 |
| 3 | TVS 5 V (D1) | Vc 9,2 V à 400 W. Tension maximale absolue de U2 (ME6211) et de U1 à vérifier : un transitoire écrêté peut la dépasser brièvement. | AV |
| 4 | Seuil OV du LM74502 | V_OV = 1,25 V × (R + 10 kΩ) / 10 kΩ. Avec R = 95,3 kΩ : **13,16 V typique**. Tolérance du seuil (1,165 à 1,333 V) : **12,27 à 14,04 V**. L'exigence « coupure au-delà de 13,5 V » (DEC-034) n'est donc pas garantie dans le pire cas. | AD, PO-35 |
| 5 | VGS des MOSFET de U5 | Le LM74502 demande VGS ≥ 15 V ou une Zener grille-source (datasheet TI). AO3400A : VGS max ±12 V → Zener 10 V (D8). | V |
| 6 | LED | (3,3 V − 2,0 V) / 680 Ω ≈ 1,9 mA. Visibilité à valider sur prototype. | V |
| 7 | Relais | Enclenchement ≤ 4,0 V (80 % de 5 V). v1.2 : diodes idéales, chute ≈ 16 mV à 200 mA : `+5V` ≈ 5,0 V. Marge ≈ 1 V. | V |
| 8 | Buzzer | 95 mA crête sous 3,6 V. Alimenté en 3,3 V, commandé en PWM 50 % : environ 45 mA en moyenne. | V ; choix PO-12 |

## 5. Bilan de courant 5 V (mis à jour)

| Consommateur | Courant | Base |
|---|---|---|
| K1 + K2 | 42,2 mA | 2 × 21,1 mA (Omron) |
| MCU | ≤ 7,0 mA | DS12232 Rev 5, tableau 25 |
| Rappels des fins de course | ≈ 13,2 mA | 4 contacts fermés × 3,3 mA (rappel 1 kΩ, PO-23) |
| LED | ≤ 7,6 mA | 4 × 1,9 mA |
| Buzzer | ≈ 45 mA moyen, 95 mA crête | §4 point 8 |
| AMC1301 côté sortie, ampli, watchdog, buffer, optos | < 15 mA | Hypothèse, à confirmer sur datasheets |
| Pont USB U13 (CH340C) | **[Hypothèse]** < 20 mA | Datasheet WCH à confirmer |
| **Total** | **≈ 130 mA moyen, ≈ 180 mA crête** | Fusible F1 : Ihold 500 mA ; budget 500 mA (DEC-032) |
