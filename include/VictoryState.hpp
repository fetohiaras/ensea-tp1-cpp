#ifndef VICTORYSTATE_HPP
#define VICTORYSTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>

class VictoryState : public State {
public:
    explicit VictoryState(Context& ctx) : State(ctx) {}
    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float) override {}
    void render(sf::RenderWindow& window) override;
private:
    sf::Text title, hint;
    sf::Text question;      
    sf::Text escHint;       
    sf::RectangleShape btnReplayRect;
    sf::Text btnReplayText;
};

#endif
