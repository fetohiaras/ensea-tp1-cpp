#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>
#include <memory>

struct Context;

// state id
enum class StateID {
    Intro,
    TeamSelect,
    PreBattle,
    Battle,
    Victory,
    Continue,
    GameOver
};

// state interface
class State {
public:
    explicit State(Context& ctx) : ctx(ctx) {}
    virtual ~State() = default;

    virtual void onEnter() {}
    virtual void handleEvent(const sf::Event& ev) = 0;
    virtual void update(float dt) {}
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void onExit() {}

protected:
    Context& ctx;
};

#endif
