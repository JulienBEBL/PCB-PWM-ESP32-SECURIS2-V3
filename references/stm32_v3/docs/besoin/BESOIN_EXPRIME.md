# Besoin exprimé

Reformulation fidèle des informations données par Julien. Aucune valeur n'est ajoutée ici : les propositions de Claude sont dans `docs/architecture/`, les décisions dans `docs/memoire/DECISIONS.md`.

| Thème | Besoin exprimé | Date | Décision |
|---|---|---|---|
| Fonction principale | Transformer un signal PWM lu en machine d'états et piloter l'actionneur. Lire en continu le PWM de manière fiable et sécurisée, et piloter selon des règles définies. | 2026-11-02 | DEC-001 |
| Machine d'états | Plusieurs états : inconnu, ON, safe, ready, action, etc. Séquences avec temporisations. | 2026-11-02 | PO-05 |
| Actionneur | Vérin électrique (moteur CC et vis), 2 fils, commande par inversion de polarité. Référence initiale : Actuonix L12-30-50-12-S. | 2026-11-02 | DEC-010 (remplacée par DEC-018) |
| Tension actionneur | Initialement « 12 ou 24 V DC, environ 1 A max ». Puis : « il faut piloter du 12 V pour ce vérin ». Source 12 V fournie directement. | 2026-11-02 | DEC-011, DEC-019, PO-03 |
| Charge | Environ 30 N. Puis : 30 N vers la sortie, valeur estimée. | 2026-11-02 | DEC-017, PO-20 |
| Course utile | Course physique recherchée : environ 3 mm (± 1 mm). | 2026-11-02 | DEC-016, PO-21 |
| Version du vérin | Le vérin peut être modifié par une autre version si besoin. | 2026-11-02 | DEC-018, PO-18 |
| Danger | Un déclenchement sans autorisation ou suite à un bug crée un très gros danger. Il faut absolument être en état SAFE en cas de problème. | 2026-11-02 | DEC-020 |
| Position sûre | Position rentrée au maximum. Pas de position intermédiaire. | 2026-11-02 | DEC-012, DEC-013 |
| Mise sous tension | Mouvement automatique vers la position sûre à l'apparition du 12 V : acceptable. | 2026-11-02 | DEC-027 |
| Isolation | Les contacts et l'alimentation du système raccordé à l'actionneur doivent être isolés absolument. Masse non commune, « surtout pas ». | 2026-11-02 | DEC-021 |
| Alimentation carte | 5 V DC externe ; GND issu du contrôleur qui génère le PWM. Courant inconnu, hypothèse 500 mA. | 2026-11-02 | DEC-031, DEC-032 |
| Entrée PWM | 1000 à 2000 µs, environ 50 Hz (non figé). Contrôleur externe. Niveau 3,3 V ou 5 V à vérifier. Câble 20 cm max. | 2026-11-02 | DEC-030, PO-04 |
| Fins de course | 4 entrées minimum, contacts mécaniques NO ou NC classiques, 2 par extrémité (redondance). Câbles 15–20 cm visés, 30 cm max. | 2026-11-02 | DEC-028 |
| Autres sorties | 3 ou 4 LED d'état, buzzer probable. | 2026-11-02 | DEC-033 |
| Capteurs | Pouvoir brancher des capteurs, à définir plus tard, secondaire. | 2026-11-02 | PO-07 |
| Armement | Cavalier physique pour qu'aucune action électrique ne soit possible côté vérin, plus un blocage mécanique. Trois niveaux : logiciel, électrique, mécanique. | 2026-11-02 | DEC-025, DEC-026 |
| Environnement | Extérieur, fixe, −10 à +50 °C environ. Vérin dans un boîtier mécanique protégé. Boîtier carte étanche possible, blindage possible. | 2026-11-02 | DEC-004, DEC-014 |
| CEM / IEM | Résister à la CEM classique et à l'impulsion électromagnétique au niveau le plus élevé possible pour cette phase. Pas de laboratoire : essais par le client. | 2026-11-02 | DEC-040 à DEC-043 |
| Endurance | Environ 200 cycles complets. Course de 2 à 3 s maximum. | 2026-11-02 | DEC-005 |
| Vitesse | S'adapte à la charge, peut varier. | 2026-11-02 | DEC-015 |
| Volumes | 1 carte (ou 2 si prototype). Max 10 cartes chez JLCPCB. Pas d'industrialisation. Secteur privé, pas grand public. | 2026-11-02 | DEC-002 |
| Outils | EasyEDA Pro. | 2026-11-02 | DEC-003 |
| Firmware | Langage connu, robuste, facile à programmer. Programmation gratuite depuis un PC. Julien connaît bien Python et Arduino, moins C/C++. Claude écrit le code, Julien teste. | 2026-11-02 | DEC-050, DEC-051 |
