#ifndef APP_H
#define APP_H

#include "SDL.h"
#include <memory>

class App
{
public:
    App();
    ~App();

    int execute();

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_win;
    std::shared_ptr<SDL_Renderer> m_renderer;
};

#endif // APP_H
