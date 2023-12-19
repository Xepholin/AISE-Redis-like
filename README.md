# AISE-Redis-like

Dans le cadre du projet d'architecture interne des systèmes d'exploitation, nous avons dû réaliser un stockage clef-valeur en adoptant le même protocole que Redis.

Le stockage clef-valeur est implémenté au sein d'un fichier texte (.txt). Chaque ligne du fichier représente une clé distincte suivie de sa valeur. Les clés et les valeurs sont séparées par un délimiteur.

Pour accéder aux clefs ainsi qu'à sa valeur, plusieurs commandes sont disponibles. Ces commandes permettent :

- Une réponse du serveur au client : PING
- L'ajouter et la mettre à jour de clé : SET
- L'affichage de la valeur d'une clé : GET
- La suppression des paires clefs-valeurs : DEL
- L'incrémentation de la valeur d'une clé : INCR
- La décrémentation de la valeur d'une clé : DECR

Toutes ses commandes mettent à jour les paires clefs-valeurs dans le fichier texte.