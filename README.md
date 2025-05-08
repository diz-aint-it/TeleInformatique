# Calculatrice Binaire

## Description
Ce projet est une application client-serveur qui permet d'effectuer des opérations arithmétiques en binaire. L'application utilise un protocole de communication fiable avec détection d'erreurs via CRC32.

## Auteurs
- Aziz Jouini
- Abdelkader Ammar

## Structure du Projet
Le projet est composé des fichiers suivants :
- `client.c` : Programme client qui envoie les requêtes de calcul
- `server.c` : Programme serveur qui effectue les calculs
- `crc32.c` et `crc32.h` : Implémentation du contrôle CRC32 pour la détection d'erreurs
- `error_simulator.c` et `error_simulator.h` : Simulateur d'erreurs pour tester la robustesse
- `structs.h` : Définitions des structures de données communes

## Prérequis
- GCC (GNU Compiler Collection)
- Bibliothèque Winsock2 (Windows)

## Compilation
Pour compiler le serveur :
```bash
gcc server.c crc32.c -o server.exe -lws2_32
```

Pour compiler le client :
```bash
gcc client.c crc32.c error_simulator.c -o client.exe -lws2_32
```

## Exécution
1. Démarrer le serveur :
```bash
./server.exe
```

2. Dans un autre terminal, démarrer le client :
```bash
./client.exe
```

## Fonctionnalités
- Opérations arithmétiques en binaire
- Détection d'erreurs via CRC32
- Communication client-serveur robuste
- Simulation d'erreurs pour les tests

## Notes
- Assurez-vous que le serveur est démarré avant le client
- Le programme utilise le protocole TCP/IP pour la communication
- Les erreurs de transmission sont automatiquement détectées et gérées 