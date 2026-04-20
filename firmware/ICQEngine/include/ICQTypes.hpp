#ifndef ICQ_TYPES_HPP
#define ICQ_TYPES_HPP

#include <cstdint>

#define TRANSPARENT_COLOR 0x0001

enum RECT_FILL_MODE{
    RECT_INSIDE_MODE,
    RECT_OUTLINE_MODE
};

struct position_t
{
    uint16_t x;
    uint16_t y;
};

struct Rect {
    position_t pos;
    uint16_t w, h;
};

struct framebuffer_t
{
    uint16_t width;
    uint16_t height;
    uint16_t *pixels;
};

struct fontmap_t
{
    uint16_t font_size_px;
    uint8_t *pixels;
};

struct Settings {
    uint8_t game_selected = 0;
};

enum class GameRequest {
    NONE,
    MAIN_MENU,
    START_GAME,
    EXIT
};

enum class AnimationType
{
    SPRITE_ANIMATION,
    MOVE_ANIMATION
};

struct SpriteAnimationData{
    framebuffer_t *AnimationFrames; //for animation
    framebuffer_t *BackgroundData; //for background
    uint16_t *frameDurations;
    int Framecnt; //for animationframes
    uint16_t frameW,frameH;
    position_t target;
    Rect BackgroundClipped;
    
};
struct MoveAnimationData {
    position_t start;        // Starting position
    position_t end;          // Target position
    position_t current;      // Current interpolated position
    uint32_t duration;       // Total duration in ms
    framebuffer_t *sprite;   // The sprite to move
    framebuffer_t* topOverlay = nullptr;
    framebuffer_t* bottomUnderlay = nullptr;
};

union AnimationData{
    SpriteAnimationData spriteAnimation;
    MoveAnimationData moveAnimation;
};
struct animation_t{
    AnimationType type;
    AnimationData data;
    
    uint16_t currentFrame;    
    uint32_t accumulatedTime;
    bool finished;
    void (*onComplete)(void* userData);
    void* userData;
    
    void resetAnimation();
};

#endif //ICQ_TYPES_HPP