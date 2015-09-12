#include <assert.h>
#include "command.h"
#include "../lifeform.h"

Command::Command(LifeForm *actor)
    : m_actor(actor)
    , m_done(false)
{
    assert(m_actor != nullptr);
}

bool Command::done() const
{
    return m_done;
}
