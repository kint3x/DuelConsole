#include <games/IGame.hpp>
#include <games/SlideLama/SlideLamaGrid.hpp>
#include <deque>


enum GAME_FSM{
    ON_TURN,
    WAITING_FOR_OPPONENT_TURN,
    CLIENT_WAITING_FOR_GRID_DATA,
};

enum GameMessageType : uint8_t{
    STONE_POSITION,
    FULL_STATE,
    TURN_INFO,
    NEXT_STONE
};

struct FullStateMessage{
    uint32_t turn_number;
    uint8_t cells[SLIDELAMA_GRIDSIZE*SLIDELAMA_GRIDSIZE]; //25bytes
    uint8_t nextBlocks[3]; //28bytes
};
struct StoneMessage{
    uint32_t turn_number;
    SlideBlockPosition position;
};

class SlideLama : public IGame
{
public:
    SlideLama(ICQEngine* engine, IPlatform *plat, bool host);
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

    
    void getNextStone();
    void makeTurn();

    SlideLamaGrid grid;
    SlideBlockPosition currentBlockSlot;
    std::deque<SlideLamaBlockType> nextBlocks;
    
    GAME_FSM state;
    uint32_t turn_number=1;
    bool isHost;
    int timeoutWaitUntil;

};
framebuffer_t *getBlockSprite(SlideLamaBlockType type);
extern animation_t BlockBreakAnim;