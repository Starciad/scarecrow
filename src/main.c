#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include "scarecrow/constants.h"
#include "scarecrow/graphics.h"
#include "scarecrow/input.h"
#include "scarecrow/player.h"
#include "scarecrow/tilemap.h"
#include "scarecrow/transition.h"
#include "scarecrow/world.h"

static SDL_Window* _sdl_window = NULL;
static SDL_Renderer* _sdl_renderer = NULL;

static bool scarecrow_try_initialize(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(SCARECROW_CONSTANTS_GAME_WINDOW_TITLE, SCARECROW_CONSTANTS_GAME_SCREEN_WIDTH, SCARECROW_CONSTANTS_GAME_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &_sdl_window, &_sdl_renderer))
    {
        SDL_Log("Window and Renderer could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(_sdl_renderer, true))
    {
        SDL_Log("Could not set vsync! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderLogicalPresentation(_sdl_renderer, SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH, SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX))
    {
        SDL_Log("Could not set logical presentation! SDL error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool scarecrow_try_load_content(void)
{
    return true;
}

static void scarecrow_begin(void)
{
    if (!scarecrow_graphics_try_load(_sdl_renderer))
    {
        SDL_Log("Failed to load graphics!");
        return;
    }

    scarecrow_player_set_x(SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH / 2 - 8);
    scarecrow_player_set_y(SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT / 2 - 8);
    scarecrow_player_set_width(SCARECROW_CONSTANTS_PLAYER_WIDTH);
    scarecrow_player_set_height(SCARECROW_CONSTANTS_PLAYER_HEIGHT);

    scarecrow_world_set_current_chunk_x(SCARECROW_CONSTANTS_WORLD_START_X);
    scarecrow_world_set_current_chunk_y(SCARECROW_CONSTANTS_WORLD_START_Y);
    scarecrow_world_load_chunk();
}

static void scarecrow_update(const float delta_time)
{
    scarecrow_input_update();
    
    if (scarecrow_transition_is_active())
    {
        scarecrow_transition_update(delta_time);
    }
    else
    {
        scarecrow_player_update(delta_time);
        scarecrow_transition_update_chunk();
    }
}

static void scarecrow_draw(void)
{
    SDL_SetRenderDrawColor(_sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_sdl_renderer);

    scarecrow_tilemap_draw(_sdl_renderer);
    scarecrow_transition_draw_overlay(_sdl_renderer);
    scarecrow_player_draw(_sdl_renderer);

    SDL_RenderPresent(_sdl_renderer);
}

static void scarecrow_game_loop(void)
{
    bool is_running = true;

    SDL_Event event;
    SDL_zero(event);

    uint64_t last_frame_time_ns = SDL_GetTicksNS();

    while (is_running)
    {
        const uint64_t frame_start_ns = SDL_GetTicksNS();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                is_running = false;
            }
        }

        const uint64_t current_frame_time_ns = SDL_GetTicksNS();
        const float delta_time = (float)(current_frame_time_ns - last_frame_time_ns) / (float)SDL_NS_PER_SECOND;
        last_frame_time_ns = current_frame_time_ns;

        scarecrow_update(delta_time);
        scarecrow_draw();

        const uint64_t frame_time_ns = SDL_GetTicksNS() - frame_start_ns;
        if (frame_time_ns < SCARECROW_CONSTANTS_GAME_TARGET_FRAME_TIME_NS)
        {
            SDL_DelayNS(SCARECROW_CONSTANTS_GAME_TARGET_FRAME_TIME_NS - frame_time_ns);
        }
    }
}

static void scarecrow_close(void)
{
    scarecrow_graphics_unload();

    SDL_DestroyRenderer(_sdl_renderer);
    _sdl_renderer = NULL;

    SDL_DestroyWindow(_sdl_window);
    _sdl_window = NULL;

    SDL_Quit();
}

int main(void)
{
    if (!scarecrow_try_initialize())
    {
        SDL_Log("Failed to initialize!");
        return 1;
    }

    if (!scarecrow_try_load_content())
    {
        SDL_Log("Failed to load content!");
        return 1;
    }

    scarecrow_begin();
    scarecrow_game_loop();
    scarecrow_close();

    return 0;
}
