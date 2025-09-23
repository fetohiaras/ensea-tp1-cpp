#ifndef BATTLESTATE_HPP
#define BATTLESTATE_HPP

#include "State.hpp"
#include "Pokemon.hpp"
#include <SFML/Graphics.hpp>

class BattleState : public State {
public:
    explicit BattleState(Context& ctx);

    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    // cuurent pokes index
    int myIdx = 0;
    int enIdx = 0;

    // sprites/text
    sf::Texture myTex, enTex;
    sf::Sprite  mySpr, enSpr;
    sf::Text    myName, enName;

    // message box and buttons
    sf::RectangleShape msgBox;
    sf::Text msgText;

    sf::RectangleShape btnAttackRect;
    sf::Text btnAttackText;

    // layout
    float leftX = 120.f, rightX = 900.f;
    float baselineY = 460.f;
    float nameOffsetY = -40.f;
    sf::Vector2f myHPPos  = {120.f, 520.f};
    sf::Vector2f enHPPos  = {900.f, 120.f};
    sf::Vector2f hpSize   = {220.f, 18.f};

    // helpers
    void rebuildVisuals();
    void setMessage(const std::string& s);
    void nextAliveIndex(bool player, int& idx);
    bool isKO(const Pokemon& p) const;
    void doPlayerAttackRound(); 
};

#endif
