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
    bool paused = m_inventory.isOpen();

    // 始终处理：扔出队列 + 作物生长 + 掉落物品 + 交互格子
    for (auto& entry : m_inventory.drainThrowQueue())
    {
        sf::Vector2i dropTile = FarmingSystem::computeInteractTile(m_player, m_map);
        if (m_map.isTileSolid(dropTile.x, dropTile.y))
            dropTile = m_map.worldToTile(m_player.position);
        sf::Vector2f dropPos = m_map.tileToWorld(dropTile);
        dropPos.x += m_map.getTileSize() / 2.f;
        dropPos.y += m_map.getTileSize() / 2.f;
        m_droppedItems.push_back(std::make_unique<DroppedItem>(dropPos, entry.type, entry.count));
    }

    for (auto& crop : m_crops)
    {
        crop->update(dt);
        sf::Vector2f tc = m_map.tileToWorld({crop->tileX(), crop->tileY()});
        crop->position = {tc.x + m_map.getTileSize() / 2.f, tc.y + m_map.getTileSize() / 2.f};
    }
    std::erase_if(m_crops, [](auto& c) { return !c->active; });

    for (auto& di : m_droppedItems)
    {
        di->update(dt);
        if (di->canPickup())
        {
            float dist = std::hypot(di->position.x - m_player.position.x,
                                     di->position.y - m_player.position.y);
            if (dist < 40.f)
            { m_inventory.addItemAuto(di->itemType(), di->count()); di->active = false; }
        }
    }
    std::erase_if(m_droppedItems, [](auto& d) { return !d->active; });

    m_interactTile = FarmingSystem::computeInteractTile(m_player, m_map);
    m_highlight.setPosition(m_map.tileToWorld(m_interactTile));

    // 以下仅在背包关闭时运行
    if (paused) return;

    sf::Vector2f oldPos = m_player.position;
    m_player.update(dt);

    m_actionCooldown -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_actionCooldown <= 0.f)
    {
        bool used = FarmingSystem::useTool(m_map, m_inventory, m_interactTile, m_crops);
        m_actionCooldown = 0.25f;

        if (used && m_map.getTileType(m_interactTile.x, m_interactTile.y) == TileType::Planted)
        {
            bool exists = false;
            for (auto& c : m_crops)
                if (c->tileX() == m_interactTile.x && c->tileY() == m_interactTile.y)
                { exists = true; break; }
            if (!exists)
                m_crops.push_back(std::make_unique<Crop>(m_interactTile.x, m_interactTile.y));
        }
    }

    sf::Vector2f newPos = m_player.position;
    sf::FloatRect bounds = m_player.getBounds();
    {
        float cT = oldPos.y - bounds.size.y / 2.f, cB = oldPos.y + bounds.size.y / 2.f;
        float nL = newPos.x - bounds.size.x / 2.f, nR = newPos.x + bounds.size.x / 2.f;
        if (m_map.isSolid({nL, cT}) || m_map.isSolid({nR, cT})
         || m_map.isSolid({nL, cB}) || m_map.isSolid({nR, cB}))
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
