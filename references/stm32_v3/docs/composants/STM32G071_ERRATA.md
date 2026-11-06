# STM32G071 — errata applicables au projet

**Source :** ES0418 « STM32G071x8/xB device errata », Rev 6 (29 septembre 2026), consultée en ligne :
https://www.st.com/resource/en/errata_sheet/es0418-stm32g071x8xb-device-errata-stmicroelectronics.pdf
Révisions de silicium couvertes : B (0x2000), Y/1/2 (0x2001), X/3 (0x2003). La révision des pièces livrées par LCSC est inconnue : le firmware la lit au démarrage (DBG_IDCODE).

Statut ST : A = contournement, P = contournement partiel, N = pas de contournement, « - » = non concerné.

## Errata qui touchent la conception

| Réf. | Sujet | B | Y/1/2 | X/3 | Impact projet | Traitement |
|---|---|---|---|---|---|---|
| 2.3.1 | GPIO affectée au DAC inutilisable en sortie | N | N | N | PA4, PA5 (`EXT_IN1`, `EXT_IN2`) | Entrées seulement |
| 2.8.6 | TIM16 / TIM17 cadencés à tort par SYSCLK | N | - | - | `BUZZER` (TIM16), `CP_DRV` (TIM17) | Lecture de la révision ; vérification des fréquences à la mise en service |
| 2.15.3 | Mauvais ajustement de la résistance « dead battery » UCPD | N | N | N | LED sur broches CC | Fonction désactivée au démarrage ; sans impact |
| 2.2.8 | Transitions sur PC13 perturbent le LSE | N | N | N | LSE non utilisé | Aucun |
| 2.2.3 | Démarrage depuis la flash par PA14-BOOT0 impossible en RDP niveau 1 | N | - | - | Si la protection en lecture est activée sur silicium B | À considérer avant d'activer RDP |
| 2.2.9 | Blocage sur incohérence des octets d'option | P | - | - | Programmation des octets d'option | Procédure de programmation documentée |
| 2.6.x | ADC (drapeau de débordement, registres, temps d'échantillonnage) | P / A / N | idem | idem | X01, X04, X05, entrées d'extension | Contournements à appliquer dans le firmware |
| 2.12.x | USART (bruit sur la réception, ABREN, ONEBIT) | A / N | idem | idem | UART de diagnostic | Pas d'autobaud ni de ONEBIT ; filtrage de la ligne RX |

## Fonctions sans errata

IWDG, WWDG, capture d'entrée et mode « PWM input » des timers : aucun erratum listé dans ES0418 Rev 6.
