#include "TeamSelectState.hpp"
#include "Context.hpp"
#include "StateMachine.hpp"
#include "SFMLHelpers.hpp"
#include "PokemonParty.hpp"
#include "PokemonAttack.hpp"
#include <algorithm>
#include <iostream>

TeamSelectState::TeamSelectState(Context& ctx) : State(ctx) {}

void TeamSelectState::onEnter() {
    // calculate dependent positions
    A_START_X = P_START_X + P_COLS * (CELL_W + GAP_X) + 120.f;
    A_START_Y = P_START_Y;

    PANEL_X = A_START_X;
    PANEL_Y = A_START_Y + A_ROWS * (CELL_H + GAP_Y) + 30.f;

    // grids
    buildGridSlots(P_COLS, P_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, P_START_X, P_START_Y, partySlots);
    buildGridSlots(A_COLS, A_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, A_START_X, A_START_Y, attackSlots);

    // titels
    buildUITexts();

    // separator
    separator = sf::RectangleShape(sf::Vector2f(2.f, (ctx.window ? ctx.window->getSize().y - 100.f : 600.f)));
    separator.setPosition(A_START_X - 60.f, 50.f);
    separator.setFillColor(sf::Color(90, 100, 120));

    // buttons
    makeButton(*ctx.font, "Retirer -> Party (DEL/Backspace)",
        {PANEL_X, PANEL_Y + PANEL_H + 12.f}, {PANEL_W * 0.65f, 38.f},
        btnReturnRect, btnReturnText);

    makeButton(*ctx.font, "Confirmer",
        {PANEL_X + PANEL_W * 0.70f, PANEL_Y + PANEL_H + 12.f}, {PANEL_W * 0.28f, 38.f},
        btnConfirmRect, btnConfirmText);

    // build starting content
    rebuildViews();
    rebuildSelectionPanel();
}

void TeamSelectState::buildUITexts() {
    if (ctx.font && ctx.font->getInfo().family != "") {
        titleParty.setFont(*ctx.font);
        titleParty.setCharacterSize(24);
        titleParty.setFillColor(sf::Color(200,220,255));
        titleParty.setString("Votre Pokemon Box (max. 30)");
        titleParty.setPosition(P_START_X, 15.f);

        titleAttack.setFont(*ctx.font);
        titleAttack.setCharacterSize(24);
        titleAttack.setFillColor(sf::Color(200,220,255));
        titleAttack.setString("Votre Party (max. 6)");
        titleAttack.setPosition(A_START_X, P_START_Y - 40.f);
    }
}

void TeamSelectState::rebuildViews() {
    buildSpritesAndLabels(*ctx.party, P_COLS, P_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, P_START_X, P_START_Y, PAD,
                          *ctx.font, pTextures, pSprites, pNames);

    buildSpritesAndLabels(*ctx.attack, A_COLS, A_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, A_START_X, A_START_Y, PAD,
                          *ctx.font, aTextures, aSprites, aNames);
}

void TeamSelectState::rebuildSelectionPanel() {
    if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < ctx.attack->size()) {
        const Pokemon& p = ctx.attack->at(static_cast<std::size_t>(selectedAttackIdx));
        buildSingleCard(p, PANEL_X, PANEL_Y, PANEL_W, PANEL_H, 10.f, *ctx.font,
                        selTexture, selSprite, selInfo);
    } else {
        selSprite = sf::Sprite();
        selInfo   = sf::Text();
    }
}

