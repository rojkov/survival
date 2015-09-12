#include "move_command.h"
#include "../lifeform.h"

MoveCommand::MoveCommand(LifeForm* actor, const WorldPoint &end)
    : Command(actor)
    , m_goal(end)
{
}

uint32_t MoveCommand::update(uint32_t elapsed)
{
    const double velocity = 0.03;
    uint32_t result(0);
    double movement = velocity * elapsed;
    auto current = m_actor->get_pos();
    auto diff = m_goal.substruct(current);
    double distance = diff.abs();
    if (movement < distance) {
        current = current.sum(diff.scale(movement/distance));
    } else {
        current = m_goal;
        m_done = true;
        result = (movement - distance)/velocity;
    }
    m_actor->move_to(current);

    return result;
}
