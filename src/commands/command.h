#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include "../lifeform.h"

class Command {
public:
    Command(std::shared_ptr<LifeForm> actor);

    bool done() const;

    virtual uint32_t update(uint32_t elapsed) = 0;

protected:
    std::shared_ptr<LifeForm> m_actor;
    bool m_done;
};

#endif // COMMAND_H
