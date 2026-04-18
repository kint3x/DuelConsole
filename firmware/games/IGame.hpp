#pragma once

#include <cstdint>
#include <platform/IPlatform.hpp>

class Engine;

class IGame
{
public:
    virtual IGame(Engine *engine) : m_engine(engine), IsRunning(true) {};
    virtual ~IGame() = default;

    // Game lifecycle
    virtual void update(const Input *input, float deltaTime) = 0;

    // State management
    virtual bool isRunning() const = 0;
    virtual void setRunning(bool running) = 0;
protected:
    Engine* m_engine;
    bool IsRunning;
};