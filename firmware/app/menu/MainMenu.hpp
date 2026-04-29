#pragma once
#include <ICQEngine/include/ICQTypes.hpp>
#include <ICQEngine/include/ICQEngine.hpp>
#include <app/IGame.hpp>
#include <app/CommonTypes.hpp>

enum MenuRetCode{
    MENU_OK,
    MENU_START_GAME
};

class MainMenu
{
public:
    MainMenu(ICQEngine* engine, GameEntity *e);
    ~MainMenu();

    MenuRetCode update(Input *input, uint32_t delta);
    void drawState();


private:
    ICQEngine* engine;
    GameEntity* game;
};

