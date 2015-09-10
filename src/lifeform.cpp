#include "SDL.h"
#include "lifeform.h"

LifeForm::LifeForm(double pos_x, double pos_y)
    : m_pos_x(pos_x)
    , m_pos_y(pos_y)
{
}

WorldPoint LifeForm::get_pos() const
{
    return WorldPoint(m_pos_x, m_pos_y);
}

void LifeForm::move_to(const WorldPoint &new_position)
{
    m_pos_x = new_position.x();
    m_pos_y = new_position.y();
}

void LifeForm::update(uint32_t elapsed)
{
}

void LifeForm::render(SDL_Renderer* renderer)
{
    SDL_Rect rect {(int)round(m_pos_x), (int)round(m_pos_y), 8, 8};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
