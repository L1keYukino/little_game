#include "GameTime.hpp"
#include <iomanip>
#include <sstream>

GameTime::GameTime() = default;

void GameTime::update(float dt)
{
    // 1 实秒 = 1 游戏分钟
    m_accumulator += dt;

    while (m_accumulator >= 1.f) // 每 1 实秒推进 1 游戏分钟
    {
        m_accumulator -= 1.f;
        m_minute++;

        if (m_minute >= 60)
        {
            m_minute = 0;
            m_hour++;
            if (m_hour >= 24)
            {
                m_hour = 0;
                m_day++;
            }
        }
    }
}

std::string GameTime::toString() const
{
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << m_hour
       << ":" << std::setw(2) << m_minute;
    return ss.str();
}
