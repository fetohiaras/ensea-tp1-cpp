#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include "Pokemon.hpp"
#include "Pokedex.hpp"
#include "PokemonParty.hpp"
#include "PokemonAttack.hpp"
#include "SFMLHelpers.hpp"

using namespace std;

int main() {
    // create pokes
    Pokedex& dex = Pokedex::instance("pokedex.csv", false);

    PokemonParty myParty;
    myParty.addFromDexByName("Bulbasaur");
    myParty.addFromDexByName("Charmander");
    myParty.addFromDexById(7); //squirtle
    myParty.addFromDexById(255); //torchic

    PokemonAttack attack;

    // open window
    sf::RenderWindow window(sf::VideoMode(1440, 1000), "Sélection d'équipe - Tapez pour sélectionner, <-/-> pour changer position");
    sf::Font font;
    loadFont(font);

    // poke party coords
    const int P_COLS = 6, P_ROWS = 5;               // party
    const int A_COLS = 3, A_ROWS = 2;               // attack 
    const float CELL_W = 96.f, CELL_H = 120.f;
    const float GAP_X  = 16.f, GAP_Y  = 40.f;
    const float PAD    = 8.f;
    const float P_START_X = 40.f, P_START_Y = 80.f;

    // attack party coords
    const float A_START_X = P_START_X + P_COLS * (CELL_W + GAP_X) + 120.f;
    const float A_START_Y = P_START_Y;

    // poke select panel coords
    const float PANEL_X = A_START_X;
    const float PANEL_Y = A_START_Y + A_ROWS * (CELL_H + GAP_Y) + 30.f;
    const float PANEL_W = 480.f;  
    const float PANEL_H = 260.f;

    // render grids, sprites and names
    std::vector<sf::RectangleShape> partySlots, attackSlots;
    buildGridSlots(P_COLS, P_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, P_START_X, P_START_Y, partySlots);
    buildGridSlots(A_COLS, A_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, A_START_X, A_START_Y, attackSlots);

    std::vector<sf::Texture> pTextures, aTextures;
    std::vector<sf::Sprite>  pSprites,  aSprites;
    std::vector<sf::Text>    pNames,    aNames;

    auto rebuildViews = [&]() {
        buildSpritesAndLabels(myParty,  P_COLS, P_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, P_START_X, P_START_Y, PAD, font,
                              pTextures, pSprites, pNames);

        buildSpritesAndLabels(attack, A_COLS, A_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, A_START_X, A_START_Y, PAD, font,
                              aTextures, aSprites, aNames);
    };
    rebuildViews();


    sf::Text titleParty  = makeTitle(font, "Votre Pokemon Party (max. 30)", P_START_X, 15.f);
    sf::Text titleAttack = makeTitle(font, "Pokemon Attack (max. 6) — cliquez pour ajouter/retirer", A_START_X, P_START_Y - 40.f);
    
    // poke attackparty selection panel
    int selectedAttackIdx = -1;
    sf::Texture selTexture; sf::Sprite selSprite; sf::Text selInfo;

    // buttons
    sf::RectangleShape btnReturnRect, btnConfirmRect;
    sf::Text btnReturnText, btnConfirmText;

    makeButton(
        font, "Retirer -> Party (DEL/Backspace)",
        {PANEL_X, PANEL_Y + PANEL_H + 12.f}, {PANEL_W * 0.65f, 38.f},
        btnReturnRect, btnReturnText
    );

    makeButton(
        font, "Confirmer l'equipe",
        {PANEL_X + PANEL_W * 0.70f, PANEL_Y + PANEL_H + 12.f}, {PANEL_W * 0.28f, 38.f},
        btnConfirmRect, btnConfirmText
    );
    
    auto rebuildSelectionPanel = [&]() {
    if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < attack.size()) {
        const Pokemon& p = attack.at(static_cast<std::size_t>(selectedAttackIdx));
        buildSingleCard(p, PANEL_X, PANEL_Y, PANEL_W, PANEL_H, 10.f, font,
                        selTexture, selSprite, selInfo);
    } else {
        selSprite = sf::Sprite();
        selInfo = sf::Text();
    }
};

    // loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // quick close
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q)
                    window.close();

                // use arrow keys to change positions in attack
                if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < attack.size()) {
                    int cur = selectedAttackIdx;
                    int curR = cur / A_COLS;
                    int curC = cur % A_COLS;
                    int tgt = -1;

                    if (event.key.code == sf::Keyboard::Left) {
                        if (curC > 0) tgt = curR * A_COLS + (curC - 1);
                    } else if (event.key.code == sf::Keyboard::Right) {
                        if (curC + 1 < A_COLS) tgt = curR * A_COLS + (curC + 1);
                    } else if (event.key.code == sf::Keyboard::Up) {
                        if (curR > 0) tgt = (curR - 1) * A_COLS + curC;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        if (curR + 1 < A_ROWS) tgt = (curR + 1) * A_COLS + curC;
                    } else if (event.key.code == sf::Keyboard::BackSpace ||
                               event.key.code == sf::Keyboard::Delete) {
                        // return to party
                        if (attack.returnToParty(myParty, static_cast<std::size_t>(cur))) {
                            selectedAttackIdx = -1;
                            rebuildViews();
                            rebuildSelectionPanel();
                        }
                    }

                    if (tgt >= 0 && static_cast<std::size_t>(tgt) < attack.size()) {
                        // simple swap (referencing via at())
                        std::swap(attack.at(static_cast<std::size_t>(cur)),
                                  attack.at(static_cast<std::size_t>(tgt)));
                        selectedAttackIdx = tgt; // follws selection
                        rebuildViews();
                        rebuildSelectionPanel();
                    }
                }
            }

            // mouse click either selects or moves from party to attackparty
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                                static_cast<float>(event.mouseButton.y));

                // buttons
                if (hitButton(btnReturnRect, mp)) {
                    if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < attack.size()) {
                        if (attack.returnToParty(myParty, static_cast<std::size_t>(selectedAttackIdx))) {
                            selectedAttackIdx = -1;
                            rebuildViews();
                            rebuildSelectionPanel();
                        }
                    }
                    continue;
                }
                if (hitButton(btnConfirmRect, mp)) {
                    std::cout << "[OK] Equipe confirmée avec " << attack.size() << " Pokemons.\n";
                    // will transition to battle screen
                    continue;
                }

                // if clicked on party, add to attackparty
                int pIdx = gridIndexAt(P_COLS, P_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, P_START_X, P_START_Y, mp);
                if (pIdx >= 0 && static_cast<std::size_t>(pIdx) < myParty.size()) {
                    if (attack.addFromParty(myParty, static_cast<std::size_t>(pIdx))) {
                        // if added and attack was empty, select last
                        selectedAttackIdx = static_cast<int>(attack.size()) - 1;
                        rebuildViews();
                        rebuildSelectionPanel();
                    }
                    continue;
                }

                // click on attack
                int aIdx = gridIndexAt(A_COLS, A_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, A_START_X, A_START_Y, mp);
                if (aIdx >= 0) {
                    // if position is occupied, select
                    if (static_cast<std::size_t>(aIdx) < attack.size()) {
                        selectedAttackIdx = aIdx;
                        rebuildSelectionPanel();
                    } else {
                        // nothing yet
                    }
                    continue;
                }
            }
        }

        // render window
        window.clear(sf::Color(30, 30, 40));

        if (font.getInfo().family != "") { window.draw(titleParty); window.draw(titleAttack); }

        // party
        for (auto& s : partySlots) window.draw(s);
        for (std::size_t i = 0; i < pNames.size(); ++i) {
            window.draw(pNames[i]);
            if (pSprites[i].getTexture() != NULL) window.draw(pSprites[i]);
        }

        // attackparty, border on selected poke
        for (std::size_t i = 0; i < attackSlots.size(); ++i) {
            if (static_cast<int>(i) == selectedAttackIdx) {
                auto r = attackSlots[i];
                r.setOutlineThickness(4.f);
                r.setOutlineColor(sf::Color(255, 220, 90));
                window.draw(r);
            } else {
                window.draw(attackSlots[i]);
            }
        }
        for (std::size_t i = 0; i < aNames.size(); ++i) {
            window.draw(aNames[i]);
            if (aSprites[i].getTexture() != NULL) window.draw(aSprites[i]);
        }

        // selection panel, selected poke, infos and buttons
        if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < attack.size()) {
            // frame
            sf::RectangleShape panel(sf::Vector2f(PANEL_W, PANEL_H));
            panel.setPosition(PANEL_X, PANEL_Y);
            panel.setFillColor(sf::Color(50, 60, 80));
            panel.setOutlineThickness(2.f);
            panel.setOutlineColor(sf::Color(180, 180, 200));
            window.draw(panel);

            if (selTexture.getSize().x > 0) window.draw(selSprite);
            if (font.getInfo().family != "") window.draw(selInfo);
        }
        window.draw(btnReturnRect); window.draw(btnReturnText);
        window.draw(btnConfirmRect); window.draw(btnConfirmText);

        window.display();
    }

    return 0;

    /*myParty.displayAll();

    myParty.at(1).launch_attack(myParty.at(0));
    myParty.at(0).displayInfo();

    myParty.removeAt(2);
    myParty.displayAll();*/

    return 0;
}



