#include <games/MainMenu/MainMenu.hpp>
#include <generated/resources.hpp>
#include <generated/fonts.hpp>

#include <iostream>

static uint16_t breakDurations[] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};

MainMenu::~MainMenu() = default;


MainMenu::MainMenu(ICQEngine *engine,IPlatform *plat) : IGame(engine, plat) {
    
    
}

void MainMenu::init() {

    // --- MAIN MENU ---
    mainMenu.items = {
        {"Create Game", [this]() {
            menuStack.push(&createGameMenu);
        }},
        {"Join Game", [this]() {
            platform->startDiscovery();
            discoveryTimer = 0;
            menuStack.push(&joinGameMenu);
        }},
        {"Settings", [this]() {
            // empty for now
        }}
    };
    mainMenu.itemCount = mainMenu.items.size();

    // --- CREATE GAME MENU ---
    createGameMenu.items = {
        {"SlideaLama", [this]() {

            // Push animation
            animation_t firstAnim(AnimationType::SPRITE_ANIMATION, 
                    SpriteAnimationData{
                        &AnimationLoader_8_150x150,
                        nullptr,
                        breakDurations,
                        8,
                        150,
                        150,
                        {220, 150},
                    });

            m_engine->animations.push({firstAnim});
            m_engine->animations.push({firstAnim});
            m_engine->animations.push({firstAnim});
            m_engine->animations.push({firstAnim});
            m_engine->animations.push({firstAnim});

            // Start advertising
            platform->startAdvertising(SLIDEALAMA);

        }},
        {"Back", [this]() {
            if (platform->isAdvertising()) {
                platform->stopAdvertising();
            }
            while(!m_engine->animations.empty())
                m_engine->animations.pop();
            menuStack.pop();
        }}
    };
    createGameMenu.itemCount = createGameMenu.items.size();

    // --- JOIN GAME MENU ---
    joinGameMenu.items = {
        {"Back", [this]() {
            platform->stopDiscovery();
            menuStack.pop();
        }}
    };
    joinGameMenu.itemCount = joinGameMenu.items.size();

    // Push main menu
    menuStack.push(&mainMenu);
}

void MainMenu::update(const Input *input, uint32_t delta) {
    // Handle input and update menu state


    if(timeoutWaitUntil > 0) {
        timeoutWaitUntil = timeoutWaitUntil-delta;
    }
    else{
        if(input->up || input->down || input->x){
            timeoutWaitUntil = 200; // 200 ms timeout
            if (input->up) {
                moveSelection(-1);
            }
            else if (input->down) {
                moveSelection(1);
            }
            else if(input->x)
            {
                handleMenuClick();
            }
        }  
    }

    // Handle Join Game dynamic list
    if (menuStack.top() == &joinGameMenu) {

        discoveryTimer += delta;

        if (discoveryTimer >= 1000) {
            discoveryTimer = 0;

            discoveredDevices.clear();
            platform->pollDiscovered(discoveredDevices);

            // rebuild menu
            joinGameMenu.items.clear();

            for (auto &dev : discoveredDevices) {
                joinGameMenu.items.push_back({
                    "TEST",  // assuming device_id has name
                    [this, dev]() {
                        // TODO: connect later
                    }
                });
            }

            // Add Back
            joinGameMenu.items.push_back({
                "Back", [this]() {
                    platform->stopDiscovery();
                    menuStack.pop();
                }
            });

            joinGameMenu.itemCount = joinGameMenu.items.size();
            joinGameMenu.selectedIndex = 0;
        }
    }

    m_engine->drawSprite(&Menu_BCG,0,0);
    drawMenu(menuStack.top());
     
}

void MainMenu::handleMenuClick() {
    Menu *menu = menuStack.top();

    if (menu->selectedIndex < 0 || menu->selectedIndex >= menu->items.size())
        return;

    menu->items[menu->selectedIndex].onClick();
}



void MainMenu::drawMenu(Menu *menu) {

    position_t pos = {120, 100};

    for (int i = 0; i < menu->items.size(); i++) {
        bool selected = (i == menu->selectedIndex);
        drawMenuItem(&menu->items[i], selected, &pos);
        pos.y += 40;
    }
}

void MainMenu::drawMenuItem(MenuItem *item, bool selected, position_t *position) {
    uint16_t color = 0x0000;
    FONT_TYPE type;
    if(selected) {
        type = FONT_SOURCECODEPROBOLD;
    }
    else{
        type = FONT_SOURCECODEPRO;
    }
    m_engine->drawText(item->text.c_str(), FONT_SIZE_32, type, color, *position, -10);
}

void MainMenu::moveSelection(int direction) {
    Menu *menu = menuStack.top();

    int newIndex = menu->selectedIndex + direction;

    if (newIndex < 0 || newIndex >= menu->itemCount)
        return;

    menu->selectedIndex = newIndex;
}