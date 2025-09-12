# ensea-tp1-cpp
Répositoire des projets de TP pour la matière "Introduction au C++".

## Projet 1: Pokémon (C++ / SFML)

Ce projet est un petit programme en C++ utilisant **SFML** pour faire un petit jeu Pokémon.

## Prérequis

- **g++** ou tout autre compilateur C++17
- **CMake** (≥ 3.16)
- **SFML** (≥ 2.5)

Sur Ubuntu/Debian, vous pouvez installer avec :

```bash
sudo apt-get install g++ cmake libsfml-dev

```
### Compilation
Pour compiler, utilisez depuis la racine du projet:
```bash
mkdir build
cd build
cmake ..
make
```
Cela génère l’exécutable:
```bash
./tictactoe
```

### Structure

include/ : fichiers d’en-tête (.hpp)

src/ : fichiers source (.cpp)

pokemon/ : images PNG des Pokémon

fonts/ : police (.ttf) pour l’affichage du texte

CMakeLists.txt : configuration du projet

### Utilisation

Lancez l’exécutable dans build.


