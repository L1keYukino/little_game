#include "FarmScene.hpp"
#include <iostream>

FarmScene::FarmScene(sf::RenderWindow& window)
    // ====== 构造地图数据 ======
    // 30列 × 26行, 每格 64px → 世界尺寸 1920×1664
    // 设计一个简单的农场布局：
    //   - 边缘是墙壁(1)
    //   - 中间大部分是草地(0)
    //   - 中央有一块可耕种区域(2)
    : m_map(nullptr, 64, {
          {1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,2,2,2,2,2,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,2,2,2,2,2,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,2,2,2,2,2,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,2,2,2,2,2,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,2,2,2,2,2,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1},
          {1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},
      })
    , m_camera(800.f, 600.f) // 摄像机视口 = 窗口大小
{
    // 把玩家放在地图中间偏左一点的位置（避开中间的墙壁）
    m_player.setPosition(sf::Vector2f(320.f, 320.f));

    // 告诉摄像机地图大小，防止边缘露出黑边
    // 30 列 × 26 行 × 64px = 1920 × 1664
    m_camera.setWorldBounds(
        static_cast<float>(m_map.getWidth()  * m_map.getTileSize()),
        static_cast<float>(m_map.getHeight() * m_map.getTileSize())
    );
}

void FarmScene::handleInput(const sf::Event& event)
{
    // 阶段 1 暂时不处理事件输入
    // 阶段 2 会在这里加：空格互动、数字键切换物品、E 打开背包等
    (void)event; // 消除未使用变量的编译警告
}

void FarmScene::update(float dt)
{
    // ====== 每帧更新顺序 ======
    // 1. 让玩家计算并执行移动
    // 2. 分轴碰撞检测（X 和 Y 独立判断，这样靠墙斜走不会卡死）
    // 3. 摄像机跟随玩家

    sf::Vector2f oldPos = m_player.getPosition();

    m_player.update(dt);

    // ====== 分轴碰撞检测 ======
    // 关键思路：X 和 Y 分开检测
    // - 先检查 X 方向（用 newX + oldY），撞了只回退 X
    // - 再检查 Y 方向（用 correctedX + newY），撞了只回退 Y
    // 这样玩家靠墙斜走时，被挡的方向停下，另一个方向继续滑动

    sf::Vector2f newPos = m_player.getPosition();
    sf::FloatRect bounds = m_player.getBounds();

    // --- 第一步：检测 X 轴 ---
    // 只关心 X 方向的变化：假设 Y 没变，只看 X 移动后会不会撞墙
    {
        float checkTop    = oldPos.y - bounds.size.y / 2.f; // 用旧 Y 来算
        float checkBottom = oldPos.y + bounds.size.y / 2.f;
        float newLeft     = newPos.x - bounds.size.x / 2.f;
        float newRight    = newPos.x + bounds.size.x / 2.f;

        bool xBlocked = m_map.isSolid(sf::Vector2f(newLeft,  checkTop))
                     || m_map.isSolid(sf::Vector2f(newRight, checkTop))
                     || m_map.isSolid(sf::Vector2f(newLeft,  checkBottom))
                     || m_map.isSolid(sf::Vector2f(newRight, checkBottom));

        if (xBlocked)
        {
            // X 方向被挡住，把 X 退回到移动前的位置
            newPos.x = oldPos.x;
            m_player.setPosition(newPos);
            bounds = m_player.getBounds(); // 同步更新包围盒
        }
    }

    // --- 第二步：检测 Y 轴 ---
    // 用修正后的 X（可能已经退回了），检查 Y 移动后是否撞墙
    {
        newPos = m_player.getPosition(); // X 可能已经回退
        bounds = m_player.getBounds();

        float checkLeft   = newPos.x - bounds.size.x / 2.f;
        float checkRight  = newPos.x + bounds.size.x / 2.f;
        float newTop      = newPos.y - bounds.size.y / 2.f;
        float newBottom   = newPos.y + bounds.size.y / 2.f;

        bool yBlocked = m_map.isSolid(sf::Vector2f(checkLeft,  newTop))
                     || m_map.isSolid(sf::Vector2f(checkRight, newTop))
                     || m_map.isSolid(sf::Vector2f(checkLeft,  newBottom))
                     || m_map.isSolid(sf::Vector2f(checkRight, newBottom));

        if (yBlocked)
        {
            newPos.y = oldPos.y;
            m_player.setPosition(newPos);
        }
    }

    // 摄像机平滑跟随玩家
    m_camera.follow(m_player.getPosition(), dt);
}

void FarmScene::render(sf::RenderWindow& window)
{
    // 1. 先应用摄像机（让 SFML 知道从哪个视角渲染）
    m_camera.apply(window);

    // 2. 画地图
    m_map.render(window);

    // 3. 画玩家
    m_player.render(window);
}
