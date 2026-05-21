#include "Crop.hpp"

Crop::Crop(int tileX, int tileY)
    : m_tileX(tileX), m_tileY(tileY)
{
    active = true;
}

void Crop::update(float dt)
{
    if (m_stage >= 2) return; // 已成熟，不再生长

    m_growthTimer += dt;
    if (m_growthTimer >= STAGE_DURATION)
    {
        m_growthTimer = 0.f;
        m_stage++;
    }
}

void Crop::render(sf::RenderWindow& window) const
{
    // 当前用纯色方块在格子上方表示，后续换精灵图
    // position 由 FarmScene 每帧根据 tile 坐标更新
    sf::RectangleShape shape(sf::Vector2f(16.f, 16.f));
    shape.setOrigin(sf::Vector2f(8.f, 8.f));
    shape.setPosition(position);

    switch (m_stage)
    {
    case 0: shape.setFillColor(sf::Color( 80, 200,  80)); break; // 种子：嫩绿
    case 1: shape.setFillColor(sf::Color(120, 220,  60)); break; // 幼苗：黄绿
    case 2: shape.setFillColor(sf::Color(255, 200,  50)); break; // 成熟：金黄
    default: shape.setFillColor(sf::Color::Magenta);     break;
    }

    window.draw(shape);
}
