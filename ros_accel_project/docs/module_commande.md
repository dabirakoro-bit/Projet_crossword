# Communication du système

## 1. Introduction

Le système utilise plusieurs moyens de communication afin d'assurer l'échange des données entre les différents composants.

Chaque liaison est adaptée au rôle du composant concerné :

- I²C pour la communication entre le capteur et l'ESP8266 ;
- Wi-Fi HTTP pour la transmission des mesures vers ROS2 ;
- Topics ROS2 pour la communication entre les nœuds logiciels ;
- Liaison série USB pour l'affichage sur Arduino.

---

## 2. Communication ADXL345 - ESP8266

L'accéléromètre ADXL345 communique avec l'ESP8266 grâce au protocole **I²C**.

Cette liaison permet à l'ESP8266 de récupérer les valeurs d'accélération selon les trois axes :

- X : mouvement latéral ;
- Y : mouvement avant/arrière ;
- Z : accélération verticale.

L'utilisation du protocole I²C permet de limiter le nombre de connexions électriques tout en conservant une communication fiable avec le capteur.

---

## 3. Communication ESP8266 - ROS2

La transmission entre l'ESP8266 et le système ROS2 est réalisée par une communication réseau Wi-Fi.

L'ESP8266 envoie régulièrement les données de l'accéléromètre sous la forme d'une requête HTTP POST contenant un message JSON.

Exemple de données envoyées :

```json
{
  "x": 0.25,
  "y": -0.63,
  "z": 9.81
}
```

Le nœud `esp_receiver_node` reçoit ces informations et les transforme en message ROS2 publié sur le topic `/accel`.

---

## 4. Communication entre les nœuds ROS2

ROS2 utilise un système de communication basé sur les topics.

Chaque nœud peut publier ou recevoir des informations sans connaître directement les autres composants du système.

Les principaux échanges sont :

| Émetteur | Topic | Récepteur |
|---|---|---|
| `esp_receiver_node` | `/accel` | `turtle_control` |
| `turtle_control` | `/turtle1/cmd_vel` | `turtlesim_node` |
| `turtlesim_node` | `/turtle1/pose` | `turtle_pose_serial_node` |

Cette architecture permet de rendre le système modulaire et facilement extensible.

---

## 5. Communication ROS2 - Arduino

La communication entre ROS2 et l'Arduino Nano est réalisée grâce à une liaison série USB.

Le nœud `turtle_pose_serial_node` récupère la position de la tortue puis transmet les informations sous forme de données série.

L'Arduino reçoit ces informations et assure uniquement la gestion de l'affichage sur l'écran LCD.

Cette séparation permet de conserver le traitement des données dans ROS2 et de réserver l'Arduino à la partie interface utilisateur.

---

## 6. Résumé

Les différentes communications utilisées permettent de créer une chaîne complète allant de l'acquisition des mouvements jusqu'à l'affichage des informations.

| Liaison | Protocole | Fonction |
|---|---|---|
| ADXL345 → ESP8266 | I²C | Acquisition des mesures |
| ESP8266 → ROS2 | HTTP / Wi-Fi | Transmission sans fil |
| ROS2 → ROS2 | Topics | Communication entre nœuds |
| ROS2 → Arduino | UART USB | Affichage des informations |
