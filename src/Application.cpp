#include "Application.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Constants.h"
#include "./Physics/Contact.h"
#include "./Physics/Force.h"
#include "./Physics/Geometry.h"
#include "Physics/Constraint.h"
#include <SDL_mouse.h>
#include <SDL_timer.h>

bool Application::IsRunning() { return running; }

void Application::Setup() {
    running = Graphics::OpenWindow();

    world = new World(-9.8);

    auto w = Graphics::Width();
    auto h = Graphics::Height();

    Body *bigBox = new Body(new BoxShape(200, 200), Graphics::Width() / 2.0,
                            Graphics::Height() / 2.0, 0.0);
    world->AddBody(bigBox);

    Body *small = new Body(new BoxShape(150, 150), Graphics::Width() / 2.0,
                           Graphics::Height() / 2.0, 1.0);
    small->rotation = 0.f / 10.f;
    world->AddBody(small);

    Body *floor =
        new Body(new BoxShape(Graphics::Width() - 50, 50),
                 Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    Body *leftWall = new Body(new BoxShape(50, Graphics::Height() - 100), 50,
                              Graphics::Height() / 2.0 - 25, 0.0);
    Body *rightWall =
        new Body(new BoxShape(50, Graphics::Height() - 100),
                 Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
    floor->restitution = 0.7;
    leftWall->restitution = 0.2;
    rightWall->restitution = 0.2;
    world->AddBody(floor);
    world->AddBody(leftWall);
    world->AddBody(rightWall);

    // world->AddForce(Vec2(1.f * PIXELS_PER_METER, 0.f));
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
            if (sym == SDLK_d) {
                debug = !debug;
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
        case SDL_MOUSEMOTION: {
            mouseCursor.x = event.motion.x;
            mouseCursor.y = event.motion.y;
            auto small = world->GetBodies()[1];
            small->position.x = mouseCursor.x;
            small->position.y = mouseCursor.y;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (!leftMouseButtonDown) {
                    leftMouseButtonDown = true;
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                auto ball = new Body(new CircleShape(30.f), x, y, 1.0);
                ball->SetTexture("./assets/basketball.png");
                ball->restitution = 0.2f;
                ball->friction = 0.4f;
                world->AddBody(ball);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                auto box = new Body(new BoxShape(60.f, 60.f), x, y, 1.0);
                box->SetTexture("./assets/crate.png");
                box->restitution = 0.2f;
                box->friction = 0.4f;
                world->AddBody(box);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                leftMouseButtonDown = false;
                // auto p = bodies[NUM_BODIES - 1];
                // auto f = p->position - mouseCursor;
                // Vec2 impulseDirection = f.UnitVector();
                // float impulseMagnitude = f.Magnitude() * 5.0f;
                // p->velocity = impulseDirection * impulseMagnitude;
            }
        }
    }
}

void Application::Update() {
    Graphics::ClearScreen(0xFF056263);

    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016f) {
        deltaTime = 0.016f;
    }
    timePreviousFrame = SDL_GetTicks();

    world->Update(deltaTime, debug);
}

void Application::Render() {
    // Draw all joints anchor points
    for (auto joint : world->GetConstraints()) {
        if (debug) {
            const Vec2 anchor = joint->a->LocalSpaceToWorldSpace(joint->aPoint);
            Graphics::DrawFillCircle(anchor.x, anchor.y, 3, 0xFF0000FF);
        }
    }

    for (auto body : world->GetBodies()) {
        Uint32 color = 0xFF00FF00;
        auto shapeType = body->shape->GetType();
        if (shapeType == ShapeType::CIRCLE) {
            CircleShape *circleShape = static_cast<CircleShape *>(body->shape);
            if (!debug && body->texture) {
                Graphics::DrawTexture(
                    body->position.x, body->position.y, circleShape->radius * 2,
                    circleShape->radius * 2, body->rotation, body->texture);
            } else {
                Graphics::DrawCircle(body->position.x, body->position.y,
                                     circleShape->radius, body->rotation,
                                     color);
            }
        } else if (shapeType == ShapeType::BOX) {
            BoxShape *boxShape = static_cast<BoxShape *>(body->shape);
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y,
                                      boxShape->width, boxShape->height,
                                      body->rotation, body->texture);
            } else {
                Graphics::DrawPolygon(body->position.x, body->position.y,
                                      boxShape->worldVertices, color);
            }
        } else if (shapeType == ShapeType::POLYGON) {
            PolygonShape *polygonShape =
                static_cast<PolygonShape *>(body->shape);
            Graphics::DrawPolygon(body->position.x, body->position.y,
                                  polygonShape->worldVertices, color);
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy() {
    delete world;
    Graphics::CloseWindow();
}
