#include "command.h"

Command::Command(std::shared_ptr<LifeForm> actor)
    : m_actor(actor)
    , m_done(false)
{
}

bool Command::done() const
{
    return m_done;
}
