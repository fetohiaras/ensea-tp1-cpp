#ifndef TEAMSELECTSTATE_HPP
#define TEAMSELECTSTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class TeamSelectState : public State {
public:
    explicit TeamSelectState(Context& ctx);

    void onEnter() override;
    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    // layout
    int   P_COLS = 6, P_ROWS = 5;         // party grid
    int   A_COLS = 3, A_ROWS = 2;         // attack grid (6)
    float CELL_W = 96.f, CELL_H = 120.f;
    float GAP_X  = 16.f, GAP_Y = 40.f;
    float PAD    = 8.f;
    float P_START_X = 40.f, P_START_Y = 80.f;

    float A_START_X = 0.f, A_START_Y = 0.f; // calculated on onEnter

    // select panel
    float PANEL_X = 0.f, PANEL_Y = 0.f, PANEL_W = 480.f, PANEL_H = 260.f;

    // UI: slots, sprites, names
    std::vector<sf::RectangleShape> partySlots, attackSlots;
    std::vector<sf::Texture> pTextures, aTextures;
    std::vector<sf::Sprite>  pSprites,  aSprites;
    std::vector<sf::Text>    pNames,    aNames;

    // AttackParty selection
    int selectedAttackIdx = -1;
    sf::Texture selTexture; sf::Sprite selSprite; sf::Text selInfo;

    // titels
    sf::Text titleParty;
    sf::Text titleAttack;

    // buttons
    sf::RectangleShape btnReturnRect, btnConfirmRect;
    sf::Text btnReturnText, btnConfirmText;

    // local helpers
    void rebuildViews();
    void rebuildSelectionPanel();
    void buildUITexts();

    // ui separator
    sf::RectangleShape separator;
};

#endif
