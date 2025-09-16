#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include "Pokemon.hpp"
#include "Pokedex.hpp"
#include "PokemonParty.hpp"
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

    // open window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Votre partie de Pokemons");
    sf::Font font;
    loadFont(font);

    const int COLS = 6;
    const int ROWS = 5;
    const float CELL_W = 96.f;
    const float CELL_H = 120.f;
    const float GAP_X  = 16.f;
    const float GAP_Y  = 40.f;
    const float PAD    = 8.f;
    const float START_X = 40.f;
    const float START_Y = 80.f;

    // render grid, sprites and names
    vector<sf::RectangleShape> slots;
    buildGridSlots(COLS, ROWS, CELL_W, CELL_H, GAP_X, GAP_Y, START_X, START_Y, slots);

    vector<sf::Texture> textures;
    vector<sf::Sprite>  sprites;
    vector<sf::Text>    names;
    buildPartySpritesAndLabels(
        myParty,
        COLS, ROWS,
        CELL_W, CELL_H,
        GAP_X, GAP_Y,
        START_X, START_Y,
        PAD,
        font,
        textures, sprites, names
    );

    sf::Text title = makeTitle(font, "Votre partie de pokemons (max. 30)", START_X, 15.f);

    // window loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Q)      window.close();
            }
        }

        window.clear(sf::Color(30, 30, 40));
        if (font.getInfo().family != "") window.draw(title);

        for (size_t i = 0; i < slots.size(); ++i) window.draw(slots[i]);
        for (size_t i = 0; i < names.size(); ++i) {
            window.draw(names[i]);
            if (sprites[i].getTexture() != NULL) window.draw(sprites[i]);
        }

        window.display();
    }

    /*myParty.displayAll();

    myParty.at(1).launch_attack(myParty.at(0));
    myParty.at(0).displayInfo();

    myParty.removeAt(2);
    myParty.displayAll();*/

    return 0;
}



