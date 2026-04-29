#include <app/DuelConsole.hpp>
#include <app/menu/MainMenu.hpp>



void DuelConsole::run(){
    GameEntity currGame;
    
    MainMenu menu(&engine,&currGame);
    


    Input i;    
    uint32_t delta;

    while (true){
        device->pollInput(&i);
        menu.update(&i,delta);

    }
}



DuelConsole::DuelConsole(PlatformAPI *deviceAPI):device(deviceAPI),engine(480, 320)
{
}


DuelConsole::~DuelConsole()
{
}


uint32_t DuelConsole::getRand()
{
    return 0;
}