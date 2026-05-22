#include "Inventory.hpp"
#include <cstring>
#include <algorithm>

static sf::String toSfString(const char* utf8)
{
    return sf::String::fromUtf8(utf8, utf8 + std::strlen(utf8));
}

Inventory::Inventory(float winW, float winH)
    : m_winW(winW), m_winH(winH)
{
    m_items[0] = ItemType::Hoe;         m_counts[0] = 1;
    m_items[1] = ItemType::WateringCan; m_counts[1] = 1;
    m_items[2] = ItemType::Seeds;       m_counts[2] = 60;
    m_items[3] = ItemType::Seeds;       m_counts[3] = 40;

    if (!m_font.openFromFile("C:/Windows/Fonts/simhei.ttf"))
        m_hasFont = false;
    else
        m_hasFont = true;

    m_slotShape.setFillColor(sf::Color(40, 40, 40, 200));
    m_slotShape.setOutlineColor(sf::Color(100, 100, 100, 200));
    m_slotShape.setOutlineThickness(2.f);

    m_selectedShape.setFillColor(sf::Color::Transparent);
    m_selectedShape.setOutlineColor(sf::Color(255, 215, 0, 220));
    m_selectedShape.setOutlineThickness(3.f);

    m_hoverShape.setFillColor(sf::Color(255, 255, 255, 40));
    m_hoverShape.setOutlineColor(sf::Color(255, 255, 255, 120));
    m_hoverShape.setOutlineThickness(2.f);
}

ItemType Inventory::getItem(int slot) const { return m_items[slot]; }
int      Inventory::getCount(int slot) const { return m_counts[slot]; }

void Inventory::setItem(int slot, ItemType type, int count)
    { m_items[slot] = type; m_counts[slot] = count; }
void Inventory::addItem(int slot, int amount) { m_counts[slot] += amount; }
bool Inventory::removeItem(int slot, int amount)
{
    if (m_counts[slot] < amount) return false;
    m_counts[slot] -= amount;
    if (m_counts[slot] <= 0) m_items[slot] = ItemType::Empty;
    return true;
}

void Inventory::setSelectedSlot(int slot)
    { if (slot >= 0 && slot < SLOT_COUNT) m_selectedSlot = slot; }
ItemType Inventory::getSelectedItem() const { return m_items[m_selectedSlot]; }

void Inventory::close()
{
    m_open = false;
    if (m_splitDialogOpen) closeSplitDialog(false);
    if (m_cursorCount > 0)
    {
        for (int i = 0; i < SLOT_COUNT; ++i)
            if (m_items[i] == ItemType::Empty)
            { m_items[i] = m_cursorType; m_counts[i] = m_cursorCount; break; }
        m_cursorType = ItemType::Empty; m_cursorCount = 0;
    }
    m_splitMode = false;
}

// ====== 位置 ======

sf::FloatRect Inventory::getHotbarSlotRect(int slot) const
{
    float slotSize = static_cast<float>(SLOT_SIZE);
    float totalW  = HOTBAR_COUNT * slotSize + (HOTBAR_COUNT - 1) * PADDING;
    float startX  = (m_winW - totalW) / 2.f;

    float y;
    if (m_open)
    {
        const float B_SLOT = 54.f, B_GAP = 6.f;
        float gridH  = BACKPACK_ROWS * B_SLOT + (BACKPACK_ROWS - 1) * B_GAP;
        float titleH = 26.f, padT = 6.f, padB = 10.f;
        float trashH = 48.f, gap = 30.f;
        float panelH = titleH + padT + gridH + padB;
        float totalH = panelH + trashH + gap + slotSize;
        float baseY  = (m_winH - totalH) / 2.f;
        y = baseY + panelH + trashH + gap;
    }
    else
    {
        y = m_winH - SLOT_SIZE - 12.f;
    }

    return sf::FloatRect(sf::Vector2f(startX + slot * (slotSize + PADDING), y),
                         sf::Vector2f(slotSize, slotSize));
}

sf::FloatRect Inventory::getBackpackSlotRect(int slot) const
{
    // 仅背包槽位 8-31，3行×8列
    int idx = slot - HOTBAR_COUNT; // 0..23
    if (idx < 0 || idx >= (SLOT_COUNT - HOTBAR_COUNT))
        return sf::FloatRect({0,0}, {0,0});

    const float B_SLOT = 54.f, B_GAP = 6.f;
    float gridW = BACKPACK_COLS * B_SLOT + (BACKPACK_COLS - 1) * B_GAP;
    float gridH = BACKPACK_ROWS * B_SLOT + (BACKPACK_ROWS - 1) * B_GAP;
    float titleH = 26.f, padT = 6.f, padB = 10.f;
    float trashH = 48.f, gap = 30.f;
    float panelH = titleH + padT + gridH + padB;
    float totalH = panelH + trashH + gap + static_cast<float>(SLOT_SIZE);
    float baseY  = (m_winH - totalH) / 2.f;
    float panelY = baseY;
    float panelX = (m_winW - gridW) / 2.f;

    int col = idx % BACKPACK_COLS;
    int row = idx / BACKPACK_COLS;
    return sf::FloatRect(sf::Vector2f(panelX + col * (B_SLOT + B_GAP),
                                       panelY + titleH + padT + row * (B_SLOT + B_GAP)),
                         sf::Vector2f(B_SLOT, B_SLOT));
}

