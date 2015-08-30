#include <SDL.h>
#include "app.h"

int main(int argc, char* argv[]) {

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    App app;
    return app.execute();
}
