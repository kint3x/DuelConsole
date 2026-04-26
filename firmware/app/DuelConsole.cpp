#include <app/DuelConsole.hpp>
#include <ICQEngine/include/ICQEngine.hpp>
#include <app/games/SlideaLama/SlideLama.hpp>

void DuelConsole::run(){

    while (true){

    }
}



DuelConsole::DuelConsole(PlatformAPI *deviceAPI):device(deviceAPI),engine(480, 320)
{
}

DuelConsole::~DuelConsole()
{
}