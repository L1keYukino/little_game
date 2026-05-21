#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// 格子类型 —— 地图上每个格子的属性
enum class TileType : int
{
    Grass    = 0, // 草地（可行走，不可耕种）
    Wall     = 1, // 障碍（不可行走）
    Dirt     = 2, // 土地（可耕种，锄头目标）
    Tilled   = 3, // 翻耕地（已翻土，水壶目标）
    Watered  = 4, // 湿润耕地（已浇水，种子目标）
    Planted  = 5  // 种植中（有种子/作物）
};

// 瓦片地图 —— 用 sf::VertexArray 批量渲染整个地图
//
// 为什么不用 400 个 sf::Sprite？
// 每个 Sprite 都是一次 draw call（CPU→GPU 通信），400 次 draw call 会严重拖慢性能
//
// VertexArray 把 400 个格子打包成一个顶点数组，一次 draw call 全部画完
// 这就是"批处理"(batching)的基本思想
class TileMap
{
public:
    TileMap(const sf::Texture* tilesetTexture, int tileSize,
            const std::vector<std::vector<int>>& mapData);

    void render(sf::RenderWindow& window) const;

    // 碰撞检测
    bool isSolid(const sf::Vector2f& worldPos) const;
    bool isTileSolid(int tileX, int tileY) const;

    int getWidth()  const { return m_width; }
    int getHeight() const { return m_height; }
    int getTileSize() const { return m_tileSize; }

    // 格子类型读写
    TileType getTileType(int tileX, int tileY) const;
    void     setTileType(int tileX, int tileY, TileType type);

    // 世界坐标 ↔ 格子坐标 互转
    sf::Vector2i worldToTile(const sf::Vector2f& worldPos) const;
    sf::Vector2f tileToWorld(const sf::Vector2i& tile) const;

private:
    void buildVertices();
    sf::Color getColorForType(TileType type) const;

    const sf::Texture* m_tileset;
    int m_tileSize;
    int m_width;
    int m_height;

    std::vector<std::vector<int>> m_data; // 内部用 int 存，和构造初始化兼容
    sf::VertexArray m_vertices;
};
