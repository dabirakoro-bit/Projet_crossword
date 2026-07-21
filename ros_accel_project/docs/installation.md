# Installation et lancement du projet

## 1. Introduction

Cette partie décrit les étapes nécessaires pour installer l'environnement logiciel et lancer le système de contrôle basé sur ROS2.

Le projet utilise :

- ROS2 Humble ;
- Python ;
- un workspace ROS2 avec `colcon` ;
- un ESP8266 programmé avec l'environnement Arduino ;
- un Arduino Nano pour l'affichage.

---

# 2. Prérequis

Avant de lancer le projet, les éléments suivants doivent être installés :

| Élément | Utilisation |
|---|---|
| Ubuntu 22.04 | Système d'exploitation recommandé pour ROS2 Humble |
| ROS2 Humble | Environnement robotique |
| Python 3 | Exécution des nœuds ROS2 |
| Arduino IDE | Programmation ESP8266 et Arduino Nano |
| Colcon | Compilation du workspace ROS2 |

Documentation officielle ROS2 :

https://docs.ros.org/en/humble/

---

# 3. Installation de ROS2

Installer ROS2 Humble en suivant la documentation officielle :

```text
https://docs.ros.org/en/humble/Installation.html
```

Après installation, charger l'environnement ROS2 :

```bash
source /opt/ros/humble/setup.bash
```

Pour charger automatiquement ROS2 à chaque ouverture de terminal :

```bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```

---

# 4. Installation du workspace ROS2

Le projet utilise un workspace nommé :

```text
ros2_ws
```

Créer le workspace :

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws
```

Placer le package du projet dans le dossier `src`.

Compiler le workspace :

```bash
colcon build
```

Après compilation, charger les nouveaux packages :

```bash
source install/setup.bash
```

---

# 5. Installation des dépendances Python

Le nœud `esp_receiver_node` utilise un serveur HTTP basé sur Flask.

Installer les dépendances nécessaires :

```bash
pip install flask
```

Le module série utilisé pour la communication avec Arduino nécessite :

```bash
sudo apt install python3-serial
```

---

# 6. Configuration de l'ESP8266

L'ESP8266 doit être configuré avec :

- le nom du réseau Wi-Fi ;
- le mot de passe du réseau ;
- l'adresse IP de l'ordinateur exécutant ROS2.

Dans le programme Arduino, modifier :

```cpp
const char* ssid = "Nom_WIFI";
const char* password = "Mot_de_passe";

const char* serverUrl = "http://IP_PC:5000/accel";
```

Après modification :

1. compiler le programme ;
2. téléverser le code dans l'ESP8266 ;
3. vérifier la connexion dans le moniteur série.

---

# 7. Configuration de l'Arduino Nano

L'Arduino Nano doit être programmé avec le programme d'affichage LCD.

La communication avec ROS2 est réalisée par le port série USB.

Vérifier le port utilisé :

```bash
ls /dev/ttyACM*
```

Exemple :

```text
/dev/ttyACM0
```

Ce port doit correspondre à celui utilisé dans le nœud :

`turtle_pose_serial_node`

---

# 8. Lancement du système

Après compilation du workspace :

```bash
cd ~/ros2_ws
source install/setup.bash
```

Lancer le système complet :

```bash
ros2 launch accel_system_cmd accel_turtle.launch.py
```

Le lancement démarre :

- le simulateur TurtleSim ;
- le nœud de contrôle ;
- le nœud de réception ESP8266 ;
- le nœud de communication série avec Arduino.

---

# 9. Vérification du fonctionnement

Vérifier les nœuds actifs :

```bash
ros2 node list
```

Vérifier les topics disponibles :

```bash
ros2 topic list
```

Afficher les données de l'accéléromètre :

```bash
ros2 topic echo /accel
```

Afficher la position de la tortue :

```bash
ros2 topic echo /turtle1/pose
```

---

# 10. Résumé

Le lancement du système suit la chaîne suivante :

```text
ESP8266
    |
    | Wi-Fi HTTP
    ↓
ROS2
    |
    | Topics
    ↓
TurtleSim
    |
    | USB Série
    ↓
Arduino Nano + LCD
```

Une fois l'ensemble configuré, le système permet de contrôler une simulation robotique à partir d'un capteur inertiel sans fil.