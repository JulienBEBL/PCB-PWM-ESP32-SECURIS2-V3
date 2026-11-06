# BOM cible — carte ESP32

**Phase :** 1. **Jalon :** G1 **validé** (DEC-027).
**Version :** v1, 2026-11-06. Dérivée de la BOM STM32 v1.3 (`references/stm32_v3/hardware/BOM_CIBLE.md`), dont les codes LCSC ont été vérifiés le 2026-11-03.
**Fichier machine :** `hardware/bom/BOM_CIBLE.csv` (une ligne par référence, repères groupés).
**Connexions :** `hardware/NETLIST_CIBLE.md`.

Statut : **V** = vérifié ; **AV** = à vérifier (code relevé par recherche, à confirmer au devis JLCPCB).

## 1. Composants retirés de la BOM STM32

| Repères | Raison |
|---|---|
| U1, Y1, C201 à C207, SW1, SW2, R201 à R204, J9, J10 | MCU et accessoires : fournis par la carte de dev (DEC-006) ; J10 supprimé (DEC-017) |
| U2, C103, C104 | +3V3 pris sur la carte de dev (DEC-026) |
| J12, U13, U14, R601, R602, C601 | USB : fourni par la carte de dev |
| U8, U9, U10, R504, R509 à R515, C508 à C516 | Mesure de courant supprimée (DEC-016) |
| LED1 à LED4, R420 à R423 | Remplacées par la LED RGB (DEC-019) |
| J11, R351 à R354, C351 | Maintenance et révision supprimées (DEC-017, DEC-022) |
| D13, D14, R334, R335, R338, R339, R342, R343, C333, C334 | Extensions 3 et 4 supprimées (DEC-021) |

## 2. Composants ajoutés ou modifiés

| Repère | Référence | LCSC | Statut | Remarque |
|---|---|---|---|---|
| A1 | ESP32-S3-DevKitC-1-**N8** | — | Achat séparé | Non assemblé par JLCPCB. Variante N8 obligatoire (`BROCHAGE_ESP32.md` A2) |
| J20, J21 | Barrette femelle 1×22, 2,54 mm, ZX-PM2.54-1-22PY (Megastar) | C7499337 | AV | Montage traversant par JLCPCB. Compatibilité avec les broches carrées de la carte de dev à vérifier (PO-02) |
| U17 | SN74AHCT1G125DBVR (TI) | C7484 | AV | Datasheet vérifiée (SCLS378P) |
| LED1 | WS2812B-B/W (Worldsemi) | C114586 | AV | Datasheet vérifiée |
| R421 | 330 Ω 0603 (0603WAF3300T5E) | C23138 | AV | Valeur [Reco] |
| R413 | 15 kΩ 0603 (0603WAF1502T5E) | C22809 | AV | Remplace 20 kΩ : `ARM_FB` ≤ 3,3 V |
| R420 | 100 kΩ 0603 | C25803 | V | Rappel de `RGB_DATA` |
| C108, C420, C421 | 1 µF, 100 nF, 100 nF 0603 | C15849, C14663 | V | Sortie U16, découplage U17 et LED1 |
| U16 | LM66100DCKR | C2869734 | V | Réaffectée vers la carte de dev |
| J8 | PZ254V-11-04P (1×4) | C492403 | V | 2 voies d'extension |

Tous les autres composants sont repris de la BOM STM32 sans changement.

Sources des codes relevés : [C7499337](https://www.lcsc.com/product-detail/C7499337.html), [C7484](https://www.lcsc.com/product-detail/C7484.html), [C114586](https://www.lcsc.com/product-detail/C114586.html), [C23138](https://jlcpcb.com/partdetail/C23138), [C22809](https://jlcpcb.com/partdetail/0603WAF1502T5E/C22809).
