#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../entities/Item.hpp"

struct ThrowEntry { ItemType type; int count; };

class Inventory
{
public:
    Inventory(float winW, float winH);

    ItemType getItem(int slot) const;
    int      getCount(int slot) const;
    void     setItem(int slot, ItemType type, int count);
    void     addItem(int slot, int amount);
    bool     removeItem(int slot, int amount);

    int  getSelectedSlot() const { return m_selectedSlot; }
    void setSelectedSlot(int slot);
    ItemType getSelectedItem() const;

    bool isOpen() const { return m_open; }
    void toggle() { m_open = !m_open; resetInteraction(); }
    void close();
    bool isSplitDialogOpen() const { return m_splitDialogOpen; }

    bool addItemAuto(ItemType type, int count);
    std::vector<ThrowEntry> drainThrowQueue();

    void update(float dt);
    void updateHover(const sf::Vector2i& mousePos);
    void render(sf::RenderWindow& window);

    void onMouseDown(bool ctrlHeld, const sf::Vector2i& mousePos);
    void onMouseUp(bool ctrlHeld, const sf::Vector2i& mousePos);
    void setShowTooltip(bool show) { m_showTooltip = show; }
    void onThrowKey();
    void onTextEntered(char32_t codepoint);

private:
    void drawSlot(sf::RenderWindow& window, int i, const sf::FloatRect& rect);
    void renderHotbar(sf::RenderWindow& window);
    void renderBackpack(sf::RenderWindow& window);
    void renderTooltip(sf::RenderWindow& window) const;
    void renderCursorItem(sf::RenderWindow& window) const;
    void renderSplitDialog(sf::RenderWindow& window);

    sf::FloatRect getHotbarSlotRect(int slot) const;
    sf::FloatRect getBackpackSlotRect(int slot) const;
    sf::FloatRect getTrashSlotRect() const;

    void pickUp(int slot, int count);
    void placeOne(int slot);
    void placeAll(int slot);
    void throwCursorItem(int count);
    void resetInteraction(); // 清除所有拖拽/拆分数残留状态

    void openSplitDialog(int sourceSlot, int targetSlot);
    void closeSplitDialog(bool confirm);
    void updateSplitSlider(const sf::Vector2i& mousePos);
    void updateSplitFromInput();
    int  splitSliderToAmount(int sliderX) const;
    int  splitAmountToSlider(int amount) const;

    static constexpr int SLOT_COUNT   = 32; // 总槽位数
    static constexpr int HOTBAR_COUNT = 8;  // 底部快捷栏槽位
    static constexpr int BACKPACK_COLS = 8; // 背包面板列数
    static constexpr int BACKPACK_ROWS = 3; // 背包面板行数
    static constexpr int SLOT_SIZE  = 48;
    static constexpr int PADDING    = 8;

    float m_winW, m_winH;

    ItemType m_items[SLOT_COUNT] = {};
    int      m_counts[SLOT_COUNT] = {};

    int  m_selectedSlot = 0;
    bool m_open = false;
    int  m_hoveredSlot = -1;

    ItemType m_cursorType  = ItemType::Empty;
    int      m_cursorCount = 0;

    bool m_mouseDown  = false;
    int  m_dragFromSlot = -1;
    bool m_splitMode  = false;
    bool m_showTooltip = false; // R键显示属性

    bool m_splitDialogOpen = false;
    int  m_splitSourceSlot = -1;
    int  m_splitTargetSlot = -1;
    int  m_splitAmount     = 0;
    int  m_splitMax        = 0;
    bool m_draggingSlider   = false;
    bool m_inputFocused      = false;
    std::string m_splitInput;
    float m_blinkTimer       = 0.f;

    ItemType m_trashItem  = ItemType::Empty;
    int      m_trashCount = 0;

    std::vector<ThrowEntry> m_throwQueue;

    float m_throwCooldown = 0.f;

    sf::RectangleShape m_slotShape;
    sf::RectangleShape m_selectedShape;
    sf::RectangleShape m_hoverShape;

    sf::Font m_font;
    bool     m_hasFont = false;
};
