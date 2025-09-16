#include "SFMLHelpers.hpp"
#include <sstream>

bool loadFont(sf::Font& font, const std::string& path) {
    if (font.loadFromFile(path)) return true;
    return font.loadFromFile("../" + path);
}

bool loadTextureById(sf::Texture& tex, int id) {
    std::string fn = "pokemon/" + std::to_string(id) + ".png";
    if (tex.loadFromFile(fn)) return true;
    return tex.loadFromFile("../" + fn);
}

void buildGridSlots(
    int cols, int rows,
    float cellW, float cellH,
    float gapX, float gapY,
    float startX, float startY,
    std::vector<sf::RectangleShape>& outSlots
) {
    outSlots.clear();
    outSlots.reserve(static_cast<std::size_t>(cols * rows));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            sf::RectangleShape rect(sf::Vector2f(cellW, cellH));
            rect.setPosition(
                startX + c * (cellW + gapX),
                startY + r * (cellH + gapY)
            );
            rect.setFillColor(sf::Color(0, 0, 0, 0));   // transparent
            rect.setOutlineThickness(2.f);
            rect.setOutlineColor(sf::Color(180, 180, 200));
            outSlots.push_back(rect);
        }
    }
}

sf::Text makeTitle(const sf::Font& font, const std::string& text, float x, float y) {
    sf::Text t;
    if (font.getInfo().family != "") {
        t.setFont(font);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color(200, 220, 255));
        t.setString(text);
        t.setPosition(x, y);
    }
    return t;
}

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
) {
    const std::size_t TOTAL = static_cast<std::size_t>(cols * rows);
    const std::size_t count = std::min<std::size_t>(party.size(), TOTAL);

    textures.clear();
    sprites.clear();
    names.clear();

    textures.reserve(TOTAL);
    sprites.reserve(TOTAL);
    names.reserve(TOTAL);

    for (std::size_t i = 0; i < count; ++i) {
        const Pokemon& p = party.at(i);

        // cell (pos + size)
        int r = static_cast<int>(i) / cols;
        int c = static_cast<int>(i) % cols;
        sf::FloatRect cell(
            startX + c * (cellW + gapX),
            startY + r * (cellH + gapY),
            cellW, cellH
        );

        // name on top of cell
        sf::Text nameText;
        if (font.getInfo().family != "") {
            nameText.setFont(font);
            nameText.setCharacterSize(14);
            nameText.setFillColor(sf::Color::White);
            nameText.setString(p.getName());

            sf::FloatRect tb = nameText.getLocalBounds();
            float tx = cell.left + (cell.width - tb.width) / 2.f - tb.left;
            float ty = cell.top + 2.f; // margin
            nameText.setPosition(tx, ty);
        }
        names.push_back(nameText);

        // ceneter texture and sprite under text
        textures.emplace_back();
        loadTextureById(textures.back(), p.getId());
        sprites.emplace_back();

        if (textures.back().getSize().x > 0) {
            sprites.back().setTexture(textures.back(), true);

            float textH = (font.getInfo().family != "" ? names.back().getLocalBounds().height + 6.f : 20.f);
            float maxW = cell.width  - 2.f * pad;
            float maxH = cell.height - textH - 2.f * pad;

            sf::FloatRect lb = sprites.back().getLocalBounds();
            float sx = maxW / lb.width;
            float sy = maxH / lb.height;
            float scale = (sx < sy ? sx : sy);
            sprites.back().setScale(scale, scale);

            sf::FloatRect gb = sprites.back().getGlobalBounds();
            float cx = cell.left + (cell.width - gb.width) * 0.5f;
            float cy = cell.top  + textH + (maxH - gb.height) * 0.5f;
            sprites.back().setPosition(cx, cy);
        }
        // if not loaded, empty sprite
    }
}
