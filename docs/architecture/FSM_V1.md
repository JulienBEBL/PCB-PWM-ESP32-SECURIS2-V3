# Machine d'états v1 (proposition)

**Statut :** proposition de Claude, **à valider par Julien** (PO-24). Répond à la demande du 2026-11-06 : 4 états, parcourus toujours dans le même sens.
**Code :** `firmware/carte_pwm_esp32_v1/`.

## 1. Les 4 états du cycle

```
        ┌──────────┐  PWM "prêt"     ┌──────────┐  PWM "action"   ┌──────────┐
   ┌──▶ │ 1 REPOS  │ ──────────────▶ │ 2 PRÊT   │ ──────────────▶ │ 3 ACTION │
   │    └──────────┘  + conditions   └──────────┘  confirmé       └──────────┘
   │          ▲                            │ abandon                    │
   │          │ rentré + PWM "repos"       ▼                            │ PWM quitte "action"
   │    ┌──────────┐ ◀─────────────────────┘                            │ ou défaut léger
   └─── │ 4 APRÈS  │ ◀──────────────────────────────────────────────────┘
        └──────────┘
```

| État | Vérin | K1 | K2 | Pompe | LED | Rôle |
|---|---|---|---|---|---|---|
| **1 REPOS** | Rentré, au repos | 0 | 0 | arrêt | Vert | État normal |
| **2 PRÊT** | Rentré, au repos | 0 | 0 | arrêt | Orange | Prêt à actionner. Aucun mouvement |
| **3 ACTION** | Sortie jusqu'aux fins de course « sorti », puis maintien | 1 puis 0 | 1 | marche puis arrêt | Bleu clignotant | Action en cours |
| **4 APRÈS** | Rentrée jusqu'aux fins de course « rentré » | 0 | 0 | arrêt | Cyan | Retour en position sûre |

Le sens est toujours 1 → 2 → 3 → 4 → 1. Un abandon depuis PRÊT passe par APRÈS, jamais par un retour en arrière.

**Relais au repos = rentrée de tige** (architecture STM32). REPOS, PRÊT et APRÈS ont donc les mêmes sorties : seul le firmware les distingue. Aucun de ces trois états ne peut faire sortir la tige.

## 2. Deux états techniques, hors cycle

Ils sont nécessaires à la sécurité. Je les ajoute à ta demande de 4 états.

| État | Rôle | Sorties |
|---|---|---|
| DÉMARRAGE | Autocontrôle pendant 0,5 s après la mise sous tension | Relais au repos |
| DÉFAUT | Anomalie grave. **Verrouillé jusqu'au prochain reset** | Relais au repos → rentrée de tige |

## 3. Lecture du PWM : 3 bandes

| Bande | Largeur d'impulsion | Sens |
|---|---|---|
| REPOS | 1000 à 1300 µs | Demande de repos |
| PRÊT | 1400 à 1600 µs | Demande d'armement |
| ACTION | 1700 à 2000 µs | Demande d'action |
| Zones mortes | 1300–1400 µs, 1600–1700 µs | Aucune transition, l'état est conservé |
| Invalide | < 900 µs, > 2100 µs, période hors 10–40 ms, plus d'impulsion depuis 100 ms | PWM perdu |

Les zones mortes évitent qu'un PWM proche d'une limite fasse osciller la carte.

## 4. Transitions

| De → vers | Condition |
|---|---|
| DÉMARRAGE → REPOS | Autocontrôle OK |
| DÉMARRAGE → DÉFAUT | Autocontrôle en échec |
| 1 REPOS → 2 PRÊT | PWM en bande PRÊT pendant **500 ms** sans interruption, **et** 12 V présent, cavalier en place, tige rentrée (R1 **et** R2 actionnées), aucune fin de course « sorti » actionnée |
| 2 PRÊT → 3 ACTION | PWM en bande ACTION pendant **300 ms** sans interruption, conditions de PRÊT toujours vraies |
| 2 PRÊT → 4 APRÈS | PWM en bande REPOS ou invalide, ou une condition de PRÊT perdue (abandon) |
| 3 ACTION → 4 APRÈS | PWM quitte la bande ACTION (repos, prêt, invalide), 12 V perdu ou cavalier retiré |
| 4 APRÈS → 1 REPOS | Tige rentrée (R1 et R2 actionnées) **et** PWM en bande REPOS |

**Pour actionner, il faut donc deux demandes successives et stables** : d'abord PRÊT, puis ACTION. Une seule valeur de PWM fausse ou un saut direct REPOS → ACTION ne suffit pas.

**Pour réarmer, il faut repasser par REPOS.** Un PWM resté en bande ACTION après un cycle ne relance pas d'action.

## 5. Déroulement de l'action

1. Entrée en ACTION : K2 puis K1 excités, pompe de charge en marche. La tige sort.
2. Dès qu'**une** fin de course « sorti » (S1 **ou** S2) est actionnée : K1 retombe, pompe arrêtée. Bornes du vérin au même potentiel : arrêt et freinage. K2 reste excité.
3. La tige reste sortie tant que le PWM reste en bande ACTION.
4. Le PWM quitte la bande ACTION → APRÈS : K2 retombe, la tige rentre.

**Hypothèse [à confirmer]** : la position « sorti » doit être maintenue tant que l'action est demandée. Si une action brève suffit (sortie puis retour automatique), il suffit de passer en APRÈS dès l'arrivée en butée.

## 6. Défauts → DÉFAUT (verrouillé)

| Défaut | Détection |
|---|---|
| Tension +5V hors 4,5–5,5 V | `V5_MON` |
| Fin de course incohérente | NO = NC sur un contact |
| « Rentré » et « sorti » en même temps | Fins de course |
| Relais vu excité alors qu'il n'est pas commandé | `K1_FB`, `K2_FB` |
| Relais commandé mais non vu excité après 50 ms | `K1_FB`, `K2_FB` |
| Sortie trop longue : pas de fin de course « sorti » après **3 s** | Temporisation (DEC-005 STM32 : course ≤ 2–3 s) |
| Rentrée trop longue : pas de « rentré » après **3 s** en APRÈS, 12 V présent | Temporisation |
| Arrêt matériel atteint pendant l'action (5e contact ouvert) | `STOP_HW_FB` |
| Désaccord durable R1 ≠ R2 en position rentrée (plus de 1 s) | Fins de course |
| Blocage du programme | Watchdog interne (reset) et watchdog externe (K2 retombe) |

Tout défaut coupe les deux relais : la tige rentre si le 12 V est présent.

Les défauts « légers » (PWM perdu, 12 V perdu, cavalier retiré) ne verrouillent pas : ils font passer en APRÈS.

## 7. Valeurs à confirmer

| Valeur | Proposition | Raison |
|---|---|---|
| Bandes PWM | 1000–1300 / 1400–1600 / 1700–2000 µs | Partage de la plage 1000–2000 µs en trois, avec marges |
| Confirmation PRÊT | 500 ms | Assez long pour ignorer un parasite, assez court pour l'opérateur |
| Confirmation ACTION | 300 ms | Idem |
| Temps max de course | 3 s | DEC-005 STM32 |
| Perte du PWM | 100 ms sans impulsion valide (5 périodes à 50 Hz) | — |
| Maintien en position sortie | Tant que la bande ACTION est demandée | Hypothèse §5 |

Ces valeurs dépendent de ce que le contrôleur externe sait envoyer. Elles sont regroupées dans `config.h` pour être changées facilement.
