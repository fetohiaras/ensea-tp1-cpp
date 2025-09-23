#include "BattleState.hpp"
#include "Context.hpp"
#include "StateMachine.hpp"
#include "SFMLHelpers.hpp"
#include "PokemonParty.hpp"
#include "PokemonAttack.hpp"
#include "Pokemon.hpp"
#include <sstream>

BattleState::BattleState(Context& ctx) : State(ctx) {}

void BattleState::onEnter() {
    // get first poke id
    myIdx = 0;
    enIdx = 0;

    // index as next poke alive
    nextAliveIndex(true,  myIdx);
    nextAliveIndex(false, enIdx);

    // msgbox
    msgBox = sf::RectangleShape({ static_cast<float>(ctx.window->getSize().x) - 80.f, 180.f });
    msgBox.setPosition(40.f, ctx.window->getSize().y - 200.f);
    msgBox.setFillColor(sf::Color(50, 60, 80));
    msgBox.setOutlineThickness(2.f);
    msgBox.setOutlineColor(sf::Color(180, 180, 200));

    msgText = sf::Text();
    if (ctx.font && ctx.font->getInfo().family != "") {
        msgText.setFont(*ctx.font);
        msgText.setCharacterSize(20);
        msgText.setFillColor(sf::Color::White);
        msgText.setPosition(msgBox.getPosition().x + 16.f, msgBox.getPosition().y + 14.f);
    }

    makeButton(*ctx.font, "Attaquer", { msgBox.getPosition().x + msgBox.getSize().x - 140.f, msgBox.getPosition().y + msgBox.getSize().y - 50.f },
               {120.f, 36.f}, btnAttackRect, btnAttackText);

    rebuildVisuals();

    // start message
    if (ctx.attack->size() == 0) {
        setMessage("Votre equipe est vide! Retour à l'écran de sélection...");
        if (ctx.machine) ctx.machine->changeState(StateID::TeamSelect);
        return;
    }
    if (ctx.enemy->size() == 0) {
        setMessage("L'equipe enemie est vide! Victoire!");
        if (ctx.machine) ctx.machine->changeState(StateID::Victory);
        return;
    }

    std::ostringstream oss;
    oss << ctx.attack->at(myIdx).getName() << " s'est entré à la bataille!\n"
        << ctx.enemy->at(enIdx).getName()  << " s'est apparu!";
    setMessage(oss.str());
}

void BattleState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Escape && ctx.window) ctx.window->close();
        if (ev.key.code == sf::Keyboard::Enter && ctx.machine) doPlayerAttackRound();
    }
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
        if (hitButton(btnAttackRect, mp)) {
            doPlayerAttackRound();
        }
    }
}

void BattleState::update(float) {}

void BattleState::render(sf::RenderWindow& window) {
    // names
    if (myName.getFont()) window.draw(myName);
    if (enName.getFont()) window.draw(enName);

    // sprites
    if (mySpr.getTexture()) window.draw(mySpr);
    if (enSpr.getTexture()) window.draw(enSpr);

    // HP bar
    const Pokemon& me = ctx.attack->at(myIdx);
    const Pokemon& en = ctx.enemy->at(enIdx);
    drawHPBar(window, myHPPos, hpSize, (float)me.getCurrHitPoint(), (float)me.getMaxHitPoint());
    drawHPBar(window, enHPPos, hpSize, (float)en.getCurrHitPoint(), (float)en.getMaxHitPoint());

    // msgbox + button
    window.draw(msgBox);
    if (msgText.getFont()) window.draw(msgText);
    window.draw(btnAttackRect); window.draw(btnAttackText);
}

