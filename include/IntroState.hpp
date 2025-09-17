#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include "Context.hpp"
#include "StateMachine.hpp"

// only methods needed are rendering and advancing to next state after receiving enter
class IntroState : public State {
public:
    explicit IntroState(Context& ctx);

    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Text title;
    sf::Text subtitle;
};

#endif
