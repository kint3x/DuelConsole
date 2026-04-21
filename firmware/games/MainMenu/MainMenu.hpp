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

struct Menu;

struct MenuItem {
    std::string text;
    MenuItemType type;

    union {
        void (*action)();     // ACTION
        Menu* submenu;       // SUBMENU
    };
};

struct Menu{
    std::string title;
    MenuItem* items;
    int itemCount;
    int perPage = 5;
    uint16_t selectedIndex = 0;
    position_t start_position;
    FONT_SIZE fontSize = FONT_SIZE_32;
    FONT_TYPE fontType = FONT_SOURCECODEPRO;
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
    std::stack<Menu> menuStack;

};


extern Menu mainMenu;