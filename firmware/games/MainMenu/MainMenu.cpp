#include <games/MainMenu/MainMenu.hpp>
#include <generated/resources.hpp>

MainMenu::~MainMenu() = default;


MainMenu::MainMenu(ICQEngine *engine) : IGame(engine) {
    
    
}

void MainMenu::init() {
    m_engine->drawSprite(Menu_BCG,0,0);

    MenuItem mainMenuItems[] = {
        {"Games", MenuItemType::ACTION, .action = []() {
            // Start game logic
        }},
        {"Settings", MenuItemType::SUBMENU, .submenu = nullptr}, // Placeholder for submenu
        {"Exit", MenuItemType::ACTION, .action = []() {
            // Exit game logic
        }}
    };
    
    menuStack.push(mainMenuItems);

    drawMenu(menuStack.top(), sizeof(mainMenuItems) / sizeof(MenuItem), 0);
}

void MainMenu::update(const Input *input, float deltaTime) {
    // Handle input and update menu state
}

bool MainMenu::isRunning() const {
    return IsRunning;
}

void MainMenu::setRunning(bool running) {
    IsRunning = running;
}   

void MainMenu::drawMenu(const MenuItem *items, int count, int selectedIndex) {
    // Draw menu items
    position_t start = {120, 100}; // Starting position for the menu
    for (int i = 0; i < count; ++i) {
        drawMenuItem(items[i], i == selectedIndex, start);
        start.y += 32; // Move down for the next item
    }
}

void MainMenu::drawMenuItem(const MenuItem &item, bool selected, position_t position) {
    uint16_t color = 0x0000;
    FONT_TYPE type;
    if(selected) {
        type = FONT_SOURCECODEPROBOLD;
    }
    else{
        type = FONT_SOURCECODEPRO;
    }
    m_engine->drawText(item.text.c_str(), FONT_SIZE_32, type, color, position, -10);
}