sf::FloatRect Inventory::getTrashSlotRect() const
{
    const float B_SLOT = 54.f, B_GAP = 6.f;
    float gridH  = BACKPACK_ROWS * B_SLOT + (BACKPACK_ROWS - 1) * B_GAP;
    float titleH = 26.f, padT = 6.f, padB = 10.f;
    float trashH = 48.f, trashW = 48.f;
    float gap    = 10.f;
    float panelH = titleH + padT + gridH + padB;
    float totalH = panelH + trashH + gap + static_cast<float>(SLOT_SIZE);
    float baseY  = (m_winH - totalH) / 2.f;
    float trashY = baseY + panelH + 3.f;

    float gridW  = BACKPACK_COLS * B_SLOT + (BACKPACK_COLS - 1) * B_GAP;
    float panelR = (m_winW + gridW) / 2.f;
    return sf::FloatRect(sf::Vector2f(panelR - trashW, trashY),
                         sf::Vector2f(trashW, trashH));
}

// ====== 交互 ======

void Inventory::pickUp(int slot, int count)
{
    if (m_items[slot] == ItemType::Empty) return;
    int take = std::min(count, m_counts[slot]);
    m_cursorType  = m_items[slot];
    m_cursorCount = take;
    m_counts[slot] -= take;
    if (m_counts[slot] <= 0) m_items[slot] = ItemType::Empty;
}

void Inventory::placeOne(int slot)
{
    if (m_cursorCount <= 0) return;
    if (m_items[slot] == ItemType::Empty)
    { m_items[slot] = m_cursorType; m_counts[slot] = 1; m_cursorCount--; }
    else if (m_items[slot] == m_cursorType)
    { m_counts[slot]++; m_cursorCount--; }
    if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;
}

void Inventory::placeAll(int slot)
{
    if (m_cursorCount <= 0) return;

    if (m_items[slot] == m_cursorType)
    {
        int maxStack = maxStackSize(m_cursorType);
        int space = maxStack - m_counts[slot];
        int add = std::min(space, m_cursorCount);
        m_counts[slot] += add;
        m_cursorCount  -= add;
        if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;
    }
    else if (m_items[slot] == ItemType::Empty)
    {
        int maxStack = maxStackSize(m_cursorType);
        int add = std::min(maxStack, m_cursorCount);
        m_items[slot] = m_cursorType;
        m_counts[slot] = add;
        m_cursorCount  -= add;
        if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;
    }
    else
    { std::swap(m_cursorType, m_items[slot]); std::swap(m_cursorCount, m_counts[slot]); }
}

void Inventory::throwCursorItem(int count)
{
    if (m_cursorCount <= 0) return;
    int toss = std::min(count, m_cursorCount);
    m_cursorCount -= toss;
    if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;
}

void Inventory::resetInteraction()
{
    m_mouseDown = false;
    m_dragFromSlot = -1;
    m_splitMode = false;
    m_draggingSlider = false;
    m_hoveredSlot = -1;
    if (m_splitDialogOpen) closeSplitDialog(false);
}

bool Inventory::addItemAuto(ItemType type, int count)
{
    int maxStack = maxStackSize(type);
    int remaining = count;

    // 1. 先尝试合并到已有同类格子
    for (int i = 0; i < SLOT_COUNT && remaining > 0; ++i)
    {
        if (m_items[i] == type && m_counts[i] < maxStack)
        {
            int space = maxStack - m_counts[i];
            int add   = std::min(space, remaining);
            m_counts[i] += add;
            remaining   -= add;
        }
    }

    // 2. 多余的放空格子
    for (int i = 0; i < SLOT_COUNT && remaining > 0; ++i)
    {
        if (m_items[i] == ItemType::Empty)
        {
            int add = std::min(maxStack, remaining);
            m_items[i] = type;
            m_counts[i] = add;
            remaining  -= add;
        }
    }

    return remaining == 0; // true=全部捡起, false=背包满了
}

std::vector<ThrowEntry> Inventory::drainThrowQueue()
{
    auto q = std::move(m_throwQueue);
    m_throwQueue.clear();
    return q;
}

