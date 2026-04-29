#include <app/DuelConsole.hpp>
#include <app/CommonTypes.hpp>

void pollInput(Input *i){

}
PlatformAPI pc = {
    .pollInput = &pollInput
};


int main() {

    DuelConsole console(&pc);
    while (true) {
        console.run();
    }

    return 0;
}