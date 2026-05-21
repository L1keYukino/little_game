#pragma once
#include "Entity.hpp"

// 作物 —— 种在地里，自动生长，成熟后收获
// 生长阶段：种子(0) → 幼苗(1) → 成熟(2)
class Crop : public Entity
{
public:
    Crop(int tileX, int tileY);

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override { return {}; }

    int tileX() const { return m_tileX; }
    int tileY() const { return m_tileY; }
    int stage() const { return m_stage; }
    bool isMature() const { return m_stage >= 2; }

private:
    int m_tileX, m_tileY;
    int m_stage = 0;               // 0=种子, 1=幼苗, 2=成熟
    float m_growthTimer = 0.f;     // 当前阶段已过秒数
    static constexpr float STAGE_DURATION = 5.f; // 每阶段 5 秒
};
