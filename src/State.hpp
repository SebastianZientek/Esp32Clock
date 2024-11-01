#pragma once

#include <memory>

template <typename Context>
class State
{
public:
    State(std::shared_ptr<Context> context)
        : m_context(context)
    {
    }
    virtual void process() = 0;
    virtual void shortClick() = 0;
    virtual void longClick() = 0;

private:
    std::shared_ptr<Context> m_context;
};
