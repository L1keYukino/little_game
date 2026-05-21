#pragma once
#include "../core/Scene.hpp"
#include "../entities/Player.hpp"
#include "../entities/Crop.hpp"
#include "../entities/DroppedItem.hpp"
#include "../world/TileMap.hpp"
#include "../core/Camera.hpp"
#include "../ui/Inventory.hpp"
#include <vector>
#include <memory>

class FarmScene : public Scene
{
public:
    FarmScene(sf::RenderWindow& window, Player& player, Inventory& inventory);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    Player&    m_player;
    TileMap    m_map;
    Camera     m_camera;
    Inventory& m_inventory;

    sf::Vector2i m_interactTile;
    sf::RectangleShape m_highlight;
    float m_actionCooldown = 0.f;

    std::vector<std::unique_ptr<DroppedItem>> m_droppedItems;
    std::vector<std::unique_ptr<Crop>>        m_crops;
};
