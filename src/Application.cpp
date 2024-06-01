#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <SDL_mouse.h>
#include <SDL_timer.h>

bool Application::IsRunning() { return running; }

void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle *smallBall = new Particle(50, 100, 2.0);
    smallBall->radius = 4.0f;
    particles.push_back(smallBall);

    Particle *bigBall = new Particle(100, 100, 5.0);
    bigBall->radius = 12.0f;
    particles.push_back(bigBall);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
}

void Application::Input() {
    static Vec2 mouseCursor;
    static bool leftMouseButtonDown = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN: {
            auto sym = event.key.keysym.sym;
            if (sym == SDLK_ESCAPE) {
                running = false;
            }
            if (sym == SDLK_UP) {
                pushForce.y = -50 * PIXELS_PER_METER;
            }
            if (sym == SDLK_RIGHT) {
                pushForce.x = 50 * PIXELS_PER_METER;
            }
            if (sym == SDLK_DOWN) {
                pushForce.y = 50 * PIXELS_PER_METER;
            }
            if (sym == SDLK_LEFT) {
                pushForce.x = -50 * PIXELS_PER_METER;
            }
            break;
        }
        case SDL_KEYUP: {
            auto sym = event.key.keysym.sym;
            if (sym == SDLK_UP) {
                pushForce.y = 0;
            }
            if (sym == SDLK_RIGHT) {
                pushForce.x = 0;
            }
            if (sym == SDLK_DOWN) {
                pushForce.y = 0;
            }
            if (sym == SDLK_LEFT) {
                pushForce.x = 0;
            }
            break;
        }
        case SDL_MOUSEMOTION:
            mouseCursor.x = event.motion.x;
            mouseCursor.y = event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (!leftMouseButtonDown) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                // auto particle = new Particle(x, y, 1.0);
                // particle->radius = 5.0f;
                // particles.push_back(particle);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                leftMouseButtonDown = false;
                auto f = particles[0]->position - mouseCursor;
                Vec2 impulseDirection = f.UnitVector();
                float impulseMagnitude = f.Magnitude() * 5.0f;
                particles[0]->velocity = impulseDirection * impulseMagnitude;
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

    for (auto particle : particles) {
        // Vec2 wind = Vec2(1.0 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        // particle->AddForce(wind);

        particle->AddForce(pushForce);

        Vec2 friction =
            Force::GenerateFrictionForce(*particle, 10 * PIXELS_PER_METER);
        particle->AddForce(friction);

        // F = mg
        Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);

        // Inside liquid
        if (particle->position.y >= liquid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 2.f);
            particle->AddForce(drag);
        }
    }

    // Vec2 attraction = Force::GenerateGravitationalForce(
    //     *particles[0], *particles[1], 5000.0, 5.0, 100.0);
    // particles[0]->AddForce(attraction);
    // particles[1]->AddForce(-attraction);

    Vec2 springForce =
        Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
    particles[0]->AddForce(springForce);

    for (auto particle : particles) {
        particle->Integrate(deltaTime);
    }

    for (auto particle : particles) {
        if (particle->position.x - particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9f;
        } else if (particle->position.x + particle->radius >=
                   Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9f;
        } else if (particle->position.y - particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9f;
        } else if (particle->position.y + particle->radius >=
                   Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9f;
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x,
                       particles[0]->position.y, 0xFF313131);

    Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2,
                           liquid.w, liquid.h, 0xFF6E3713);

    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y,
                                 particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto particle : particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}
