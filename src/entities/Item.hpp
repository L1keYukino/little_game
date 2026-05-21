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

// 每种物品的堆叠上限（一个格子最多放几个）
inline int maxStackSize(ItemType type)
{
    switch (type)
    {
    case ItemType::Hoe:         return 1;   // 工具一槽一个
    case ItemType::WateringCan: return 1;
    case ItemType::Seeds:       return 60;  // 材料一槽 60
    case ItemType::Turnip:      return 60;
    default:                    return 0;
    }
}

// 根据物品类型返回对应的工具类型
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
