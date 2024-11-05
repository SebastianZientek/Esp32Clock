#pragma once

#include <memory>
#include "Event.hpp"


class StateKeeper;

class State
{
public:
    State(std::shared_ptr<StateKeeper> stateKeeper):
        m_stateKeeper{stateKeeper}
    {
    }
    virtual void process() = 0;
    virtual void onEvent(Event event) = 0;

    template<typename StateType, typename ...Args>
    void changeState(Args... args)
    {
        m_stateKeeper->state = std::make_shared<StateType>(m_stateKeeper, std::forward<Args>(args)...);
    }

private:
    std::shared_ptr<StateKeeper> m_stateKeeper;
};

struct StateKeeper
{
    std::shared_ptr<State> state;
};
