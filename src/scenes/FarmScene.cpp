#include "FarmScene.hpp"
#include "../systems/FarmingSystem.hpp"
#include <cmath>

FarmScene::FarmScene(sf::RenderWindow& window, Player& player, Inventory& inventory)
    : m_player(player)
    , m_inventory(inventory)
    , m_map(nullptr, 64, {
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
    , m_camera(800.f, 600.f)
{
    m_player.position = sf::Vector2f(320.f, 320.f);
    m_camera.setWorldBounds(
        static_cast<float>(m_map.getWidth()  * m_map.getTileSize()),
        static_cast<float>(m_map.getHeight() * m_map.getTileSize()));

    float ts = static_cast<float>(m_map.getTileSize());
    m_highlight.setSize(sf::Vector2f(ts, ts));
    m_highlight.setFillColor(sf::Color(255, 255, 255, 60));
    m_highlight.setOutlineColor(sf::Color(255, 255, 255, 180));
    m_highlight.setOutlineThickness(2.f);
}

void FarmScene::handleInput(const sf::Event& event)
{
    (void)event;
}

void FarmScene::update(float dt)
{
    if (m_inventory.isOpen()) return;

    // 处理扔出队列 → 生成地上掉落物品
    for (auto& entry : m_inventory.drainThrowQueue())
    {
        // 优先扔在玩家面前，面前是障碍就扔玩家脚下
        sf::Vector2i dropTile = FarmingSystem::computeInteractTile(m_player, m_map);
        if (m_map.isTileSolid(dropTile.x, dropTile.y))
            dropTile = m_map.worldToTile(m_player.position);

        sf::Vector2f dropPos  = m_map.tileToWorld(dropTile);
        dropPos.x += m_map.getTileSize() / 2.f;
        dropPos.y += m_map.getTileSize() / 2.f;
        m_droppedItems.push_back(std::make_unique<DroppedItem>(dropPos, entry.type, entry.count));
    }

    sf::Vector2f oldPos = m_player.position;
    m_player.update(dt);

    // 工具使用 / 作物收获（委托给 FarmingSystem）
    m_actionCooldown -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_actionCooldown <= 0.f)
    {
        bool used = FarmingSystem::useTool(m_map, m_inventory, m_interactTile,
                                            m_crops);
        m_actionCooldown = 0.25f;

        // 播种后为该格子创建作物
        if (used && m_map.getTileType(m_interactTile.x, m_interactTile.y) == TileType::Planted)
        {
            // 检查是否已有作物（防止重复创建）
            bool exists = false;
            for (auto& c : m_crops)
                if (c->tileX() == m_interactTile.x && c->tileY() == m_interactTile.y)
                { exists = true; break; }
            if (!exists)
                m_crops.push_back(std::make_unique<Crop>(m_interactTile.x, m_interactTile.y));
        }
    }

    // 分轴碰撞检测
    sf::Vector2f newPos = m_player.position;
    sf::FloatRect bounds = m_player.getBounds();

    {
        float cTop = oldPos.y - bounds.size.y / 2.f, cBot = oldPos.y + bounds.size.y / 2.f;
        float nL = newPos.x - bounds.size.x / 2.f, nR = newPos.x + bounds.size.x / 2.f;
        if (m_map.isSolid({nL, cTop}) || m_map.isSolid({nR, cTop})
         || m_map.isSolid({nL, cBot}) || m_map.isSolid({nR, cBot}))
        { newPos.x = oldPos.x; m_player.position = newPos; bounds = m_player.getBounds(); }
    }
    {
        newPos = m_player.position; bounds = m_player.getBounds();
        float cL = newPos.x - bounds.size.x / 2.f, cR = newPos.x + bounds.size.x / 2.f;
        float nT = newPos.y - bounds.size.y / 2.f, nB = newPos.y + bounds.size.y / 2.f;
        if (m_map.isSolid({cL, nT}) || m_map.isSolid({cR, nT})
         || m_map.isSolid({cL, nB}) || m_map.isSolid({cR, nB}))
        { newPos.y = oldPos.y; m_player.position = newPos; }
    }

    // 交互格子
    m_interactTile = FarmingSystem::computeInteractTile(m_player, m_map);
    m_highlight.setPosition(m_map.tileToWorld(m_interactTile));

    // 更新作物
    for (auto& crop : m_crops)
    {
        crop->update(dt);
        sf::Vector2f tc = m_map.tileToWorld({crop->tileX(), crop->tileY()});
        crop->position = {tc.x + m_map.getTileSize() / 2.f, tc.y + m_map.getTileSize() / 2.f};
    }
    std::erase_if(m_crops, [](auto& c) { return !c->active; });

    // 更新地上掉落物品 + 自动吸附拾取
    for (auto& di : m_droppedItems)
    {
        di->update(dt);

        // 玩家靠近时自动捡起
        float dist = std::hypot(di->position.x - m_player.position.x,
                                 di->position.y - m_player.position.y);
        if (di->canPickup() && dist < 40.f)
        {
            m_inventory.addItemAuto(di->itemType(), di->count());
            di->active = false;
        }
    }

    // 清理已拾取的物品
    std::erase_if(m_droppedItems, [](auto& d) { return !d->active; });

    m_camera.follow(m_player.position, dt);
}

void FarmScene::render(sf::RenderWindow& window)
{
    m_camera.apply(window);
    m_map.render(window);
    if (!m_inventory.isOpen()) window.draw(m_highlight);
    for (auto& crop : m_crops) crop->render(window);
    for (auto& di : m_droppedItems) di->render(window);
    m_player.render(window);
}
