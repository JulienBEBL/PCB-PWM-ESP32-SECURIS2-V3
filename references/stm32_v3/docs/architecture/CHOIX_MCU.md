# Choix du microcontrôleur — comparatif

**Statut :** **STM32G071CBT6 (LQFP48) validé** (DEC-036, session 05). Vérifications d'approvisionnement et de fabrication en section 12.
**Objectif du document :** permettre à Julien de comprendre le fonctionnement, l'intégration PCB et la complexité de chaque solution avant de choisir.

---

## 1. Rôle du MCU dans l'architecture

| Le MCU porte | Le MCU ne porte pas (matériel indépendant) |
|---|---|
| Mesure du PWM, validation des trames | Retour en position sûre (relais au repos) |
| Machine d'états, temporisations, séquences | Barrière d'armement (cavalier, DEC-025) |
| Lecture et cohérence des fins de course | Arrêt matériel en position d'action (DEC-029) |
| Diagnostic : relecture relais, présence 12 V | Coupure sur blocage logiciel (pompe de charge, watchdog externe) |
| LED, buzzer, traces de diagnostic | Isolation 5 V / 12 V |

Conséquence : le MCU n'a pas besoin d'être un composant certifié sécurité. Il doit être **déterministe, bien documenté, simple à intégrer et facile à programmer et déboguer**.

## 2. Besoin en E/S (mis à jour session 03)

| Fonction | E/S | Remarque |
|---|---|---|
| Entrée PWM | 1 | Broche de capture d'un timer |
| Fins de course, lecture antivalente NO + NC de chaque contact | 8 | 4 contacts × 2 (PO-06). 4 si une seule voie par contact |
| Capteurs d'extension | 4 | Réservation (PO-07) |
| Présence 12 V | 1 | DEC-024 |
| Relecture K1, K2 | 2 | DEC-023 |
| Relecture cavalier | 1 | DEC-025 |
| Relecture arrêt matériel | 1 | Optionnel (DEC-029) |
| Commande K1, K2 | 2 | |
| Pompe de charge | 1 | Sortie timer |
| Rafraîchissement watchdog externe | 1 | |
| LED | 4 | PO-12 |
| Buzzer | 1 | Sortie timer si buzzer passif |
| UART diagnostic / programmation | 2 | |
| Débogage | 1 à 2 | SWD = 2, UPDI = 1 |
| **Total fonctionnel** | **≈ 31** | Hors quartz, reset et alimentation |

Une marge d'environ 20 % conduit à **≥ 37 E/S disponibles**, soit un boîtier **48 broches** minimum.

## 3. Comment un MCU fonctionne sur cette carte

| Bloc interne | Rôle ici | Ce que cela impose au PCB |
|---|---|---|
| Alimentation et supervision (POR / BOR) | Le MCU reste en reset tant que sa tension est insuffisante. Pendant le reset, **toutes les broches sont en haute impédance**. | Découplage sur chaque broche d'alimentation. Résistances de rappel externes sur les commandes de relais pour garantir « relais au repos » pendant le reset. |
| Horloge | Cadence le CPU et les timers. Sa précision fixe l'erreur de mesure du PWM. | Quartz et deux condensateurs si l'oscillateur interne n'est pas assez précis. |
| Timer en capture | Mesure la largeur et la période du PWM en matériel, sans latence logicielle. | Le PWM doit arriver sur une broche reliée à un canal de capture. |
| GPIO | Lit les fins de course, commande les relais et les LED. | Protection et filtrage des entrées externes (ESD, RC, résistances série). |
| Watchdog interne | Réinitialise le MCU si le programme ne le rafraîchit plus. | Aucun. |
| UART | Traces de diagnostic, éventuellement programmation. | Connecteur 3 ou 4 broches. |
| Port de programmation / débogage | Chargement du firmware, points d'arrêt. | Connecteur dédié, accessible boîtier ouvert. |
| Mémoire flash | Contient le firmware. Interne pour tous les candidats retenus. | Aucun. |

### Mesure du PWM en matériel

| Famille | Mécanisme | Résolution typique |
|---|---|---|
| STM32 | Timer en mode « PWM input » : front montant → remise à zéro du compteur et capture de la période ; front descendant → capture de la largeur. Deux registres lus par le firmware. | 1 µs avec un compteur à 1 MHz (réglable plus fin) |
| AVR 0 / Dx | Timer TCB en mode « mesure de largeur d'impulsion » : remise à zéro sur un front, capture sur le front opposé. | 0,05 à 0,1 µs selon l'horloge |
| AVR classique (328PB) | Capture ICP sur un timer 16 bits, changement de front par le firmware | 0,5 µs à 16 MHz avec prédiviseur 8 |

