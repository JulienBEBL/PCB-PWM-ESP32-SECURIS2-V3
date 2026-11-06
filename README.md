# PCB PWM sécurisé V3 — version ESP32

Carte de repli de la carte STM32 V3 (`JulienBEBL/PCB_PWM_SECURISE_V3`).

Même fonction et même cahier des charges : lecture d'une consigne PWM, machine d'états, commande fail-safe d'un vérin 12 V par 2 relais DPDT, retour en position sûre sur tout défaut.

Différence principale : le microcontrôleur est une **carte de développement ESP32 enfichée**, amovible. Toutes les communications radio (Wi-Fi, Bluetooth) sont désactivées.

- Bureau d'études : BEBL
- Conception : Julien, avec l'appui de Claude
- Point d'entrée de la documentation : [`CLAUDE.md`](CLAUDE.md), puis [`docs/memoire/ETAT_PROJET.md`](docs/memoire/ETAT_PROJET.md)
- Licence : Apache 2.0
