## Évaluation du Code

Le code démontre une bonne compréhension de :

- **Pattern Singleton** :  
  Bien implémenté avec une instance unique du `Pokedex`.

- **Parsing CSV** :  
  Gestion robuste des fichiers avec validation des données :
  - suppression des espaces,
  - skip de l'en-tête,
  - filtrage des "Mega" (même si ce filtrage reste optionnel).

- **Gestion des exceptions** :  
  Les erreurs de lecture de fichier et de conversion sont correctement traitées.

- **Organisation du projet** :  
  Les fichiers `.cpp` et `.hpp` auraient pu étre  séparés, ce qui améliore la lisibilité du code.  
  La présence d’un fichier `README.md` est également une bonne pratique pour documenter le projet.  
  Il manque toutefois un fichier `.gitignore` afin d’éviter de pousser des fichiers inutiles.

- **Conception orientée objet (héritage)** :  
  Actuellement, la classe `PokemonAttack` hérite directement de `PokemonVector`.  
  Une amélioration aurait été de la faire **hériter de `PokemonParty`**, qui elle-même hérite de `PokemonVector`.  
  Cela aurait permis de mieux illustrer le principe d’héritage en chaîne et la spécialisation progressive des classes :
  - `PokemonVector` = structure de base pour stocker les Pokémon
  - `PokemonParty` = une équipe générique de Pokémon sans limite
  - `PokemonAttack` = un cas particulier d’équipe (`Party`) limitée à 6 Pokémon.  
    Ce choix aurait renforcé la cohérence et la compréhension de la logique du jeu.

J'ai également beaucoup apprécié l'**interface graphique**, claire et interactive.

---

### 💡 Intervention proposée

**Fonctionnalité : Ajouter un système de sons Pokémon**
- Intégrer un son de démarrage, créant une expérience audio plus immersive.
- Enrichir l'interface graphique par une ambiance sonore adaptée, renforçant l'immersion du joueur.
