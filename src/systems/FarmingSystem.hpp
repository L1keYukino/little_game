#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../entities/Player.hpp"
#include "../entities/Crop.hpp"
#include "../world/TileMap.hpp"
#include "../ui/Inventory.hpp"

namespace FarmingSystem
{

sf::Vector2i computeInteractTile(const Player& player, const TileMap& map);

// 农耕操作：工具使用 或 作物收获（空格键）
// 返回 true 表示执行了动作
bool useTool(TileMap& map, Inventory& inventory, const sf::Vector2i& tile,
             std::vector<std::unique_ptr<Crop>>& crops);

} // namespace FarmingSystem