// ---------- helpers ----------
void BattleState::rebuildVisuals() {
    // player on left
    const Pokemon& me = ctx.attack->at(myIdx);
    myTex = sf::Texture();
    loadTextureById(myTex, me.getId());
    mySpr = sf::Sprite();
    if (myTex.getSize().x > 0) {
        mySpr.setTexture(myTex, true);
        auto lb = mySpr.getLocalBounds();
        float scale = 280.f / std::max(lb.width, lb.height);
        mySpr.setScale(scale, scale);
        auto gb = mySpr.getGlobalBounds();
        mySpr.setPosition(leftX, baselineY - gb.height);
    }
    myName = sf::Text();
    if (ctx.font && ctx.font->getInfo().family != "") {
        myName.setFont(*ctx.font);
        myName.setCharacterSize(22);
        myName.setFillColor(sf::Color::White);
        myName.setString(me.getName());
        auto nb = myName.getLocalBounds();
        myName.setPosition(leftX, baselineY - (mySpr.getGlobalBounds().height) + nameOffsetY);
    }

    // enemy on right
    const Pokemon& en = ctx.enemy->at(enIdx);
    enTex = sf::Texture();
    loadTextureById(enTex, en.getId());
    enSpr = sf::Sprite();
    if (enTex.getSize().x > 0) {
        enSpr.setTexture(enTex, true);
        auto lb = enSpr.getLocalBounds();
        float scale = 280.f / std::max(lb.width, lb.height);
        enSpr.setScale(scale, scale);
        auto gb = enSpr.getGlobalBounds();
        enSpr.setPosition(rightX, 200.f);

        // hp bar under sprite
        auto enGB = enSpr.getGlobalBounds();
        enHPPos = { enGB.left, enGB.top + enGB.height + 8.f };
    }
    enName = sf::Text();
    if (ctx.font && ctx.font->getInfo().family != "") {
        enName.setFont(*ctx.font);
        enName.setCharacterSize(22);
        enName.setFillColor(sf::Color(255,210,210));
        enName.setString(en.getName());
        enName.setPosition(enSpr.getGlobalBounds().left, enSpr.getGlobalBounds().top - 34.f);
    }
}

void BattleState::setMessage(const std::string& s) {
    if (msgText.getFont()) msgText.setString(s);
}

bool BattleState::isKO(const Pokemon& p) const {
    return p.getCurrHitPoint() <= 0.0;
}

void BattleState::nextAliveIndex(bool player, int& idx) {
    auto& team = player ? static_cast<PokemonVector&>(*ctx.attack)
                        : static_cast<PokemonVector&>(*ctx.enemy);
    int n = static_cast<int>(team.size());
    while (idx < n) {
        if (team.at(static_cast<std::size_t>(idx)).getCurrHitPoint() > 0.0) break;
        ++idx;
    }
}

void BattleState::doPlayerAttackRound() {
    auto& me = ctx.attack->at(static_cast<std::size_t>(myIdx));
    auto& en = ctx.enemy->at(static_cast<std::size_t>(enIdx));

    std::ostringstream oss;

    // player attack
    oss << me.getName() << " attaque!\n";
    me.launch_attack(en);

    // check if enemy is ko
    if (isKO(en)) {
        oss << en.getName() << " s'est tombé!\n";
        // goto next poke
        ++enIdx;
        nextAliveIndex(false, enIdx);
        if (enIdx >= static_cast<int>(ctx.enemy->size())) {
            // victory
            setMessage(oss.str() + "\nVous avez gagné!");
            if (ctx.machine) ctx.machine->changeState(StateID::Victory);
            return;
        } else {
            oss << "L'ennemi a envoyé " << ctx.enemy->at(static_cast<std::size_t>(enIdx)).getName() << "!";
        }
        setMessage(oss.str());
        rebuildVisuals();
        return; //no counterattack if fainted
    }

    // enemy conterattacks
    oss << en.getName() << " a lancé son contre-attaque!\n";
    en.launch_attack(me);
    if (isKO(me)) {
        oss << me.getName() << " s'est tombé!\n";
        // advances player team
        ++myIdx;
        nextAliveIndex(true, myIdx);
        if (myIdx >= static_cast<int>(ctx.attack->size())) {
            // transition to continuestate on defeat
            setMessage(oss.str() + "\nVous etes vaincu...");
            if (ctx.machine) ctx.machine->changeState(StateID::Continue);
            return;
        } else {
            oss << "Vous envoyez " << ctx.attack->at(static_cast<std::size_t>(myIdx)).getName() << "!";
        }
    }

    setMessage(oss.str());
    rebuildVisuals();
}
