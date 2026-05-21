#include "FarmingSystem.hpp"
#include "../entities/Tool.hpp"
#include "../entities/Item.hpp"

namespace FarmingSystem
{

sf::Vector2i computeInteractTile(const Player& player, const TileMap& map)
{
    sf::Vector2i pt = map.worldToTile(player.position);
    switch (player.getFacing())
    {
    case Facing::Up:    return {pt.x,     pt.y - 1};
    case Facing::Down:  return {pt.x,     pt.y + 1};
    case Facing::Left:  return {pt.x - 1, pt.y};
    case Facing::Right: return {pt.x + 1, pt.y};
    default:            return pt;
    }
}

bool useTool(TileMap& map, Inventory& inventory, const sf::Vector2i& tile,
             std::vector<std::unique_ptr<Crop>>& crops)
{
    TileType tt = map.getTileType(tile.x, tile.y);

    // 空格对着有作物的格子 → 收获（成熟）/ 提示未成熟
    if (tt == TileType::Planted)
    {
        for (auto& c : crops)
        {
            if (c->tileX() == tile.x && c->tileY() == tile.y)
            {
                if (c->isMature())
                {
                    // 收获：加萝卜到背包，移除作物，恢复耕地
                    inventory.addItemAuto(ItemType::Turnip, 1);
                    c->active = false;
                    map.setTileType(tile.x, tile.y, TileType::Tilled);
                    return true;
                }
                // 未成熟，不操作
                return false;
            }
        }
        return false;
    }

    // 工具使用
    ItemType item = inventory.getSelectedItem();
    ToolType tool = itemToTool(item);
    if (tool == ToolType::None) return false;

    if (!canUseTool(tool, tt)) return false;

    map.setTileType(tile.x, tile.y, getResultTileType(tool));

    if (tool == ToolType::Seeds)
        inventory.removeItem(inventory.getSelectedSlot(), 1);

    return true;
}

} // namespace FarmingSystem
