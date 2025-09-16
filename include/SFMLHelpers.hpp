#ifndef SFMLHELPERS_HPP
#define SFMLHELPERS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "PokemonParty.hpp"

// loads fonts from folder
bool loadFont(sf::Font& font, const std::string& path = "fonts/Helvetica Black Condensed.ttf");

// load poke texture by id
bool loadTextureById(sf::Texture& tex, int id);

// grid
void buildGridSlots(
    int cols, int rows,
    float cellW, float cellH,
    float gapX, float gapY,
    float startX, float startY,
    std::vector<sf::RectangleShape>& outSlots
);

// prints sprites and names from the pokes in the party
void buildPartySpritesAndLabels(
    const PokemonParty& party,
    int cols, int rows,
    float cellW, float cellH,
    float gapX, float gapY,
    float startX, float startY,
    float pad,
    const sf::Font& font,
    std::vector<sf::Texture>& textures,
    std::vector<sf::Sprite>& sprites,
    std::vector<sf::Text>& names
);

// creates a title
sf::Text makeTitle(const sf::Font& font, const std::string& text, float x, float y);

#endif 
