#include "PreBattleState.hpp"
#include "Context.hpp"
#include "StateMachine.hpp"
#include "SFMLHelpers.hpp"
#include "Pokedex.hpp"
#include "Pokemon.hpp"
#include "PokemonAttack.hpp"
#include "PokemonParty.hpp"
#include <algorithm>
#include <iostream>

// reusing buildSpritesAndLabels with local vector
struct PreBattleState::EnemyAdapter : public PokemonVector {
    std::vector<Pokemon>* arr;
    explicit EnemyAdapter(std::vector<Pokemon>& v) : arr(&v) {}
    void add(const Pokemon& p) override { arr->push_back(p); }
    void removeAt(std::size_t i) override { if (i < arr->size()) arr->erase(arr->begin()+static_cast<long>(i)); }
    std::size_t size() const override { return arr->size(); }
    const Pokemon& at(std::size_t i) const override { return arr->at(i); }
    Pokemon& at(std::size_t i) override { return arr->at(i); }
    void displayAll() const override {} // currently not used
};

PreBattleState::PreBattleState(Context& ctx) : State(ctx) {}

void PreBattleState::onEnter() {
    // layout: player team, vs, enemy team
    const auto winW = ctx.window ? ctx.window->getSize().x : 1400u;
    const float gridWidth  = COLS * CELL_W + (COLS - 1) * GAP_X;
    const float gapBetween = 200.f;

    LEFT_X   = 120.f;
    RIGHT_X  = LEFT_X + gridWidth + gapBetween;
    CENTER_X = LEFT_X + gridWidth + gapBetween * 0.5f;

    // slots
    buildGridSlots(COLS, ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, LEFT_X, TOP_Y, mySlots);
    buildGridSlots(COLS, ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, RIGHT_X, TOP_Y, enemySlots);

    buildEnemyTeam();  // builds enemy team
    buildTitles();     // text: "Votre equipe" / "Adversaire"
    buildVS();         // VS sprite
    buildButtons();    // back/advance

    rebuildViews();    // sprites + names
}

void PreBattleState::buildEnemyTeam() {
    enemyData.clear();
    enemyData.reserve(static_cast<std::size_t>(ROWS * COLS));

    // Builds a team of 6 from the pokedex by id
    const int ids[] = { 52, 63, 92, 27, 129, 41 }; // Meowth, Abra, Gastly, Sandshrew, Magikarp, Zubat
    for (int id : ids) {
        Pokemon tmp(0, "", 0, 0, 0, 0, 0);
        if (ctx.pokedex && ctx.pokedex->cloneById(id, tmp)) {
            enemyData.push_back(tmp); // insert a valid poke
            if (enemyData.size() == static_cast<std::size_t>(ROWS * COLS)) break;
        }
    }
}

void PreBattleState::buildTitles() {
    if (ctx.font && ctx.font->getInfo().family != "") {
        titleMine.setFont(*ctx.font);
        titleMine.setCharacterSize(24);
        titleMine.setFillColor(sf::Color(200,220,255));
        titleMine.setString("Votre equipe (attaque)");
        titleMine.setPosition(LEFT_X, TOP_Y - 40.f);

        titleEnemy.setFont(*ctx.font);
        titleEnemy.setCharacterSize(24);
        titleEnemy.setFillColor(sf::Color(255,200,200));
        titleEnemy.setString("Equipe adverse");
        titleEnemy.setPosition(RIGHT_X, TOP_Y - 40.f);
    }
}

