#ifndef ICQ_TYPES_HPP
#define ICQ_TYPES_HPP

#include <cstdint>
#include <variant>

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

struct SpriteAnimationData {
    framebuffer_t* AnimationFrames = nullptr;
    framebuffer_t* BackgroundData = nullptr;
    uint16_t* frameDurations = nullptr;
    int Framecnt = 0;
    uint16_t frameW = 0;
    uint16_t frameH = 0;
    position_t target{0, 0};
    Rect BackgroundClipped{0, 0, 0, 0};
};

struct MoveAnimationData {
    position_t start{0, 0};
    position_t end{0, 0};
    position_t current{0, 0};
    uint32_t duration = 0;
    framebuffer_t* sprite = nullptr;
    framebuffer_t* topOverlay = nullptr;
    framebuffer_t* bottomUnderlay = nullptr;
};

using AnimationData = std::variant<SpriteAnimationData, MoveAnimationData>;
struct animation_t {
    AnimationType type = AnimationType::SPRITE_ANIMATION;
    AnimationData data = SpriteAnimationData{};

    uint16_t currentFrame = 0;
    uint32_t accumulatedTime = 0;
    bool finished = false;
    void (*onComplete)(void*) = nullptr;
    void* userData = nullptr;

    animation_t() = default;

    animation_t(AnimationType t, AnimationData d)
        : type(t),
          data(std::move(d)),
          currentFrame(0),
          accumulatedTime(0),
          finished(false),
          onComplete(nullptr),
          userData(nullptr)
    {}

    void resetAnimation();
};

#endif //ICQ_TYPES_HPP