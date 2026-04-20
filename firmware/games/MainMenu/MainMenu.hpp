#pragma once

#include <games/IGame.hpp>
#include <string>
#include <stack>

enum class MenuItemType {
    ACTION,        // triggers something (Start Game)
    SUBMENU,       // opens another menu
    TOGGLE,        // on/off (sound, fullscreen)
    VALUE_INT,     // integer value (volume 0–100)
    VALUE_FLOAT,   // float value (gamma, sensitivity)
    SELECT,        // choose from list (resolution, difficulty)
    INPUT,         // text input (player name)
    BACK,          // go back (often better than EXIT)
    SEPARATOR,     // visual only (no interaction)
};

struct Menu;

struct MenuItem {
    std::string text;
    MenuItemType type;

    union {
        void (*action)();     // ACTION
        Menu* submenu;       // SUBMENU
        bool* toggleValue;    // TOGGLE
        int* intValue;        // VALUE_INT
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
    MainMenu(ICQEngine* engine);
    ~MainMenu() override;

    // IGame interface
    void update(const Input *input, uint32_t delta) override;
    void init() override;

private:
    void moveSelection(int direction);
    void selectItem();
    void drawMenu(Menu *menu);
    void drawMenuItem(MenuItem *item, bool selected, position_t *position);


    uint64_t timeoutWaitUntil = 0;
    std::stack<Menu *> menuStack;

};


extern Menu mainMenu;