void Inventory::onMouseDown(bool ctrlHeld, const sf::Vector2i& mousePos)
{
    if (!m_open) return;

    // 拆分对话框打开时
    if (m_splitDialogOpen)
    {
        float dlgX = (m_winW - 320.f) / 2.f, dlgY = (m_winH - 180.f) / 2.f;

        int handleX = splitAmountToSlider(m_splitAmount);
        if (std::abs(mousePos.x - handleX) < 20.f && mousePos.y > dlgY + 60.f && mousePos.y < dlgY + 110.f)
        { m_draggingSlider = true; m_inputFocused = false; return; }

        if (mousePos.x > dlgX + 130.f && mousePos.x < dlgX + 190.f && mousePos.y > dlgY + 38.f && mousePos.y < dlgY + 66.f)
        { m_inputFocused = true; return; }

        m_inputFocused = false;

        if (mousePos.x > dlgX + 55.f && mousePos.x < dlgX + 140.f && mousePos.y > dlgY + 105.f && mousePos.y < dlgY + 130.f)
        { closeSplitDialog(true); return; }

        if (mousePos.x > dlgX + 175.f && mousePos.x < dlgX + 260.f && mousePos.y > dlgY + 105.f && mousePos.y < dlgY + 130.f)
        { closeSplitDialog(false); return; }
        return;
    }

    m_mouseDown = true;
    m_dragFromSlot = m_hoveredSlot;

    // 点击垃圾桶：拿起里面的物品
    if (m_trashItem != ItemType::Empty && getTrashSlotRect().contains(sf::Vector2f(mousePos)))
    {
        if (m_cursorCount > 0)
        {
            // 手里有物品 → 交换
            std::swap(m_cursorType, m_trashItem);
            std::swap(m_cursorCount, m_trashCount);
        }
        else
        {
            // 手里空 → 拿出
            m_cursorType = m_trashItem; m_cursorCount = m_trashCount;
            m_trashItem = ItemType::Empty; m_trashCount = 0;
        }
        m_dragFromSlot = -1;
        return;
    }

    m_splitMode = ctrlHeld && m_hoveredSlot >= 0 && m_cursorCount == 0
                  && m_counts[m_hoveredSlot] >= 2;

    if (m_hoveredSlot < 0) { m_splitMode = false; return; }

    if (m_cursorCount > 0)
    {
        if (m_items[m_hoveredSlot] == m_cursorType || m_items[m_hoveredSlot] == ItemType::Empty)
            placeAll(m_hoveredSlot);
        else
            placeAll(m_hoveredSlot);
    }
    else
    {
        pickUp(m_hoveredSlot, m_counts[m_hoveredSlot]);
    }
}

void Inventory::onMouseUp(bool ctrlHeld, const sf::Vector2i& mousePos)
{
    m_mouseDown = false;
    m_draggingSlider = false;

    if (!m_open || m_splitDialogOpen) return;
    if (m_cursorCount == 0) { m_splitMode = false; return; }

    // Ctrl+拖到空格子 → 弹出拆分对话框
    if (m_splitMode && m_hoveredSlot >= 0 && m_items[m_hoveredSlot] == ItemType::Empty
        && m_dragFromSlot >= 0 && m_dragFromSlot != m_hoveredSlot)
    {
        openSplitDialog(m_dragFromSlot, m_hoveredSlot);
        m_splitMode = false;
        m_dragFromSlot = -1;
        return;
    }

    m_splitMode = false;

    // Ctrl + 拖到垃圾桶 → 拆分数对话框
    if (getTrashSlotRect().contains(sf::Vector2f(mousePos)) && m_cursorCount > 0 && ctrlHeld)
    {
        openSplitDialog(m_dragFromSlot, -2); // target=-2 表示"垃圾桶"
        m_dragFromSlot = -1;
        return;
    }

    // 拖到垃圾桶 → 全部放入
    if (getTrashSlotRect().contains(sf::Vector2f(mousePos)) && m_cursorCount > 0)
    {
        m_trashItem  = m_cursorType;
        m_trashCount = m_cursorCount;
        m_cursorType  = ItemType::Empty;
        m_cursorCount = 0;
        m_dragFromSlot = -1;
        return;
    }

    if (m_hoveredSlot >= 0)
    {
        placeAll(m_hoveredSlot);
    }

    // Ctrl + 拖到外面 → 拆分数对话框
    if (m_cursorCount > 0 && m_hoveredSlot < 0 && ctrlHeld)
    {
        openSplitDialog(m_dragFromSlot, -1);
        m_dragFromSlot = -1;
        return;
    }

    // 手里还有物品且在外面 → 扔到地上
    if (m_cursorCount > 0 && m_hoveredSlot < 0)
    {
        m_throwQueue.push_back({m_cursorType, m_cursorCount});
        m_cursorType  = ItemType::Empty;
        m_cursorCount = 0;
    }

    // 手里还有且在同格 → 归位
    if (m_cursorCount > 0 && m_dragFromSlot >= 0)
    {
        if (m_items[m_dragFromSlot] == m_cursorType)
        { m_counts[m_dragFromSlot] += m_cursorCount; m_cursorType = ItemType::Empty; m_cursorCount = 0; }
        else if (m_items[m_dragFromSlot] == ItemType::Empty)
        { m_items[m_dragFromSlot] = m_cursorType; m_counts[m_dragFromSlot] = m_cursorCount; m_cursorType = ItemType::Empty; m_cursorCount = 0; }
        else
            placeAll(m_dragFromSlot);
    }

    m_dragFromSlot = -1;
}

