#include "Game.hpp"
#include <stdexcept>

namespace {
    // 每帧的默认底色（在场景画之前全屏填充）
    constexpr sf::Color CLEAR_COLOR(20, 20, 20);
}

Game::Game()
    // 创建 800x600 的窗口，标题 "Farm Game"
    : m_window(sf::RenderWindow(sf::VideoMode({800, 600}), "Farm Game"))
{
    // 限制帧率到 60 FPS —— 简单但有效的做法
    // 另一种方式是 vsync: m_window.setVerticalSyncEnabled(true)
    // setFramerateLimit 的好处是不依赖显示器刷新率
    m_window.setFramerateLimit(60);
}

Game::~Game()
{
    // 场景栈中的 unique_ptr 会自动清理，不需要手动 delete
    // 但要确保在窗口关闭前清理（不然 OpenGL 上下文可能已销毁）
    while (!m_scenes.empty())
        m_scenes.pop();
}

void Game::run()
{
    // ====== 游戏主循环 ======
    // 这是整个游戏的心脏，每帧执行一次
    // 经典三步骤：输入 → 更新 → 渲染

    while (m_window.isOpen())
    {
        // 1. 计算 deltaTime（距上一帧的秒数）
        //    乘以 dt 让所有运动与帧率无关
        //    比如 speed=200 表示每秒 200px，不管帧率是 30 还是 60
        float dt = m_clock.restart().asSeconds();

        // 2. 处理输入（事件队列）
        handleInput();

        // 3. 更新游戏逻辑
        update(dt);

        // 4. 渲染
        render();
    }
}

void Game::handleInput()
{
    // 事件轮询 —— 遍历窗口的事件队列
    // 每一帧可能有 0 个、1 个或多个事件（按键、鼠标、关闭窗口等）
    while (auto event = m_window.pollEvent())
    {
        // 关闭窗口按钮（点 × 或 Alt+F4）
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
            return;
        }

        // 把事件传递给当前场景处理
        if (!m_scenes.empty())
            m_scenes.top()->handleInput(*event);
    }
}

void Game::update(float dt)
{
    // 更新当前活跃场景（栈顶）
    if (!m_scenes.empty())
        m_scenes.top()->update(dt);
}

void Game::render()
{
    // 清空窗口（用深色背景，防止闪烁）
    m_window.clear(CLEAR_COLOR);

    // 让当前场景绘制自己
    if (!m_scenes.empty())
        m_scenes.top()->render(m_window);

    // 双缓冲：后台缓冲画好后，翻转到前台显示
    // SFML 的 display() 内部做了 swap buffers
    m_window.display();
}

void Game::changeScene(std::unique_ptr<Scene> newScene)
{
    // 替换场景：先弹出旧场景，压入新场景
    if (!m_scenes.empty())
        m_scenes.pop();
    m_scenes.push(std::move(newScene));
}

void Game::pushScene(std::unique_ptr<Scene> scene)
{
    m_scenes.push(std::move(scene));
}

void Game::popScene()
{
    if (!m_scenes.empty())
        m_scenes.pop();
}
