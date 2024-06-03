#pragma once

#include "./Graphics.h"
#include "Physics/Body.h"
#include <vector>

class Application {
  private:
    bool debug = false;
    bool running = false;
    int timePreviousFrame = 0;
    std::vector<Body *> bodies;
    Vec2 pushForce = Vec2(0.0, 0.0);

    SDL_Rect liquid;

    Vec2 anchor = Vec2(500.0, 0.0);
    float k = 1500;
    float restLength = 200;
    const int NUM_BODIES = 1;

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
