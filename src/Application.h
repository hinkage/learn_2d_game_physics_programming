#pragma once

#include "./Graphics.h"

class Application {
  private:
    bool running = false;

  public:
    Application() = default;
    ~Application() = default;

    bool IsRunning();
    void Setup();
    void Input();
    void Update();
    void Render();
    void Destroy();
};
