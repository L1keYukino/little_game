#pragma once
#include "../world/TileMap.hpp" // TileType 枚举

// 工具类型 —— 决定空格键对格子做什么
enum class ToolType
{
    Hoe,            // 锄头：翻土
    WateringCan,    // 水壶：浇水
    Seeds,          // 种子：种植
    None            // 空手
};

// 检查某个工具能否对某个格子类型生效
inline bool canUseTool(ToolType tool, TileType tileType)
{
    switch (tool)
    {
    case ToolType::Hoe:
        return tileType == TileType::Dirt;
    case ToolType::WateringCan:
        return tileType == TileType::Tilled;
    case ToolType::Seeds:
        return tileType == TileType::Watered;
    default:
        return false;
    }
}

// 获取工具使用后的目标格子类型
inline TileType getResultTileType(ToolType tool)
{
    switch (tool)
    {
    case ToolType::Hoe:
        return TileType::Tilled;
    case ToolType::WateringCan:
        return TileType::Watered;
    case ToolType::Seeds:
        return TileType::Planted;
    default:
        return TileType::Grass;
    }
}
