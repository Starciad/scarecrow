#include "scarecrow/transition.h"

#include <SDL3/SDL.h>

#include "scarecrow/constants.h"
#include "scarecrow/player.h"
#include "scarecrow/tilemap.h"
#include "scarecrow/world.h"

typedef enum scarecrow_transition_phase
{
    SCARECROW_TRANSITION_PHASE_NONE,
    SCARECROW_TRANSITION_PHASE_FADE_OUT,
    SCARECROW_TRANSITION_PHASE_MOVE,
    SCARECROW_TRANSITION_PHASE_FADE_IN,
} scarecrow_transition_phase;

static float scarecrow_transition_destination_x = 0.0f;
static float scarecrow_transition_destination_y = 0.0f;
static float scarecrow_transition_elapsed_time = 0.0f;
static float scarecrow_transition_render_x = 0.0f;
static float scarecrow_transition_render_y = 0.0f;
static float scarecrow_transition_start_x = 0.0f;
static float scarecrow_transition_start_y = 0.0f;
static scarecrow_transition_phase scarecrow_transition_current_phase = SCARECROW_TRANSITION_PHASE_NONE;

bool scarecrow_transition_is_active(void)
{
    return scarecrow_transition_current_phase != SCARECROW_TRANSITION_PHASE_NONE;
}

float scarecrow_transition_get_render_x(void)
{
    return scarecrow_transition_render_x;
}

float scarecrow_transition_get_render_y(void)
{
    return scarecrow_transition_render_y;
}

void scarecrow_transition_start_chunk(const int8_t direction_x, const int8_t direction_y)
{
    scarecrow_transition_current_phase = SCARECROW_TRANSITION_PHASE_FADE_OUT;

    scarecrow_transition_destination_x = (float)scarecrow_player_get_x();
    scarecrow_transition_destination_y = (float)scarecrow_player_get_y();
    scarecrow_transition_elapsed_time = 0.0f;
    scarecrow_transition_start_x = (float)scarecrow_player_get_x();
    scarecrow_transition_start_y = (float)scarecrow_player_get_y();
    scarecrow_transition_render_x = scarecrow_transition_start_x;
    scarecrow_transition_render_y = scarecrow_transition_start_y;

    if (direction_x < 0)
    {
        scarecrow_world_set_current_chunk_x(scarecrow_world_get_current_chunk_x() - 1);
        scarecrow_transition_destination_x = (float)(SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH - scarecrow_player_get_width());
    }
    else if (direction_x > 0)
    {
        scarecrow_world_set_current_chunk_x(scarecrow_world_get_current_chunk_x() + 1);
        scarecrow_transition_destination_x = 0.0f;
    }

    if (direction_y < 0)
    {
        scarecrow_world_set_current_chunk_y(scarecrow_world_get_current_chunk_y() - 1);
        scarecrow_transition_destination_y = (float)(SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT - scarecrow_player_get_height());
    }
    else if (direction_y > 0)
    {
        scarecrow_world_set_current_chunk_y(scarecrow_world_get_current_chunk_y() + 1);
        scarecrow_transition_destination_y = 0.0f;
    }
}

void scarecrow_transition_update_chunk(void)
{
    if (scarecrow_transition_is_active())
    {
        return;
    }

    int8_t direction_x = 0;
    int8_t direction_y = 0;

    if (scarecrow_player_get_x() < 0 && scarecrow_world_get_current_chunk_x() > 0)
    {
        direction_x = -1;
    }
    else if (scarecrow_player_get_x() + scarecrow_player_get_width() > SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH && scarecrow_world_get_current_chunk_x() < SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH - 1)
    {
        direction_x = 1;
    }

    if (scarecrow_player_get_y() < 0 && scarecrow_world_get_current_chunk_y() > 0)
    {
        direction_y = -1;
    }
    else if (scarecrow_player_get_y() + scarecrow_player_get_height() > SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT && scarecrow_world_get_current_chunk_y() < SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT - 1)
    {
        direction_y = 1;
    }

    if (direction_x != 0 || direction_y != 0)
    {
        scarecrow_transition_start_chunk(direction_x, direction_y);
        return;
    }

    if (scarecrow_player_get_x() < 0)
    {
        scarecrow_player_set_x(0);
    }
    else if (scarecrow_player_get_x() + scarecrow_player_get_width() > SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH)
    {
        scarecrow_player_set_x(SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH - scarecrow_player_get_width());
    }

    if (scarecrow_player_get_y() < 0)
    {
        scarecrow_player_set_y(0);
    }
    else if (scarecrow_player_get_y() + scarecrow_player_get_height() > SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT)
    {
        scarecrow_player_set_y(SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT - scarecrow_player_get_height());
    }
}

