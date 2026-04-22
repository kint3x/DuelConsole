#pragma once

#include <games/IGame.hpp>
#include <string>
#include <stack>

enum class MainMenuState{
    MENU,
    ADVERTISING_GAME,
    DISCOVERING,
};

enum class MenuItemType {
    ACTION,        // triggers something (Start Game)
    SUBMENU,       // opens another menu
    CHANGE_STATE,  //change state to defined
    BACK,          // go back (often better than EXIT)


};

struct MenuItem {
    std::string text;
    std::function<void()> onClick;
};

struct Menu {
    std::vector<MenuItem> items;
    int selectedIndex = 0;
    int itemCount = 0;
};

class MainMenu : public IGame
{
public:
    MainMenu(ICQEngine* engine, IPlatform* plat);
    ~MainMenu() override;

    // IGame interface
    void update(const Input *input, uint32_t delta) override;
    void init() override;

    MainMenuState menuState = MainMenuState::MENU;
private:
    void moveSelection(int direction);
    void selectItem();
    void handleMenuClick();
    void drawMenu(Menu *menu);
    void drawMenuItem(MenuItem *item, bool selected, position_t *position);


    int timeoutWaitUntil = 0;
    std::stack<Menu*> menuStack;

    Menu mainMenu;
    Menu createGameMenu;
    Menu joinGameMenu;
    std::vector<device_id> discoveredDevices;
    uint32_t discoveryTimer = 0;

};

