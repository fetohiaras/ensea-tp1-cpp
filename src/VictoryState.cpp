#include "VictoryState.hpp"
#include "Context.hpp"
#include "StateMachine.hpp"
#include "SFMLHelpers.hpp"

void VictoryState::onEnter() {
    //music
    if (ctx.sound) {
        ctx.musicRequest = "victory.ogg";    
        ctx.musicDirty = true;
    }
    auto sz = ctx.window->getSize();

    // win text
    if (ctx.font) {
        title.setFont(*ctx.font);
        title.setCharacterSize(48);
        title.setFillColor(sf::Color(220, 255, 220));
        title.setString("Victoire !");
        auto tb = title.getLocalBounds();
        title.setPosition((sz.x - tb.width)/2.f - tb.left, sz.y * 0.28f);

        // play again btn
        const float btnW = 300.f, btnH = 44.f;
        sf::Vector2f btnPos(
            (sz.x - btnW) / 2.f,
            title.getPosition().y + tb.height + 60.f // 60 px under title
        );
        makeButton(*ctx.font, "Jouer une autre fois", btnPos, {btnW, btnH}, btnReplayRect, btnReplayText);

        // esc to quit
        escHint.setFont(*ctx.font);
        escHint.setCharacterSize(18);
        escHint.setFillColor(sf::Color(200, 210, 220));
        escHint.setString("Appuyez sur ESC pour quitter");
        auto hb = escHint.getLocalBounds();
        escHint.setPosition((sz.x - hb.width)/2.f - hb.left, btnPos.y + btnH + 28.f);
    }
}

void VictoryState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        if (ctx.window) ctx.window->close();
        return;
    }
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
        if (hitButton(btnReplayRect, mp)) {
            if (ctx.machine) ctx.machine->changeState(StateID::TeamSelect);
        }
    }
}

void VictoryState::render(sf::RenderWindow& window) {
    if (title.getFont()) window.draw(title);
    window.draw(btnReplayRect);
    window.draw(btnReplayText);
    if (escHint.getFont()) window.draw(escHint);
}


