# Problèmes rencontrés et solutions

## 1. Introduction

Cette partie présente les principaux problèmes rencontrés lors du développement du système ainsi que les solutions mises en place.

Ces informations permettent de faciliter le diagnostic, la maintenance et l'évolution du projet.

---

# 2. Problème de communication ESP8266 - ROS2

## Symptôme

L'ESP8266 était connecté au réseau Wi-Fi, mais les données d'accélération n'étaient pas reçues par ROS2.

## Cause

Le système ROS2 était exécuté dans un environnement WSL, tandis que le serveur HTTP chargé de recevoir les données était exécuté depuis l'environnement Linux.

La communication nécessitait donc d'utiliser une adresse IP accessible depuis l'ESP8266.

## Solution

Vérifier l'adresse IPv4 de l'ordinateur :

```bash
ipconfig
```

Puis utiliser cette adresse dans le programme ESP8266 :

```cpp
const char* serverUrl = "http://IP_PC:5000/accel";
```

Le serveur Flask doit écouter sur toutes les interfaces réseau :

```python
app.run(host='0.0.0.0', port=5000)
```

Cette configuration permet à l'ESP8266 d'envoyer les données vers le nœud ROS2.

---

# 3. Problème de lancement des nœuds ROS2

## Symptôme

Après l'exécution du fichier launch :

```bash
ros2 launch accel_system_cmd accel_turtle.launch.py
```

certains nœuds nécessaires au fonctionnement du système n'étaient pas actifs.

La commande suivante ne retournait pas tous les nœuds attendus :

```bash
ros2 node list
```

## Cause

Certains nœuds existaient dans le package ROS2 mais n'étaient pas déclarés dans le fichier de lancement.

Un nœud ROS2 n'est exécuté automatiquement que s'il est ajouté dans le fichier `launch`.

## Solution

Vérifier le fichier :

```text
launch/accel_turtle.launch.py
```

et ajouter tous les nœuds nécessaires au fonctionnement du système.

Exemple :

```python
Node(
    package='accel_system_cmd',
    executable='esp_receiver_node',
    name='esp_receiver_node'
)
```

Après modification :

```bash
colcon build
```

Puis :

```bash
source install/setup.bash
```

Relancer ensuite le système.

## Vérification

Les nœuds actifs doivent correspondre à l'architecture finale :

```bash
ros2 node list
```

Exemple :

```text
/turtlesim
/turtle_control
/esp_receiver_node
/pose_serial
```

---

# 4. Problème de fichier Launch ROS2 non trouvé

## Symptôme

Lors du lancement :

```bash
ros2 launch accel_system_cmd accel_turtle.launch.py
```

ROS2 indique que le fichier launch n'a pas été trouvé.

## Cause

Le fichier launch peut exister dans le projet mais ne pas être installé correctement dans le package ROS2.

## Solution

Vérifier :

- la présence du fichier dans le dossier `launch` ;
- la configuration du fichier `setup.py` ;
- l'installation correcte des fichiers du package.

Après chaque modification :

```bash
colcon build
```

Puis :

```bash
source install/setup.bash
```

---

# 5. Problème de communication série avec Arduino

## Symptôme

Le nœud `turtle_pose_serial_node` ne parvient pas à envoyer les données vers l'Arduino Nano.

## Causes possibles

- mauvais port série sélectionné ;
- permissions insuffisantes ;
- Arduino non reconnu par le système.

## Solution

Identifier les ports disponibles :

```bash
ls /dev/ttyACM*
```

Exemple :

```text
/dev/ttyACM0
```

Ajouter l'utilisateur au groupe permettant l'accès aux ports série :

```bash
sudo usermod -a -G dialout $USER
```

Puis redémarrer la session utilisateur.

---

# 6. Conflit de topics ROS2

## Symptôme

Le même topic ROS2 possède plusieurs types de messages incompatibles.

Exemple :

```text
/accel
```

## Cause

Plusieurs versions du programme utilisaient le même nom de topic avec des types différents.

## Solution

Vérifier les informations du topic :

```bash
ros2 topic info /accel
```

Le système final utilise :

```text
Topic : /accel

Type :
std_msgs/msg/Float32MultiArray
```

Cette configuration garantit une communication cohérente entre les différents nœuds.

---

# 7. Problème de connexion Wi-Fi ESP8266

## Symptôme

L'ESP8266 ne transmet plus les données vers ROS2.

## Vérifications

Vérifier :

- le nom du réseau Wi-Fi ;
- le mot de passe ;
- la connexion de l'ESP8266 au réseau ;
- l'adresse IP du serveur ;
- le fonctionnement du serveur Flask.

Le moniteur série de l'ESP8266 permet également de vérifier l'état de la connexion.

---

# 8. Commandes utiles pour le diagnostic

## Vérifier les nœuds actifs

```bash
ros2 node list
```

## Vérifier les topics disponibles

```bash
ros2 topic list
```

## Afficher les données de l'accéléromètre

```bash
ros2 topic echo /accel
```

## Afficher la position de TurtleSim

```bash
ros2 topic echo /turtle1/pose
```

## Vérifier un topic

```bash
ros2 topic info /accel
```

---

# 9. Conclusion

Les principaux problèmes rencontrés concernaient :

- la communication réseau entre l'ESP8266 et ROS2 ;
- l'utilisation de ROS2 dans un environnement WSL ;
- la configuration du fichier launch ;
- la communication série avec l'Arduino ;
- la cohérence des topics ROS2.

La séparation du système en plusieurs modules indépendants permet cependant de tester chaque partie séparément et facilite le dépannage.