void PreBattleState::buildVS() {
    // tries to load vs sprite, if not use text
    vsTexture = sf::Texture();
    if (vsTexture.loadFromFile("assets/versusSmall.png") || vsTexture.loadFromFile("../assets/versusSmall.png")) {
        vsSprite = sf::Sprite(vsTexture);
        // scale to ~120px height
        sf::FloatRect lb = vsSprite.getLocalBounds();
        float scale = 120.f / lb.height;
        vsSprite.setScale(scale, scale);
        sf::FloatRect gb = vsSprite.getGlobalBounds();
        float x = CENTER_X - gb.width * 0.5f;
        float y = TOP_Y + (ROWS * (CELL_H + GAP_Y) - GAP_Y) * 0.5f - gb.height * 0.5f;
        vsSprite.setPosition(x, y);
        vsText = sf::Text(); // no text
    } else {
        if (ctx.font && ctx.font->getInfo().family != "") {
            vsText.setFont(*ctx.font);
            vsText.setCharacterSize(96);
            vsText.setFillColor(sf::Color(255, 215, 0));
            vsText.setString("VS");
            sf::FloatRect tb = vsText.getLocalBounds();
            float x = CENTER_X - tb.width*0.5f - tb.left;
            float y = TOP_Y + (ROWS * (CELL_H + GAP_Y) - GAP_Y) * 0.5f - tb.height*0.5f;
            vsText.setPosition(x, y);
        }
        vsSprite = sf::Sprite(); // empty sprite
    }
}

void PreBattleState::buildButtons() {
    // transition buttons
    const float btnW = 280.f, btnH = 40.f;
    const float margin = 40.f;
    float baseY = TOP_Y + ROWS * (CELL_H + GAP_Y) + 100.f;

    makeButton(*ctx.font, "Retourner a selection de l'equipe ", {LEFT_X, baseY}, {btnW, btnH}, btnBackRect, btnBackText);
    makeButton(*ctx.font, "Avancer au combat", {RIGHT_X + (COLS*CELL_W + (COLS-1)*GAP_X) - btnW, baseY}, {btnW, btnH}, btnGoRect, btnGoText);
}

void PreBattleState::rebuildViews() {
    // player team = ctx.attack
    buildSpritesAndLabels(*ctx.attack,
                          COLS, ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, LEFT_X, TOP_Y, PAD,
                          *ctx.font, myTextures, mySprites, myNames);

    // enemies via adapter
    EnemyAdapter adapter(enemyData);
    buildSpritesAndLabels(adapter,
                          COLS, ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, RIGHT_X, TOP_Y, PAD,
                          *ctx.font, enemyTextures, enemySprites, enemyNames);
}

void PreBattleState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Escape && ctx.window) ctx.window->close();
        if (ev.key.code == sf::Keyboard::Enter && ctx.machine) {
            // ENTER as advance hotkey
            ctx.machine->changeState(StateID::Battle);
        }
    }

    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp(static_cast<float>(ev.mouseButton.x),
                        static_cast<float>(ev.mouseButton.y));
        if (hitButton(btnBackRect, mp)) {
            if (ctx.machine) ctx.machine->changeState(StateID::TeamSelect);
            return;
        }
        if (hitButton(btnGoRect, mp)) {
            if (ctx.machine) ctx.machine->changeState(StateID::Battle);
            return;
        }
    }
}

void PreBattleState::update(float) {}

void PreBattleState::render(sf::RenderWindow& window) {
    // titles
    if (titleMine.getFont())  window.draw(titleMine);
    if (titleEnemy.getFont()) window.draw(titleEnemy);

    // slots
    for (auto& s : mySlots)    window.draw(s);
    for (auto& s : enemySlots) window.draw(s);

    // names and sprites
    for (std::size_t i = 0; i < myNames.size(); ++i) {
        window.draw(myNames[i]);
        if (mySprites[i].getTexture() != NULL) window.draw(mySprites[i]);
    }
    for (std::size_t i = 0; i < enemyNames.size(); ++i) {
        window.draw(enemyNames[i]);
        if (enemySprites[i].getTexture() != NULL) window.draw(enemySprites[i]);
    }

    // VS
    if (vsTexture.getSize().x > 0) window.draw(vsSprite);
    else if (vsText.getFont())     window.draw(vsText);

    // buttons
    window.draw(btnBackRect);  window.draw(btnBackText);
    window.draw(btnGoRect);    window.draw(btnGoText);
}
