#include "Application.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Constants.h"
#include "./Physics/Contact.h"
#include "./Physics/Force.h"
#include "./Physics/Geometry.h"
#include <SDL_mouse.h>
#include <SDL_timer.h>

bool Application::IsRunning() { return running; }

void Application::Setup() {
    running = Graphics::OpenWindow();

    auto w = Graphics::Width();
    auto h = Graphics::Height();
    Body *floor = new Body(new BoxShape(w - 50, 50), w / 2.0, h - 50, 0.0);
    Body *leftWall = new Body(new BoxShape(50, h - 100), 50, h / 2.0 - 25, 0.0);
    Body *rightWall =
        new Body(new BoxShape(50, h - 100), w - 50, h / 2.0 - 25, 0.0);
    floor->restitution = 0.5f;
    leftWall->restitution = 0.5f;
    rightWall->restitution = 0.5f;
    bodies.push_back(floor);
    bodies.push_back(leftWall);
    bodies.push_back(rightWall);

    Body *bigBox = new Body(new BoxShape(200, 200), w / 2.0, h / 2.0, 0.0);
    bigBox->SetTexture("./assets/crate.png");
    bigBox->rotation = 1.4f;
    bigBox->restitution = 1.f;
    bodies.push_back(bigBox);

    Body *p2 = new Body(new CircleShape(50), w / 2.0, h / 2.0, 1.0);
    bigBox->rotation = 1.4f;
    bigBox->restitution = 0.5f;
    bodies.push_back(p2);
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
        case SDL_MOUSEMOTION:
            mouseCursor.x = event.motion.x;
            mouseCursor.y = event.motion.y;
            // bodies[1]->position.x = mouseCursor.x;
            // bodies[1]->position.y = mouseCursor.y;
            break;
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
                bodies.push_back(ball);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                auto box = new Body(new BoxShape(60.f, 60.f), x, y, 1.0);
                box->SetTexture("./assets/crate.png");
                box->restitution = 0.2f;
                box->friction = 0.4f;
                bodies.push_back(box);
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

    for (auto body : bodies) {
        // body->AddForce(pushForce);

        // Vec2 friction =
        //     Force::GenerateFrictionForce(*body, 10 * PIXELS_PER_METER);
        // body->AddForce(friction);

        // F = mg
        Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        body->AddForce(weight);

        // wind
        // body->AddForce(Vec2(20.f * PIXELS_PER_METER, 0.f));
    }

    for (auto body : bodies) {
        body->Update(deltaTime);
    }

    for (int i = 0; i < bodies.size() - 1; i++) {
        for (int j = i + 1; j < bodies.size(); j++) {
            Body *a = bodies[i];
            Body *b = bodies[j];
            a->isColliding = false;
            b->isColliding = false;
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) {
                contact.ResolveCollision();

                if (debug) {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y,
                                             3, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3,
                                             0xFFFF00FF);
                    Graphics::DrawLine(contact.start.x, contact.start.y,
                                       contact.start.x + contact.normal.x * 15,
                                       contact.start.y + contact.normal.y * 15,
                                       0xFFFF00FF);
                    a->isColliding = true;
                    b->isColliding = true;
                }
            }
        }
    }
}

void Application::Render() {
    for (auto body : bodies) {
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
    for (auto body : bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}
