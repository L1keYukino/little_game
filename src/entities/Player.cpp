#include "Player.hpp"

Player::Player()
{
    m_shape.setSize(sf::Vector2f(32.f, 32.f));
    m_shape.setOrigin(sf::Vector2f(16.f, 16.f));
    m_shape.setFillColor(sf::Color::Green);

    position = sf::Vector2f(400.f, 300.f);
    m_shape.setPosition(position);
}

void Player::update(float dt)
{
    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    { moveDir.y -= 1.f; m_facing = Facing::Up; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    { moveDir.y += 1.f; m_facing = Facing::Down; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    { moveDir.x -= 1.f; m_facing = Facing::Left; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    { moveDir.x += 1.f; m_facing = Facing::Right; }

    if (moveDir.x != 0.f && moveDir.y != 0.f)
        moveDir *= 0.7071f;

    position += moveDir * m_speed * dt;
    m_shape.setPosition(position);
}

void Player::render(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::FloatRect Player::getBounds() const
{
    return m_shape.getGlobalBounds();
}
