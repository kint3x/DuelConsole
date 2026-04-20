#include <games/SlideLama/SlideLama.hpp>
#include <ICQEngine/include/ICQTypes.hpp>
#include <generated/resources.hpp>

// Define the frame durations array first
static uint16_t breakDurations[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

animation_t BlockBreakAnim(
    AnimationType::SPRITE_ANIMATION,
    SpriteAnimationData{
        &AnimationBreak_10_35x40,
        nullptr,
        breakDurations,
        10,
        35,
        40,
        {0, 0},
        {0, 0, 35, 40}
    }
);