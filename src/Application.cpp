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

    SDL_Surface *bgSurface = IMG_Load("./assets/angrybirds/background.png");
    if (bgSurface) {
        bgTexture = SDL_CreateTextureFromSurface(Graphics::renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    Body *bird =
        new Body(new CircleShape(45), 100, Graphics::Height() / 2.0 + 220, 3.0);
    bird->SetTexture("./assets/angrybirds/bird-red.png");
    world->AddBody(bird);

    Body *floor =
        new Body(new BoxShape(Graphics::Width() - 50, 50),
                 Graphics::Width() / 2.0, Graphics::Height() / 2.0 + 290, 0.0);
    Body *leftFence = new Body(new BoxShape(50, Graphics::Height() - 200), 0,
                               Graphics::Height() / 2.0 - 35, 0.0);
    Body *rightFence =
        new Body(new BoxShape(50, Graphics::Height() - 200), Graphics::Width(),
                 Graphics::Height() / 2.0 - 35, 0.0);
    world->AddBody(floor);
    world->AddBody(leftFence);
    world->AddBody(rightFence);

    for (int i = 1; i <= 4; i++) {
        float mass = 10.0 / (float)i;
        Body *box = new Body(new BoxShape(50, 50), 600,
                             floor->position.y - i * 55, mass);
        box->SetTexture("./assets/angrybirds/wood-box.png");
        box->friction = 0.9;
        box->restitution = 0.1;
        world->AddBody(box);
    }

    Body *plank1 = new Body(new BoxShape(50, 150), Graphics::Width() / 2.0 + 20,
                            floor->position.y - 100, 5.0);
    Body *plank2 =
        new Body(new BoxShape(50, 150), Graphics::Width() / 2.0 + 180,
                 floor->position.y - 100, 5.0);
    Body *plank3 =
        new Body(new BoxShape(250, 25), Graphics::Width() / 2.0 + 100.0f,
                 floor->position.y - 200, 2.0);
    plank1->SetTexture("./assets/angrybirds/wood-plank-solid.png");
    plank2->SetTexture("./assets/angrybirds/wood-plank-solid.png");
    plank3->SetTexture("./assets/angrybirds/wood-plank-cracked.png");
    world->AddBody(plank1);
    world->AddBody(plank2);
    world->AddBody(plank3);

    std::vector<Vec2> triangleVertices = {Vec2(30, 30), Vec2(-30, 30),
                                          Vec2(0, -30)};
    Body *triangle = new Body(new PolygonShape(triangleVertices),
                              plank3->position.x, plank3->position.y - 50, 0.5);
    triangle->SetTexture("./assets/angrybirds/wood-triangle.png");
    world->AddBody(triangle);

    int numRows = 5;
    for (int col = 0; col < numRows; col++) {
        for (int row = 0; row < col; row++) {
            float x =
                (plank3->position.x + 200.0f) + col * 50.0f - (row * 25.0f);
            float y = (floor->position.y - 50.0f) - row * 52.0f;
            float mass = (5.0f / (row + 1.0f));
            Body *box = new Body(new BoxShape(50, 50), x, y, mass);
            box->friction = 0.9;
            box->restitution = 0.0;
            box->SetTexture("./assets/angrybirds/wood-box.png");
            world->AddBody(box);
        }
    }

    int numSteps = 10;
    int spacing = 33;
    Body *startStep = new Body(new BoxShape(80, 20), 200, 200, 0.0);
    startStep->SetTexture("./assets/angrybirds/rock-bridge-anchor.png");
    world->AddBody(startStep);
    Body *last = floor;
    for (int i = 1; i <= numSteps; i++) {
        float x = startStep->position.x + 30 + (i * spacing);
        float y = startStep->position.y + 20;
        float mass = (i == numSteps) ? 0.0 : 3.0;
        Body *step = new Body(new CircleShape(15), x, y, mass);
        step->SetTexture("./assets/angrybirds/wood-bridge-step.png");
        world->AddBody(step);
        JointConstraint *joint =
            new JointConstraint(last, step, step->position);
        world->AddConstraint(joint);
        last = step;
    }
    Body *endStep = new Body(new BoxShape(80, 20), last->position.x + 60,
                             last->position.y - 20, 0.0);
    endStep->SetTexture("./assets/angrybirds/rock-bridge-anchor.png");
    world->AddBody(endStep);

    Body *pig1 = new Body(new CircleShape(30), plank1->position.x + 80,
                          floor->position.y - 50, 3.0);
    Body *pig2 = new Body(new CircleShape(30), plank2->position.x + 400,
                          floor->position.y - 50, 3.0);
    Body *pig3 = new Body(new CircleShape(30), plank2->position.x + 460,
                          floor->position.y - 50, 3.0);
    Body *pig4 = new Body(new CircleShape(30), 220, 130, 1.0);
    pig1->SetTexture("./assets/angrybirds/pig-1.png");
    pig2->SetTexture("./assets/angrybirds/pig-2.png");
    pig3->SetTexture("./assets/angrybirds/pig-1.png");
    pig4->SetTexture("./assets/angrybirds/pig-2.png");
    world->AddBody(pig1);
    world->AddBody(pig2);
    world->AddBody(pig3);
    world->AddBody(pig4);
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
                world->GetBodies()[0]->ApplyImpulseLinear(Vec2(0.0, -600.0));
            }
            if (sym == SDLK_RIGHT) {
                pushForce.x = 50 * PIXELS_PER_METER;
                world->GetBodies()[0]->ApplyImpulseLinear(Vec2(+400.0, 0.0));
            }
            if (sym == SDLK_DOWN) {
                pushForce.y = 50 * PIXELS_PER_METER;
            }
            if (sym == SDLK_LEFT) {
                pushForce.x = -50 * PIXELS_PER_METER;
                world->GetBodies()[0]->ApplyImpulseLinear(Vec2(-400.0, 0.0));
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
            // auto small = world->GetBodies()[1];
            // small->position.x = mouseCursor.x;
            // small->position.y = mouseCursor.y;
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
                Body *box = new Body(new BoxShape(60, 60), x, y, 1.0);
                box->SetTexture("./assets/angrybirds/rock-box.png");
                box->angularVelocity = 0.0;
                box->friction = 0.9;
                world->AddBody(box);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                Body *rock = new Body(new CircleShape(30), x, y, 1.0);
                rock->SetTexture("./assets/angrybirds/rock-round.png");
                rock->friction = 0.4;
                world->AddBody(rock);
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
    Graphics::DrawTexture(Graphics::Width() / 2.0, Graphics::Height() / 2.0,
                          Graphics::Width(), Graphics::Height(), 0.0f,
                          bgTexture);

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
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y,
                                      polygonShape->width, polygonShape->height,
                                      body->rotation, body->texture);
            } else {
                Graphics::DrawPolygon(body->position.x, body->position.y,
                                      polygonShape->worldVertices, 0xFF0000FF);
            }
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy() {
    delete world;
    Graphics::CloseWindow();
}
