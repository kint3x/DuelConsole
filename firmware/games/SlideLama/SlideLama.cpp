#include <games/SlideLama/SlideLama.hpp>
#include <ICQEngine/include/ICQEngine.hpp>
#include <generated/resources.hpp>

#include <iostream>



framebuffer_t *getBlockSprite(SlideLamaBlockType type) {
    switch(type) {
        case SlideLamaBlockType::BELL:
            return &SlideLamaBell;
        case SlideLamaBlockType::BANANA:
            return &SlideLamaBanana;
        case SlideLamaBlockType::PLUM:
            return &SlideLamaPlum;
        case SlideLamaBlockType::PEAR:
            return &SlideLamaPear;
        case SlideLamaBlockType::CHERRY:
            return &SlideLamaCherry;
        case SlideLamaBlockType::BAR:
            return &SlideLamaBar;
        case SlideLamaBlockType::SEVEN:
            return &SlideLamaSeven;
        default:
            return nullptr; // or a default sprite for EMPTY
    }
}


SlideLama::~SlideLama() = default;

SlideLama::SlideLama(ICQEngine *engine, IPlatform* plat, bool host) : IGame(engine,plat),grid(engine),timeoutWaitUntil(0),isHost(host) {
    
    
}

void SlideLama::drawBlockQueue(){
    position_t block1={430,131};
    position_t block2={445,78};
    
        
    framebuffer_t* sprite =getBlockSprite(nextBlocks[1]);
    m_engine->drawSprite(sprite, block1.x, block1.y);

    sprite = getBlockSprite(nextBlocks[2]);
    m_engine->drawSprite(sprite, block2.x, block2.y);

}

void SlideLama::init() {
    drawBackground();

    if(isHost){
        grid.generateCells();
        nextBlocks.push_back(grid.pickRandomBlock());
        nextBlocks.push_back(grid.pickRandomBlock());
        nextBlocks.push_back(grid.pickRandomBlock());
        state = ON_TURN;
    }
    
    
    currentBlockSlot = {SlideBlockSlotSide::RIGHT, nextBlocks.front(), 4};
  
}

void SlideLama::getNextStone(){

}

void SlideLama::makeTurn(){

    slideBlock(currentBlockSlot.type, &currentBlockSlot);
    state=WAITING_FOR_OPPONENT_TURN;
    turn_number++;
    //TODO SEND TURN TO OPPONENT
    nextBlocks.pop_front();

    if(isHost){
        nextBlocks.push_back(grid.pickRandomBlock());     
    }
    else{
        
        //get next block from host and push it back
    }

    currentBlockSlot.type=nextBlocks.front();
    
}

void SlideLama::update(const Input *input, uint32_t delta) {
    // Handle input and update game state
    (void) input;
    (void) delta;
    
    switch (state)
    {
        case ON_TURN:
            //handle input
            if(timeoutWaitUntil>0) {
                timeoutWaitUntil=timeoutWaitUntil-delta;
            }
            else{
                if(input->up || input->down || input->left || input->right) {
                    timeoutWaitUntil = 150; // 200 ms timeout
                    moveCurrSlideStone(input);
                }
                if(input->x) {  
                    std::cout << "Pressed X" << std::endl; 
                    timeoutWaitUntil = 150; // 200 ms timeout
                    makeTurn();
                }
            }
            break;
        

        default:
            break;
    }

    if(m_engine->animations.empty()){
        drawLogicGrid(); //DrawAlwaysLogicState
        drawTurnSlideStone(delta, currentBlockSlot);
        drawBlockQueue();
   
    }
    else{
        drawClipSides();
    }

    
}

void SlideLama::drawClipSides()
{
    Rect clip = {{340,40},70,210};
    m_engine->drawSpriteClipped(&SlideaLamaBCG,0,0,clip);
    clip = {{145,0},200,42};
    m_engine->drawSpriteClipped(&SlideaLamaBCG,0,0,clip);
    clip = {{71,35},70,210};
    m_engine->drawSpriteClipped(&SlideaLamaBCG,0,0,clip);
}

void SlideLama::drawTurnSlideStone(uint32_t delta,SlideBlockPosition &slideBlock)
{
    position_t pos;
    uint16_t skewAdj = 4;
    
    // Floating animation (left-right movement)
    static uint32_t floatTime = 0;
    static int8_t floatOffset = 0;
    static int8_t floatDirection = 1;
    
    // Update floating animation
    floatTime += delta;
    if (floatTime > 100) {  // Move every 10ms
        floatTime = 0;
        floatOffset += floatDirection;
        
        if (floatOffset >= 3) floatDirection = -1;  // Move right 3 pixels
        if (floatOffset <= -3) floatDirection = 1;  // Move left 3 pixels
    }

    if(slideBlock.side == SlideBlockSlotSide::TOP) {
        pos.x = slideBlock.index*35 + LEFT_TOP_GRID_START_POS.x +floatOffset;
        pos.y = 2;
    }
    else if(slideBlock.side == SlideBlockSlotSide::LEFT) {
        pos.x = LEFT_TOP_GRID_START_POS.x - 60 - slideBlock.index*skewAdj + floatOffset;  // Add float offset
        pos.y = slideBlock.index*41 + LEFT_TOP_GRID_START_POS.y-6;
    }
    else { // RIGHT
        pos.x = 60+LEFT_TOP_GRID_START_POS.x + 4*35 + slideBlock.index*skewAdj + floatOffset;  // Add float offset
        pos.y = slideBlock.index*41 + LEFT_TOP_GRID_START_POS.y-5;
    }
    
    framebuffer_t* sprite = getBlockSprite(slideBlock.type);
    if(sprite) {
        drawClipSides();
        m_engine->drawSprite(sprite, pos.x, pos.y);
    }
}

