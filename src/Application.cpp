#include "Application.h"
#include "./Physics/Constants.h"
#include <SDL_timer.h>

bool Application::IsRunning() { return running; }

void Application::Setup() {
    running = Graphics::OpenWindow();
    particle = new Particle(50, 100, 1.0);
    particle->radius = 4.0f;
}

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
                break;
            }
        }
    }
}

void Application::Update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016f) {
        deltaTime = 0.016f;
    }
    timePreviousFrame = SDL_GetTicks();

    particle->acceleration =
        Vec2(1.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);
    particle->velocity += particle->acceleration * deltaTime;
    particle->position += particle->velocity * deltaTime;

    if (particle->position.x - particle->radius <= 0) {
        particle->position.x = particle->radius;
        particle->velocity.x *= -0.9f;
    } else if (particle->position.x + particle->radius >= Graphics::Width()) {
        particle->position.x = Graphics::Width() - particle->radius;
        particle->velocity.x *= -0.9f;
    } else if (particle->position.y - particle->radius <= 0) {
        particle->position.y = particle->radius;
        particle->velocity.y *= -0.9f;
    } else if (particle->position.y + particle->radius >= Graphics::Height()) {
        particle->position.y = Graphics::Height() - particle->radius;
        particle->velocity.y *= -0.9f;
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->position.x, particle->position.y,
                             particle->radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

void Application::Destroy() {
    delete particle;
    Graphics::CloseWindow();
}