void scarecrow_transition_update(const float delta_time)
{
    if (!scarecrow_transition_is_active())
    {
        return;
    }

    scarecrow_transition_elapsed_time += delta_time;

    if (scarecrow_transition_current_phase == SCARECROW_TRANSITION_PHASE_FADE_OUT && scarecrow_transition_elapsed_time >= SCARECROW_CONSTANTS_TRANSITION_FADE_TIME)
    {
        scarecrow_transition_elapsed_time -= SCARECROW_CONSTANTS_TRANSITION_FADE_TIME;
        
        scarecrow_tilemap_clear();
        scarecrow_world_load_chunk();
        scarecrow_player_set_x((int8_t)scarecrow_transition_destination_x);
        scarecrow_player_set_y((int8_t)scarecrow_transition_destination_y);
        scarecrow_player_set_velocity_x(0.0f);
        scarecrow_player_set_velocity_y(0.0f);
        scarecrow_player_set_remainder_x(0.0f);
        scarecrow_player_set_remainder_y(0.0f);
        scarecrow_player_set_is_climbing(false);
        scarecrow_player_set_is_grounded(false);

        scarecrow_transition_current_phase = SCARECROW_TRANSITION_PHASE_MOVE;
    }
    else if (scarecrow_transition_current_phase == SCARECROW_TRANSITION_PHASE_MOVE && scarecrow_transition_elapsed_time >= SCARECROW_CONSTANTS_TRANSITION_MOVE_TIME)
    {
        scarecrow_transition_elapsed_time -= SCARECROW_CONSTANTS_TRANSITION_MOVE_TIME;
        scarecrow_transition_render_x = scarecrow_transition_destination_x;
        scarecrow_transition_render_y = scarecrow_transition_destination_y;
        scarecrow_transition_current_phase = SCARECROW_TRANSITION_PHASE_FADE_IN;
    }
    else if (scarecrow_transition_current_phase == SCARECROW_TRANSITION_PHASE_FADE_IN && scarecrow_transition_elapsed_time >= SCARECROW_CONSTANTS_TRANSITION_FADE_TIME)
    {
        scarecrow_transition_current_phase = SCARECROW_TRANSITION_PHASE_NONE;
        scarecrow_transition_elapsed_time = 0.0f;
        scarecrow_transition_render_x = scarecrow_transition_destination_x;
        scarecrow_transition_render_y = scarecrow_transition_destination_y;
    }

    if (scarecrow_transition_current_phase == SCARECROW_TRANSITION_PHASE_MOVE)
    {
        float progress = scarecrow_transition_elapsed_time / SCARECROW_CONSTANTS_TRANSITION_MOVE_TIME;
        progress = progress * progress * (3.0f - 2.0f * progress);
        scarecrow_transition_render_x = scarecrow_transition_start_x + (scarecrow_transition_destination_x - scarecrow_transition_start_x) * progress;
        scarecrow_transition_render_y = scarecrow_transition_start_y + (scarecrow_transition_destination_y - scarecrow_transition_start_y) * progress;
    }
}

void scarecrow_transition_draw_overlay(SDL_Renderer* renderer)
{
    if (!scarecrow_transition_is_active())
    {
        return;
    }

    float darkness = 1.0f;

    if (scarecrow_transition_current_phase == SCARECROW_TRANSITION_PHASE_FADE_OUT)
    {
        darkness = scarecrow_transition_elapsed_time / SCARECROW_CONSTANTS_TRANSITION_FADE_TIME;
    }
    else if (scarecrow_transition_current_phase == SCARECROW_TRANSITION_PHASE_FADE_IN)
    {
        darkness = 1.0f - scarecrow_transition_elapsed_time / SCARECROW_CONSTANTS_TRANSITION_FADE_TIME;
    }

    darkness = darkness < 0.0f ? 0.0f : darkness > 1.0f ? 1.0f : darkness;
    
    const SDL_FRect screen_rect =
    {
        0.0f,
        0.0f,
        (float)SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH,
        (float)SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT
    };

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, (uint8_t)(darkness * 255.0f));
    SDL_RenderFillRect(renderer, &screen_rect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