Les trois mécanismes suffisent pour une impulsion de 1000 à 2000 µs. Ce critère ne départage pas les candidats.

### Erreur d'horloge

L'erreur relative de l'horloge se reporte sur la mesure. À 1500 µs : 1 % → 15 µs ; 3 % → 45 µs. Avec un quartz (≤ 50 ppm), l'erreur devient négligeable (< 0,1 µs). **[Reco]** Quartz ou oscillateur externe quel que soit le MCU, sauf si les seuils de la FSM laissent une marge supérieure à l'erreur de l'oscillateur interne sur toute la plage de température.

### Compatibilité PWM 3,3 V / 5 V

**[Reco]** Un buffer à trigger de Schmitt en entrée, alimenté par le rail du MCU et tolérant 5 V, traite ce point quel que soit le MCU. Il apporte aussi le filtrage et la protection ESD. Ce critère ne départage donc pas les candidats.

---

## 4. Panorama des solutions existantes

Première sélection sur les critères éliminatoires : framework Arduino mature (DEC-050), pas de radio, ≥ 37 E/S, mémoire programme interne, disponibilité LCSC plausible.

| Famille / référence | Verdict | Motif |
|---|---|---|
| **STM32G071CBT6** (ST, Cortex-M0+) | Retenu | Voir §5 |
| **STM32C031C6T6 / STM32C071C8T6** (ST, Cortex-M0+) | Retenu | Variante économique du G0 |
| **ATmega4809** (Microchip, AVR 8 bits « megaAVR 0 ») | Retenu | Voir §5 |
| **AVR128DB48 / AVR64DB48** (Microchip, AVR Dx) | Retenu | Voir §5 |
| ATmega328PB (AVR classique, base Arduino Uno) | Écarté | 27 E/S dont quartz et reset : environ 24 utiles pour 31 nécessaires |
| ATmega2560 (Arduino Mega) | Écarté | 100 broches, ancien, surdimensionné, coût |
| Renesas RA4M1 (Arduino Uno R4) | Écarté | Cœur Arduino officiel mais conçu pour les cartes Arduino ; une carte personnalisée demande d'adapter les définitions de carte. Écosystème moins documenté hors Arduino. À reconsidérer seulement si le 5 V natif devient prioritaire. |
| SAMD21G18 (Arduino Zero) | Écarté | Aucun avantage sur le STM32G0 ; chargement initial du bootloader avec une sonde dédiée ; 3,3 V uniquement |
| RP2040 / RP2350 (Raspberry Pi) | Écarté | Programme en flash QSPI externe (composant et point de défaillance supplémentaires), boîtier QFN 0,4 mm |
| ESP32 (toutes variantes) | Écarté | Radio intégrée inutile, broches de configuration actives au démarrage |
| nRF52 (Nordic) | Écarté | Radio intégrée inutile |
| STM32F103 (« Blue Pill ») | Écarté | Génération ancienne, nombreuses contrefaçons sur le marché, remplacé par le G0 |
| GD32, APM32, CH32 (compatibles ou RISC-V chinois) | Écarté | Support Arduino moins mature, documentation partielle, outils de programmation propriétaires (CH32) |
| PIC16 / PIC18 (Microchip) | Écarté | Pas de support Arduino courant (DEC-050) |
| Logique câblée, CPLD, FPGA | Écarté | FSM avec temporisations et diagnostic : complexité sans bénéfice |
| Module Arduino ou carte de développement soudée sur la carte | Option d'intégration, voir §7 | Ce n'est pas un MCU différent, c'est une autre manière d'intégrer l'un des retenus |

---

## 5. Comparatif détaillé des candidats retenus

