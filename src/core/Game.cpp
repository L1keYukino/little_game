#include "Game.hpp"

namespace {
    constexpr sf::Color CLEAR_COLOR(20, 20, 20);
}

Game::Game()
    : m_window(sf::RenderWindow(sf::VideoMode({1280, 720}), "Farm Game"))
    , m_inventory(1280.f, 720.f)
{
    m_window.setFramerateLimit(60);

    if (m_timeFont.openFromFile("C:/Windows/Fonts/simhei.ttf"))
        m_hasTimeFont = true;
}

Game::~Game()
{
    while (!m_scenes.empty())
        m_scenes.pop();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        handleInput();
        update(dt);
        render();
    }
}

// ====== 全局输入（与场景无关的通用快捷键） ======

void Game::handleGlobalInput(const sf::Event& event)
{
    // E 键切换背包
    if (auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::E)
        {
            if (m_inventory.isOpen()) m_inventory.close();
            else                     m_inventory.toggle();
        }
        if (m_inventory.isOpen() && key->code == sf::Keyboard::Key::Escape)
            m_inventory.close();
    }

    // 鼠标事件（仅背包打开时）
    bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
             || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl);

    if (auto* m = event.getIf<sf::Event::MouseButtonPressed>())
        if (m->button == sf::Mouse::Button::Left && m_inventory.isOpen())
            m_inventory.onMouseDown(ctrl, m->position);

    if (auto* m = event.getIf<sf::Event::MouseButtonReleased>())
        if (m->button == sf::Mouse::Button::Left && m_inventory.isOpen())
            m_inventory.onMouseUp(ctrl, m->position);

    // 滚轮切换物品
    if (auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        int slot = m_inventory.getSelectedSlot();
        if (wheel->delta > 0) slot--; else slot++;
        if (slot < 0) slot = 7;
        if (slot > 7) slot = 0;
        m_inventory.setSelectedSlot(slot);
    }

    // 拆分对话框文本输入
    if (auto* t = event.getIf<sf::Event::TextEntered>())
        if (m_inventory.isSplitDialogOpen())
            m_inventory.onTextEntered(t->unicode);
}

void Game::handleInput()
{
    while (auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
            return;
        }

        // 全局快捷键先处理（E、Esc、滚轮、Q、数字键、背包鼠标交互）
        handleGlobalInput(*event);

        // 背包打开时不把事件传给场景（游戏暂停）
        if (!m_inventory.isOpen() && !m_scenes.empty())
            m_scenes.top()->handleInput(*event);
    }
}

void Game::update(float dt)
{
    m_gameTime.update(dt);
    m_inventory.update(dt);

    // R 键显示物品名称（仅背包打开时生效）
    m_inventory.setShowTooltip(
        m_inventory.isOpen() &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)
    );

    // Q 键扔物品（背包开关都能用）
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        m_inventory.onThrowKey();

    // 数字键 1-8 切换物品（背包开关都能用）
    static const sf::Keyboard::Key numKeys[8] = {
        sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2,
        sf::Keyboard::Key::Num3, sf::Keyboard::Key::Num4,
        sf::Keyboard::Key::Num5, sf::Keyboard::Key::Num6,
        sf::Keyboard::Key::Num7, sf::Keyboard::Key::Num8,
    };
    for (int i = 0; i < 8; ++i)
        if (sf::Keyboard::isKeyPressed(numKeys[i]))
            m_inventory.setSelectedSlot(i);

    // 背包打开时不更新场景（游戏暂停）
    if (!m_inventory.isOpen() && !m_scenes.empty())
        m_scenes.top()->update(dt);
}

void Game::render()
{
    m_window.clear(CLEAR_COLOR);

    // 场景渲染
    if (!m_scenes.empty())
        m_scenes.top()->render(m_window);

    // 全局 HUD（背包、快捷栏）—— 用屏幕坐标，不受场景摄像机影响
    sf::View oldView = m_window.getView();
    m_window.setView(m_window.getDefaultView());

    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    m_inventory.updateHover(mousePos);
    m_inventory.render(m_window);

    // 右上角时间显示
    if (m_hasTimeFont)
    {
        std::string timeStr = m_gameTime.toString();
        sf::Text timeText(m_timeFont);
        timeText.setString(timeStr);
        timeText.setCharacterSize(22);
        timeText.setFillColor(sf::Color::White);

        sf::FloatRect tb = timeText.getLocalBounds();

        // 底色（暗色背景 + 半透明）
        sf::RectangleShape timeBg(sf::Vector2f(tb.size.x + 16.f, tb.size.y + 10.f));
        timeBg.setPosition(sf::Vector2f(1280.f - tb.size.x - 24.f, 6.f));
        timeBg.setFillColor(sf::Color(20, 20, 20, 200));
        timeBg.setOutlineColor(sf::Color(60, 60, 60, 180));
        timeBg.setOutlineThickness(1.f);
        m_window.draw(timeBg);

        timeText.setPosition(sf::Vector2f(1280.f - tb.size.x - 16.f, 8.f));
        m_window.draw(timeText);
    }

    m_window.setView(oldView);
    m_window.display();
}

void Game::changeScene(std::unique_ptr<Scene> newScene)
{
    if (!m_scenes.empty()) m_scenes.pop();
    m_scenes.push(std::move(newScene));
}

void Game::pushScene(std::unique_ptr<Scene> scene)
{
    m_scenes.push(std::move(scene));
}

void Game::popScene()
{
    if (!m_scenes.empty()) m_scenes.pop();
}
