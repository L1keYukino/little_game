#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../entities/Item.hpp"

class Inventory
{
public:
    Inventory();

    ItemType getItem(int slot) const;
    int      getCount(int slot) const;
    void     setItem(int slot, ItemType type, int count);
    void     addItem(int slot, int amount);
    bool     removeItem(int slot, int amount);

    int  getSelectedSlot() const { return m_selectedSlot; }
    void setSelectedSlot(int slot);
    ItemType getSelectedItem() const;

    bool isOpen() const { return m_open; }
    void toggle() { m_open = !m_open; }
    void close();
    bool isSplitDialogOpen() const { return m_splitDialogOpen; }

    void update(float dt);
    void updateHover(const sf::Vector2i& mousePos);
    void render(sf::RenderWindow& window);

    void onMouseDown(bool ctrlHeld, const sf::Vector2i& mousePos);
    void onMouseUp(bool ctrlHeld, const sf::Vector2i& mousePos);
    void onThrowKey();
    void onTextEntered(char32_t codepoint);  // 文本输入（拆分对话框数字）

private:
    void drawSlot(sf::RenderWindow& window, int i, const sf::FloatRect& rect);
    void renderHotbar(sf::RenderWindow& window);
    void renderBackpack(sf::RenderWindow& window);
    void renderTooltip(sf::RenderWindow& window) const;
    void renderCursorItem(sf::RenderWindow& window) const;
    void renderSplitDialog(sf::RenderWindow& window);

    sf::FloatRect getHotbarSlotRect(int slot) const;
    sf::FloatRect getBackpackSlotRect(int slot) const;

    void pickUp(int slot, int count);
    void placeOne(int slot);
    void placeAll(int slot);
    void throwCursorItem(int count);

    // 拆分对话框
    void openSplitDialog(int sourceSlot, int targetSlot);
    void closeSplitDialog(bool confirm);
    void updateSplitSlider(const sf::Vector2i& mousePos);
    void updateSplitFromInput();                // 从文本输入更新数量
    int  splitSliderToAmount(int sliderX) const;
    int  splitAmountToSlider(int amount) const;

    static constexpr int SLOT_COUNT = 8;
    static constexpr int SLOT_SIZE  = 48;
    static constexpr int PADDING    = 8;

    ItemType m_items[SLOT_COUNT] = {};
    int      m_counts[SLOT_COUNT] = {};

    int  m_selectedSlot = 0;
    bool m_open = false;
    int  m_hoveredSlot = -1;

    ItemType m_cursorType  = ItemType::Empty;
    int      m_cursorCount = 0;

    bool m_mouseDown  = false;
    int  m_dragFromSlot = -1;
    bool m_splitMode  = false;     // Ctrl+拖拽模式（松手到空格→弹拆分对话框）

    // 拆分对话框
    bool m_splitDialogOpen = false;
    int  m_splitSourceSlot = -1;    // 物品来源格子
    int  m_splitTargetSlot = -1;    // 物品目标格子
    int  m_splitAmount     = 0;
    int  m_splitMax        = 0;
    bool m_draggingSlider   = false;
    bool m_inputFocused      = false;
    std::string m_splitInput;
    float m_blinkTimer       = 0.f;   // 光标闪烁计时

    float m_throwCooldown = 0.f;

    sf::RectangleShape m_slotShape;
    sf::RectangleShape m_selectedShape;
    sf::RectangleShape m_hoverShape;

    sf::Font m_font;
    bool     m_hasFont = false;
};
