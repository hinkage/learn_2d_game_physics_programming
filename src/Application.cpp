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

    Body *bob = new Body(new CircleShape(5), Graphics::Width() / 2.0,
                         Graphics::Height() / 2.0 - 200, 0.0);
    Body *head = new Body(new CircleShape(25), bob->position.x,
                          bob->position.y + 70, 5.0);
    Body *torso = new Body(new BoxShape(50, 100), head->position.x,
                           head->position.y + 80, 3.0);
    Body *leftArm = new Body(new BoxShape(15, 70), torso->position.x - 32,
                             torso->position.y - 10, 1.0);
    Body *rightArm = new Body(new BoxShape(15, 70), torso->position.x + 32,
                              torso->position.y - 10, 1.0);
    Body *leftLeg = new Body(new BoxShape(20, 90), torso->position.x - 20,
                             torso->position.y + 97, 1.0);
    Body *rightLeg = new Body(new BoxShape(20, 90), torso->position.x + 20,
                              torso->position.y + 97, 1.0);
    bob->SetTexture("./assets/ragdoll/bob.png");
    head->SetTexture("./assets/ragdoll/head.png");
    torso->SetTexture("./assets/ragdoll/torso.png");
    leftArm->SetTexture("./assets/ragdoll/leftArm.png");
    rightArm->SetTexture("./assets/ragdoll/rightArm.png");
    leftLeg->SetTexture("./assets/ragdoll/leftLeg.png");
    rightLeg->SetTexture("./assets/ragdoll/rightLeg.png");

    world->AddBody(bob);
    world->AddBody(head);
    world->AddBody(torso);
    world->AddBody(leftArm);
    world->AddBody(rightArm);
    world->AddBody(leftLeg);
    world->AddBody(rightLeg);

    JointConstraint *string = new JointConstraint(bob, head, bob->position);
    JointConstraint *neck =
        new JointConstraint(head, torso, head->position + Vec2(0, 25));
    JointConstraint *leftShoulder =
        new JointConstraint(torso, leftArm, torso->position + Vec2(-28, -45));
    JointConstraint *rightShoulder =
        new JointConstraint(torso, rightArm, torso->position + Vec2(+28, -45));
    JointConstraint *leftHip =
        new JointConstraint(torso, leftLeg, torso->position + Vec2(-20, +50));
    JointConstraint *rightHip =
        new JointConstraint(torso, rightLeg, torso->position + Vec2(+20, +50));
    world->AddConstraint(string);
    world->AddConstraint(neck);
    world->AddConstraint(leftShoulder);
    world->AddConstraint(rightShoulder);
    world->AddConstraint(leftHip);
    world->AddConstraint(rightHip);

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
            auto bob = world->GetBodies()[0];
            Vec2 direction =
                (Vec2(mouseCursor.x, mouseCursor.y) - bob->position)
                    .Normalize();
            float speed = 1.f;
            bob->position += direction * speed;
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
    Body *bob = world->GetBodies()[0];
    Body *head = world->GetBodies()[1];
    Graphics::DrawLine(bob->position.x, bob->position.y, head->position.x,
                       head->position.y, 0xFF555555);

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
