#include "TileMap.hpp"

TileMap::TileMap(const sf::Texture* tilesetTexture, int tileSize,
                  const std::vector<std::vector<int>>& mapData)
    : m_tileset(tilesetTexture)
    , m_tileSize(tileSize)
    , m_data(mapData)
{
    m_height = static_cast<int>(m_data.size());
    m_width  = m_height > 0 ? static_cast<int>(m_data[0].size()) : 0;

    buildVertices();
}

void TileMap::buildVertices()
{
    // ====== sf::VertexArray 构建 ======
    //
    // 原理：
    //   SFML 3.0 移除了 Quads，每个瓦片用 2 个三角形（6 个顶点）
    //   三角形 1: 左上 → 右上 → 右下
    //   三角形 2: 左上 → 右下 → 左下
    //   总顶点数 = 瓦片数 × 6
    //
    // 每个顶点的信息：
    //   position: 屏幕上的像素位置
    //   color:    顶点颜色（纯色模式）或白色（纹理模式）
    //   texCoords: 纹理坐标（UV），告诉 GPU 从图块集的哪个位置采样
    //
    // 当前阶段用纯色（无纹理），后续加载 tileset 后改为 UV 坐标

    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(m_width * m_height * 6);

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            // 计算这个格子 4 个角的像素位置
            float left   = static_cast<float>(x * m_tileSize);
            float top    = static_cast<float>(y * m_tileSize);
            float right  = left + m_tileSize;
            float bottom = top  + m_tileSize;

            // 在顶点数组中的索引：第 (y*width + x) 个 tile，6 个顶点
            int idx = (y * m_width + x) * 6;

            int tileType = m_data[y][x];
            sf::Color color = getColorForType(tileType);

            // 三角形 1: 左上(0) → 右上(1) → 右下(2)
            m_vertices[idx + 0].position = sf::Vector2f(left,  top);
            m_vertices[idx + 0].color    = color;

            m_vertices[idx + 1].position = sf::Vector2f(right, top);
            m_vertices[idx + 1].color    = color;

            m_vertices[idx + 2].position = sf::Vector2f(right, bottom);
            m_vertices[idx + 2].color    = color;

            // 三角形 2: 左上(3) → 右下(4) → 左下(5)
            m_vertices[idx + 3].position = sf::Vector2f(left,  top);
            m_vertices[idx + 3].color    = color;

            m_vertices[idx + 4].position = sf::Vector2f(right, bottom);
            m_vertices[idx + 4].color    = color;

            m_vertices[idx + 5].position = sf::Vector2f(left,  bottom);
            m_vertices[idx + 5].color    = color;
        }
    }
}

sf::Color TileMap::getColorForType(int type) const
{
    // 纯色模式 —— 等有了图块集纹理后，这些颜色会被纹理替代
    switch (type)
    {
    case 0: return sf::Color( 34, 139,  34); // 草地：森林绿
    case 1: return sf::Color(100, 100, 120); // 墙壁/水：灰蓝
    case 2: return sf::Color(139,  90,  43); // 耕地：棕色
    case 3: return sf::Color(144, 238, 144); // 耕种过的湿润土地：浅绿
    default: return sf::Color::Magenta;       // 洋红 = 未定义类型（调试用）
    }
}

void TileMap::render(sf::RenderWindow& window) const
{
    // 一次 draw call 完成整张地图的渲染！
    // 这就是 VertexArray 的威力
    if (m_tileset)
    {
        // 有纹理时：绑定纹理，画纹理四边形
        window.draw(m_vertices, m_tileset);
    }
    else
    {
        // 无纹理（纯色模式）：直接画
        window.draw(m_vertices);
    }
}

bool TileMap::isSolid(const sf::Vector2f& worldPos) const
{
    // 将世界坐标转换为格子坐标，检查那个格子是否是障碍物
    sf::Vector2i tile = worldToTile(worldPos);

    // 边界检查：超出地图范围的坐标视为实心（不可走出地图）
    if (tile.x < 0 || tile.x >= m_width || tile.y < 0 || tile.y >= m_height)
        return true;

    return isTileSolid(tile.x, tile.y);
}

bool TileMap::isTileSolid(int tileX, int tileY) const
{
    // 类型 1 = 墙壁/水 = 不可行走
    return m_data[tileY][tileX] == 1;
}

int TileMap::getTileType(int tileX, int tileY) const
{
    if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
        return -1;
    return m_data[tileY][tileX];
}

void TileMap::setTileType(int tileX, int tileY, int type)
{
    if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
        return;

    m_data[tileY][tileX] = type;

    // 修改了地图数据后，需要更新对应顶点的颜色
    // 在阶段 2 中这会很频繁（锄地会改变格子类型）
    // 每个 tile = 6 个顶点（2 个三角形）
    int idx = (tileY * m_width + tileX) * 6;
    sf::Color color = getColorForType(type);
    for (int i = 0; i < 6; ++i)
        m_vertices[idx + i].color = color;
}

sf::Vector2i TileMap::worldToTile(const sf::Vector2f& worldPos) const
{
    // 世界坐标 → 格子坐标（整数除法，自动向下取整）
    // 例如 worldPos=(150, 90), tileSize=64 → tile=(2, 1)
    return sf::Vector2i(
        static_cast<int>(worldPos.x) / m_tileSize,
        static_cast<int>(worldPos.y) / m_tileSize
    );
}

sf::Vector2f TileMap::tileToWorld(int tileX, int tileY) const
{
    // 格子坐标 → 世界坐标（格子的左上角）
    return sf::Vector2f(
        static_cast<float>(tileX * m_tileSize),
        static_cast<float>(tileY * m_tileSize)
    );
}
