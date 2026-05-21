#pragma once
#include "../core/Scene.hpp"
#include "../entities/Player.hpp"
#include "../world/TileMap.hpp"
#include "../core/Camera.hpp"
#include "../ui/Inventory.hpp"

class FarmScene : public Scene
{
public:
    FarmScene(sf::RenderWindow& window, Inventory& inventory);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Vector2i computeInteractTile() const;
    void useTool(const sf::Vector2i& tile);

    Player    m_player;
    TileMap   m_map;
    Camera    m_camera;
    Inventory& m_inventory;              // 全局背包引用（由 Game 拥有）

    sf::Vector2i m_interactTile;
    sf::RectangleShape m_highlight;
    float m_actionCooldown = 0.f;
};
