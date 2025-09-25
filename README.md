# ensea-tp1-cpp: Projet Pokémon (C++ / SFML)

Ce projet est un programme en C++ utilisant **SFML** pour faire un jeu Pokémon, en combinant des concepts fondamentaux du C++ (programmation orientée objet, héritage, design patterns) et une bibliothèque graphique moderne (SFML) pour créer une interface interactive.

Le projet commence par une base textuelle (affichage en console, logique de combat) puis évolue vers une version graphique avec gestion des sprites, affichage d’équipes, transitions d’états et interactions utilisateur.

## Développement

### Classes initiales

#### Classe Pokemon

Cette classe représente l’unité de base du jeu : un Pokémon. Elle contient les attributs principaux :

- identifiant (id) 
- nom 
- points de vie (max et courant)
- attaque
- défense
- génération/évolution 

Elle définit les constructeurs (y compris le constructeur de copie) et le destructeur, ainsi que fournit les getters et setters pour tous les attributs.

![Alt text](/images/pokemon_attributes.png)

Pour le combat, elle implémente les fonctions suivantes :

- launch_attack() : calcule les dégâts et réduit les HP de l’adversaire.

- displayInfo() : affiche les informations détaillées du Pokémon (utile au mode texte).

![Alt text](/images/pokemon_attack.png)

Cette classe, combiné avec les classes d'ensembles de pokémons, permis de tester la logique du jeu en mode console avant de passer à l’interface graphique.

#### Classe PokemonVector

Cette classe sert d’interface commune à toutes les structures de collections de Pokémon. Elle définit des méthodes virtuelles pures :

- add
- removeAt
- size
- at
- displayAll

Cette interface garantit aussi que toutes les classes dérivées (Pokedex, Party, Attack) respectent la même API, et cela simplifie le code et rend l’architecture plus claire.

#### Classe Pokedex

Cette classe represente l'ensemble de pokémons disponibles dans la Pokedex en les lisant d'un fichier .csv qui contient toutes les infos pour les pokémons (nom, attributs, etc.); elle utilise le design pattern Singleton afin qu'il n’existe qu’une seule instance d'elle. À partir de cette base de données, les autres classes peuvent cloner des Pokémon pour les utiliser.

La classe fournit deux méthodes principales :

- cloneById(int id)

- cloneByName(const std::string& name)

![Alt text](/images/pokedex.png)

#### Classe PokemonParty
Cette classe représente l’ensemble de Pokémons disponibles au joueur comme à la "Pokémon Box" disponible dans les jeux. Elle permet d’ajouter des Pokémon depuis la Pokédex, soit par id ou par nom, ainsi que les fonctions de gestion de l’équipe: affichage, ajout, suppression. Elle interagit également avec la PokemonAttack pour envoyer ou récupérer des Pokémon.

#### Classe PokemonAttack
Représente l’équipe d’attaque active du joueur (limite de 6 Pokémon). Elle sert à choisir les Pokémon qui participeront effectivement au combat, avec les fonctions pour ajouter les Pokémons depuis la PokemonParty, retirer ou renvoyer un Pokémon vers la Party, gérer les échanges (déplacements multiples), avec un affichage textuel similaire à la Party.
Cette classe sera utilisée pour gerer les pokémons de chaque joueur lors de la logique de combat.

### Graphismes et SFML
Le projet est ensuite passé du mode console à une version graphique grâce à la bibliothèque SFML.
Le professeur nous a fourni un code de base minimal, que fait que l'ouverture d’une fenêtre noire et l'affichage d’un cercle vert.

![Alt text](/images/hello_sfml.png)

Après ce point de départ, on peut comprendre la boucle d’événements (sf::RenderWindow, sf::Event) et le cycle effacer -> dessiner -> afficher.

#### Affichage des pokémons
À partir de ce code, nous avons connecté la logique déjà existante des classes (Pokemon, Pokedex, PokemonParty, etc.) et affiché dans une fenêtre les informations détaillées des Pokémon sélectionnés (id, nom, HP, attaque, défense) . C’était la première étape vers une vraie interface graphique Pokémon.