| Critère | STM32G071CBT6 | STM32C031C6T6 | ATmega4809 | AVR128DB48 |
|---|---|---|---|---|
| Cœur, fréquence max | Cortex-M0+ 32 bits, 64 MHz | Cortex-M0+ 32 bits, 48 MHz | AVR 8 bits, 20 MHz | AVR 8 bits, 24 MHz |
| Flash / RAM | 128 Ko / 36 Ko (32 Ko avec parité matérielle) | 32 Ko / 12 Ko | 48 Ko / 6 Ko | 128 Ko / 16 Ko |
| Tension d'alimentation | 1,7–3,6 V | 2,0–3,6 V | 1,8–5,5 V | 1,8–5,5 V |
| Boîtier 48 broches, E/S | LQFP48 0,5 mm, jusqu'à 44 | LQFP48 0,5 mm, jusqu'à 45 | TQFP48 0,5 mm, 41 | TQFP48 0,5 mm, 40 (+ PF6 en entrée seule) |
| Timers utiles | 1 avancé, 1 × 32 bits, 5 × 16 bits | Équivalent, moins nombreux | 1 TCA, 4 TCB | 2 TCA, 4 TCB, 1 TCD |
| Horloge interne | HSI 16 MHz ±1 % (datasheet ST) | HSI 48 MHz (précision à lire) | OSC20M : ±1,5 % à 25 °C, ±3 % sur la plage complète | Oscillateur interne (précision à lire) |
| Quartz principal | Oui (4–48 MHz) | Oui | **Non** : horloge externe (oscillateur) seulement | Oui |
| Watchdog | IWDG sur oscillateur LSI indépendant + WWDG (fenêtré) | IWDG + WWDG | WDT sur oscillateur 32 kHz interne, mode fenêtré | WDT, mode fenêtré |
| Supervision | POR, BOR programmable, PVD, surveillance d'horloge (CSS) | POR, BOR, CSS | POR, BOD | POR, BOD |
| Intégrité mémoire | Parité SRAM, unité CRC | Unité CRC | CRCSCAN (CRC de la flash en matériel) | CRCSCAN |
| Logique programmable interne | Non | Non | CCL : 4 LUT | CCL : 6 LUT |
| Cœur Arduino | STM32duino (maintenu par ST) | STM32duino | MegaCoreX, cœur Nano Every | DxCore (communautaire, très actif) |
| Programmation | SWD (sonde ST-LINK) ou bootloader UART en ROM | Idem | UPDI : 1 broche, adaptateur USB-série + résistance | UPDI |
| Débogage pas à pas (points d'arrêt) | **Oui**, avec une sonde ST-LINK (PlatformIO, STM32CubeIDE) | Oui | Sonde Microchip dédiée (PICkit, Atmel-ICE) et outils Microchip | Idem 4809 |
| Carte de développement pour coder avant le PCB | Nucleo-G071RB (ST-LINK intégré) | Nucleo-C031C6 | Arduino Nano Every, Curiosity Nano | Curiosity Nano AVR128DB48 |
| Référence LCSC | C432212 | À vérifier | C189475 (ATMEGA4809-AFR) | À vérifier |
| Stock LCSC, catégorie Basic / Extended | À vérifier au gel du schéma | À vérifier | À vérifier | À vérifier |

Sources : page produit ST STM32G071CB ; datasheets Microchip megaAVR 0-series et AVR DB (sections « Oscillators and Clocks », « Peripheral Overview »). Toute valeur marquée « à lire » ou « à vérifier » doit être confirmée dans la datasheet PDF avant le gel du choix.

## 6. Intégration PCB : ce qu'il faut dessiner

| Élément | STM32G0 / C0 | ATmega4809 | AVR DB |
|---|---|---|---|
| Régulateur 5 V → 3,3 V | **Oui** (LDO, ≥ 100 mA) | Non : fonctionne en 5 V | Non : fonctionne en 5 V |
| Découplage | 100 nF par broche VDD + 4,7 µF ; filtrage VDDA si la broche est séparée dans le boîtier retenu (à lire dans le brochage) | 100 nF par broche VDD + 1 µF | 100 nF par broche VDD + 1 µF ; broche VDDIO2 à relier |
| Reset | NRST : 100 nF vers la masse (rappel interne) | Broche UPDI/RESET partagée | Idem |
| Mode de démarrage | Broche BOOT0 partagée avec SWCLK (PA14) et octets d'option : **piège classique à figer au premier flash** | Aucun | Aucun |
| Horloge | Quartz + 2 condensateurs de charge (valeur calculée selon la capacité de charge du quartz) | Oscillateur externe si la précision interne est insuffisante | Quartz + 2 condensateurs |
| Connecteur de programmation | SWD 5 broches (3,3 V, SWDIO, SWCLK, NRST, GND) | UPDI 3 broches (UPDI, 5 V, GND) | UPDI 3 broches |
| Nombre de composants autour du MCU | ≈ 12 à 15 | ≈ 6 à 8 | ≈ 8 à 10 |
| Contraintes de routage | Boîtier 0,5 mm (assemblé par JLCPCB), quartz au plus près | Idem sans quartz | Idem |
| Rails sur la carte | 5 V (relais, buzzer) + 3,3 V (MCU, logique) | 5 V seul | 5 V seul |

Point commun : les trois boîtiers 48 broches au pas de 0,5 mm sont assemblés par JLCPCB. Le soudage manuel reste possible avec du flux et une panne fine, mais il est délicat.

## 7. Intégration en module

Une carte de développement (Nano Every, carte STM32 compacte) soudée ou embrochée sur la carte principale.

| Avantages | Inconvénients |
|---|---|
| Aucun risque de routage autour du MCU | Connecteurs à broches sensibles aux vibrations et à l'humidité (extérieur, DEC-004) |
| USB et programmation intégrés | Régulateur, convertisseur USB et protections du module non maîtrisés |
| Remplacement immédiat en cas de destruction | Plan de masse interrompu : défavorable à l'objectif CEM / IEM (DEC-040, DEC-041) |
| | Encombrement en hauteur, vernis de tropicalisation difficile |
| | Brochage imposé par le module |

**[Reco]** Module pour le banc de développement firmware, MCU soudé pour la carte finale.

## 8. Complexité, synthèse

| Axe | STM32G071 | STM32C031 | ATmega4809 | AVR128DB48 |
|---|---|---|---|---|
| Intégration matérielle | Moyenne (régulateur, quartz, BOOT0) | Moyenne | **Faible** | Faible à moyenne (quartz) |
| Configuration firmware | Moyenne : nombreux périphériques, mais le cœur Arduino masque l'essentiel | Moyenne, mémoire plus juste | Faible à moyenne | Faible à moyenne |
| Mise au point (debug) | **Facile** : points d'arrêt avec une sonde peu chère | Facile | Traces série surtout ; points d'arrêt avec sonde Microchip | Idem 4809 |
| Robustesse / supervision | **La plus complète** (CSS, parité RAM, double watchdog) | Bonne | Bonne (CRCSCAN), pas de quartz | Bonne (CRCSCAN, quartz) |
| Marge pour les évolutions | Large | Faible | Moyenne | Large |
| Documentation et communauté | Très large | Large | Large | Moyenne (DxCore) |

## 9. Recommandation

**[Reco]** **STM32G071CBT6**, avec deux réserves explicites :
- il impose un rail 3,3 V et un quartz, donc environ 6 composants de plus qu'un AVR ;
- la configuration de démarrage (BOOT0 / octets d'option) doit être figée et documentée dès la première carte.

**Alternative sérieuse :** **AVR128DB48**. Elle fonctionne directement en 5 V, accepte un quartz, offre 40 E/S et 128 Ko de flash. Ses limites : débogage pas à pas plus coûteux et cœur Arduino communautaire.

**ATmega4809 :** c'est l'intégration la plus simple. Mais il n'accepte pas de quartz principal, et son oscillateur interne dérive de ±3 % sur toute la plage de température. Ce n'est acceptable que si les seuils de la FSM laissent cette marge.

## 10. Démarche proposée pour décider

1. Choisir sur les critères ci-dessus.
2. Commander la carte de développement correspondante (§5) : le firmware démarre **avant** le PCB, sur banc avec le vérin.
3. Vérifier le stock LCSC et la catégorie (Basic / Extended) de la référence finale au moment du gel du schéma.

---

## 11. Intégration concrète du STM32G071 sur la carte (session 04)

Contexte : MCU soudé directement sur le PCB, pas de module (DEC-035). Julien ne veut pas d'un composant lourd à intégrer.

### Pourquoi le G0 est la famille STM32 la plus simple à intégrer

| Ce qui rend un STM32 lourd à intégrer | STM32G071 |
|---|---|
| Nombreuses paires d'alimentation (F4, H7 : 4 à 10 paires VDD/VSS) | Une seule paire VDD/VDDA – VSS/VSSA, plus VREF+ et VBAT |
| Alimentation du cœur externe ou SMPS (H7, U5) | Régulateur interne, aucun composant |
| Boîtier BGA ou QFN au pas de 0,4 mm | LQFP48, pattes visibles, pas de 0,5 mm |
| Quartz obligatoire pour l'USB ou une horloge précise | Quartz facultatif (HSI 16 MHz ±1 % selon ST) |
| Broche BOOT0 dédiée à câbler | Pas de broche dédiée : BOOT0 est partagé avec une GPIO et réglé par octets d'option |

### Schéma minimal autour du MCU

```
 5 V carte ──► LDO 3,3 V ──┬──► VDD/VDDA ─┬─ 100 nF + 4,7 µF ── GND
                           │              │
                           ├──► VREF+ ────┴─ 100 nF
                           └──► VBAT (relié à VDD, 100 nF)

 NRST ── 100 nF ── GND          (rappel interne, bouton reset facultatif)

 PF0-OSC_IN / PF1-OSC_OUT ── quartz HSE + 2 condensateurs   (empreinte prévue, montage facultatif)
 (PC14/PC15 = oscillateur 32 kHz LSE, non utilisé)

 Connecteur SWD : 3V3 · SWDIO (PA13) · SWCLK (PA14) · NRST · GND
 Connecteur UART : TX · RX · GND   (traces de diagnostic + bootloader ROM)
```

Valeurs de découplage : à appliquer selon la note d'application ST AN5096 (« Getting started with STM32G0 hardware development ») au moment du schéma.

| Composant | Quantité |
|---|---|
| STM32G071CBT6 (LCSC C432212) | 1 |
| Régulateur 5 V → 3,3 V (référence à choisir parmi les pièces Basic JLCPCB) | 1 |
| Condensateurs (LDO, découplage, NRST) | 6 à 8 |
| Quartz et 2 condensateurs de charge | 3 (facultatif) |
| Connecteur SWD | 1 |
| Connecteur UART | 1 (facultatif si UART sur le connecteur SWD) |
| **Total** | **≈ 10 à 14** |

### Encombrement et routage

- Boîtier : 7 × 7 mm, 9 × 9 mm avec les pattes. Zone MCU avec découplage et quartz : de l'ordre de 15 × 15 mm (estimation, à confirmer au placement).
- Routage faisable en 2 couches. **[Reco]** 4 couches pour l'objectif CEM / IEM (DEC-040, DEC-041) : plan de masse continu sous le MCU, sans surcoût bloquant chez JLCPCB pour 5 à 10 cartes.
- Assemblage par JLCPCB (pas de soudure manuelle du MCU nécessaire). Reprise manuelle possible au fer avec flux.

### Broches réservées

| Broche | Usage imposé |
|---|---|
| PA13, PA14 | SWD (PA14 porte aussi BOOT0) |
| PA9, PA10 (ou PA2, PA3) | USART du bootloader ROM (programmation sans sonde) |
| Broche de canal 1 d'un timer | Entrée PWM (mode « PWM input ») |
| PF0, PF1 | Quartz HSE, si monté. Correction session 06 : la version précédente citait PC14 / PC15, qui sont les broches du quartz 32 kHz (LSE) |

Le reste est réparti au plan de brochage, après gel de la liste des E/S. Avec 44 GPIO disponibles en LQFP48 et environ 31 nécessaires, la marge est d'environ 13 broches.

### Variante plus petite : STM32G071KBT6 (LQFP32)

| | LQFP48 (CBT6) | LQFP32 (KBT6, LCSC C529348) |
|---|---|---|
| GPIO | 44 | 30 |
| Pas des pattes | 0,5 mm | 0,8 mm (soudure manuelle plus facile) |
| Compatible avec le besoin actuel (≈ 31 E/S) | Oui, marge ≈ 13 | **Non** : il manque au moins 1 E/S, sans marge, et 2 de plus avec un quartz |

Le LQFP32 n'est viable qu'en réduisant le besoin : 4 entrées de fin de course au lieu de 8 (PO-06), ou 2 capteurs d'extension au lieu de 4 (PO-07).

### Côté firmware

- Cœur STM32duino : variante générique « G071CBTx » sélectionnée dans l'IDE Arduino ou PlatformIO. Pas de fichier de carte à créer.
- Horloge : HSI interne par défaut. Le passage au quartz se fait en redéfinissant une fonction de configuration d'horloge dans le code (pris en charge par Claude).
- Octets d'option (démarrage, BOR) : réglés une fois avec STM32CubeProgrammer, procédure à documenter dans `firmware/`.

Source GPIO et bootloader : datasheet ST STM32G071x8/xB (tableau 2, section 3.5).

---

## 12. Vérifications avant validation (session 05)

Julien valide sous condition d'absence de problème physique, d'assemblage, d'approvisionnement JLCPCB / LCSC, de débogage et de fabrication.

| Critère | Constat (2026-11-02) | Source | Verdict |
|---|---|---|---|
| Référence LCSC | C432212, STM32G071CBT6, LQFP-48 (7 × 7 mm) | Page JLCPCB et page LCSC du composant | OK |
| Stock LCSC | 988 pièces en stock | Page LCSC | OK pour 10 cartes. Relevé ponctuel : à revérifier à la commande |
| Prix unitaire | 2,48 $ (1+), 2,15 $ (10+) | Page LCSC | OK |
| Catégorie JLCPCB | **Extended** | Page JLCPCB | OK : frais fixes de chargement par référence Extended, montant à lire au devis |
| Type d'assemblage | SMT, PCBA « Economic » et « Standard » | Page JLCPCB | OK |
| Sensibilité à l'humidité | MSL 3 | Page JLCPCB | Sans impact : géré par JLCPCB |
| Réservation | Pièces achetables à l'avance dans la bibliothèque JLCPCB (non expédiables séparément) | Page JLCPCB | Recommandé si le stock baisse |
| Boîtier | LQFP 0,5 mm, pattes visibles | Datasheet ST | OK : inspection visuelle et reprise au fer possibles |
| Symbole et empreinte EasyEDA Pro | Présents dans la bibliothèque EasyEDA Pro (934 en stock le 2026-11-03) | Capture de Julien : `raw/2026-11-03_easyeda_C432212_bibliotheque.png` | OK (session 08) |
| Débogage | SWD, sonde ST-LINK, points d'arrêt sous PlatformIO / STM32CubeIDE | ST | OK. Utiliser une sonde ST d'origine (STLINK-V3MINIE) ou le ST-LINK intégré à une carte Nucleo, pas un clone |
| Programmation sans sonde | Bootloader ROM sur USART (PA9 / PA10 ou PA2 / PA3) | Datasheet ST, section 3.5 | OK, solution de secours |

**Aucun point bloquant.** Toutes les conditions de DEC-036 sont vérifiées.

Sources :
- https://jlcpcb.com/partdetail/STM32G071CBT6/C432212
- https://lcsc.com/product-detail/image/STM32G071CBT6_C432212.html

---

## 13. Précision de l'oscillateur interne HSI16 (session 08)

Source : DS12232 Rev 2, tableau 41.

| Paramètre | Condition | Valeur |
|---|---|---|
| Fréquence après calibration usine | VDD = 3,0 V, TA = 30 °C | 15,88 à 16,08 MHz, soit −0,75 % / +0,5 % |
| Dérive en température | TA = 0 à 85 °C | −1 % / +1 % |
| Dérive en température | TA = −40 à 125 °C | −2 % / +1,5 % |
| Dérive en tension | VDD = 1,62 à 3,6 V | −0,1 % / +0,05 % |

La plage du projet (−10 à +50 °C, DEC-004) sort de la bande 0 à 85 °C : seule la bande −40 à 125 °C s'applique. Erreur cumulée la plus défavorable, sans recalibration : environ **−2,85 % / +2,05 %**, soit environ −43 µs / +31 µs sur une impulsion de 1500 µs.

**[Reco]** Monter le quartz HSE (PF0 / PF1). Avec un quartz de ±50 ppm, l'erreur devient inférieure à 0,1 µs et ne contraint plus les seuils de la FSM. **Validé : DEC-060.**

---

## 14. Mise à jour DS12232 Rev 5 (session 10)

- Tableau 41 (HSI16) inchangé : conclusions du §13 confirmées.
- Figure 13 : le condensateur de 1 µF sur VREF+ n'est requis qu'avec VREFBUF (non utilisé) : 100 nF seul.
- Figure 23, note 2 : le niveau de NRST doit dépasser VIH(NRST) à la mise sous tension.
- §3.5 : le bootloader ROM sort certaines GPIO de l'état haute impédance → règle R9 et brochage v2 (`PLAN_BROCHAGE_MCU.md`).
- Errata ES0418 Rev 6 : `docs/composants/STM32G071_ERRATA.md`.
