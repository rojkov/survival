#include "SDL.h"
#include "lifeform.h"
#include "world.h"
#include "worldposition.h"
#include "commands/move_command.h"
#include "gameconstants.h"

LifeForm::LifeForm(std::weak_ptr<World> world, double pos_x, double pos_y)
    : m_world(world)
    , m_pos_x(pos_x)
    , m_pos_y(pos_y)
    , m_focused(false)
{
    auto world_p = m_world.lock();
    if (!world_p) {
        return;
    }

    m_visited_tiles.insert(world_p->location(get_pos()));
}

WorldPosition LifeForm::get_pos() const
{
    return WorldPosition(m_pos_x, m_pos_y);
}

void LifeForm::move_to(const WorldPosition &new_position)
{
    auto world = m_world.lock();
    if (!world) {
        return;
    }

    auto old_loc = world->location(get_pos());
    auto new_loc = world->location(new_position);
    if (new_loc != old_loc && m_unvisited_tiles.count(new_loc)) {
        m_unvisited_tiles.erase(new_loc);
        m_visited_tiles.insert(new_loc);
    }

    m_pos_x = new_position.x;
    m_pos_y = new_position.y;
}

void LifeForm::patrol(const std::unordered_set<GridLocation>& locations)
{
    if (!m_focused) {
        return;
    }

    m_visited_tiles.clear();
    m_unvisited_tiles = locations;
}

void LifeForm::handle_event(const SDL_Event &event)
{
    auto world = m_world.lock();
    if (!world) {
        return;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            const WorldRect viewport(world->get_viewport());
            WorldPosition pos = get_pos();
            WorldRect rect((int32_t)round(pos.x) - 8/2, (int32_t)round(pos.y) - 8/2, 8, 8);
            if (rect.contains(WorldPoint(event.button.x + viewport.x, event.button.y + viewport.y))) {
                set_focused(true);
            } else {
                set_focused(false);
            }
        } else if (focused() && event.button.button == SDL_BUTTON_RIGHT) {
            // Cancel m_lifeform's commands
            while (!m_commands.empty()) {
                m_commands.pop();
            }
            const WorldRect viewport(world->get_viewport());
            for (auto pt : world->get_path(get_pos(), WorldPosition(event.button.x + viewport.x,
                                                                 event.button.y + viewport.y))) {
                m_commands.emplace(new MoveCommand(this, WorldPosition(pt.x, pt.y)));
            }
        }
    }
}

void LifeForm::update(uint32_t elapsed)
{
    if (m_commands.empty() && !m_unvisited_tiles.empty()) {
        auto world = m_world.lock();
        if (!world) {
            return;
        }

        auto current_tile = world->location(get_pos());
        auto closest_tile = world->closest(current_tile, m_unvisited_tiles);
        if (current_tile == closest_tile) {
            m_unvisited_tiles.erase(current_tile);
            m_visited_tiles.insert(current_tile);
        } else {
            int x, y;
            std::tie(x, y) = closest_tile;
            for (auto pt : world->get_path(get_pos(),
                                           WorldPosition(x * TILE_WIDTH + TILE_WIDTH/2,
                                                         y * TILE_HEIGHT + TILE_HEIGHT/2))) {
                m_commands.emplace(new MoveCommand(this, WorldPosition(pt.x, pt.y)));
            }
        }
    }

    if (m_commands.empty()) {
        return;
    }

    auto command = m_commands.front().get();
    uint32_t timeleft = elapsed;
    while (timeleft) {
        timeleft = command->update(timeleft);
        if (command->done()) {
            m_commands.pop();
            if (m_commands.empty()) {
                break;
            }
            command = m_commands.front().get();
        }
    }
}

void LifeForm::render(SDL_Renderer* renderer)
{
    auto world = m_world.lock();
    if (!world) {
        return;
    }
    const WorldRect viewport(world->get_viewport());

    if (!m_visited_tiles.empty()) {
        SDL_BlendMode oldMode;
        int rv = SDL_GetRenderDrawBlendMode(renderer, &oldMode);
        if (rv != 0) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Failed to get blend mode: %s", SDL_GetError());
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 30);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        const WorldRect bigger_view(geom::rect::enlarge(viewport, TILE_WIDTH));
        for (auto tile : m_visited_tiles) {
            int x, y;
            std::tie(x, y) = tile;
            const WorldRect tile_rect(x * TILE_WIDTH, y * TILE_HEIGHT,
                                      TILE_WIDTH, TILE_HEIGHT);
            if (!tile_rect.is_inside(bigger_view)) {
                continue;
            }
            SDL_Rect rect(world->to_sdl_rect(tile_rect));
            SDL_RenderFillRect(renderer, &rect);
        }
        SDL_SetRenderDrawBlendMode(renderer, oldMode);
    }

    const WorldRect body((int32_t)round(m_pos_x) - 8/2, (int32_t)round(m_pos_y) - 8/2, 8, 8);
    if (!body.is_inside(geom::rect::enlarge(viewport, 8))) {
        return;
    }

    SDL_Rect rect(world->to_sdl_rect(body));
    SDL_SetRenderDrawColor(renderer, 255, 255, m_focused ? 0 : 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
