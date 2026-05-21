#pragma once
#include <string>
#include "Tool.hpp" // ToolType 枚举

// 物品类型
enum class ItemType
{
    Empty,         // 空
    Hoe,           // 锄头：翻土
    WateringCan,   // 水壶：浇水
    Seeds,         // 种子：播种
    Turnip,        // 萝卜：收获的作物（阶段2.3用）
};

// 根据物品类型返回中文名（用于悬浮提示）
inline const char* itemName(ItemType type)
{
    switch (type)
    {
    case ItemType::Hoe:         return "锄头";
    case ItemType::WateringCan: return "水壶";
    case ItemType::Seeds:       return "种子";
    case ItemType::Turnip:      return "萝卜";
    default:                    return "";
    }
}

// 根据物品类型返回对应的工具类型（可装备的物品）
// 只有工具类物品才有对应的 ToolType
// 这个转换用于背包选中物品后 → 设置当前工具
inline ToolType itemToTool(ItemType type)
{
    switch (type)
    {
    case ItemType::Hoe:         return ToolType::Hoe;
    case ItemType::WateringCan: return ToolType::WateringCan;
    case ItemType::Seeds:       return ToolType::Seeds;
    default:                    return ToolType::None;
    }
}
