# Reprise de la carte STM32

Source : dépôt `JulienBEBL/PCB_PWM_SECURISE_V3`, commit `e73ff39` (2026-11-06). Copie figée : `references/stm32_v3/`.

Principe (DEC-002, DEC-004) : **tout ce qui n'est pas lié au STM32 est repris tel quel.** Seuls changent le microcontrôleur et les fonctions que la carte de développement ESP32 fournit déjà.

Légende : **R** = repris tel quel · **A** = adapté · **S** = supprimé · **?** = à trancher (PO cité).

## 1. Décisions STM32

| Décisions STM32 | Sujet | Statut ici | Remarque |
|---|---|---|---|
| DEC-001 à DEC-005 | Fonction, usage, outils, environnement, endurance | R | DEC-002 (volume) et DEC-003 (EasyEDA, JLCPCB) inchangés |
| DEC-010 à DEC-019 | Vérin L12, 12 V, position sûre rentrée, course 3 mm, 30 N | R | Version du vérin : STM32 PO-18, commune aux deux cartes |
| DEC-020 à DEC-029 | Sécurité : aucune défaillance simple, isolation, 2 relais DPDT, relecture, opto 12 V, cavalier, 3 barrières, arrêt matériel | R | Cœur du projet. Confirmé par DEC-003 de ce dépôt |
| DEC-030 à DEC-034 | Entrées PWM, alimentation 5 V, LED, buzzer, extensions, protection 12 V | R | |
| DEC-035 | MCU soudé sur le PCB | **S** | Remplacé par DEC-006 (carte de dev enfichée) |
| DEC-036 | STM32G071CBT6 | **S** | Remplacé par l'ESP32-S3-DevKitC-1-N8 (DEC-015) |
| DEC-037, DEC-038 | Fins de course NO + NC, 8 entrées | R | |
| DEC-039 | Fonctions X01 à X08 | A | X05 supprimée (DEC-016). X01, X02 reprises. X03 supprimée (DEC-022). X04 supprimée (DEC-017). `BROCHAGE_ESP32.md` §3 |
| DEC-040 à DEC-043 | CEM / IEM | R | Point d'attention : la carte de dev n'est pas conçue pour ces niveaux (PO-03) |
| DEC-050, DEC-051 | C++ / Arduino, Claude écrit, Julien teste | R | Cible : ESP32 Arduino au lieu de STM32duino |
| DEC-052, DEC-053 | Dépôt GitHub, licence Apache 2.0 | A | Dépôt séparé (DEC-013), même licence |
| DEC-060 | Quartz HSE | **S** | Fourni par la carte de dev |
| DEC-061, DEC-063 | EasyEDA piloté par Claude Code via l'API | **S** | Conception manuelle par Julien (DEC-001) |
| DEC-062 | Assemblage complet JLCPCB | R | Confirmé par DEC-012 |
| DEC-064 | Jalons G1, G2 | — | Propres au dépôt STM32 |
| DEC-065 | Seuil de surtension LM74502 | R | |
| DEC-066 | USB-C + CH340C + boutons BOOT / RESET | **S** | Fournis par la carte de dev |
| DEC-067 | Diodes idéales LM66100 | A | U15 (entrée 5 V) à garder ou non selon PO-07. U16 (USB) supprimée |
| DEC-068, DEC-072 | Carte 120 × 80 mm, 4 trous M3, connecteurs sur bords opposés | ? | Dimensions à revoir avec la carte de dev (PO-10) |
| DEC-069 | Isolement ≥ 2 mm entre domaines | R | |
| DEC-070 | 4 couches S / GND / Alim / S | R | Confirmé par DEC-009 |
| DEC-071, DEC-073 | Borniers à vis enfichables, armement J7 | R | |
| DEC-074 | TVS sur `NO_FEED`, LED « 3,3 V présent », points de test | R | |
| DEC-075 à DEC-078 | Sérigraphie, règles de routage, échange de broches STM32 | A | Règles de routage reprises. DEC-078 propre au STM32 |

## 2. Blocs du schéma STM32 (BOM v1.3)

| Bloc | Repères STM32 | Statut | Remarque |
|---|---|---|---|
| Microcontrôleur, découplage | U1 | **S** | Carte de dev ESP32 sur barrettes femelles |
| Régulateur 3,3 V | U2 | ? | Régulateur de la carte de dev ou régulateur propre (PO-07) |
| Quartz | Y1 | **S** | |
| USB-C, CH340C, ESD USB, boutons | J12, U13, U14, U16 | **S** | |
| SWD | J9 | **S** | |
| Liaison série J10 (USART2) | J10 | **S** | DEC-017 |
| Entrée PWM : ESD, buffer de Schmitt | D9, U3 | R | Compatible 3,3 / 5 V : le niveau du PWM reste non bloquant |
| Fins de course : rappel, filtre RC, ESD | U11, U12, R, C | R | |
| Entrées d'extension | D11 à D14, R, C | A | 2 entrées au lieu de 4 (DEC-021) : D13, D14 et leurs passifs supprimés |
| Pompe de charge (chaîne K1) | D7, Q3, Q4 | R | Signal carré généré par l'ESP32 (LEDC) |
| Watchdog externe (chaîne K2) | U4, Q5, Q6 | R | |
| Relais DPDT et relecture | K1, K2, D4, D5 | R | |
| Cavalier d'armement | J7 | R | |
| Protection 12 V | U5, Q8, Q9, D2, D3, D8, F2 | R | |
| Opto présence 12 V, arrêt matériel | U6, U7 | R | |
| Mesure isolée du courant (I_MON) | U8, U9, U10 | **S** | DEC-016. Retirer aussi le shunt et les composants passifs associés |
| LED, buzzer, sortie STATUS | LED1 à LED5, LS1, Q2, Q7 | A | LED1 à LED4 remplacées par 1 WS2812B + adaptateur 74AHCT1G125 (DEC-019, DEC-024). LED5, buzzer, STATUS repris |
| Alimentation 5 V : TVS, fusible | D1, F1 | R | |
| Connecteurs | J1 à J8, J13 à J15 | R | Brochage des borniers inchangé : câblage externe identique pour les deux cartes |
| Points de test | TP1 à TP7 | R | |
