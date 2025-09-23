#include "ContinueState.hpp"
#include "Context.hpp"
#include "StateMachine.hpp"
#include "SFMLHelpers.hpp"

void ContinueState::onEnter() {
    auto sz = ctx.window->getSize();

    // title
    if (ctx.font) {
        title.setFont(*ctx.font);
        title.setCharacterSize(40);
        title.setFillColor(sf::Color(255, 220, 220));
        title.setString("Vous avez perdu. Continuer ?");
        const auto tb = title.getLocalBounds();
        title.setPosition((sz.x - tb.width) / 2.f - tb.left, sz.y * 0.28f);

        // exit text
        hint.setFont(*ctx.font);
        hint.setCharacterSize(18);
        hint.setFillColor(sf::Color(200, 210, 220));
        hint.setString("Appuyez sur ESC pour quitter");

        const float btnW = 260.f, btnH = 40.f;
        const float gap  = 28.f;
        const float y = title.getPosition().y + tb.height + 50.f;

        // yes button
        makeButton(*ctx.font,
                   "Oui (retour a la selection)",
                   { (sz.x / 2.f) - (btnW + gap/2.f), y },
                   { btnW, btnH },
                   btnYesRect, btnYesText);

        // no button
        makeButton(*ctx.font,
                   "Non (quitter)",
                   { (sz.x / 2.f) + (gap/2.f), y },
                   { btnW, btnH },
                   btnNoRect, btnNoText);

        const float hintY = y + btnH + 28.f;
        const auto hb = hint.getLocalBounds();
        hint.setPosition((sz.x - hb.width) / 2.f - hb.left, hintY);
    }
}

void ContinueState::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        if (ctx.window) ctx.window->close();
        return;
    }
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mp(static_cast<float>(ev.mouseButton.x),
                              static_cast<float>(ev.mouseButton.y));
        if (hitButton(btnYesRect, mp)) {
            if (ctx.machine) ctx.machine->changeState(StateID::TeamSelect);
        } else if (hitButton(btnNoRect, mp)) {
            if (ctx.window) ctx.window->close();
        }
    }
}

void ContinueState::render(sf::RenderWindow& window) {
    if (title.getFont()) window.draw(title);

    window.draw(btnYesRect);
    window.draw(btnYesText);
    window.draw(btnNoRect);
    window.draw(btnNoText);

    if (hint.getFont()) window.draw(hint);
}
