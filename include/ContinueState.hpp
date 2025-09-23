#ifndef CONTINUESTATE_HPP
#define CONTINUESTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>

class ContinueState : public State {
public:
    explicit ContinueState(Context& ctx) : State(ctx) {}
    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float) override {}
    void render(sf::RenderWindow& window) override;

private:
    sf::Text title, hint;
    sf::RectangleShape btnYesRect, btnNoRect;
    sf::Text btnYesText, btnNoText;
};

#endif
