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

    Vec2 anchor = Vec2(500.0, 0.0);
    float k = 300;
    float restLength = 200;
    const int NUM_PARTICLES = 15;

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
