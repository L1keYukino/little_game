#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "Scene.hpp"
#include "../entities/Player.hpp"
#include "../ui/Inventory.hpp"
#include "../world/GameTime.hpp"

// 游戏主类 —— 主循环 + 场景管理 + 全局系统（背包、玩家）
class Game
{
public:
    Game();
    ~Game();

    void run();
    void changeScene(std::unique_ptr<Scene> newScene);
    void pushScene(std::unique_ptr<Scene> scene);
    void popScene();

    sf::RenderWindow& getWindow()   { return m_window; }
    Player&   getPlayer()           { return m_player; }
    Inventory& getInventory()       { return m_inventory; }

private:
    void handleInput();
    void update(float dt);
    void render();
    void handleGlobalInput(const sf::Event& event);

    sf::RenderWindow m_window;
    sf::Clock        m_clock;
    std::stack<std::unique_ptr<Scene>> m_scenes;

    // 全局系统
    Player    m_player;
    Inventory m_inventory;
    GameTime  m_gameTime;

    // 时间显示字体
    sf::Font m_timeFont;
    bool     m_hasTimeFont = false;
};
