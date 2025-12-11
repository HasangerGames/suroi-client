#include <random>

#include "gmi/Application.h"
#include "gmi/Container.h"
#include "gmi/Graphics.h"
#include "gmi/Sprite.h"

using namespace gmi;

Application* gmiMain() {
    const auto app = new Application({
        .title = "Suroi",
        .backgroundColor = Color::fromHsl(0.26, 0.41, 0.38)
    });

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);

    app->texture().loadSpritesheet("../assets/normal-cf5a29b4@1x.json");
    app->texture().loadSpritesheet("../assets/normal-c6581886@1x.json");
    app->sound().load("swing", "../assets/swing.mp3");

    auto* graphics = app->stage().createChild<Graphics>();
    graphics
        ->fillPoly({
            {30, 40},
            {120, 20},
            {250, 60},
            {290, 150},
            {260, 250},
            {180, 290},
            {90, 270},
            {20, 200},
            {40, 120}
        }, {1.0f, 0.0f, 1.0f, 0.0f})
        .fillPoly({
            {410, 420},
            {470, 405},
            {540, 430},
            {580, 490},
            {595, 560},
            {570, 610},
            {510, 645},
            {445, 630},
            {415, 580},
            {405, 520}
        }, {1.0f, 1.0f, 0.0f, 0.0f})
        .fillRect({300, 100, 100, 100}, {1.0f, 0.0f, 0.0f})
        .fillRect({500, 100, 100, 100}, {1.0f, 1.0f, 0.0f});

    auto* player = app->stage().createChild<Container>(math::Transform({400, 300}));

    player->createChild<Sprite>("hasanger_base");

    auto* leftFist = player->createChild<Sprite>("hasanger_fist", math::Transform({38, -35}));
    auto* rightFist = player->createChild<Sprite>("hasanger_fist", math::Transform{{38, 35}});

    app->addEventListener(SDL_EVENT_MOUSE_MOTION, [app, player](const SDL_Event& event) {
        const auto [w, h] = app->getSize();
        player->setRotation(math::angleBetween(
            math::Vec2::fromAny(w / 2, h / 2),
            {event.motion.x, event.motion.y}
        ) + math::PI);
    });

    app->addEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN, [app, leftFist, rightFist](const SDL_Event& event) {
        static auto gen = std::mt19937(std::random_device()());
        static auto dist = std::uniform_real_distribution<float>(0, 1);
        bool altFist = dist(gen) < 0.5;
        Sprite* fist = altFist ? leftFist : rightFist;
        float y = altFist ? -10 : 10;
        fist->animate({
            .prop = math::TransformProps::Position,
            .target = {75, y},
            .duration = 125,
            .easing = math::Easing::cubicOut,
            .yoyo = true
        });
        app->sound().play("swing");
    });

    return app;
}