void Inventory::onThrowKey()
{
    if (m_throwCooldown > 0.f) return;
    m_throwCooldown = 0.15f;
    int slot = m_open ? m_hoveredSlot : m_selectedSlot;
    if (slot < 0 || m_items[slot] == ItemType::Empty) return;

    // 通过扔出队列 → 在场景中生成 DroppedItem
    m_throwQueue.push_back({m_items[slot], 1});
    removeItem(slot, 1);
}

// ====== 更新 ======

void Inventory::update(float dt)
{
    m_throwCooldown -= dt;
    m_blinkTimer += dt;
    if (m_blinkTimer > 1.0f) m_blinkTimer -= 1.0f; // 1 秒一个周期
}

void Inventory::updateHover(const sf::Vector2i& mousePos)
{
    if (m_draggingSlider) { updateSplitSlider(mousePos); return; }
    if (m_splitDialogOpen) { m_hoveredSlot = -1; return; }

    m_hoveredSlot = -1;

    // 背包打开时先检查背包面板槽位(8-31)，再检查热键栏(0-7)
    if (m_open)
    {
        for (int i = HOTBAR_COUNT; i < SLOT_COUNT; ++i)
            if (getBackpackSlotRect(i).contains(sf::Vector2f(mousePos)))
            { m_hoveredSlot = i; return; }
    }
    for (int i = 0; i < HOTBAR_COUNT; ++i)
        if (getHotbarSlotRect(i).contains(sf::Vector2f(mousePos)))
        { m_hoveredSlot = i; return; }
}

void Inventory::onTextEntered(char32_t codepoint)
{
    if (!m_splitDialogOpen || !m_inputFocused) return;

    if (codepoint == '\b') // 退格
    {
        if (!m_splitInput.empty()) m_splitInput.pop_back();
    }
    else if (codepoint >= '0' && codepoint <= '9')
    {
        m_splitInput += static_cast<char>(codepoint);
    }

    updateSplitFromInput();
}

// ====== 拆分对话框 ======

void Inventory::openSplitDialog(int sourceSlot, int targetSlot)
{
    m_splitDialogOpen = true;
    m_splitSourceSlot = sourceSlot;
    m_splitTargetSlot = targetSlot;
    m_splitMax = m_cursorCount; // 光标上的物品总数
    m_splitAmount = m_splitMax / 2;
    m_splitInput = std::to_string(m_splitAmount);
    m_draggingSlider = false;
}

void Inventory::closeSplitDialog(bool confirm)
{
    // 确认前同步输入框
    updateSplitFromInput();
    if (m_splitAmount < 1) m_splitAmount = 1;

    // targetSlot==-1 扔地上, targetSlot==-2 放垃圾桶
    if (confirm && m_splitSourceSlot >= 0 && m_splitTargetSlot < 0
        && m_cursorCount > 0 && m_splitAmount > 0)
    {
        int toss = std::min(m_splitAmount, m_cursorCount);

        if (m_splitTargetSlot == -2) // 垃圾桶
        {
            m_trashItem  = m_cursorType;
            m_trashCount = toss;
        }
        else // -1: 扔地上
        {
            m_throwQueue.push_back({m_cursorType, toss});
        }

        m_cursorCount -= toss;
        if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;

        // 剩余归位
        if (m_cursorCount > 0)
        {
            if (m_items[m_splitSourceSlot] == m_cursorType)
                m_counts[m_splitSourceSlot] += m_cursorCount;
            else if (m_items[m_splitSourceSlot] == ItemType::Empty)
            { m_items[m_splitSourceSlot] = m_cursorType; m_counts[m_splitSourceSlot] = m_cursorCount; }
            m_cursorType = ItemType::Empty; m_cursorCount = 0;
        }
    }
    else if (confirm && m_splitSourceSlot >= 0 && m_splitTargetSlot >= 0
             && m_cursorCount > 0 && m_splitAmount > 0)
    {
        int toTarget = std::min(m_splitAmount, m_cursorCount);
        m_items[m_splitTargetSlot] = m_cursorType;
        m_counts[m_splitTargetSlot] = toTarget;
        m_cursorCount -= toTarget;
        if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;

        if (m_cursorCount > 0)
        {
            if (m_items[m_splitSourceSlot] == m_cursorType)
                m_counts[m_splitSourceSlot] += m_cursorCount;
            else if (m_items[m_splitSourceSlot] == ItemType::Empty)
            { m_items[m_splitSourceSlot] = m_cursorType; m_counts[m_splitSourceSlot] = m_cursorCount; }
            m_cursorType = ItemType::Empty; m_cursorCount = 0;
        }
    }
    else
    {
        // 取消或无效：全部还给原格
        if (m_cursorCount > 0 && m_splitSourceSlot >= 0)
        {
            if (m_items[m_splitSourceSlot] == m_cursorType)
                m_counts[m_splitSourceSlot] += m_cursorCount;
            else if (m_items[m_splitSourceSlot] == ItemType::Empty)
            { m_items[m_splitSourceSlot] = m_cursorType; m_counts[m_splitSourceSlot] = m_cursorCount; }
            m_cursorType = ItemType::Empty; m_cursorCount = 0;
        }
    }

    m_splitDialogOpen = false;
    m_splitSourceSlot = -1;
    m_splitTargetSlot = -1;
    m_splitAmount     = 0;
    m_splitMax        = 0;
    m_splitInput.clear();
    m_draggingSlider  = false;
}

