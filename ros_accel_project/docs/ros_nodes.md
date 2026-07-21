# Description des nœuds ROS2

## 1. Introduction

Le système ROS2 est organisé autour de plusieurs nœuds indépendants ayant chacun une fonction précise.

Chaque nœud communique avec les autres grâce au système de publication et d'abonnement basé sur les topics ROS2.

L'architecture utilisée dans ce projet comprend quatre nœuds principaux :

| Nœud | Fonction principale |
|---|---|
| `esp_receiver_node` | Réception des données envoyées par l'ESP8266 |
| `turtle_control` | Conversion des données d'accélération en commandes de mouvement |
| `turtlesim_node` | Simulation du robot |
| `turtle_pose_serial_node` | Transmission de la position vers l'Arduino |

---

# 2. Vue globale de la communication

Le fonctionnement général des nœuds est le suivant :

```text
ESP8266
   |
   | HTTP POST
   ↓
esp_receiver_node
   |
   | /accel
   ↓
turtle_control
   |
   | /turtle1/cmd_vel
   ↓
turtlesim_node
   |
   | /turtle1/pose
   ↓
turtle_pose_serial_node
   |
   | USB Serial
   ↓
Arduino Nano
```

---

# 3. esp_receiver_node

## Rôle

Le nœud `esp_receiver_node` assure l'interface entre l'ESP8266 et ROS2.

Il reçoit les mesures d'accélération envoyées par l'ESP8266 via une requête HTTP et les publie dans l'environnement ROS2.

## Entrée

Données JSON envoyées par l'ESP8266 :

```text
{
"x": valeur,
"y": valeur,
"z": valeur
}
```

## Sortie

Topic publié :

| Topic | Type | Description |
|---|---|---|
| `/accel` | `Float32MultiArray` | Valeurs d'accélération X, Y, Z |

---

# 4. turtle_control

## Rôle

Le nœud `turtle_control` transforme les données provenant de l'accéléromètre en commandes de déplacement pour TurtleSim.

Il analyse les valeurs reçues sur `/accel` puis calcule les vitesses linéaires et angulaires à appliquer.

## Entrée

| Topic | Type |
|---|---|
| `/accel` | `Float32MultiArray` |

## Sortie

| Topic | Type |
|---|---|
| `/turtle1/cmd_vel` | `geometry_msgs/Twist` |

---

# 5. turtlesim_node

## Rôle

Le nœud `turtlesim_node` fournit un environnement de simulation permettant de tester la commande du robot avant une utilisation sur un système réel.

Il reçoit les commandes de vitesse et met à jour la position de la tortue.

## Entrée

| Topic | Type |
|---|---|
| `/turtle1/cmd_vel` | `geometry_msgs/Twist` |

## Sortie

| Topic | Type |
|---|---|
| `/turtle1/pose` | `turtlesim/Pose` |

---

# 6. turtle_pose_serial_node

## Rôle

Le nœud `turtle_pose_serial_node` récupère la position de la tortue simulée et transmet les informations vers l'Arduino Nano.

Il permet d'assurer le lien entre ROS2 et la partie électronique d'affichage.

## Entrée

| Topic | Type |
|---|---|
| `/turtle1/pose` | `turtlesim/Pose` |

## Sortie

Communication série USB vers Arduino Nano.

---

# 7. Résumé

L'organisation en plusieurs nœuds permet de séparer les différentes fonctions du système :

- acquisition des données avec l'ESP8266 ;
- traitement et contrôle avec ROS2 ;
- simulation avec TurtleSim ;
- affichage avec Arduino.

Cette architecture facilite la maintenance du projet et permet une évolution future vers un robot réel.