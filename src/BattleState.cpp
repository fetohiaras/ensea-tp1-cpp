#include "BattleState.hpp"
#include "Context.hpp"
#include "StateMachine.hpp"

BattleState::BattleState(Context& ctx) : State(ctx) {}

void BattleState::onEnter() {
    if (ctx.font && ctx.font->getInfo().family != "") {
        title.setFont(*ctx.font);
        title.setCharacterSize(36);
        title.setFillColor(sf::Color(220,230,255));
        title.setString("Battle");

        hint.setFont(*ctx.font);
        hint.setCharacterSize(20);
        hint.setFillColor(sf::Color(200,210,230));
        hint.setString("Appuyez ESC pour sortir ou ENTER pour retourner a la selection");
    }
    if (ctx.window) {
        auto sz = ctx.window->getSize();
        auto tb = title.getLocalBounds();
        title.setPosition((sz.x - tb.width)/2.f - tb.left, sz.y * 0.35f);
        auto hb = hint.getLocalBounds();
        hint.setPosition((sz.x - hb.width)/2.f - hb.left, title.getPosition().y + tb.height + 20.f);
    }
}

void BattleState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Escape && ctx.window) ctx.window->close();
        if (ev.key.code == sf::Keyboard::Enter && ctx.machine) ctx.machine->changeState(StateID::TeamSelect);
    }
}

void BattleState::update(float) {}
void BattleState::render(sf::RenderWindow& window) {
    if (title.getFont()) window.draw(title);
    if (hint.getFont())  window.draw(hint);
}
