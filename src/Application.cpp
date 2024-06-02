#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <SDL_mouse.h>
#include <SDL_timer.h>

bool Application::IsRunning() { return running; }

void Application::Setup() {
    running = Graphics::OpenWindow();

    Body *p = new Body(new BoxShape(200, 100), Graphics::Width() / 2.0,
                       Graphics::Height() / 2.0, 2.0);
    bodies.push_back(p);
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
                // auto bodies = new Body(x, y, 1.0);
                // body->radius = 5.0f;
                // body.push_back(body);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                leftMouseButtonDown = false;
                auto p = bodies[NUM_BODIES - 1];
                auto f = p->position - mouseCursor;
                Vec2 impulseDirection = f.UnitVector();
                float impulseMagnitude = f.Magnitude() * 5.0f;
                p->velocity = impulseDirection * impulseMagnitude;
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

    for (auto body : bodies) {
        body->AddForce(pushForce);

        // Vec2 friction =
        //     Force::GenerateFrictionForce(*body, 10 * PIXELS_PER_METER);
        // body->AddForce(friction);

        // F = mg
        // Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        // body->AddForce(weight);

        float torque = 5000.f;
        body->AddTorque(torque);
    }

    for (auto body : bodies) {
        body->Update(deltaTime);
    }

    for (auto body : bodies) {
        if (body->shape->GetType() == ShapeType::CIRCLE) {
            CircleShape *circleShape = static_cast<CircleShape *>(body->shape);
            if (body->position.x - circleShape->radius <= 0) {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9;
            } else if (body->position.x + circleShape->radius >=
                       Graphics::Width()) {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9f;
            }
            if (body->position.y - circleShape->radius <= 0) {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9f;
            } else if (body->position.y + circleShape->radius >=
                       Graphics::Height()) {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9f;
            }
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    for (auto body : bodies) {
        auto shapeType = body->shape->GetType();
        if (shapeType == ShapeType::CIRCLE) {
            CircleShape *circleShape = static_cast<CircleShape *>(body->shape);
            Graphics::DrawCircle(body->position.x, body->position.y,
                                 circleShape->radius, body->rotation,
                                 0xFFFFFFFF);
        } else if (shapeType == ShapeType::BOX) {
            BoxShape *boxShape = static_cast<BoxShape *>(body->shape);
            Graphics::DrawPolygon(body->position.x, body->position.y,
                                  boxShape->worldVertices, 0xFFFFFFFF);
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto body : bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}