void TeamSelectState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Escape) {
            if (ctx.window) ctx.window->close();
        }

        // change positions in AttackParty using arrow keys
        if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < ctx.attack->size()) {
            int cur = selectedAttackIdx;
            int curR = cur / A_COLS;
            int curC = cur % A_COLS;
            int tgt = -1;

            if (ev.key.code == sf::Keyboard::Left)  { if (curC > 0)          tgt = curR * A_COLS + (curC - 1); }
            if (ev.key.code == sf::Keyboard::Right) { if (curC + 1 < A_COLS)  tgt = curR * A_COLS + (curC + 1); }
            if (ev.key.code == sf::Keyboard::Up)    { if (curR > 0)          tgt = (curR - 1) * A_COLS + curC; }
            if (ev.key.code == sf::Keyboard::Down)  { if (curR + 1 < A_ROWS)  tgt = (curR + 1) * A_COLS + curC; }

            if (tgt >= 0 && static_cast<std::size_t>(tgt) < ctx.attack->size()) {
                std::swap(ctx.attack->at(static_cast<std::size_t>(cur)),
                          ctx.attack->at(static_cast<std::size_t>(tgt)));
                selectedAttackIdx = tgt;
                rebuildViews();
                rebuildSelectionPanel();
            }

            // remove to Party
            if (ev.key.code == sf::Keyboard::BackSpace || ev.key.code == sf::Keyboard::Delete) {
                if (ctx.attack->returnToParty(*ctx.party, static_cast<std::size_t>(cur))) {
                    selectedAttackIdx = -1;
                    rebuildViews();
                    rebuildSelectionPanel();
                }
            }
        }
    }

    // mouse click
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp(static_cast<float>(ev.mouseButton.x),
                        static_cast<float>(ev.mouseButton.y));

        // buttons
        if (hitButton(btnReturnRect, mp)) {
            if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < ctx.attack->size()) {
                if (ctx.attack->returnToParty(*ctx.party, static_cast<std::size_t>(selectedAttackIdx))) {
                    selectedAttackIdx = -1;
                    rebuildViews();
                    rebuildSelectionPanel();
                }
            }
            return;
        }

        if (hitButton(btnConfirmRect, mp)) {
            // transition to pre combat screen
            if (ctx.machine) ctx.machine->changeState(StateID::PreBattle);
            return;
        }

        // click on party to add to AttackParty
        int pIdx = gridIndexAt(P_COLS, P_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, P_START_X, P_START_Y, mp);
        if (pIdx >= 0 && static_cast<std::size_t>(pIdx) < ctx.party->size()) {
            if (ctx.attack->addFromParty(*ctx.party, static_cast<std::size_t>(pIdx))) {
                selectedAttackIdx = static_cast<int>(ctx.attack->size()) - 1; // selects recently added poke
                rebuildViews();
                rebuildSelectionPanel();
            }
            return;
        }

        // click on AttackParty
        int aIdx = gridIndexAt(A_COLS, A_ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, A_START_X, A_START_Y, mp);
        if (aIdx >= 0) {
            if (static_cast<std::size_t>(aIdx) < ctx.attack->size()) {
                selectedAttackIdx = aIdx;
                rebuildSelectionPanel();
            }
            return;
        }
    }
}

void TeamSelectState::update(float) {
   
}

void TeamSelectState::render(sf::RenderWindow& window) {
    // bg, separators and titles
    if (titleParty.getFont())  window.draw(titleParty);
    if (titleAttack.getFont()) window.draw(titleAttack);
    window.draw(separator);

    // party
    for (auto& s : partySlots) window.draw(s);
    for (std::size_t i = 0; i < pNames.size(); ++i) {
        window.draw(pNames[i]);
        if (pSprites[i].getTexture() != NULL) window.draw(pSprites[i]);
    }

    // AttackParty with highlight on selected
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

    // selection panel with selected poke + infos
    if (selectedAttackIdx >= 0 && static_cast<std::size_t>(selectedAttackIdx) < ctx.attack->size()) {
        sf::RectangleShape panel(sf::Vector2f(PANEL_W, PANEL_H));
        panel.setPosition(PANEL_X, PANEL_Y);
        panel.setFillColor(sf::Color(50, 60, 80));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(sf::Color(180, 180, 200));
        window.draw(panel);

        if (selTexture.getSize().x > 0) window.draw(selSprite);
        if (selInfo.getFont()) window.draw(selInfo);
    }
    window.draw(btnReturnRect);  window.draw(btnReturnText);
    window.draw(btnConfirmRect); window.draw(btnConfirmText);
}
