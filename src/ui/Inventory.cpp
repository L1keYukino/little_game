#include "Inventory.hpp"
#include <cstring>
#include <algorithm>

static sf::String toSfString(const char* utf8)
{
    return sf::String::fromUtf8(utf8, utf8 + std::strlen(utf8));
}

Inventory::Inventory()
{
    m_items[0] = ItemType::Hoe;         m_counts[0] = 1;
    m_items[1] = ItemType::WateringCan; m_counts[1] = 1;
    m_items[2] = ItemType::Seeds;       m_counts[2] = 10;

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
    float totalWidth = SLOT_COUNT * SLOT_SIZE + (SLOT_COUNT - 1) * PADDING;
    float startX = (800.f - totalWidth) / 2.f;
    float y = 600.f - SLOT_SIZE - 12.f;
    return sf::FloatRect(sf::Vector2f(startX + slot * (SLOT_SIZE + PADDING), y),
                         sf::Vector2f(SLOT_SIZE, SLOT_SIZE));
}

sf::FloatRect Inventory::getBackpackSlotRect(int slot) const
{
    const float B_SLOT = 72.f, B_GAP = 8.f, COLS = 4.f;
    float gridW = COLS * B_SLOT + (COLS - 1) * B_GAP;
    float panelX = 400.f - gridW / 2.f, panelY = 220.f;
    int col = slot % 4, row = slot / 4;
    return sf::FloatRect(sf::Vector2f(panelX + col * (B_SLOT + B_GAP),
                                       panelY + row * (B_SLOT + B_GAP)),
                         sf::Vector2f(B_SLOT, B_SLOT));
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
    { m_counts[slot] += m_cursorCount; m_cursorType = ItemType::Empty; m_cursorCount = 0; }
    else if (m_items[slot] == ItemType::Empty)
    { m_items[slot] = m_cursorType; m_counts[slot] = m_cursorCount; m_cursorType = ItemType::Empty; m_cursorCount = 0; }
    else
    { std::swap(m_cursorType, m_items[slot]); std::swap(m_cursorCount, m_counts[slot]); }
}

void Inventory::throwCursorItem(int count)
{
    if (m_cursorCount <= 0) return;
    m_cursorCount -= std::min(count, m_cursorCount);
    if (m_cursorCount <= 0) m_cursorType = ItemType::Empty;
}

void Inventory::onMouseDown(bool ctrlHeld, const sf::Vector2i& mousePos)
{
    if (!m_open) return;

    // 拆分对话框打开时
    if (m_splitDialogOpen)
    {
        // 检测滑条手柄点击
        int handleX = splitAmountToSlider(m_splitAmount);
        if (std::abs(mousePos.x - handleX) < 20.f && mousePos.y > 260.f && mousePos.y < 310.f)
        { m_draggingSlider = true; m_inputFocused = false; return; }

        // 点击输入框 → 聚焦
        if (mousePos.x > 370.f && mousePos.x < 430.f && mousePos.y > 238.f && mousePos.y < 266.f)
        { m_inputFocused = true; return; }

        // 点击其他地方 → 取消聚焦
        m_inputFocused = false;

        // 确认按钮
        if (mousePos.x > 295.f && mousePos.x < 380.f && mousePos.y > 305.f && mousePos.y < 330.f)
        { closeSplitDialog(true); return; }
        // 取消按钮
        if (mousePos.x > 415.f && mousePos.x < 500.f && mousePos.y > 305.f && mousePos.y < 330.f)
        { closeSplitDialog(false); return; }
        return;
    }

    m_mouseDown = true;
    m_dragFromSlot = m_hoveredSlot;
    m_splitMode = ctrlHeld && m_hoveredSlot >= 0 && m_cursorCount == 0
                  && m_counts[m_hoveredSlot] >= 2; // Ctrl+按住准备拆分

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

void Inventory::onMouseUp(bool ctrlHeld, const sf::Vector2i& /*mousePos*/)
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

    if (m_hoveredSlot >= 0)
    {
        placeAll(m_hoveredSlot);
    }

    // 如果手里还有 → 归位到原格子
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
    if (!m_open || m_splitDialogOpen) { m_hoveredSlot = -1; return; }
    m_hoveredSlot = -1;
    for (int i = 0; i < SLOT_COUNT; ++i)
        if (getBackpackSlotRect(i).contains(sf::Vector2f(mousePos)))
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
    // 确认前用输入框内容同步一次数值（防止删空后直接点确认拿到旧值）
    updateSplitFromInput();
    if (m_splitAmount < 1) m_splitAmount = 1;

    if (confirm && m_splitSourceSlot >= 0 && m_splitTargetSlot >= 0
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
    const float barX = 280.f, barW = 240.f;
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
    const float barX = 280.f, barW = 240.f;
    float t = std::clamp((sliderX - barX) / barW, 0.f, 1.f);
    return m_splitMax > 1 ? static_cast<int>(1 + t * (m_splitMax - 1) + 0.5f) : 1;
}

int Inventory::splitAmountToSlider(int amount) const
{
    const float barX = 280.f, barW = 240.f;
    if (m_splitMax <= 1) return static_cast<int>(barX);
    float t = static_cast<float>(amount - 1) / (m_splitMax - 1);
    return static_cast<int>(barX + t * barW);
}

void Inventory::renderSplitDialog(sf::RenderWindow& window)
{
    if (!m_splitDialogOpen) return;

    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 140));
    window.draw(overlay);

    sf::RectangleShape dlgBg(sf::Vector2f(320.f, 180.f));
    dlgBg.setPosition(sf::Vector2f(240.f, 200.f));
    dlgBg.setFillColor(sf::Color(50, 50, 50, 240));
    dlgBg.setOutlineColor(sf::Color(150, 150, 150, 255));
    dlgBg.setOutlineThickness(2.f);
    window.draw(dlgBg);

    if (!m_hasFont) return;

    sf::Text title(m_font);
    title.setString(toSfString("拆分物品"));
    title.setCharacterSize(18);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(400.f - title.getLocalBounds().size.x / 2.f, 208.f));
    window.draw(title);

    // 数字输入框
    sf::RectangleShape inputBg(sf::Vector2f(60.f, 28.f));
    inputBg.setPosition(sf::Vector2f(370.f, 238.f));
    inputBg.setFillColor(sf::Color(30, 30, 30, 255));
    inputBg.setOutlineColor(m_inputFocused ? sf::Color::White
                                           : sf::Color(140, 140, 140, 255));
    inputBg.setOutlineThickness(m_inputFocused ? 2.f : 1.f);
    window.draw(inputBg);

    // 数字文本
    sf::Text inputText(m_font);
    inputText.setString(m_splitInput);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    float ix = 400.f - inputText.getLocalBounds().size.x / 2.f;
    inputText.setPosition(sf::Vector2f(ix, 240.f));
    window.draw(inputText);

    // 闪烁光标：贴着文本末尾的细竖线
    if (m_inputFocused && m_blinkTimer < 0.5f)
    {
        sf::FloatRect tb = inputText.getLocalBounds();
        float cursorX = ix + tb.size.x + 2.f;
        sf::RectangleShape cursor(sf::Vector2f(2.f, 22.f));
        cursor.setPosition(sf::Vector2f(cursorX, 240.f));
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    // 滑条
    const float barX = 280.f, barY = 280.f, barW = 240.f, barH = 10.f;
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
    confirmText.setPosition(sf::Vector2f(300.f, 310.f)); window.draw(confirmText);

    sf::Text cancelText(m_font); cancelText.setString(toSfString("[ 取消 ]"));
    cancelText.setCharacterSize(16); cancelText.setFillColor(sf::Color(255, 100, 100));
    cancelText.setPosition(sf::Vector2f(420.f, 310.f)); window.draw(cancelText);
}

