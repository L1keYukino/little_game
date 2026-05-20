#pragma once
#include "../core/Scene.hpp"
#include "../entities/Player.hpp"
#include "../world/TileMap.hpp"
#include "../core/Camera.hpp"

// 农场场景 —— 游戏的核心场景
//
// 这个类"拥有"玩家、地图、摄像机，并在每帧协调它们：
//   update:  玩家移动 → 碰撞检测 → 摄像机跟随
//   render:  摄像机应用到窗口 → 画地图 → 画玩家
//
// 注意：FarmScene 不直接继承 sf::Drawable，
// 而是通过 render(window) 方法，因为 Scene 基类定义了这个接口

class FarmScene : public Scene
{
public:
    FarmScene(sf::RenderWindow& window);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    Player m_player;
    TileMap m_map;
    Camera m_camera;
};