void Inventory::updateSplitSlider(const sf::Vector2i& mousePos)
{
    m_splitAmount = splitSliderToAmount(mousePos.x);
    m_splitInput = std::to_string(m_splitAmount);
}

void Inventory::updateSplitFromInput()
{
    if (m_splitInput.empty()) { m_splitAmount = 1; return; }

    int val = 0;
    try { val = std::stoi(m_splitInput); } catch (...) { m_splitAmount = 1; return; }

    if (val < 1) val = 1;
    if (val > m_splitMax) { val = m_splitMax; m_splitInput = std::to_string(val); }
    m_splitAmount = val;
}

int Inventory::splitSliderToAmount(int sliderX) const
{
    float dlgX  = (m_winW - 320.f) / 2.f;
    float barX  = dlgX + 40.f;
    float barW  = 320.f - 80.f;
    float t = std::clamp((sliderX - barX) / barW, 0.f, 1.f);
    return m_splitMax > 1 ? static_cast<int>(1 + t * (m_splitMax - 1) + 0.5f) : 1;
}

int Inventory::splitAmountToSlider(int amount) const
{
    float dlgX = (m_winW - 320.f) / 2.f;
    float barX = dlgX + 40.f;
    float barW = 320.f - 80.f;
    if (m_splitMax <= 1) return static_cast<int>(barX);
    float t = static_cast<float>(amount - 1) / (m_splitMax - 1);
    return static_cast<int>(barX + t * barW);
}

