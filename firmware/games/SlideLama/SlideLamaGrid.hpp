#pragma once
#include <ICQEngine/include/ICQTypes.hpp>
#include <ICQEngine/include/ICQEngine.hpp>

#define SLIDELAMA_GRIDSIZE 5

const position_t LEFT_TOP_GRID_START_POS = {152,45}; // starting position left corner of 0,0
const uint32_t blockSpanFreq[] = {
    0,//EMPTY
    18,//BELL
    16,//BANANA
    15,//PLUM
    14,//PEAR
    14,//CHERRY
    12,//BAR
    11//SEVEN
};

enum class SlideLamaBlockType {
    EMPTY,
    BELL,
    BANANA,
    PLUM,
    PEAR,
    CHERRY,
    BAR,
    SEVEN,
    COUNT
};

enum class SlideBlockSlotSide {
    LEFT,
    TOP,
    RIGHT
};

struct SlideBlockPosition{
   SlideBlockSlotSide side;
   SlideLamaBlockType type;
   uint8_t index; // 0-4 for left and right, 0-4 for top 
};

enum class ResolveDirection {
    HORIZONTAL,
    VERTICAL
};
 
struct resolvedSet {
    position_t tilePos;
    uint8_t count;
    SlideLamaBlockType type;
    ResolveDirection direction;
};



class SlideLamaGrid {
public:
    SlideLamaGrid(ICQEngine *m_engine);

    SlideLamaBlockType cells[SLIDELAMA_GRIDSIZE * SLIDELAMA_GRIDSIZE];  // or enum Tile

    SlideLamaBlockType* at(position_t pos);

    void init(std::vector<SlideLamaBlockType> blocks);
    SlideLamaBlockType pickRandomBlock();
    void generateCells();
    void putExact(position_t pos, SlideLamaBlockType type);
    position_t gridIdxToFramePos(uint16_t x,uint16_t y);

    void slideFromRight(SlideLamaBlockType block, int row);
    void slideFromLeft(SlideLamaBlockType block, int row);
    void slideFromTop(SlideLamaBlockType block, int column);

    void slideBlock(SlideLamaBlockType block, SlideBlockPosition* slot);

    bool findSetToResolve(resolvedSet &set);

    void resolveSet(const resolvedSet &set);

    void resolveGravity();
    uint16_t resolveMatches();

    ICQEngine *m_engine;
};