#include "Player.hpp"
#include <cmath>

Player::Player()
{
    // 当前用绿色方块表示玩家，32x32 像素
    // 后续替换为 sf::Sprite + sf::Texture 加载精灵图
    m_shape.setSize(sf::Vector2f(32.f, 32.f));

    // 把方块的中心设为原点（默认原点在左上角）
    // 这样 position 就是方块中心，做碰撞检测更方便
    m_shape.setOrigin(sf::Vector2f(16.f, 16.f));

    m_shape.setFillColor(sf::Color::Green);

    // 初始位置放在地图中央（稍后由 FarmScene 设置正确位置）
    m_shape.setPosition(sf::Vector2f(400.f, 300.f));
}

void Player::update(float dt)
{
    // ====== 实时按键移动 ======
    // 用 sf::Keyboard::isKeyPressed 检测"按住"
    // 这和在 handleInput 里处理 sf::Event::KeyPressed 不同：
    //   - KeyPressed 事件：只在一瞬间触发（按下那一刻）
    //   - isKeyPressed:    持续检测（按住就返回 true）
    //
    // 移动用 isKeyPressed，技能/菜单用 KeyPressed 事件

    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        moveDir.y -= 1.f;
        m_facing = Facing::Up;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        moveDir.y += 1.f;
        m_facing = Facing::Down;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        moveDir.x -= 1.f;
        m_facing = Facing::Left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        moveDir.x += 1.f;
        m_facing = Facing::Right;
    }

    // ====== 对角线速度修正 ======
    // 如果同时按 W 和 D，moveDir = (1, -1)，长度 = √2 ≈ 1.414
    // 这意味着斜着走比直走快 41%！不公平
    // 解决方法：正则化向量，让斜向速度 = 直向速度
    if (moveDir.x != 0.f && moveDir.y != 0.f)
    {
        // 1/√2 ≈ 0.7071
        moveDir *= 0.7071f;
    }

    // 最终位移 = 方向 × 速度 × 时间
    // 这个位移会在 FarmScene 里做碰撞检测后再应用
    m_shape.move(moveDir * m_speed * dt);
}

void Player::render(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::FloatRect Player::getBounds() const
{
    // 返回玩家的全局包围盒（用于碰撞检测）
    return m_shape.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const
{
    return m_shape.getPosition();
}

void Player::setPosition(const sf::Vector2f& pos)
{
    m_shape.setPosition(pos);
}

void Player::move(const sf::Vector2f& offset)
{
    m_shape.move(offset);
}