void Inventory::renderSplitDialog(sf::RenderWindow& window)
{
    if (!m_splitDialogOpen) return;

    sf::RectangleShape overlay(sf::Vector2f(m_winW, m_winH));
    overlay.setFillColor(sf::Color(0, 0, 0, 140));
    window.draw(overlay);

    const float DLG_W = 320.f, DLG_H = 180.f;
    float dlgX = (m_winW - DLG_W) / 2.f;
    float dlgY = (m_winH - DLG_H) / 2.f;

    sf::RectangleShape dlgBg(sf::Vector2f(DLG_W, DLG_H));
    dlgBg.setPosition(sf::Vector2f(dlgX, dlgY));
    dlgBg.setFillColor(sf::Color(50, 50, 50, 240));
    dlgBg.setOutlineColor(sf::Color(150, 150, 150, 255));
    dlgBg.setOutlineThickness(2.f);
    window.draw(dlgBg);

    if (!m_hasFont) return;

    sf::Text title(m_font);
    title.setString(toSfString("拆分物品"));
    title.setCharacterSize(18);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(dlgX + DLG_W / 2.f - title.getLocalBounds().size.x / 2.f, dlgY + 8.f));
    window.draw(title);

    // 数字输入框
    sf::RectangleShape inputBg(sf::Vector2f(60.f, 28.f));
    inputBg.setPosition(sf::Vector2f(dlgX + DLG_W / 2.f - 30.f, dlgY + 38.f));
    inputBg.setFillColor(sf::Color(30, 30, 30, 255));
    inputBg.setOutlineColor(m_inputFocused ? sf::Color::White : sf::Color(140, 140, 140, 255));
    inputBg.setOutlineThickness(m_inputFocused ? 2.f : 1.f);
    window.draw(inputBg);

    sf::Text inputText(m_font);
    inputText.setString(m_splitInput);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    float ix = dlgX + DLG_W / 2.f - inputText.getLocalBounds().size.x / 2.f;
    inputText.setPosition(sf::Vector2f(ix, dlgY + 40.f));
    window.draw(inputText);

    if (m_inputFocused && m_blinkTimer < 0.5f)
    {
        sf::FloatRect tb = inputText.getLocalBounds();
        float cursorX = ix + tb.size.x + 2.f;
        sf::RectangleShape cursor(sf::Vector2f(2.f, 22.f));
        cursor.setPosition(sf::Vector2f(cursorX, dlgY + 40.f));
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    // 滑条
    const float barX = dlgX + 40.f, barY = dlgY + 80.f, barW = DLG_W - 80.f, barH = 10.f;
    sf::RectangleShape track(sf::Vector2f(barW, barH));
    track.setPosition(sf::Vector2f(barX, barY));
    track.setFillColor(sf::Color(80, 80, 80, 255));
    window.draw(track);

    int handleX = splitAmountToSlider(m_splitAmount);
    float filledW = handleX - barX;
    if (filledW > 0.f)
    {
        sf::RectangleShape filled(sf::Vector2f(filledW, barH));
        filled.setPosition(sf::Vector2f(barX, barY));
        filled.setFillColor(sf::Color(100, 180, 255, 200));
        window.draw(filled);
    }

    const float handleR = 10.f;
    sf::CircleShape handle(handleR);
    handle.setPosition(sf::Vector2f(static_cast<float>(handleX) - handleR, barY - handleR + barH / 2.f));
    handle.setFillColor(sf::Color(200, 200, 200, 255));
    handle.setOutlineColor(sf::Color::White);
    handle.setOutlineThickness(1.f);
    window.draw(handle);

    sf::Text minText(m_font); minText.setString("1");
    minText.setCharacterSize(12); minText.setFillColor(sf::Color(180, 180, 180));
    minText.setPosition(sf::Vector2f(barX - 15.f, barY - 5.f)); window.draw(minText);

    sf::Text maxText(m_font); maxText.setString(std::to_string(m_splitMax));
    maxText.setCharacterSize(12); maxText.setFillColor(sf::Color(180, 180, 180));
    maxText.setPosition(sf::Vector2f(barX + barW + 5.f, barY - 5.f)); window.draw(maxText);

    sf::Text confirmText(m_font); confirmText.setString(toSfString("[ 确认 ]"));
    confirmText.setCharacterSize(16); confirmText.setFillColor(sf::Color(100, 255, 100));
    confirmText.setPosition(sf::Vector2f(dlgX + 60.f, dlgY + 110.f)); window.draw(confirmText);

    sf::Text cancelText(m_font); cancelText.setString(toSfString("[ 取消 ]"));
    cancelText.setCharacterSize(16); cancelText.setFillColor(sf::Color(255, 100, 100));
    cancelText.setPosition(sf::Vector2f(dlgX + 180.f, dlgY + 110.f)); window.draw(cancelText);
}

// ====== 渲染 ======

void Inventory::render(sf::RenderWindow& window)
{
    if (m_open) renderBackpack(window);
    renderHotbar(window);
    if (m_cursorCount > 0 && !m_splitDialogOpen) renderCursorItem(window);
    if (m_splitDialogOpen) renderSplitDialog(window);
    renderTooltip(window); // 最顶层
}

void Inventory::renderHotbar(sf::RenderWindow& window)
{
    // 底部 8 格快捷栏
    for (int i = 0; i < HOTBAR_COUNT; ++i)
        drawSlot(window, i, getHotbarSlotRect(i));
}

