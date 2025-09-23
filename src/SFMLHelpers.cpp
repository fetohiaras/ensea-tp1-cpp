#include "SFMLHelpers.hpp"
#include <sstream>
#include <algorithm>

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
) {
    const std::size_t TOTAL = static_cast<std::size_t>(cols * rows);
    const std::size_t count = std::min<std::size_t>(vec.size(), TOTAL);

    textures.clear(); sprites.clear(); names.clear();
    textures.reserve(TOTAL); sprites.reserve(TOTAL); names.reserve(TOTAL);

    for (std::size_t i = 0; i < count; ++i) {
        const Pokemon& p = vec.at(i);

        // cell pos and size
        int r = static_cast<int>(i) / cols;
        int c = static_cast<int>(i) % cols;
        sf::FloatRect cell(
            startX + c * (cellW + gapX),
            startY + r * (cellH + gapY),
            cellW, cellH
        );

        // name on top
        sf::Text nameText;
        if (font.getInfo().family != "") {
            nameText.setFont(font);
            nameText.setCharacterSize(14);
            nameText.setFillColor(sf::Color::White);
            nameText.setString(p.getName());
            sf::FloatRect tb = nameText.getLocalBounds();
            float tx = cell.left + (cell.width - tb.width) / 2.f - tb.left;
            float ty = cell.top + 2.f;
            nameText.setPosition(tx, ty);
        }
        names.push_back(nameText);

        // render texture and sprite with text on top
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
    }
}

int gridIndexAt(
    int cols, int rows,
    float cellW, float cellH,
    float gapX, float gapY,
    float startX, float startY,
    sf::Vector2f mousePos
) {
    // convert mouse click to grid coordinates
    float relX = mousePos.x - startX;
    float relY = mousePos.y - startY;
    if (relX < 0 || relY < 0) return -1;

    float strideX = cellW + gapX;
    float strideY = cellH + gapY;

    int col = static_cast<int>(relX / strideX);
    int row = static_cast<int>(relY / strideY);
    if (col < 0 || col >= cols || row < 0 || row >= rows) return -1;

    // check if click was inside cell grid and not on gap between text
    float offX = relX - col * strideX;
    float offY = relY - row * strideY;
    if (offX > cellW || offY > cellH) return -1;

    return row * cols + col;
}

void buildSingleCard(
    const Pokemon& p,
    float x, float y,
    float maxW, float maxH,
    float pad,
    const sf::Font& font,
    sf::Texture& outTexture,
    sf::Sprite& outSprite,
    sf::Text& outInfo
) {
    // sprite
    outTexture = sf::Texture(); // resets
    loadTextureById(outTexture, p.getId());
    outSprite  = sf::Sprite();

    // info
    outInfo = sf::Text();
    if (font.getInfo().family != "") {
        std::ostringstream oss;
        oss << p.getName() << " (ID " << p.getId() << ")\n"
            << "HP: " << p.getCurrHitPoint() << "/" << p.getMaxHitPoint() << "\n"
            << "Atk: " << p.getAttack() << "   Def: " << p.getDefense() << "\n"
            << "Evolution: " << p.getEvolution();
        outInfo.setFont(font);
        outInfo.setCharacterSize(18);
        outInfo.setFillColor(sf::Color::White);
        outInfo.setString(oss.str());
        outInfo.setPosition(x + pad, y + pad);
    }

    float textBlockH = (font.getInfo().family != "" ? outInfo.getLocalBounds().height + 10.f : 0.f);
    float imgAreaX = x + pad;
    float imgAreaY = y + pad + textBlockH;
    float imgAreaW = maxW - 2.f * pad;
    float imgAreaH = maxH - textBlockH - 2.f * pad;

    if (outTexture.getSize().x > 0) {
        outSprite.setTexture(outTexture, true);
        sf::FloatRect lb = outSprite.getLocalBounds();
        float sx = imgAreaW / lb.width;
        float sy = imgAreaH / lb.height;
        float scale = (sx < sy ? sx : sy);
        outSprite.setScale(scale, scale);

        sf::FloatRect gb = outSprite.getGlobalBounds();
        float cx = imgAreaX + (imgAreaW - gb.width) * 0.5f;
        float cy = imgAreaY + (imgAreaH - gb.height) * 0.5f;
        outSprite.setPosition(cx, cy);
    }
}

void makeButton(
    const sf::Font& font,
    const std::string& label,
    sf::Vector2f pos, sf::Vector2f size,
    sf::RectangleShape& outRect,
    sf::Text& outText
) {
    outRect = sf::RectangleShape(size);
    outRect.setPosition(pos);
    outRect.setFillColor(sf::Color(60, 70, 90));
    outRect.setOutlineThickness(2.f);
    outRect.setOutlineColor(sf::Color(180, 180, 200));

    outText = sf::Text();
    if (font.getInfo().family != "") {
        outText.setFont(font);
        outText.setCharacterSize(18);
        outText.setFillColor(sf::Color::White);
        outText.setString(label);
        sf::FloatRect tb = outText.getLocalBounds();
        float tx = pos.x + (size.x - tb.width) / 2.f - tb.left;
        float ty = pos.y + (size.y - tb.height) / 2.f - tb.top;
        outText.setPosition(tx, ty);
    }
}

bool hitButton(const sf::RectangleShape& rect, sf::Vector2f mouse) {
    return rect.getGlobalBounds().contains(mouse);
}

void drawHPBar(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f size, float currHP, float maxHP) {
    if (maxHP <= 0.f) maxHP = 1.f;
    if (currHP < 0.f) currHP = 0.f;
    if (currHP > maxHP) currHP = maxHP;

    sf::RectangleShape back(size);
    back.setPosition(pos);
    back.setFillColor(sf::Color(60, 60, 60));
    back.setOutlineThickness(2.f);
    back.setOutlineColor(sf::Color(180, 180, 200));
    window.draw(back);

    // change bar color according to hp status
    float ratio = currHP / maxHP;
    sf::RectangleShape bar({ size.x * ratio, size.y });
    bar.setPosition(pos);
    
    if (ratio > 0.5f) bar.setFillColor(sf::Color(80, 200, 120));
    else if (ratio > 0.25f) bar.setFillColor(sf::Color(220, 200, 60));
    else bar.setFillColor(sf::Color(220, 80, 60));
    window.draw(bar);
}
