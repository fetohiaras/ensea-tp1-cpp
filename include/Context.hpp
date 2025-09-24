#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SFML/Graphics.hpp>

// Forward declarations to avoid dependencies
class Pokedex;
class PokemonParty;
class PokemonAttack;
class StateMachine;
class SoundManager; 

struct Context {
    sf::RenderWindow* window = nullptr;
    sf::Font* font = nullptr;
    Pokedex* pokedex = nullptr;
    PokemonParty* party = nullptr;
    PokemonAttack* attack = nullptr;
    PokemonParty* enemy = nullptr;
    StateMachine* machine = nullptr; // change state

    SoundManager* sound = nullptr;         // added by game
    std::string   musicRequest;            // request for sound file
    bool          musicDirty = false;      // true if state changed desired music

};

#endif 
