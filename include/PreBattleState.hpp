#ifndef PREBATTLESTATE_HPP
#define PREBATTLESTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class PreBattleState : public State {
public:
    explicit PreBattleState(Context& ctx);

    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    // layout, two grids for teams
    int   COLS = 3, ROWS = 2;
    float CELL_W = 96.f, CELL_H = 120.f;
    float GAP_X  = 16.f, GAP_Y = 40.f;
    float PAD    = 8.f;

    // areas
    float LEFT_X = 120.f, TOP_Y = 120.f;   // player team left
    float RIGHT_X = 0.f;                   // calculated from onEnter
    float CENTER_X = 0.f;                  // center for VS sprite

    // player team
    std::vector<sf::RectangleShape> mySlots;
    std::vector<sf::Texture> myTextures;
    std::vector<sf::Sprite>  mySprites;
    std::vector<sf::Text>    myNames;

    // enemy team (local vector)
    struct EnemyAdapter; // wrapper to reuse helpers
    std::vector<class Pokemon> enemyData;
    std::vector<sf::RectangleShape> enemySlots;
    std::vector<sf::Texture> enemyTextures;
    std::vector<sf::Sprite>  enemySprites;
    std::vector<sf::Text>    enemyNames;

    // titles and VS
    sf::Text titleMine, titleEnemy;
    sf::Texture vsTexture; sf::Sprite vsSprite;
    sf::Text vsText;

    // buttons
    sf::RectangleShape btnBackRect, btnGoRect;
    sf::Text btnBackText, btnGoText;

    // helpers
    void buildEnemyTeam(); // fills enemyData from pokedex
    void rebuildViews();
    void buildTitles();
    void buildVS();
    void buildButtons();
};

#endif
