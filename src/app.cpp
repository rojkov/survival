#include "app.h"
#include "world.h"

#define MS_PER_FRAME 16

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
                                        SDL_RENDERER_ACCELERATED),
                     SDL_DestroyRenderer);
    if (m_renderer == nullptr) {
        return -1;
    }

    World world(m_renderer);
    bool done(false);
    SDL_Event Event;
    uint32_t previous(SDL_GetTicks());

    while (!done) {
        uint32_t current = SDL_GetTicks();
        uint32_t elapsed = current - previous;
        previous = current;

        // Handle pending events
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_QUIT) {
                done = true;
            }
        }

        world.update(elapsed);
        world.render();
        int32_t delay = current + MS_PER_FRAME - SDL_GetTicks();
        if (delay > 0) {
            SDL_Delay(delay);
        } else {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Oops, can't keep up with rendering");
        }
    }

    return 0;
}
