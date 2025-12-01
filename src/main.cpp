#include <random>

#include "gmi/Application.h"
#include "gmi/Container.h"
#include "gmi/Sprite.h"

using namespace gmi;

Application* gmiMain() {
    const auto app = new Application({
        .title = "Suroi",
        .backgroundColor = Color::fromHsl(0.26, 0.41, 0.38),
        .backend = BackendType::Sdl
    });

    SDL_Cursor* cursor{SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR)};
    SDL_SetCursor(cursor);

    Container* player{app->getStage().createChild<Container>(math::Transform{{400, 300}})};

    Texture& baseTexture{app->loadTexture("../assets/hasanger_base.png")};
    player->createChild<Sprite>(baseTexture);

    Texture& fistTexture{app->loadTexture("../assets/hasanger_fist.png")};
    Sprite* leftFist{player->createChild<Sprite>(fistTexture, math::Transform{{38, -35}})};
    Sprite* rightFist{player->createChild<Sprite>(fistTexture, math::Transform{{38, 35}})};

    app->addEventListener(SDL_EVENT_MOUSE_MOTION, [app, player](const SDL_Event& event) {
        const auto [w, h] = app->getSize();
        player->setRotation(math::angleBetween(
            math::Vec2::fromInt(w / 2, h / 2),
            {event.motion.x, event.motion.y}
        ) + math::PI);
    });

    app->addEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN, [app, leftFist, rightFist](const SDL_Event& event) {
        static auto gen = std::mt19937{std::random_device{}()};
        static auto dist = std::uniform_real_distribution<float>(0, 1);
        bool altFist = dist(gen) < 0.5;
        auto fist = altFist ? leftFist : rightFist;
        const float y = altFist ? -10 : 10;
        app->getTweenManager().addTween({
            .values = {
                {&fist->m_transform.position.x, 75},
                {&fist->m_transform.position.y, y}
            },
            .duration = 125,
            .ease = math::Easing::cubicOut,
            .yoyo = true,
            .onUpdate = [fist] { fist->updateAffine(); }
        });
    });

    return app;
}
