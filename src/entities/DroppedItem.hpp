#pragma once
#include "Entity.hpp"
#include "Item.hpp"

// 地上掉落的物品 —— 从背包拖出时生成，会微微上下浮动
class DroppedItem : public Entity
{
public:
    DroppedItem(const sf::Vector2f& pos, ItemType type, int count);

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    ItemType itemType() const { return m_itemType; }
    int      count()    const { return m_count; }

    bool canPickup() const { return m_pickupTimer <= 0.f; }

private:
    ItemType m_itemType;
    int      m_count;
    float    m_bobTimer    = 0.f;
    float    m_pickupTimer = 1.5f; // 扔出后 1.5 秒内不可拾取

    sf::RectangleShape m_icon;
};
