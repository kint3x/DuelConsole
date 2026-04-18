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


};