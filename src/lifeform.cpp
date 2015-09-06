#include "SDL.h"
#include "lifeform.h"

LifeForm::LifeForm(double pos_x, double pos_y)
    : m_pos_x(pos_x)
    , m_pos_y(pos_y)
{
}

void LifeForm::update(uint32_t elapsed)
{
    m_pos_x += 0.03 * elapsed;
    m_pos_y += 0.03 * elapsed;
}

void LifeForm::render(SDL_Renderer* renderer)
{
    SDL_Rect rect {(int)round(m_pos_x), (int)round(m_pos_y), 8, 8};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
