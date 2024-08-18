#include "../core.h"
#include "../types.h"

#include <algorithm>

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

namespace inx
{
    struct InputState
    {
        Keyboard keyboard;
    };

    static InputState INPUT_STATE;

    void Keyboard::init()
    {
        auto& kb = INPUT_STATE.keyboard;

        // TODO(selina): should probably have a warning of some kind here - 18/08
        if (kb._initialized) return;

        kb._initialized = true;
        std::fill(kb._is_down.begin(), kb._is_down.end(), false);
        std::fill(kb._was_down.begin(), kb._was_down.end(), false);
    }

    void Keyboard::update()
    {
        auto& kb = INPUT_STATE.keyboard;

        kb._was_down = kb._is_down;
        
        // populdate is_down with new state from SDL
        const u8* state = SDL_GetKeyboardState(nullptr);
        for (size_t i = 4; i < KEY_COUNT; i++)
            kb._is_down[i] = state[i];
    }

    bool Keyboard::key_up(Key key)
    {
        auto& kb = INPUT_STATE.keyboard;

        // not down + was down = just went up
        auto result = !kb._is_down[key] && kb._was_down[key];
        return result;
    }

    bool Keyboard::key_down(Key key)
    {
        auto& kb = INPUT_STATE.keyboard;

        // is down + was not down = just went down
        auto result = kb._is_down[key] && !kb._was_down[key];
        return result;
    }

    bool Keyboard::key(Key key)
    {
        auto& kb = INPUT_STATE.keyboard;

        auto result = kb._is_down[key];
        return result;
    }
} // namespace inx