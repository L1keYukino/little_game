#pragma once
#include <SFML/Graphics.hpp>

// 四个面对方向
enum class Facing { Down, Up, Left, Right };

// 玩家角色
//
// 当前阶段用彩色方块表示，等有了精灵图再替换纹理
// 但动画和方向切换的逻辑已经写好了，届时只需加载纹理即可
class Player
{
public:
    Player();

    // 处理 WASD 移动输入（实时检测按键状态）
    // 注意：移动输入在 update 里处理，不在 handleInput
    // 因为我们要检测"按键是否按住"，而不是"按键是否按下了一次"
    void update(float dt);

    // 渲染玩家到窗口
    void render(sf::RenderWindow& window) const;

    // 获取玩家的碰撞矩形（用于和墙壁碰撞检测）
    sf::FloatRect getBounds() const;

    // 位置 getter/setter
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    // 获取玩家面对方向（互动时用来判断选择哪个格子）
    Facing getFacing() const { return m_facing; }

    // 移动玩家（由 FarmScene 在碰撞检测后调用）
    void move(const sf::Vector2f& offset);

private:
    // 当前用纯色方块表示玩家，之后换成 sf::Sprite + sf::Texture
    sf::RectangleShape m_shape;

    Facing m_facing = Facing::Down;
    float m_speed = 200.f; // 每秒移动像素数
};
