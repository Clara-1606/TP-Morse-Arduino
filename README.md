# Morse Arduino

## Contexte 

Le but de ce TP est de pouvoir communiquer en code Morse entre le moniteur série et un montage Arduino.
On utilisera pour cela :
- La liaison série UART
- Les entrées analogiques et numériques
- Les sorties numériques
- La programmation orientée objet en C++ et les bibliothèques Arduino communes.

## Projet 

Le Code Morse associe aux lettres et aux chiffres des combinaisons de signaux longs et courts séparés par des interruptions de signal.

On alternera alors entre :
- Un appui court ( . )
- Un appui long 〰 dont la longueur est environ 3 fois celle d'un appui court ( - )
- Une pause (aussi longue qu'un appui court) 

Exemple : 
La lettre S est encodée par ...
La lettre O est encodée par ---

SOS ->  ... --- ...

Grâce à ce projet nous pouvons :  
- Envoyer du texte depuis la liaison série et l'émettre en code morse lumineux (Scott) à travers l'affichage d'une LED
- Entrer du Morse à l'aide d'un bouton et afficher le texte correspondant sur le moniteur série.

On contrôle la cadence du code (c'est à dire le temps représentant un appui court) grâce à un potentiomètre pour contrôler la vitesse de clignotement de la LED et la "vitesse de lecture" du bouton.


## Lancer le projet

Ce projet a été réalisé pendant la période du confinement, nous n'avions donc pas accès au matériel.
Nous avons donc utiliser tinkercad.

Voici le lien du projet avec le montage effectué : https://www.tinkercad.com/things/aTjUMLuxAuq

