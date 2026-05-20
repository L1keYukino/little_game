#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// 瓦片地图 —— 用 sf::VertexArray 批量渲染整个地图
//
// 为什么不用 400 个 sf::Sprite？
// 每个 Sprite 都是一次 draw call（CPU→GPU 通信），400 次 draw call 会严重拖慢性能
//
// VertexArray 把 400 个格子打包成一个顶点数组，一次 draw call 全部画完
// 这就是"批处理"(batching)的基本思想
//
// 地图数据用二维 int 数组存储，每个格子的值：
//   0 = 草地（可行走，绿色）
//   1 = 墙壁/水（不可行走，蓝色）
//   2 = 土地（可耕种，棕色）

class TileMap
{
public:
    // tilesetTexture: 图块集纹理（当前阶段传 nullptr，用纯色代替）
    // tileSize: 每个格子的像素大小（如 64）
    // mapData: 二维 int 数组，格子属性
    TileMap(const sf::Texture* tilesetTexture, int tileSize,
            const std::vector<std::vector<int>>& mapData);

    // 渲染整个地图（在 window.draw(map) 时调用）
    void render(sf::RenderWindow& window) const;

    // 碰撞检测：检查某个世界坐标位置是否可以行走
    // 返回 true 表示该位置是障碍物（不可行走）
    bool isSolid(const sf::Vector2f& worldPos) const;

    // 检查某个格子的坐标是否可行走
    bool isTileSolid(int tileX, int tileY) const;

    // 获取地图尺寸（格数）
    int getWidth()  const { return m_width; }
    int getHeight() const { return m_height; }
    int getTileSize() const { return m_tileSize; }

    // 获取某个格子的类型（0=草地, 1=障碍, 2=土地）
    int getTileType(int tileX, int tileY) const;

    // 修改某个格子的类型（锄地、种菜时会用到）
    void setTileType(int tileX, int tileY, int type);

    // 世界坐标 ↔ 格子坐标 互转
    sf::Vector2i worldToTile(const sf::Vector2f& worldPos) const;
    sf::Vector2f tileToWorld(int tileX, int tileY) const;

private:
    // 构建顶点数组（根据 mapData 给每个格子设置颜色/UV）
    void buildVertices();

    // 根据格子类型返回颜色
    sf::Color getColorForType(int type) const;

    const sf::Texture* m_tileset; // 图块纹理（nullptr 时用纯色）
    int m_tileSize;
    int m_width;
    int m_height;

    // 格子数据：每个格子的类型值
    std::vector<std::vector<int>> m_data;

    // 顶点数组：一次渲染整张地图
    // 每个格子 = 2 个三角形 = 6 个顶点，用 sf::Quads 就是 4 个顶点
    sf::VertexArray m_vertices;
};
