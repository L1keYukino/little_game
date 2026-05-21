#pragma once
#include <SFML/Graphics.hpp>

// 游戏中所有"有位置、能更新、能渲染"的东西的基类
// Player、Crop、NPC、Monster 都继承它
// 好处：FarmScene 用 vector<Entity*> 统一遍历，加新类型不改循环
class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) const = 0;

    // 碰撞包围盒（玩家 vs 墙壁、子弹 vs 怪物等）
    virtual sf::FloatRect getBounds() const = 0;

    // 世界坐标
    sf::Vector2f position;

    // active=false 表示"已死/已收获/待清理"，场景在 update 后统一移除
    bool active = true;
};
