#ifndef SFMLHELPERS_HPP
#define SFMLHELPERS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "PokemonVector.hpp" 

// loads fonts from folder
bool loadFont(sf::Font& font, const std::string& path = "fonts/pokemon-red-blue-green-yellow-edition-font.otf");

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
// generic for party and attackparty
void buildSpritesAndLabels(
    const PokemonVector& vec,
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

// gives index of grid cell clicked
int gridIndexAt(
    int cols, int rows,
    float cellW, float cellH,
    float gapX, float gapY,
    float startX, float startY,
    sf::Vector2f mousePos
);

// renders pokemon in detail for operations
void buildSingleCard(
    const Pokemon& p,
    float x, float y,            // upper left corner
    float maxW, float maxH,      // max area
    float pad,                   // padding 
    const sf::Font& font,
    sf::Texture& outTexture,
    sf::Sprite& outSprite,
    sf::Text& outInfo
);

// button
void makeButton(
    const sf::Font& font,
    const std::string& label,
    sf::Vector2f pos, sf::Vector2f size,
    sf::RectangleShape& outRect,
    sf::Text& outText
);

// button hittest 
bool hitButton(const sf::RectangleShape& rect, sf::Vector2f mouse);

// draw hp bar 
void drawHPBar(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f size,
               float currHP, float maxHP);

#endif 
