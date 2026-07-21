# Matériel utilisé

## 1. Introduction

Cette partie présente les différents composants matériels utilisés pour réaliser le système de contrôle robotique.

Le système est composé de deux parties matérielles principales :

- **un module de commande**, chargé de mesurer les mouvements de l'utilisateur et de transmettre les données vers ROS2 ;
- **un module d'affichage**, chargé de restituer les informations reçues depuis ROS2.

La séparation de ces deux modules permet de conserver une architecture flexible et évolutive.

---

# 2. Présentation du matériel

| Composant | Fonction |
|---|---|
| ESP8266 | Acquisition des données et communication Wi-Fi |
| ADXL345 | Mesure des accélérations selon trois axes |
| Ordinateur | Exécution de ROS2 et traitement des données |
| Arduino Nano | Gestion de l'affichage |
| Écran LCD I²C | Interface utilisateur |
| Câble USB | Communication série entre ROS2 et Arduino |

---

# 3. Module de commande

Le module de commande constitue la partie embarquée permettant à l'utilisateur d'interagir avec le système.

Il est composé de :

- un ESP8266 ;
- un accéléromètre ADXL345.

Son rôle est de mesurer les mouvements de l'utilisateur puis de transmettre les informations vers l'environnement ROS2.

![Module de commande ESP8266 + ADXL345](images/module_commande.jpeg)

---

## 3.1 Accéléromètre ADXL345

L'ADXL345 est un accéléromètre numérique trois axes permettant de mesurer les accélérations selon les axes :

- **X** : mouvement latéral ;
- **Y** : mouvement avant/arrière ;
- **Z** : accélération verticale.

La communication entre l'ADXL345 et l'ESP8266 est réalisée grâce au protocole **I²C**.

Les connexions principales sont :

| ADXL345 | ESP8266 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | SDA |
| SCL | SCL |

Les mesures récupérées sont ensuite envoyées vers ROS2 afin d'être utilisées pour commander la simulation robotique.

---

## 3.2 ESP8266

L'ESP8266 est utilisé comme unité d'acquisition et de communication sans fil.

Ses principales fonctions sont :

- initialiser et lire le capteur ADXL345 ;
- récupérer les valeurs d'accélération X, Y et Z ;
- se connecter au réseau Wi-Fi ;
- transmettre les données vers ROS2 grâce à une requête HTTP.

Le choix de l'ESP8266 est lié à :

- son module Wi-Fi intégré ;
- son faible coût ;
- sa simplicité d'utilisation ;
- sa compatibilité avec de nombreux capteurs.

---

# 4. Module d'affichage

Le module d'affichage constitue l'interface de restitution des informations pour l'utilisateur.

Il est composé d'un Arduino Nano associé à un écran LCD utilisant une interface I²C.

![Module affichage Arduino Nano + LCD](images/module_affichage.jpeg)

---

## 4.1 Arduino Nano

L'Arduino Nano est utilisé pour gérer l'affichage des informations provenant du système ROS2.

Il reçoit les données transmises par le nœud `turtle_pose_serial_node` via une communication série USB.

Ses fonctions principales sont :

- réception des données série ;
- interprétation des informations reçues ;
- gestion de l'écran LCD.

L'Arduino n'effectue pas le traitement principal des données. Il joue uniquement le rôle d'interface utilisateur.

---

## 4.2 Écran LCD I²C

L'écran LCD permet d'afficher les informations retournées par le système.

L'utilisation d'une interface I²C permet de réduire le nombre de connexions nécessaires entre l'Arduino et l'écran.

Les informations affichées peuvent inclure :

- la position de la tortue ;
- sa direction ;
- l'état du système.

---

# 5. Organisation matérielle globale

L'organisation matérielle peut être résumée par le schéma suivant :

```text
              Module de commande

        ADXL345
            |
            | I²C
            ↓
        ESP8266
            |
            | Wi-Fi HTTP
            ↓

          PC ROS2

            |
            | USB Série
            ↓

             Arduino Nano
                  |
                  ↓
               LCD
```

---

# 6. Résumé

Le système repose sur une séparation claire entre les différentes fonctions matérielles :

- l'**ADXL345** réalise l'acquisition des mouvements ;
- l'**ESP8266** assure la communication sans fil ;
- **ROS2** réalise le traitement et le contrôle ;
- l'**Arduino Nano** assure uniquement l'affichage.

Cette organisation facilite la maintenance du système et permet une évolution future vers une application robotique réelle.