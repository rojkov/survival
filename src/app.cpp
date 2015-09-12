#include "app.h"
#include "world.h"
#include "viewport.h"

App::App()
    : m_win(nullptr, SDL_DestroyWindow)
    , m_renderer(nullptr)
{
}

App::~App()
{
    SDL_Quit();
}

int App::execute()
{
    // TODO: init only used subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return -1;
    }

    m_win.reset(SDL_CreateWindow("Survival",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 640, 480,
                                 0));
    if (m_win == nullptr) {
        return -1;
    }

    m_renderer.reset(SDL_CreateRenderer(m_win.get(), -1,
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
                     SDL_DestroyRenderer);
    if (m_renderer == nullptr) {
        return -1;
    }

    Viewport viewport(Rect(0, 0, 640, 480));
    std::shared_ptr<World> world = std::make_shared<World>(m_renderer, viewport.get_rect());
    world->add_entity(std::make_shared<LifeForm>(world, 50, 50));
    world->add_entity(std::make_shared<LifeForm>(world, 150, 200));
    bool done(false);
    SDL_Event event;
    uint32_t previous(SDL_GetTicks());

    while (!done) {
        uint32_t current = SDL_GetTicks();
        uint32_t elapsed = current - previous;
        previous = current;

        // Handle pending events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }

        const uint8_t* current_key_states = SDL_GetKeyboardState(nullptr);
        if (current_key_states[SDL_SCANCODE_UP]) {
            viewport.move(Point(0, -1));
        } else if (current_key_states[SDL_SCANCODE_DOWN]) {
            viewport.move(Point(0, 1));
        } else if (current_key_states[SDL_SCANCODE_LEFT]) {
            viewport.move(Point(-1, 0));
        } else if (current_key_states[SDL_SCANCODE_RIGHT]) {
            viewport.move(Point(1, 0));
        }

        world->handle_event(event);

        world->update(elapsed);
        world->render(viewport.get_rect());
    }

    return 0;
}
