// 游戏入口 —— main() 只做两件事：
//   1. 创建 Game 对象
//   2. 设置初始场景并运行
//
// 所有游戏逻辑都在 Game、Scene、FarmScene 等类中
// main.cpp 保持极简

#include "core/Game.hpp"
#include "scenes/FarmScene.hpp"

int main()
{
    Game game;

    // 创建一个农场场景并设为当前场景
    // std::make_unique 创建独占所有权的智能指针（C++14）
    // Game 接管这个场景的生命周期
    game.changeScene(std::make_unique<FarmScene>(game.getWindow()));

    // 运行游戏主循环（阻塞，直到关闭窗口）
    game.run();

    // game 析构时自动清理窗口和所有场景
    return 0;
}
