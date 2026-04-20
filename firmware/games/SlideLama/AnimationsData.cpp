#include <games/SlideLama/SlideLama.hpp>
#include <ICQEngine/include/ICQTypes.hpp>
#include <generated/resources.hpp>

// Define the frame durations array first
static uint16_t breakDurations[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

animation_t BlockBreakAnim = {
    .type = AnimationType::SPRITE_ANIMATION,
    .data = {
        .spriteAnimation = {
            .AnimationFrames = &AnimationBreak_10_35x40,  // Set this to your actual animation frames
            .BackgroundData = nullptr,   // If not used
            .frameDurations = breakDurations,
            .Framecnt = 10,
            .frameW = 35,    // Adjust based on your frame width
            .frameH = 40,    // Adjust based on your frame height
            .target = {0, 0}, // Set default or specific position
            .BackgroundClipped = {0, 0, 35, 40}  // If not used
        }
    },
    .currentFrame = 0,
    .accumulatedTime = 0,
    .finished = false
};