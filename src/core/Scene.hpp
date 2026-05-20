#pragma once
#include <SFML/Graphics.hpp>

// 场景抽象基类 —— 游戏的每个"画面"都继承它
// 例如：标题画面、农场画面、室内画面
// 这样 Game 类不需要知道具体是哪个场景，只调用这三个接口
class Scene
{
public:
    virtual ~Scene() = default;

    // 处理输入事件（键盘按下、鼠标点击等）
    // 参数是 const 引用，因为我们只读取事件，不修改 window
    virtual void handleInput(const sf::Event& event) = 0;

    // 更新游戏逻辑（移动、碰撞、计时等）
    // dt = deltaTime，距上一帧过去了多少秒
    // 所有和时间相关的计算都乘以 dt，保证不同帧率下速度一致
    virtual void update(float dt) = 0;

    // 渲染当前场景到窗口
    virtual void render(sf::RenderWindow& window) = 0;
};