void Inventory::renderBackpack(sf::RenderWindow& window)
{
    sf::RectangleShape overlay(sf::Vector2f(m_winW, m_winH));
    overlay.setFillColor(sf::Color(0, 0, 0, 100)); window.draw(overlay);

    // 布局常量
    const float B_SLOT = 54.f, B_GAP = 6.f;
    float gridW  = 8.f * B_SLOT + 7.f * B_GAP;
    float gridH  = 3.f * B_SLOT + 2.f * B_GAP;
    float titleH = 26.f, padT = 6.f, padB = 10.f, padH = 20.f;
    float panelW = gridW + padH * 2.f;
    float panelH = titleH + padT + gridH + padB;
    float panelX = (m_winW - panelW) / 2.f;

    float trashH = 48.f, trashW = 48.f, gap = 30.f;
    float totalH = panelH + trashH + gap + SLOT_SIZE;
    float baseY  = (m_winH - totalH) / 2.f;
    float panelY = baseY;
    float hotbarY = baseY + panelH + trashH + gap;

    // 背包矩形背景
    sf::RectangleShape panelBg(sf::Vector2f(panelW, panelH));
    panelBg.setPosition(sf::Vector2f(panelX, panelY));
    panelBg.setFillColor(sf::Color(60, 60, 60, 240));
    panelBg.setOutlineColor(sf::Color(130, 130, 130, 255));
    panelBg.setOutlineThickness(3.f);
    window.draw(panelBg);

    // 物品栏仅在位置上移，不加额外背景
    if (m_hasFont)
    {
        sf::Text title(m_font); title.setString(toSfString("物品栏"));
        title.setCharacterSize(18); title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(m_winW / 2.f - title.getLocalBounds().size.x / 2.f, panelY + 4.f));
        window.draw(title);
    }

    // 24 个格子
    // 背包面板只画槽位 8-31（3行×8列）
    for (int i = HOTBAR_COUNT; i < SLOT_COUNT; ++i)
        drawSlot(window, i, getBackpackSlotRect(i));

    // 垃圾桶
    {
        sf::FloatRect tr = getTrashSlotRect();
        sf::RectangleShape trashBg(tr.size);
        trashBg.setPosition(tr.position);
        trashBg.setFillColor(sf::Color(40, 30, 30, 200));
        trashBg.setOutlineColor(sf::Color(150, 80, 80, 200));
        trashBg.setOutlineThickness(2.f);
        window.draw(trashBg);

        if (m_trashItem != ItemType::Empty)
        {
            float tiSize = tr.size.x * 0.55f;
            sf::RectangleShape icon(sf::Vector2f(tiSize, tiSize));
            icon.setOrigin(sf::Vector2f(tiSize / 2.f, tiSize / 2.f));
            icon.setPosition(sf::Vector2f(tr.position.x + tr.size.x / 2.f,
                                           tr.position.y + tr.size.y / 2.f));
            switch (m_trashItem)
            {
            case ItemType::Hoe: icon.setFillColor(sf::Color(150,150,150)); break;
            case ItemType::WateringCan: icon.setFillColor(sf::Color(80,130,200)); break;
            case ItemType::Seeds: icon.setFillColor(sf::Color(200,180,80)); break;
            case ItemType::Turnip: icon.setFillColor(sf::Color(255,100,100)); break;
            default: break;
            }
            window.draw(icon);
        }

        if (m_hasFont)
        {
            sf::Text trashLabel(m_font);
            trashLabel.setString(toSfString("垃圾桶"));
            trashLabel.setCharacterSize(10);
            trashLabel.setFillColor(sf::Color(200, 120, 120));
            trashLabel.setPosition(sf::Vector2f(tr.position.x + 2.f, tr.position.y + tr.size.y + 2.f));
            window.draw(trashLabel);
        }
    }

}

void Inventory::drawSlot(sf::RenderWindow& window, int i, const sf::FloatRect& rect)
{
    if (m_items[i] == ItemType::Empty)
    {
        sf::RectangleShape emptyBg(rect.size);
        emptyBg.setPosition(rect.position);
        emptyBg.setFillColor(sf::Color(38, 38, 38));
        emptyBg.setOutlineColor(sf::Color(55, 55, 55));
        emptyBg.setOutlineThickness(1.f);
        window.draw(emptyBg);

        if (i == m_selectedSlot)
        { m_selectedShape.setSize(rect.size); m_selectedShape.setPosition(rect.position); window.draw(m_selectedShape); }

        return;
    }

    m_slotShape.setSize(rect.size); m_slotShape.setPosition(rect.position); window.draw(m_slotShape);

    if (i == m_selectedSlot)
    { m_selectedShape.setSize(rect.size); m_selectedShape.setPosition(rect.position); window.draw(m_selectedShape); }

    if (m_open && i == m_hoveredSlot)
    { m_hoverShape.setSize(rect.size); m_hoverShape.setPosition(rect.position); window.draw(m_hoverShape); }

    float fs = rect.size.x;
    float iconSize = fs * 0.55f;
    sf::RectangleShape icon(sf::Vector2f(iconSize, iconSize));
    icon.setOrigin(sf::Vector2f(iconSize / 2.f, iconSize / 2.f));
    icon.setPosition(sf::Vector2f(rect.position.x + fs / 2.f, rect.position.y + fs / 2.f));
    switch (m_items[i])
    {
    case ItemType::Hoe:         icon.setFillColor(sf::Color(150,150,150)); break;
    case ItemType::WateringCan: icon.setFillColor(sf::Color(80,130,200));  break;
    case ItemType::Seeds:       icon.setFillColor(sf::Color(200,180,80));  break;
    case ItemType::Turnip:      icon.setFillColor(sf::Color(255,100,100)); break;
    default:                    icon.setFillColor(sf::Color::Magenta);     break;
    }
    window.draw(icon);

    if (m_hasFont)
    {
        if (m_counts[i] > 1)
        {
            sf::Text countText(m_font); countText.setString(std::to_string(m_counts[i]));
            countText.setCharacterSize(static_cast<unsigned int>(fs * 0.24f));
            countText.setFillColor(sf::Color(220,220,220));
            float tw = countText.getLocalBounds().size.x;
            countText.setPosition(sf::Vector2f(rect.position.x + fs - tw - 3.f, rect.position.y + fs - 13.f));
            window.draw(countText);
        }
    }
}

