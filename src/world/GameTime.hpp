#pragma once
#include <string>

// 游戏内时间 —— 1 实秒 = 1 游戏分钟，24 实分 = 1 游戏天
class GameTime
{
public:
    GameTime();

    void update(float dt); // dt = 实秒

    int  hour()   const { return m_hour; }
    int  minute() const { return m_minute; }
    int  day()    const { return m_day; }

    // "06:30" 格式的字符串
    std::string toString() const;

private:
    int m_hour   = 6;   // 0-23
    int m_minute = 0;   // 0-59
    int m_day    = 1;   // 第几天

    float m_accumulator = 0.f; // 累积实秒
};