![Alt text](/images/first_application.png)

On a ensuite enrichi la fenêtre pour afficher une grille de Pokémon depuis la PokemonParty, permettre le transfert de Pokémon de la Party vers l’AttackParty (et inversement), ajouter le repositionnement des Pokémon à l’intérieur de l’AttackParty, utiliser les flèches du clavier et les clics pour gérer la sélection (merci à Claude pour les calculs des cordonnées graphiques!).

![Alt text](/images/second_application.png)

À ce stage, il était possible de composer son équipe d’attaque graphiquement, en temps réel à partir d'une initialisation manuelle de la PokemonParty du joueur en copiant quelques pokemons de la Pokedex afin de les rendre disponibles pour ajouter à la AttackParty.

#### SFMLHelpers
Au départ, toute la logique graphique (chargement des sprites, dessin de rectangles/textes, gestion des clics clavier/souris) était codée directement dans le main.cpp, et cela rendait le code difficile à lire et à maintenir.

On a donc introduit un module séparé : SFMLHelpers.hpp / .cpp. Ce fichier regroupe toutes les fonctions utilitaires SFML : chargement de textures et polices (loadTextureById, loadFont), création d’éléments graphiques (makeButton, zones de texte, etc.), gestion simplifiée des événements (clics, touches).

![Alt text](/images/sfmlhelpers.png)

Grâce à cette refactorisation, la fonction main s'est devenue beaucoup plus claire et concise, ne contenant que la logique de haut niveau et il est désormais simple d’ajouter ou de modifier l’interface sans surcharger le coeur du jeu.

