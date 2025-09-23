#include "IntroState.hpp"
#include "StateMachine.hpp"
#include "Game.hpp" 
#include "Context.hpp"
#include <string>

IntroState::IntroState(Context& ctx) : State(ctx) {}

void IntroState::onEnter() {
    if (ctx.font && ctx.font->getInfo().family != "") {
        title.setFont(*ctx.font);
        title.setCharacterSize(48);
        title.setFillColor(sf::Color(220, 230, 255));
        title.setString("Pokemon - ENSEA Edition");

        subtitle.setFont(*ctx.font);
        subtitle.setCharacterSize(24);
        subtitle.setFillColor(sf::Color(200, 210, 230));
        subtitle.setString("Appuyez ENTER ou cliquez pour avancer");
    }

    // centralize
    if (ctx.window) {
        auto sz = ctx.window->getSize();
        sf::FloatRect tb = title.getLocalBounds();
        title.setPosition( (sz.x - tb.width)/2.f - tb.left, sz.y * 0.35f );

        sf::FloatRect sb = subtitle.getLocalBounds();
        subtitle.setPosition( (sz.x - sb.width)/2.f - sb.left, title.getPosition().y + tb.height + 20.f );
    }
}

void IntroState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
        title.setFillColor(sf::Color(255, 255, 160)); // visual feedback 
        // signaled via character size and treated on update
        title.setCharacterSize(49);
    }
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        title.setFillColor(sf::Color(255, 255, 160));
        title.setCharacterSize(49);
    }
}

void IntroState::update(float) {
    // change state (if activated by flag)
    if (title.getCharacterSize() == 49 && ctx.machine) {
        ctx.machine->changeState(StateID::TeamSelect);
    }
}

void IntroState::render(sf::RenderWindow& window) {
    if (title.getFont()) window.draw(title);
    if (subtitle.getFont()) window.draw(subtitle);
}
