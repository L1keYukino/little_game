#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "Scene.hpp"
#include "../ui/Inventory.hpp"

// 游戏主类 —— 主循环 + 场景管理 + 全局系统（背包）
class Game
{
public:
    Game();
    ~Game();

    void run();
    void changeScene(std::unique_ptr<Scene> newScene);
    void pushScene(std::unique_ptr<Scene> scene);
    void popScene();

    sf::RenderWindow& getWindow() { return m_window; }
    Inventory& getInventory()     { return m_inventory; }

private:
    void handleInput();
    void update(float dt);
    void render();

    // 处理与场景无关的全局输入（背包快捷键等）
    void handleGlobalInput(const sf::Event& event);

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    std::stack<std::unique_ptr<Scene>> m_scenes;

    // 背包是全局系统，任何场景都能用，不归属某一场景
    Inventory m_inventory;
};