void Inventory::renderTooltip(sf::RenderWindow& window) const
{
    if (m_hoveredSlot < 0 || !m_hasFont) return;
    if (m_items[m_hoveredSlot] == ItemType::Empty) return;
    if (!m_open && m_hoveredSlot >= HOTBAR_COUNT) return;

    sf::FloatRect slotRect = (m_hoveredSlot >= HOTBAR_COUNT)
        ? getBackpackSlotRect(m_hoveredSlot)
        : getHotbarSlotRect(m_hoveredSlot);

    const char* name = itemName(m_items[m_hoveredSlot]);
    bool showProps = m_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);

    if (showProps)
    {
        // 属性模式：仅显示"属性"二字
        sf::Text titleText(m_font); titleText.setString(toSfString("属性"));
        titleText.setCharacterSize(13); titleText.setFillColor(sf::Color(200, 180, 255));

        float bgW = titleText.getLocalBounds().size.x + 14.f;
        float bgH = titleText.getLocalBounds().size.y + 10.f;
        float tipX = slotRect.position.x + slotRect.size.x + 6.f;
        float tipY = slotRect.position.y + slotRect.size.y / 2.f - bgH / 2.f;
        if (tipX + bgW > m_winW) tipX = slotRect.position.x - bgW - 6.f;
        if (tipY < 0.f) tipY = 2.f;

        sf::RectangleShape tipBg(sf::Vector2f(bgW, bgH));
        tipBg.setPosition(sf::Vector2f(tipX, tipY));
        tipBg.setFillColor(sf::Color(20, 20, 20, 235));
        tipBg.setOutlineColor(sf::Color(100, 80, 160, 220));
        tipBg.setOutlineThickness(1.f);
        window.draw(tipBg);

        titleText.setPosition(sf::Vector2f(tipX + 7.f, tipY + 4.f));
        window.draw(titleText);
    }
    else
    {
        // 仅名称
        sf::Text nameText(m_font); nameText.setString(toSfString(name));
        nameText.setCharacterSize(13); nameText.setFillColor(sf::Color(255, 255, 200));

        float bgW = nameText.getLocalBounds().size.x + 12.f;
        float bgH = nameText.getLocalBounds().size.y + 8.f;
        float tipX = slotRect.position.x + slotRect.size.x + 6.f;
        float tipY = slotRect.position.y + slotRect.size.y / 2.f - bgH / 2.f;
        if (tipX + bgW > m_winW) tipX = slotRect.position.x - bgW - 6.f;
        if (tipY < 0.f) tipY = 2.f;

        sf::RectangleShape tipBg(sf::Vector2f(bgW, bgH));
        tipBg.setPosition(sf::Vector2f(tipX, tipY));
        tipBg.setFillColor(sf::Color(20, 20, 20, 235));
        tipBg.setOutlineColor(sf::Color(80, 80, 80, 180));
        tipBg.setOutlineThickness(1.f);
        window.draw(tipBg);

        nameText.setPosition(sf::Vector2f(tipX + 5.f, tipY + 3.f));
        window.draw(nameText);
    }
}

void Inventory::renderCursorItem(sf::RenderWindow& window) const
{
    sf::Vector2i rawMouse = sf::Mouse::getPosition(window);
    float x = static_cast<float>(rawMouse.x) - 16.f, y = static_cast<float>(rawMouse.y) - 16.f;
    sf::RectangleShape icon(sf::Vector2f(32.f, 32.f));
    icon.setPosition(sf::Vector2f(x, y));
    switch (m_cursorType)
    {
    case ItemType::Hoe: icon.setFillColor(sf::Color(150,150,150)); break;
    case ItemType::WateringCan: icon.setFillColor(sf::Color(80,130,200)); break;
    case ItemType::Seeds: icon.setFillColor(sf::Color(200,180,80)); break;
    case ItemType::Turnip: icon.setFillColor(sf::Color(255,100,100)); break;
    default: break;
    }
    window.draw(icon);
    if (m_hasFont && m_cursorCount > 1)
    {
        sf::Text countText(m_font); countText.setString(std::to_string(m_cursorCount));
        countText.setCharacterSize(14); countText.setFillColor(sf::Color::White);
        countText.setPosition(sf::Vector2f(x + 34.f, y + 20.f)); window.draw(countText);
    }
}
