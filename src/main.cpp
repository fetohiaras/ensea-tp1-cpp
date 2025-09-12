#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include "Pokemon.hpp"
#include "Pokedex.hpp"
#include "PokemonParty.hpp"

using namespace std;

static bool loadTextureById(sf::Texture& tex, int id) {
    string fn = "pokemon/" + to_string(id) + ".png";
    if (tex.loadFromFile(fn)) return true;
    return tex.loadFromFile("../" + fn);
}

static bool loadFont(sf::Font& font, const string& path = "fonts/Helvetica Black Condensed.ttf") {
    if (font.loadFromFile(path)) return true;
    return font.loadFromFile("../" + path);
}

// print poke info
static sf::Text makeInfoText(const Pokemon& p, const sf::Font& font) {
    ostringstream oss;
    oss << "ID: " << p.getId() << "\n"
        << "Nom: " << p.getName() << "\n"
        << "HP: " << p.getCurrHitPoint() << "/" << p.getMaxHitPoint() << "\n"
        << "Atk: " << p.getAttack() << "   Def: " << p.getDefense() << "\n"
        << "Evolution: " << p.getEvolution();

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(22);
    t.setFillColor(sf::Color::White);
    t.setString(oss.str());
    t.setPosition(380.f, 60.f);
    return t;
}

int main() {

    /*sf::RenderWindow window(sf::VideoMode(800, 600), "Hello SFML");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0; */
    // ==== SON AU DÉMARRAGE ====
    sf::SoundBuffer startupBuffer;
    sf::Sound startupSound;

    // Charger et jouer le son
    if (startupBuffer.loadFromFile("resources/1-01. Opening.mp3")) {
        startupSound.setBuffer(startupBuffer);
        startupSound.setVolume(70.0f);
        startupSound.play();
        std::cout << "Son de démarrage joué!" << std::endl;
    } else {
        std::cerr << "Erreur: Impossible de charger le son de démarrage" << std::endl;
    }

    Pokedex& dex = Pokedex::instance("pokedex.csv", false);

    // try adding by name and id
    PokemonParty myParty;
    myParty.addFromDexByName("Bulbasaur");
    myParty.addFromDexByName("Charmander");
    myParty.addFromDexById(7); // squirtle
    myParty.addFromDexById(255); // torchic

    sf::RenderWindow window(sf::VideoMode(800, 600), "Ma partie de Pokemons");

    //check for fonts
    sf::Font font;
    if (!loadFont(font)) {};

    int sel = 0; // shows first
    auto rebuildDrawable = [&](int idx, sf::Sprite& spr, sf::Text& info){
        const Pokemon& p = myParty.at(idx);

        // sprite
        sf::Texture* tex = new sf::Texture();              
        if (!loadTextureById(*tex, p.getId())) {
            // fallback: empty square if image not found
            delete tex;
            spr = sf::Sprite(); // empty sprite
        } else {
            spr.setTexture(*tex, true);
            // dimension to fit
            float scale = 5.0f; // adjust
            spr.setScale(scale, scale);
            spr.setPosition(60.f, 240.f);
        }

        // text
        if (font.getInfo().family != "") {
            info = makeInfoText(p, font);
        } else {
            info = sf::Text(); // no font, no text
        }
    };

    sf::Sprite sprite;
    sf::Text info;
    rebuildDrawable(sel, sprite, info);

    // loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) { // quit on esc or q
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Q) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Right) { // change poke based on keypress, left or right, ordered by id
                    if (myParty.size() > 0) {
                        sel = (sel + 1) % static_cast<int>(myParty.size());
                        rebuildDrawable(sel, sprite, info);
                    }
                } else if (event.key.code == sf::Keyboard::Left) {
                    if (myParty.size() > 0) {
                        sel = (sel - 1 + static_cast<int>(myParty.size())) % static_cast<int>(myParty.size());
                        rebuildDrawable(sel, sprite, info);
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 40));
        window.draw(sprite);
        if (font.getInfo().family != "") window.draw(info);
        window.display();
    }

    return 0;

    /*myParty.displayAll();

    myParty.at(1).launch_attack(myParty.at(0));
    myParty.at(0).displayInfo();

    myParty.removeAt(2);
    myParty.displayAll();

    return 0;*/
}

