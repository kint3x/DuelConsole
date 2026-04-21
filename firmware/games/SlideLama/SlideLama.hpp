#include <games/IGame.hpp>
#include <games/SlideLama/SlideLamaGrid.hpp>
#include <deque>


enum GAME_FSM{
    ON_TURN,
    WAITING_FOR_OPPONENT_TURN,
    CLIENT_WAITING_FOR_GRID_DATA,
};


class SlideLama : public IGame
{
public:
    SlideLama(ICQEngine* engine, IPlatform *plat);
    ~SlideLama() override;

    // IGame interface
    void update(const Input *input, uint32_t delta) override;
    void init() override;


private:
    void drawBackground();
    void drawLogicGrid();
    void drawClipSides();
    void drawBlockQueue();
    void drawTurnSlideStone(uint32_t delta,SlideBlockPosition &slideBlock);
    void moveCurrSlideStone(const Input *input);
    void slideBlock(SlideLamaBlockType block, SlideBlockPosition* slot);

    
    SlideLamaGrid grid;
    SlideBlockPosition currentBlockSlot;
    std::deque<SlideLamaBlockType> nextBlocks;
    bool onTurn=true;
    bool isHost = true;
    int timeoutWaitUntil = 0;

};
framebuffer_t *getBlockSprite(SlideLamaBlockType type);
extern animation_t BlockBreakAnim;