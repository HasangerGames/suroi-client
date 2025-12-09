#include <random>

#include "gmi/Application.h"
#include "gmi/Container.h"
#include "gmi/Sprite.h"

using namespace gmi;

Application* gmiMain() {
    const auto app = new Application({
        .title = "Suroi",
        .backgroundColor = Color::fromHsl(0.26, 0.41, 0.38)
    });

    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);

    auto* player = app->stage().createChild<Container>(math::Transform{{400, 300}});

    app->texture().loadSpritesheet("../assets/normal-cf5a29b4@1x.json");
    app->texture().loadSpritesheet("../assets/normal-c6581886@1x.json");

    player->createChild<Sprite>("hasanger_base");

    auto* leftFist = player->createChild<Sprite>("hasanger_fist", math::Transform({38, -35}));
    auto* rightFist = player->createChild<Sprite>("hasanger_fist", math::Transform{{38, 35}});

    app->sound().load("swing", "../assets/swing.mp3");

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
