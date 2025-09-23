#include "Game.hpp"
#include "SFMLHelpers.hpp"
#include "Context.hpp"

#include "IntroState.hpp"
#include "TeamSelectState.hpp"
#include "PreBattleState.hpp"
#include "BattleState.hpp"
#include "VictoryState.hpp"
#include "ContinueState.hpp"

Game::Game()
: window(sf::VideoMode(1400, 1000), "Pokemon - ENSEA Version"),
  pokedex(&Pokedex::instance("pokedex.csv", false)),
  machine(*(new Context)) 
{
    // font
    loadFont(font);

    // base data
    party.addFromDexByName("Bulbasaur");
    party.addFromDexByName("Charmander");
    party.addFromDexById(7); // squirtle
    party.addFromDexById(255); // torchic
    party.addFromDexByName("Lugia");



    // context
    ctx.window  = &window;
    ctx.font    = &font;
    ctx.pokedex = pokedex;
    ctx.party   = &party;
    ctx.attack  = &attack;
    ctx.enemy   = &enemy;
    ctx.machine = &machine;

    // update reference to statemachine by passing new one
    StateMachine* old = &machine;
    new (&machine) StateMachine(ctx);
    (void)old;

    // saves states
    machine.registerState(StateID::Intro,      [](Context& c){ return std::make_unique<IntroState>(c); });
    machine.registerState(StateID::TeamSelect, [](Context& c){ return std::make_unique<TeamSelectState>(c); });
    machine.registerState(StateID::PreBattle,  [](Context& c){ return std::make_unique<PreBattleState>(c); });
    machine.registerState(StateID::Battle,     [](Context& c){ return std::make_unique<BattleState>(c); });
    machine.registerState(StateID::Victory,     [](Context& c){ return std::make_unique<VictoryState>(c); });
    machine.registerState(StateID::Continue,     [](Context& c){ return std::make_unique<ContinueState>(c); });


    // intro state
    machine.changeState(StateID::Intro);
}

//destructor
Game::~Game() {}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event ev{};
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (machine.current()) machine.current()->handleEvent(ev);
        }

        float dt = clock.restart().asSeconds();
        if (machine.current()) machine.current()->update(dt);

        window.clear(sf::Color(30, 30, 40));
        if (machine.current()) machine.current()->render(window);
        window.display();
    }
}
