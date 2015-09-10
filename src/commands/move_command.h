#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command.h"
#include "../geometry.h"

class MoveCommand : public Command
{
public:
    MoveCommand(std::shared_ptr<LifeForm> actor, const WorldPoint &end);

    uint32_t update(uint32_t elapsed) override;

private:
    WorldPoint m_goal;
};

#endif // MOVE_COMMAND_H
