#ifndef BATTLESTATE_HPP
#define BATTLESTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>

class BattleState : public State {
public:
    explicit BattleState(Context& ctx);

    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Text title;
    sf::Text hint;
};

#endif
