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

struct MenuItem {
    std::string text;
    MenuItemType type;

    union {
        void (*action)();     // ACTION
        MenuItem* submenu;     // SUBMENU
        bool* toggleValue;    // TOGGLE
        int* intValue;        // VALUE_INT
    };
};


class MainMenu : public IGame
{
public:
    MainMenu(ICQEngine* engine);
    ~MainMenu() override;

    // IGame interface
    void update(const Input *input, float deltaTime) override;
    void init() override;
    bool isRunning() const override;
    void setRunning(bool running) override;

private:
    void moveSelection(int direction);
    void selectItem();
    void drawMenu(const MenuItem *items, int count, int selectedIndex);
    void drawMenuItem(const MenuItem &item, bool selected, position_t position);

    std::stack<MenuItem*> menuStack;
};