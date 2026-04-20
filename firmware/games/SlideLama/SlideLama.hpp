#include <games/IGame.hpp>
#include <games/SlideLama/SlideLamaGrid.hpp>
#include <queue>



class SlideLama : public IGame
{
public:
    SlideLama(ICQEngine* engine);
    ~SlideLama() override;

    // IGame interface
    void update(const Input *input, uint32_t delta) override;
    void init() override;



private:
    void drawBackground();
    void drawLogicGrid();
    void drawCurrSlideStone(uint32_t delta);
    void moveCurrSlideStone(const Input *input);
    void slideBlock(SlideLamaBlockType block, SlideBlockPosition* slot);
    bool isHosted = false;

    SlideLamaGrid grid;
    SlideBlockPosition currentBlockSlot;
    int timeoutWaitUntil = 0;
    std::queue<SlideLamaBlockType> nextBlocks;

};
framebuffer_t *getBlockSprite(SlideLamaBlockType type);
extern animation_t BlockBreakAnim;