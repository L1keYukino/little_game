#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "Scene.hpp"

// 游戏主类 —— 负责主循环和场景管理
//
// 职责：
// 1. 创建和管理 SFML 窗口
// 2. 运行游戏主循环（输入→更新→渲染）
// 3. 管理场景栈（切换场景、压入弹出场景）
//
// 场景栈是什么？
// 比如打开背包时，背包 UI（暂停场景）压到农场场景上面
// 关闭背包时弹出，恢复农场
// 这里用 std::stack 存储场景指针

class Game
{
public:
    Game();
    ~Game();

    // 运行游戏（阻塞，直到窗口关闭）
    void run();

    // 切换场景（替换当前场景）
    void changeScene(std::unique_ptr<Scene> newScene);

    // 在场景栈上压入新场景（比如打开菜单）
    void pushScene(std::unique_ptr<Scene> scene);

    // 弹出栈顶场景
    void popScene();

    sf::RenderWindow& getWindow() { return m_window; }

private:
    void handleInput();
    void update(float dt);
    void render();

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // 场景栈：栈顶是当前活跃场景
    // 用 unique_ptr 确保场景对象自动释放
    std::stack<std::unique_ptr<Scene>> m_scenes;
};
