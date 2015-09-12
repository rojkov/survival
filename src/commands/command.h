#ifndef COMMAND_H
#define COMMAND_H

#include <memory>

class LifeForm;

class Command {
public:
    Command(LifeForm *actor);

    bool done() const;

    virtual uint32_t update(uint32_t elapsed) = 0;

protected:
    LifeForm* m_actor;
    bool m_done;
};

#endif // COMMAND_H
