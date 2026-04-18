#include <games/MainMenu/MainMenu.hpp>
#include <generated/resources.hpp>  
#include <generated/fonts.hpp>

MenuItem mainMenuItems[] = {
    {"Games", MenuItemType::ACTION, .action = []() {
        // Start game logic
    }},
    {"Settings", MenuItemType::SUBMENU, .submenu = nullptr}, // Placeholder for submenu
    {"Exit", MenuItemType::ACTION, .action = []() {
        // Exit game logic
    }}
};

Menu mainMenu = {
        .title = "Main Menu",
        .items = mainMenuItems,
        .itemCount = 3,
        .perPage = 5,
        .selectedIndex = 0,
        .start_position = {120, 100},
    };