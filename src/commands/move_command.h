#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command.h"
#include "../worldposition.h"

class MoveCommand : public Command
{
public:
    MoveCommand(LifeForm* actor, const WorldPosition &end);

    uint32_t update(uint32_t elapsed) override;

private:
    WorldPosition m_goal;
};

#endif // MOVE_COMMAND_H