void SlideLama::drawBackground()
{
    // Draw the background of the game
    // This is a placeholder implementation, you can replace it with your actual drawing code
    m_engine->drawSprite(&SlideaLamaBCG, 0, 0);
}


void SlideLama::drawLogicGrid(){
    
    Rect r = {{static_cast<uint16_t>(LEFT_TOP_GRID_START_POS.x-5), static_cast<uint16_t>(LEFT_TOP_GRID_START_POS.y-5)}, 5*35+10, 5*40+10};
    m_engine->drawSpriteClipped(&SlideaLamaBCG,0,0,r);

    for(uint16_t y=0; y < SLIDELAMA_GRIDSIZE; y++) {
        for(uint16_t x=0; x < SLIDELAMA_GRIDSIZE; x++) {
            SlideLamaBlockType block = *grid.at({x,y});
            // Draw the block at position (x, y) based on its type
            position_t pos= {static_cast<uint16_t>(LEFT_TOP_GRID_START_POS.x + x*35), static_cast<uint16_t>(LEFT_TOP_GRID_START_POS.y + y*40)};
            framebuffer_t* sprite = getBlockSprite(block);
            if (sprite) {
                m_engine->drawSprite(sprite, pos.x, pos.y);
            }
        }
    }
}

void SlideLama::moveCurrSlideStone(const Input *input) {
    // Update currentBlockSlot based on input
    if(input->up){
        if(currentBlockSlot.side == SlideBlockSlotSide::RIGHT || currentBlockSlot.side == SlideBlockSlotSide::LEFT) {
            if(currentBlockSlot.index > 0) {
                currentBlockSlot.index--;
            }
            else{
                currentBlockSlot.index = (currentBlockSlot.side == SlideBlockSlotSide::RIGHT) ? 4 : 0;
                currentBlockSlot.side = SlideBlockSlotSide::TOP;
            }
        }
    }
    if(input->down){
        if(currentBlockSlot.side == SlideBlockSlotSide::RIGHT || currentBlockSlot.side == SlideBlockSlotSide::LEFT) {
            if(currentBlockSlot.index < 4) {
                currentBlockSlot.index++;
            }
        }
    }
    if(input->left){
        if(currentBlockSlot.side == SlideBlockSlotSide::TOP) {
            if(currentBlockSlot.index > 0) {
                currentBlockSlot.index--;
            }
            else{
                currentBlockSlot.side = SlideBlockSlotSide::LEFT;
                currentBlockSlot.index = 0;
            }
        }
        if(currentBlockSlot.side == SlideBlockSlotSide::RIGHT) {
            currentBlockSlot.side = SlideBlockSlotSide::LEFT;
        }
    }
    if(input->right){
        if(currentBlockSlot.side == SlideBlockSlotSide::TOP) {
            if(currentBlockSlot.index < 4) {
                currentBlockSlot.index++;
            }
            else{
                currentBlockSlot.side = SlideBlockSlotSide::RIGHT;
                currentBlockSlot.index = 0;
            }
        }
        if(currentBlockSlot.side == SlideBlockSlotSide::LEFT) {
            currentBlockSlot.side = SlideBlockSlotSide::RIGHT;
        }
    }

}

void SlideLama::slideBlock(SlideLamaBlockType block, SlideBlockPosition* slot)
{
    switch (slot->side)
    {
        case SlideBlockSlotSide::TOP:
            grid.slideFromTop(block, slot->index);
            break;
        case SlideBlockSlotSide::RIGHT:
            grid.slideFromRight(block, slot->index);
            break;
        case SlideBlockSlotSide::LEFT:
            grid.slideFromLeft(block, slot->index);
            break;
        default:
            break;
    }
    
    grid.resolveGravity();
    resolvedSet set;

    uint16_t move_score = 0;
    uint16_t combo_multiplier = 1;
    (void) move_score;
    (void) combo_multiplier;
    
    while(grid.findSetToResolve(set))
    {
        std::vector<animation_t> parallel_animations;
        for(uint16_t i=0; i<set.count; i++) {
            if(set.direction == ResolveDirection::HORIZONTAL) {
                auto& sprite = std::get<SpriteAnimationData>(BlockBreakAnim.data);
                sprite.BackgroundData = getBlockSprite(set.type);
                sprite.target = grid.gridIdxToFramePos(set.tilePos.x + i, set.tilePos.y);
                parallel_animations.push_back(BlockBreakAnim);
            }
            else {
               auto& sprite = std::get<SpriteAnimationData>(BlockBreakAnim.data);
                sprite.BackgroundData = getBlockSprite(set.type);
                sprite.target = grid.gridIdxToFramePos(set.tilePos.x, set.tilePos.y + i);
                parallel_animations.push_back(BlockBreakAnim);
            }
        }
        m_engine->animations.push(std::move(parallel_animations));
        
        grid.resolveSet(set);
        grid.resolveGravity();
    }

}