#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SFML/Graphics.hpp>

// Forward declarations to avoid dependencies
class Pokedex;
class PokemonParty;
class PokemonAttack;
class StateMachine;

struct Context {
    sf::RenderWindow* window = nullptr;
    sf::Font* font = nullptr;
    Pokedex* pokedex = nullptr;
    PokemonParty* party = nullptr;
    PokemonAttack* attack = nullptr;
    StateMachine* machine = nullptr; // change state
};

#endif 
