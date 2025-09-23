#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "StateMachine.hpp"
#include "Pokedex.hpp"
#include "PokemonParty.hpp"
#include "PokemonAttack.hpp"
#include "Context.hpp"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    sf::RenderWindow window;
    sf::Font font;

    Pokedex* pokedex;   // store pointers, singleton is managed in other class
    PokemonParty party;
    PokemonAttack attack;
    PokemonParty enemy;

    Context ctx;
    StateMachine machine;
};

#endif
