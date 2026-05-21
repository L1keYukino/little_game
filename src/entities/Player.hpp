#pragma once
#include "Entity.hpp"

enum class Facing { Down, Up, Left, Right };

// 玩家角色 —— 继承 Entity，由 Game 全局持有，场景通过引用访问
class Player : public Entity
{
public:
    Player();

    void update(float dt) override;
    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    Facing getFacing() const { return m_facing; }

private:
    sf::RectangleShape m_shape;
    Facing m_facing = Facing::Down;
    float m_speed = 200.f;
};
