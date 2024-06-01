#pragma once

#include "./Graphics.h"
#include "Physics/Particle.h"
#include <vector>

class Application {
  private:
    bool running = false;
    int timePreviousFrame = 0;
    std::vector<Particle *> particles;
    Vec2 pushForce = Vec2(0.0, 0.0);

    SDL_Rect liquid;

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