// ====== 渲染 ======

void Inventory::render(sf::RenderWindow& window)
{
    if (m_open) renderBackpack(window); else renderHotbar(window);
    if (m_cursorCount > 0 && !m_splitDialogOpen) renderCursorItem(window);
    if (m_splitDialogOpen) renderSplitDialog(window);
}

void Inventory::renderHotbar(sf::RenderWindow& window)
{ for (int i = 0; i < SLOT_COUNT; ++i) drawSlot(window, i, getHotbarSlotRect(i)); }

void Inventory::renderBackpack(sf::RenderWindow& window)
{
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 100)); window.draw(overlay);

    sf::RectangleShape panelBg(sf::Vector2f(440.f, 280.f));
    panelBg.setPosition(sf::Vector2f(400.f - 220.f, 130.f));
    panelBg.setFillColor(sf::Color(60, 60, 60, 240));
    panelBg.setOutlineColor(sf::Color(130, 130, 130, 255));
    panelBg.setOutlineThickness(3.f); window.draw(panelBg);

    if (m_hasFont)
    {
        sf::Text title(m_font); title.setString(toSfString("物品栏"));
        title.setCharacterSize(20); title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(400.f - title.getLocalBounds().size.x / 2.f, 138.f));
        window.draw(title);
    }

    for (int i = 0; i < SLOT_COUNT; ++i) drawSlot(window, i, getBackpackSlotRect(i));
    renderTooltip(window);
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
    sf::RectangleShape icon(sf::Vector2f(fs * 0.5f, fs * 0.5f));
    icon.setPosition(sf::Vector2f(rect.position.x + fs * 0.25f, rect.position.y + fs * 0.08f));
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
        sf::Text nameText(m_font); nameText.setString(toSfString(itemName(m_items[i])));
        nameText.setCharacterSize(static_cast<unsigned int>(fs * 0.22f));
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(sf::Vector2f(rect.position.x + (fs - nameText.getLocalBounds().size.x) / 2.f,
                                           rect.position.y + fs * 0.58f));
        window.draw(nameText);

        if (m_counts[i] > 1)
        {
            sf::Text countText(m_font); countText.setString(std::to_string(m_counts[i]));
            countText.setCharacterSize(static_cast<unsigned int>(fs * 0.2f));
            countText.setFillColor(sf::Color(220,220,220));
            countText.setPosition(sf::Vector2f(rect.position.x + fs - 24.f, rect.position.y + fs - 20.f));
            window.draw(countText);
        }
    }
}

void Inventory::renderTooltip(sf::RenderWindow& window) const
{
    if (!m_open || m_hoveredSlot < 0 || !m_hasFont) return;
    if (m_items[m_hoveredSlot] == ItemType::Empty) return;

    sf::Text tooltip(m_font); tooltip.setString(toSfString(itemName(m_items[m_hoveredSlot])));
    tooltip.setCharacterSize(14); tooltip.setFillColor(sf::Color(255,255,200));

    sf::Vector2i rawMouse = sf::Mouse::getPosition(window);
    tooltip.setPosition(sf::Vector2f(static_cast<float>(rawMouse.x) + 16.f, static_cast<float>(rawMouse.y) + 16.f));
    sf::FloatRect tb = tooltip.getLocalBounds();

    sf::RectangleShape tipBg(sf::Vector2f(tb.size.x + 12.f, tb.size.y + 8.f));
    tipBg.setPosition(sf::Vector2f(static_cast<float>(rawMouse.x) + 12.f, static_cast<float>(rawMouse.y) + 14.f));
    tipBg.setFillColor(sf::Color(0,0,0,200));
    window.draw(tipBg); window.draw(tooltip);
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
