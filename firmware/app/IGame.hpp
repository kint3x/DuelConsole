#pragma once

#include <cstdint>
#include <ICQEngine/include/ICQEngine.hpp>
#include <app/CommonTypes.hpp>


class IGame
{
public:
    IGame(ICQEngine* engine) : m_engine(engine){};
    virtual ~IGame() = default;

    // Game lifecycle
    virtual void init() {};
    virtual void update(const Input *input, uint32_t delta) = 0;
    

protected:
    ICQEngine* m_engine;
};

typedef uint32_t Opponent_ID; 

enum GameType{
    SLIDEALAMA
};

enum GameMode{
    SINGLE_PLAYER,
    MULTI_PLAYER
};


struct GameEntity{
    GameType type;
    GameMode gm;
    Opponent_ID opp_id;
    IGame *game;
};