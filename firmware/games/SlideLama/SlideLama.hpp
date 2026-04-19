#include <games/IGame.hpp>
#include <games/SlideLama/SlideLamaGrid.hpp>
#include <queue>



class SlideLama : public IGame
{
public:
    SlideLama(ICQEngine* engine);
    ~SlideLama() override;

    // IGame interface
    void update(const Input *input, uint64_t globtime) override;
    void init() override;



private:
    void drawBackground();
    void drawGrid();
    void drawCurrSlideStone();
    void moveCurrSlideStone(const Input *input);
    void slideBlock(SlideLamaBlockType block, SlideBlockPosition* slot);
    bool isHosted = false;

    SlideLamaGrid grid;
    SlideBlockPosition currentBlockSlot;
    uint64_t timeoutWaitUntil = 0;
    std::queue<SlideLamaBlockType> nextBlocks;

};