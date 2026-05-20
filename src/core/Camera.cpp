#include "Camera.hpp"
#include <algorithm>

Camera::Camera(float width, float height)
{
    // sf::View: SFML 的"观察世界的窗口"
    m_view = sf::View(sf::Vector2f(width / 2.f, height / 2.f),
                        sf::Vector2f(width, height));
}

void Camera::setWorldBounds(float width, float height)
{
    // 设置地图像素尺寸，follow() 会自动限制摄像头不超出这个范围
    m_worldWidth  = width;
    m_worldHeight = height;
}

void Camera::follow(const sf::Vector2f& target, float dt)
{
    // 平滑跟随 —— 指数衰减缓动
    sf::Vector2f current = m_view.getCenter();
    sf::Vector2f offset = target - current;
    sf::Vector2f newCenter = current + offset * m_followSpeed * dt;

    // 限制取景框不超出地图边界
    //
    // 为什么需要这个：
    //   取景框中心太靠左 → 左边缘超出地图 → 看到黑边
    //   所以中心必须在 [halfWidth, worldWidth - halfWidth] 之间
    //
    // 比如地图宽 1920，取景框宽 800 (half=400)：
    //   中心 x < 400  → 左边缘 < 0   → 不允许，锁在 400
    //   中心 x > 1520 → 右边缘 > 1920 → 不允许，锁在 1520
    if (m_worldWidth > 0.f && m_worldHeight > 0.f)
    {
        sf::Vector2f halfSize = m_view.getSize() / 2.f;

        if (m_worldWidth <= halfSize.x * 2.f)
            newCenter.x = m_worldWidth / 2.f;  // 地图比屏幕小 → 居中
        else
            newCenter.x = std::clamp(newCenter.x, halfSize.x, m_worldWidth - halfSize.x);

        if (m_worldHeight <= halfSize.y * 2.f)
            newCenter.y = m_worldHeight / 2.f;
        else
            newCenter.y = std::clamp(newCenter.y, halfSize.y, m_worldHeight - halfSize.y);
    }

    m_view.setCenter(newCenter);
}

void Camera::apply(sf::RenderWindow& window)
{
    // 把取景框设置到窗口，之后所有绘制都基于 View 的坐标系统
    window.setView(m_view);
}

sf::FloatRect Camera::getViewBounds() const
{
    sf::Vector2f center = m_view.getCenter();
    sf::Vector2f size   = m_view.getSize();
    return sf::FloatRect(
        sf::Vector2f(center.x - size.x / 2.f, center.y - size.y / 2.f),
        size
    );
}
