# Installation — pilotage d'EasyEDA Pro par Claude Code (Windows)

**Statut :** reprise du dépôt STM32 (v1.1), adaptée à la carte ESP32. Seuls changent le projet EasyEDA et le dossier du dépôt.
**Objet :** procédure unique d'installation, puis routine de début et de fin de session.
**Poste :** PC de Julien, Windows, EasyEDA Pro V3.2.209, Claude Code installé.

---

## 1. Précaution préalable

EasyEDA signale qu'un projet modifié en V3 ne s'ouvre plus dans son dernier état en V2.2. **Ne pas ouvrir les projets clients en V3** (Thales, Clean & Protech, intercepteur, Dustbuster) tant que la migration n'est pas décidée. Les exporter en `.epro` au préalable si une ouverture est nécessaire.

## 2. Installation (une seule fois)

| Étape | Action | Contrôle |
|---|---|---|
| 1 | Installer Node.js LTS **22 ou plus récent** (nodejs.org). L'extension recommande la 22 LTS ; le skill exige 18 au minimum. | `node -v` dans PowerShell |
| 2 | Vérifier Git for Windows (déjà requis par Claude Code). | `git --version` |
| 3 | Installer le skill (PowerShell) :<br>`git clone https://github.com/easyeda/easyeda-api-skill "$env:USERPROFILE\.claude\skills\easyeda-api"`<br>`cd "$env:USERPROFILE\.claude\skills\easyeda-api"`<br>`npm install` | `SKILL.md` présent à la racine du dossier `easyeda-api` (pas de sous-dossier en double) |
| 4 | Télécharger l'extension Run API Gateway (`.eext`) : https://jlc-ext.com/item/oshwhub/run-api-gateway | Version 1.0.6 ou plus |
| 5 | Dans EasyEDA Pro : ouvrir le gestionnaire d'extensions, importer le fichier `.eext`. | Extension listée |
| 6 | Dans le gestionnaire d'extensions, pour Run API Gateway, cocher **Allow interactive with external** et **Show at header menu** (libellés V3.2.209 ; la doc de l'extension cite « Allow External Interaction » et « Show in Top Menu »). | Menu **API Gateway** visible dans la barre du haut |
| 7 | Dans EasyEDA Pro, espace de travail Personal : **ne pas créer de projet vide** : dupliquer le projet `PCB_PWM_SECURISE_V3` sous le nom `PCB_PWM_ESP32_V3` (voir `CONSIGNES_EASYEDA_CLAUDE_CODE.md` §3, étape E0). Laisser le nouveau projet ouvert. | Projet ouvert dans l'éditeur |

Sans l'option « Allow interactive with external », la connexion échoue **sans message d'erreur**.

**Mode Full Online obligatoire.** En Full Offline, l'extension n'est pas chargée : le menu API Gateway et l'espace Personal disparaissent. Un projet créé dans ce mode est local et peut porter le même nom que le projet en ligne : Claude Code identifie le projet par son uuid (consignes, règle S1).

## 3. Début de session

| Étape | Action | Contrôle |
|---|---|---|
| 1 | Ouvrir EasyEDA Pro et le projet `PCB_PWM_ESP32_V3`. **Fermer le projet STM32.** | — |
| 2 | Ouvrir une fenêtre PowerShell **dédiée** au pont et la laisser ouverte :<br>`node "$env:USERPROFILE\.claude\skills\easyeda-api\scripts\bridge-server.mjs"` | Le port choisi (49620 à 49629) s'affiche |
| 3 | Dans EasyEDA : **API Gateway > Reconnect**. | — |
| 4 | Dans une autre fenêtre PowerShell :<br>`curl.exe http://localhost:49620/health` (remplacer 49620 par le port affiché) | `"service":"easyeda-bridge"` et `"edaConnected":true` |
| 5 | Ouvrir le clone du dépôt et lancer Claude Code :<br>`cd "<dossier du clone de PCB-PWM-ESP32-SECURIS2-V3>"`<br>`git pull`<br>`claude` | Claude Code démarre dans le dépôt |
| 6 | Coller le prompt de la phase en cours. | — |

Le pont est lancé dans une fenêtre séparée, pas par Claude Code en tâche de fond : dans le retour d'expérience JLCPCB, le pont lancé en tâche de fond s'est arrêté deux fois par manque de mémoire.

Claude Code demandera l'autorisation d'exécuter `curl` vers `localhost`. L'accepter pour la session.

**Appel manuel de `POST /execute` depuis PowerShell.** Écrire le corps JSON dans un fichier, puis l'envoyer avec `--data-binary`. Cela évite les problèmes d'échappement des guillemets :

```powershell
$f = "$env:TEMPeda_call.json"
[IO.File]::WriteAllText($f, '{"code":"const p = await eda.dmt_Project.getCurrentProjectInfo(); return JSON.stringify(p);"}')
curl.exe -s -X POST http://localhost:49620/execute -H "Content-Type: application/json" --data-binary "@$f"
```

Le fichier doit être en UTF-8 sans BOM (`WriteAllText` le fait). `Set-Content -Encoding ascii` corrompt les caractères non ASCII, présents par exemple dans le fabricant `ST(意法半导体)`. Claude Code envoie ses appels par un petit script Node.js placé hors du dépôt.

## 4. Fin de session

1. Vérifier que Claude Code a enregistré le projet, commité et poussé.
2. Arrêter le pont : `Ctrl+C` dans sa fenêtre PowerShell.
3. Dans EasyEDA : **API Gateway > Stop Connection**.

Le pont n'a aucune authentification : tout programme local peut exécuter du code dans EasyEDA tant qu'il tourne.

## 5. Dépannage

| Symptôme | Cause probable | Action |
|---|---|---|
| `edaConnected: false` | Option « Allow interactive with external » décochée, ou 5 tentatives échouées | Cocher l'option, puis API Gateway > Reconnect |
| Menu API Gateway et espace Personal absents | EasyEDA en Full Offline | Repasser en Full Online. Ne pas utiliser un projet créé en Full Offline |
| `curl` ne répond sur aucun port | Pont arrêté | Relancer le pont, puis Reconnect |
| « Handshake failed: unexpected service » | Autre programme sur le port | `netstat -ano \| findstr 4962` ; fermer le programme, relancer |
| Échec derrière un proxy | Proxy système ou logiciel d'entreprise interceptant `ws://127.0.0.1` | Vérifier Paramètres Windows > Réseau > Proxy |
| Plusieurs fenêtres EDA détectées | Plusieurs éditeurs ouverts | Fermer les autres, ou indiquer la bonne fenêtre à Claude Code |
| Blocage complet | — | Quitter Claude Code et EasyEDA, rouvrir EasyEDA, relancer le pont, Reconnect, relancer Claude Code |

## 6. Routage (phase 5, plus tard)

À installer seulement si le routage automatique hors zones critiques est retenu :

- Java 21 ;
- Freerouting v2.1.0 (les versions 2.2 et suivantes exigent Java 25).

## Sources

- https://github.com/easyeda/easyeda-api-skill
- https://jlc-ext.com/item/oshwhub/run-api-gateway
- https://github.com/easyeda/eext-run-api-gateway/blob/main/FAQ.en.md
- https://jlcpcb.com/blog/claude-code-pcb-design
