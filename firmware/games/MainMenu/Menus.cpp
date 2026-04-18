#include <games/MainMenu/MainMenu.hpp>
#include <generated/resources.hpp>  
#include <generated/fonts.hpp>

MenuItem mainMenuItems[] = {
    {"Create Game", MenuItemType::ACTION, .action = []() {
        // Start game logic
    }},
    {"Join Game", MenuItemType::ACTION, .action = []() {
        // Start game logic
    }},
    {"Settings", MenuItemType::SUBMENU, .submenu = nullptr}, // Placeholder for submenu
};

Menu mainMenu = {
        .title = "Main Menu",
        .items = mainMenuItems,
        .itemCount = sizeof(mainMenuItems) / sizeof(MenuItem),
        .perPage = 5,
        .selectedIndex = 0,
        .start_position = {120, 100},
    };