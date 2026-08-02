# Détection automatique de la polarité d'une LED avec Arduino

## Présentation

Ce projet est une **preuve de concept (Proof of Concept - PoC)** permettant d'identifier automatiquement la polarité d'une LED classique à l'aide d'un Arduino Uno.

Le système permet de déterminer :

- l'anode de la LED ;
- la cathode de la LED.

La détection est réalisée sans connaître au préalable le sens de branchement de la LED.

Le résultat est affiché automatiquement dans le moniteur série Arduino.

Ce projet constitue une première étape vers un projet plus ambitieux : la conception d'un système capable d'identifier automatiquement le brochage et la polarité d'une matrice LED 8×8 inconnue.

---

# Objectif du projet

L'objectif principal est de valider une méthode permettant de reconnaître automatiquement le sens de conduction d'une LED.

Cette étape est nécessaire avant de passer à un problème plus complexe :

> Identifier automatiquement une matrice LED 8×8 dont les broches ne sont pas documentées.

Une matrice LED inconnue peut poser plusieurs problèmes :

- matrice à anode commune ou cathode commune ;
- identification des lignes ;
- identification des colonnes ;
- absence de marquage sur les broches.

Avant de développer un système complet, le principe est d'abord testé sur un composant simple : une LED unique.

---

# Matériel utilisé

- Arduino Uno
- LED classique
- Résistance de protection de 1 kΩ
- Câbles de connexion

---

# Schéma de câblage

La LED est volontairement branchée dans un sens inconnu.

```
Arduino D2 ---- 1 kΩ ----●---- LED ---- Arduino D3
                          |
                          |
                         A0
```

## Connexions utilisées

| Fonction | Broche Arduino |
|---|---|
| Première broche de test | D2 |
| Deuxième broche de test | D3 |
| Mesure analogique | A0 |

---

# Principe de fonctionnement

La méthode consiste à tester les deux directions possibles de circulation du courant dans la LED.

L'Arduino applique successivement deux configurations :

## Test 1

Configuration :

```
D2 = HIGH
D3 = LOW
```

La tension au point de mesure A0 est enregistrée.

---

## Test 2

Configuration :

```
D2 = LOW
D3 = HIGH
```

Une deuxième mesure est réalisée.

---

# Analyse des mesures

La tension mesurée par l'entrée analogique est convertie par l'ADC de l'Arduino.

La valeur retournée est comprise entre :

```
0 → 1023
```

correspondant approximativement à :

```
0 V → 5 V
```

Cependant, la valeur exacte peut varier selon :

- la tension d'alimentation ;
- la tolérance de la résistance ;
- la tension directe de la LED ;
- les variations de mesure de l'ADC.

Pour cette raison, le programme utilise des **seuils de détection** plutôt que des valeurs fixes.

---

# Logique de détection

Le programme compare les deux mesures obtenues.

## Cas 1 : conduction dans le sens D2 vers D3

Exemple :

```
Test 1 : valeur élevée
Test 2 : valeur proche de 0
```

Conclusion :

```
Anode  : D2
Cathode : D3
```

---

## Cas 2 : conduction dans le sens D3 vers D2

Exemple :

```
Test 1 : valeur proche de 0
Test 2 : valeur élevée
```

Conclusion :

```
Anode  : D3
Cathode : D2
```

---

# Choix des seuils

Les seuils utilisés dans le programme ne correspondent pas à une valeur exacte.

Ils servent uniquement à distinguer les différents états électriques.

Les valeurs ont été choisies expérimentalement après plusieurs essais.

Cette méthode permet de prendre en compte :

- les variations de tension ;
- les différences entre les LEDs ;
- les erreurs de conversion analogique.

---

# Résultat expérimental

Exemple obtenu lors des tests :

```
Test 1 (D2 HIGH -> D3 LOW) : 564

Test 2 (D3 HIGH -> D2 LOW) : 0
```

Résultat :

```
LED détectée !

Anode : broche D2
Cathode : broche D3
```

---

# Programme utilisé

Le programme réalise automatiquement :

1. La configuration des broches D2 et D3.
2. Le premier test de polarité.
3. Le deuxième test de polarité.
4. La lecture analogique sur A0.
5. La comparaison des résultats.
6. L'affichage de l'anode et de la cathode.

---

# Limites actuelles

Cette preuve de concept fonctionne pour une LED simple.

Cependant, une matrice LED 8×8 introduit de nouvelles contraintes :

- plusieurs LEDs connectées entre elles ;
- présence de lignes et colonnes ;
- chemins électriques indirects ;
- nécessité de limiter le courant pendant les tests.

---

# Évolution vers le projet principal

Ce projet constitue une première brique pour un système plus complet :

## Identification automatique d'une matrice LED 8×8

L'objectif final est de développer un système capable de déterminer automatiquement :

- si la matrice possède une anode commune ou une cathode commune ;
- quelles broches correspondent aux lignes ;
- quelles broches correspondent aux colonnes.

Le système final utilisera :

- deux registres à décalage 74HC595 ;
- une méthode de test automatique des différentes combinaisons de broches ;
- une analyse des réponses électriques de la matrice.

---

# Conclusion

Cette expérience a permis de valider un principe simple de reconnaissance de polarité basé sur :

- l'application de deux tensions opposées ;
- la mesure analogique de la réponse du circuit ;
- l'analyse des différences entre les deux configurations.

Ce prototype représente une première étape expérimentale avant le développement d'un analyseur automatique de matrices LED 8×8.