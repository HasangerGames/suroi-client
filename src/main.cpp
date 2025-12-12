#include <random>

#include "gmi/Application.h"
#include "gmi/Container.h"
#include "gmi/Graphics.h"
#include "gmi/Sprite.h"

using namespace gmi;

Application* gmiMain() {
    auto app = new Application({
        .title = "Suroi",
        .backgroundColor = Color::fromHsl(0.26, 0.41, 0.38)
    });

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);

    app->texture().loadSpritesheet("../assets/normal-cf5a29b4@1x.json");
    app->texture().loadSpritesheet("../assets/normal-c6581886@1x.json");
    app->sound().load("swing", "../assets/swing.mp3");

    auto& graphics = app->stage().createChild<Graphics>();
    graphics
        .fillPoly({
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
            {410 + 100, 420 - 100},
            {470 + 100, 405 - 100},
            {540 + 100, 430 - 100},
            {580 + 100, 490 - 100},
            {595 + 100, 560 - 100},
            {570 + 100, 610 - 100},
            {510 + 100, 645 - 100},
            {445 + 100, 630 - 100},
            {415 + 100, 580 - 100},
            {405 + 100, 520 - 100}
        }, {1.0f, 1.0f, 0.0f, 0.0f})
        .fillRect({300, 100, 100, 100}, {1.0f, 0.0f, 0.0f})
        .fillRect({500, 100, 100, 100}, {1.0f, 1.0f, 0.0f})
        .drawLine({
            {(700 - 42) + 100, 18},
            {(700 - 87) + 100, 55},
            {(700 - 128) + 100, 40},
            {(700 - 171) + 100, 92},
            {(700 - 212) + 100, 130},
            {(700 - 256) + 100, 118},
            {(700 - 295) + 100, 165},
            {(700 - 340) + 100, 190},
            {(700 - 382) + 100, 175},
            {(700 - 427) + 100, 220},
            {(700 - 470) + 100, 265},
            {(700 - 512) + 100, 250},
            {(700 - 556) + 100, 305},
            {(700 - 600) + 100, 340},
            {(700 - 645) + 100, 380},
            {(700 - 684) + 100, 430},
            {(700 - 700) + 100, 500}
        }, {
            .width = 8,
            .cap = LineCap::Round
        })
        .drawLine({
            {30, 40},
            {120, 20},
            {250, 60},
            {290, 150},
            {260, 250},
            {180, 290},
            {90, 270},
            {20, 200},
            {40, 120}
        }, {
            .width = 15,
            .color = Color::fromRgba(255, 255, 192, 192),
            .cap = LineCap::Round,
            .join = LineJoin::Round,
            .closedShape = true
        })
        .fillCircle({300, 450}, 100, 100, Color::fromRgba(255, 0, 255, 255));

    auto& player = app->stage().createChild<Container>(math::Transform({400, 300}));

    player.createChild<Sprite>("hasanger_base");

    auto& leftFist = player.createChild<Sprite>("hasanger_fist", math::Transform({38, -35}));
    auto& rightFist = player.createChild<Sprite>("hasanger_fist", math::Transform{{38, 35}});

    app->addEventListener(SDL_EVENT_MOUSE_MOTION, [app, &player](const SDL_Event& event) {
        const auto [w, h] = app->getSize();
        player.setRotation(math::angleBetween(
            math::Vec2::fromAny(w / 2, h / 2),
            {event.motion.x, event.motion.y}
        ) + math::PI);
    });

    app->addEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN, [app, &leftFist, &rightFist](const SDL_Event& event) {
        static auto gen = std::mt19937(std::random_device()());
        static auto dist = std::uniform_real_distribution<float>(0, 1);
        bool altFist = dist(gen) < 0.5;
        Sprite& fist = altFist ? leftFist : rightFist;
        float y = altFist ? -10 : 10;
        fist.animate({
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
