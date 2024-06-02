#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <SDL_mouse.h>
#include <SDL_timer.h>

bool Application::IsRunning() { return running; }

void Application::Setup() {
    running = Graphics::OpenWindow();

    int wh[][2] = {
        {100, 100},
        {300, 100},
        {300, 300},
        {100, 300},
    };
    for (int i = 0; i < NUM_BODIES; i++) {
        Body *p = new Body(wh[i][0], wh[i][1], 2.0);
        p->radius = 6;
        bodies.push_back(p);
    }

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
        // Vec2 wind = Vec2(1.0 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        // body->AddForce(wind);

        body->AddForce(pushForce);

        // Vec2 friction =
        //     Force::GenerateFrictionForce(*body, 10 * PIXELS_PER_METER);
        // body->AddForce(friction);

        // F = mg
        Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
        body->AddForce(weight);

        // drag
        Vec2 drag = Force::GenerateDragForce(*body, 0.02f);
        body->AddForce(drag);

        // Inside liquid
        // if (body->position.y >= liquid.y) {
        //     Vec2 drag = Force::GenerateDragForce(*body, 2.f);
        //     body->AddForce(drag);
        // }
    }

    int co[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 2}, {1, 3},
    };
    for (int i = 0; i < 6; i++) {
        Body *p0 = bodies[co[i][0]];
        Body *p1 = bodies[co[i][1]];
        Vec2 sf = Force::GenerateSpringForce(*p0, *p1, restLength, k);
        p0->AddForce(sf);
        p1->AddForce(-sf);
    }

    for (auto body : bodies) {
        body->Integrate(deltaTime);
    }

    for (auto body : bodies) {
        if (body->position.x - body->radius <= 0) {
            body->position.x = body->radius;
            body->velocity.x *= -0.9f;
        } else if (body->position.x + body->radius >=
                   Graphics::Width()) {
            body->position.x = Graphics::Width() - body->radius;
            body->velocity.x *= -0.9f;
        }
        if (body->position.y - body->radius <= 0) {
            body->position.y = body->radius;
            body->velocity.y *= -0.9f;
        } else if (body->position.y + body->radius >=
                   Graphics::Height()) {
            body->position.y = Graphics::Height() - body->radius;
            body->velocity.y *= -0.9f;
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    int co[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 2}, {1, 3},
    };
    for (int i = 0; i < 6; i++) {
        auto p0 = bodies[co[i][0]];
        auto p1 = bodies[co[i][1]];
        Graphics::DrawLine(p0->position.x, p0->position.y, p1->position.x,
                           p1->position.y, 0xFF313131);
    }

    // Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2,
    //                        liquid.w, liquid.h, 0xFF6E3713);

    for (auto body : bodies) {
        Graphics::DrawFillCircle(body->position.x, body->position.y,
                                 body->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto body : bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}
