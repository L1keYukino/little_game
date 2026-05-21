#include "DroppedItem.hpp"
#include <cmath>

DroppedItem::DroppedItem(const sf::Vector2f& pos, ItemType type, int count)
    : m_itemType(type), m_count(count)
{
    position = pos;
    active = true;

    m_icon.setSize(sf::Vector2f(16.f, 16.f));
    m_icon.setOrigin(sf::Vector2f(8.f, 8.f));

    switch (type)
    {
    case ItemType::Hoe:         m_icon.setFillColor(sf::Color(150,150,150)); break;
    case ItemType::WateringCan: m_icon.setFillColor(sf::Color(80,130,200));  break;
    case ItemType::Seeds:       m_icon.setFillColor(sf::Color(200,180,80));  break;
    case ItemType::Turnip:      m_icon.setFillColor(sf::Color(255,100,100)); break;
    default:                    m_icon.setFillColor(sf::Color::White);       break;
    }
}

void DroppedItem::update(float dt)
{
    if (m_pickupTimer > 0.f) m_pickupTimer -= dt;

    m_bobTimer += dt * 5.f;
    float bob = std::sin(m_bobTimer) * 6.f;
    m_icon.setPosition(sf::Vector2f(position.x, position.y + bob));
}

void DroppedItem::render(sf::RenderWindow& window) const
{
    window.draw(m_icon);
}

sf::FloatRect DroppedItem::getBounds() const
{
    return m_icon.getGlobalBounds();
}