### Code review
Avant de passer à la refactorisation complète avec le design pattern State, un code review a été effectué par une collégue de classe, [Chahrazed BAATOUT](https://github.com/ChahrazedBaatout). Le projet a été partagé sur GitHub, et cette collégue a ouvert un branch séparé pour y apporter des remarques.

Ce travail a permis de vérifier ce qui a été bien implementé et aussi de mettre en évidence certains points d’amélioration dans la structure du code, notablement l'effacement de quelques fonctionnalités que n'étaient pas dans le cahier des charges du projet et que dont on n'avait pas le besoin d'ajouter.

![Alt text](/images/codereview.png)

#### Ajout du son (SoundManager)
En plus du review, une nouvelle fonctionnalité a été ajoutée: un SoundManager basé sur les librairies audio SFML.
Cette classe permettait de charger et jouer un son à partir d’un fichier (.ogg, .wav, etc.), et une première version a été intégrée directement dans le main.cpp.

![Alt text](/images/soundmanager.png)

Par la suite, lors de l’introduction du design pattern State, cette gestion du son a été intégrée proprement à la machine d’états : chaque State peut spécifier une musique à jouer, la classe Game gère le SoundManager centralisé, une transition entre deux états coupe ou remplace la musique en cours.

Cela a permis d’obtenir une expérience plus immersive, avec une musique d’introduction dans l’IntroState, une musique différente pendant la TeamSelect, un fond sonore spécifique pendant le BattleState, etc.

### Machines à état et design pattern STATE

Après avoir implémenté les premières fonctionnalités graphiques (affichage des Pokémon, gestion de la Party et de l’AttackParty), notre professeur (Mr. TAUVEL) nous a présenté le design pattern “State”. Son principe est de modéliser le jeu comme une machine à états finis (FSM), où chaque écran (intro, sélection, combat, etc.) devient un état indépendant avec ses propres méthodes.

Afin de faciliter le travail, la structure de machine à états suivante était suggeré:

![Alt text](/images/statemachine_original.png)

Alors, on a décidé d'implémenter la machine à états avec les suivantes méthodes à partager entre les états:

- onEnter() : initialisation à l’entrée dans l’état,

- handleEvent() : gestion des entrées clavier/souris,

- update() : logique de l’état,

- render() : affichage graphique.

#### Première version

Vu que le choix était libre entre implémenter la machine à états donnée ou de faire une autre, on a adapté le modèle donné pour créer une version simplifiée du jeu :

- Intro: écran d’introduction, passe à la sélection sur touche/cliquer.

- TeamSelect: choix de l’équipe du joueur (transferts Party <-> AttackParty).

- Battle: combat contre une équipe ennemie fixe.

- Victory / Continue: écrans de fin, Victory ne permettant que sortir du jeu et COntinue permettant de jouer encore (revenir à la sélection de l'équipe).

![Alt text](/images/statemachine_v1.png)


#### Nouvelles versions

Par la suite, la structure a été enrichie :

 - PreBattleState : Affiche l’équipe du joueur vs l’équipe ennemie et permet de confirmer ou de revenir à la sélection.

 - VictoryState : Ajout d’un bouton “Jouer une autre fois”, la possibilité de retourner directement à la sélection d’équipe, et l'option de quitter le jeu avec ESC.

 - ContinueState : En cas de défaite, proposition : “Continuer ?” avec Oui (retour à la sélection) et Non (Game Over, jeu fini).

Ces transitions rendent le jeu plus fluide et rejouable avec une architecture claire.

![Alt text](/images/statemachine_v2.png)

Pour la modélisation d'états final, on a rajouté plus d'options de navigation avec les touches, comme sortie du logiciel avec ESC dans toutes les écrans.

![Alt text](/images/statemachine_final.png)

### Combat
Le combat a été implementé en suivant la même logique de la classe Pokemon du début: le joueur clique sur le bouton d'attaque, et si son pokémon a un attaque plus fort que la défense du pokémon adversaire en jeu, il causera un dommage fixe égal à (défense du pokémon adversaire - attaque du pokémon joueur); Le même s'applique pour l'adversaire en faisant son contre-attaque.
Si un des pokemons est KO, le prochain pokémon de l'AttackParty sera sorti automatiquuement, jusqu'à une des équipes à été épuisé, dont la transition pour l'état Victory ou Continue sera calculé.
On a aussi ajouté une affichage dynamique de l'HP de chaque pokémon pendant le combat au méthode de Render du BattleState, avec différentes couleurs pour chaque "niveau" de HP.

![Alt text](/images/combat.png)

### Travail restant et améliorations possibles

Bien que la structure générale du projet soit en place (gestion des Pokémon, sélection d’équipe, intégration de SFML et machine d’états pour la navigation entre les écrans), certaines parties du gameplay restent encore à développer:
- Ajouter une logique de combat plus complexe (maintenant, le joueur ne peut rien faire qu'attaquer et attendre le contre-attaque de l'ennemi, tous basés sur les valeurs d'attaque et défense de chaque pokemon fournis via le fichier .csv, sans quelque caractère aléatoire au dommage posé par attaque)
- Ajouter des backgrounds pour chaque écran afin de faire le jeu plus beau
- Régler les glitchs d'affichage de texte avec charactères spéciaux (accents, etc.), probablement liés à la police choisi
- Ajouter des animations pour attaque et défense

Cependant, le développement de ce jeu m'a apporté une grande opportunité d'avoir une bonne introduction au développement logiciel avec C++ dans un cadre très ludique et intéressant.


## Prérequis pour testage

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

### Structure des fichiers

include/ : fichiers d’en-tête (.hpp)

src/ : fichiers source (.cpp)

pokemon/ : images PNG des Pokémon

fonts/ : police (.ttf) pour l’affichage du texte

assets/ : sprites divers

sound/ : fichiers de son 

CMakeLists.txt : configuration du projet

### Utilisation

Lancez l’exécutable dans build avec 
```bash
./tictactoe
```

## Disclaimer et license
Ce projet a été développé uniquement à des fins pédagogiques dans le cadre d’un cours d’introduction au C++.  
Il n’a aucune vocation commerciale.  

Les éléments utilisés, tels que les noms, images et sons liés à la franchise *Pokémon*, sont la propriété exclusive de Nintendo Co., Ltd., GAME FREAK Inc., Creatures Inc. et The Pokémon Company.  
Tous droits leur sont réservés.

