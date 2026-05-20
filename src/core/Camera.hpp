#pragma once
#include <SFML/Graphics.hpp>

// 2D 摄像机 —— 让画面跟随玩家移动
//
// 原理：
// 世界很大（比如 40x30 个格子，每个 64px → 2560x1920 的世界）
// 但屏幕只能显示 800x600
// 摄像机定义一个"视口"，只渲染世界的一部分
//
// SFML 的 sf::View 就是做这个的：
// - setCenter() 决定看哪里
// - setSize() 决定看多大
// 我们把 View 设置给 window，SFML 自动裁剪渲染范围
class Camera
{
public:
    Camera(float width, float height);

    // 让摄像机中心跟随目标位置（玩家坐标）
    // 会在 update 里每帧调用
    void follow(const sf::Vector2f& target, float dt);

    // 把当前 View 应用到 window（在 render 之前调用）
    void apply(sf::RenderWindow& window);

    // 获取当前摄像机视野范围（用于判断哪些东西需要渲染）
    sf::FloatRect getViewBounds() const;

    // 设置世界边界（地图的像素宽高）
    // 设置后摄像机不会移出这个范围，避免看到地图外面的黑边
    void setWorldBounds(float width, float height);

private:
    sf::View m_view;

    float m_followSpeed = 5.0f; // 平滑跟随速度
    float m_worldWidth  = 0.f;  // 地图像素宽度（0=不限制）
    float m_worldHeight = 0.f;  // 地图像素高度
};
