#include "FarmScene.hpp"

FarmScene::FarmScene(sf::RenderWindow& window, Inventory& inventory)
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
    , m_camera(800.f, 600.f)
    , m_inventory(inventory)
{
    m_player.setPosition(sf::Vector2f(320.f, 320.f));
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
    // 场景特定输入暂时为空（背包、滚轮、数字键、Q 由 Game 全局处理）
    (void)event;
}

void FarmScene::update(float dt)
{
    // 背包打开时暂停（由 Game 保证，这里做兜底）
    if (m_inventory.isOpen()) return;

    sf::Vector2f oldPos = m_player.getPosition();
    m_player.update(dt);

    // 工具使用（空格键 + 冷却）
    m_actionCooldown -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_actionCooldown <= 0.f)
    {
        useTool(m_interactTile);
        m_actionCooldown = 0.25f;
    }

    // 分轴碰撞检测
    sf::Vector2f newPos = m_player.getPosition();
    sf::FloatRect bounds = m_player.getBounds();

    // X
    {
        float checkTop = oldPos.y - bounds.size.y / 2.f, checkBottom = oldPos.y + bounds.size.y / 2.f;
        float newLeft = newPos.x - bounds.size.x / 2.f, newRight = newPos.x + bounds.size.x / 2.f;
        if (m_map.isSolid({newLeft, checkTop}) || m_map.isSolid({newRight, checkTop})
         || m_map.isSolid({newLeft, checkBottom}) || m_map.isSolid({newRight, checkBottom}))
        {
            newPos.x = oldPos.x; m_player.setPosition(newPos); bounds = m_player.getBounds();
        }
    }
    // Y
    {
        newPos = m_player.getPosition(); bounds = m_player.getBounds();
        float checkLeft = newPos.x - bounds.size.x / 2.f, checkRight = newPos.x + bounds.size.x / 2.f;
        float newTop = newPos.y - bounds.size.y / 2.f, newBottom = newPos.y + bounds.size.y / 2.f;
        if (m_map.isSolid({checkLeft, newTop}) || m_map.isSolid({checkRight, newTop})
         || m_map.isSolid({checkLeft, newBottom}) || m_map.isSolid({checkRight, newBottom}))
        {
            newPos.y = oldPos.y; m_player.setPosition(newPos);
        }
    }

    m_interactTile = computeInteractTile();
    m_highlight.setPosition(m_map.tileToWorld(m_interactTile));
    m_camera.follow(m_player.getPosition(), dt);
}

void FarmScene::render(sf::RenderWindow& window)
{
    m_camera.apply(window);
    m_map.render(window);
    if (!m_inventory.isOpen()) window.draw(m_highlight); // 背包打开时隐藏农耕高亮
    m_player.render(window);
    // HUD（背包/快捷栏）由 Game 全局渲染，场景不负责
}

sf::Vector2i FarmScene::computeInteractTile() const
{
    sf::Vector2i pt = m_map.worldToTile(m_player.getPosition());
    switch (m_player.getFacing())
    {
    case Facing::Up:    return {pt.x, pt.y - 1};
    case Facing::Down:  return {pt.x, pt.y + 1};
    case Facing::Left:  return {pt.x - 1, pt.y};
    case Facing::Right: return {pt.x + 1, pt.y};
    default:            return pt;
    }
}

void FarmScene::useTool(const sf::Vector2i& tile)
{
    ItemType item = m_inventory.getSelectedItem();
    ToolType tool = itemToTool(item);
    if (tool == ToolType::None) return;

    TileType tt = m_map.getTileType(tile.x, tile.y);
    if (!canUseTool(tool, tt)) return;

    m_map.setTileType(tile.x, tile.y, getResultTileType(tool));

    if (tool == ToolType::Seeds)
        m_inventory.removeItem(m_inventory.getSelectedSlot(), 1);
}
