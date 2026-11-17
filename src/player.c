#include "scarecrow/player.h"

#include <SDL3/SDL.h>

#include "scarecrow/constants.h"
#include "scarecrow/input.h"
#include "scarecrow/tilemap.h"
#include "scarecrow/transition.h"

static float scarecrow_player_remainder_x = 0.0f;
static float scarecrow_player_remainder_y = 0.0f;
static float scarecrow_player_velocity_x = 0.0f;
static float scarecrow_player_velocity_y = 0.0f;

static int8_t scarecrow_player_x = 0;
static int8_t scarecrow_player_y = 0;
static uint8_t scarecrow_player_height = 0;
static uint8_t scarecrow_player_width = 0;

static bool scarecrow_player_is_climbing = false;
static bool scarecrow_player_is_dead = false;
static bool scarecrow_player_is_grounded = false;

float scarecrow_player_get_remainder_x(void)
{
    return scarecrow_player_remainder_x;
}

float scarecrow_player_get_remainder_y(void)
{
    return scarecrow_player_remainder_y;
}

float scarecrow_player_get_velocity_x(void)
{
    return scarecrow_player_velocity_x;
}

float scarecrow_player_get_velocity_y(void)
{
    return scarecrow_player_velocity_y;
}

int8_t scarecrow_player_get_x(void)
{
    return scarecrow_player_x;
}

int8_t scarecrow_player_get_y(void)
{
    return scarecrow_player_y;
}

uint8_t scarecrow_player_get_width(void)
{
    return scarecrow_player_width;
}

uint8_t scarecrow_player_get_height(void)
{
    return scarecrow_player_height;
}

bool scarecrow_player_get_is_climbing(void)
{
    return scarecrow_player_is_climbing;
}

bool scarecrow_player_get_is_dead(void)
{
    return scarecrow_player_is_dead;
}

bool scarecrow_player_get_is_grounded(void)
{
    return scarecrow_player_is_grounded;
}

void scarecrow_player_set_remainder_x(const float value)
{
    scarecrow_player_remainder_x = value;
}

void scarecrow_player_set_remainder_y(const float value)
{
    scarecrow_player_remainder_y = value;
}

void scarecrow_player_set_velocity_x(const float value)
{
    scarecrow_player_velocity_x = value;
}

void scarecrow_player_set_velocity_y(const float value)
{
    scarecrow_player_velocity_y = value;
}

void scarecrow_player_set_x(const int8_t value)
{
    scarecrow_player_x = value;
}

void scarecrow_player_set_y(const int8_t value)
{
    scarecrow_player_y = value;
}

void scarecrow_player_set_width(const uint8_t value)
{
    scarecrow_player_width = value;
}

void scarecrow_player_set_height(const uint8_t value)
{
    scarecrow_player_height = value;
}

void scarecrow_player_set_is_climbing(const bool value)
{
    scarecrow_player_is_climbing = value;
}

void scarecrow_player_set_is_dead(const bool value)
{
    scarecrow_player_is_dead = value;
}

void scarecrow_player_set_is_grounded(const bool value)
{
    scarecrow_player_is_grounded = value;
}

static bool scarecrow_player_find_climbable_tile(uint8_t* target_tile_x, uint8_t* target_tile_y)
{
    const int8_t right = scarecrow_player_x + scarecrow_player_width - 1;
    const int8_t bottom = scarecrow_player_y + scarecrow_player_height - 1;

    int8_t start_x = scarecrow_player_x / SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;
    int8_t end_x = right / SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;
    int8_t start_y = scarecrow_player_y / SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;
    int8_t end_y = bottom / SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;

    if (end_x < 0 || end_y < 0 || start_x >= SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH || start_y >= SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT)
    {
        return false;
    }

    start_x = start_x < 0 ? 0 : start_x;
    start_y = start_y < 0 ? 0 : start_y;
    end_x = end_x >= SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH ? SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH - 1 : end_x;
    end_y = end_y >= SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT ? SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT - 1 : end_y;

    for (uint8_t tile_y = start_y; tile_y <= end_y; ++tile_y)
    {
        for (uint8_t tile_x = start_x; tile_x <= end_x; ++tile_x)
        {
            if (scarecrow_tilemap_is_tile_climbable(scarecrow_tilemap_get_tile_type(tile_x, tile_y)))
            {
                *target_tile_x = tile_x;
                *target_tile_y = tile_y;
                return true;
            }
        }
    }

    return false;
}

static bool scarecrow_player_is_on_climbable_tile(void)
{
    uint8_t tile_x;
    uint8_t tile_y;

    return scarecrow_player_find_climbable_tile(&tile_x, &tile_y);
}

static void scarecrow_player_set_position_to_climbable_tile(void)
{
    uint8_t tile_x;
    uint8_t tile_y;

    if (scarecrow_player_find_climbable_tile(&tile_x, &tile_y))
    {
        scarecrow_player_x = tile_x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;
        scarecrow_player_y = tile_y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;
    }
}

static void scarecrow_player_update_grounded(void)
{
    scarecrow_player_is_grounded = scarecrow_tilemap_is_collide_at_tile(scarecrow_player_x, scarecrow_player_y + 1, scarecrow_player_width, scarecrow_player_height, 1);
}

static void scarecrow_player_update_horizontal(const float delta_time)
{
    if (scarecrow_player_is_climbing)
    {
        scarecrow_player_velocity_x = 0.0f;
    }
    else
    {
        int8_t horizontal_input = scarecrow_input_is_left_pressed() ? -1 : scarecrow_input_is_right_pressed() ? 1 : 0;
        float target_velocity_x = horizontal_input * SCARECROW_CONSTANTS_PLAYER_WALK_SPEED;

        // Horizontal movement
        if (horizontal_input != 0)
        {
            // Acceleration
            if (scarecrow_player_velocity_x < target_velocity_x)
            {
                scarecrow_player_velocity_x += SCARECROW_CONSTANTS_PLAYER_ACCELERATION * delta_time;

                if (scarecrow_player_velocity_x > target_velocity_x)
                {
                    scarecrow_player_velocity_x = target_velocity_x;
                }
            }
            else if (scarecrow_player_velocity_x > target_velocity_x)
            {
                scarecrow_player_velocity_x -= SCARECROW_CONSTANTS_PLAYER_ACCELERATION * delta_time;

                if (scarecrow_player_velocity_x < target_velocity_x)
                {
                    scarecrow_player_velocity_x = target_velocity_x;
                }
            }
        }
        else
        {
            // Deceleration
            if (scarecrow_player_velocity_x > 0.0f)
            {
                scarecrow_player_velocity_x -= SCARECROW_CONSTANTS_PLAYER_DECELERATION * delta_time;

                if (scarecrow_player_velocity_x < 0.0f)
                {
                    scarecrow_player_velocity_x = 0.0f;
                }
            }
            else if (scarecrow_player_velocity_x < 0.0f)
            {
                scarecrow_player_velocity_x += SCARECROW_CONSTANTS_PLAYER_DECELERATION * delta_time;

                if (scarecrow_player_velocity_x > 0.0f)
                {
                    scarecrow_player_velocity_x = 0.0f;
                }
            }
        }
    }
}

static void scarecrow_player_update_vertical(void)
{
    if (scarecrow_player_is_climbing)
    {
        // Climbing movement
        if (scarecrow_input_is_up_pressed())
        {
            scarecrow_player_velocity_y = -SCARECROW_CONSTANTS_PLAYER_CLIMB_SPEED;
        }
        else if (scarecrow_input_is_down_pressed())
        {
            scarecrow_player_velocity_y = SCARECROW_CONSTANTS_PLAYER_CLIMB_SPEED;
        }
        else
        {
            scarecrow_player_velocity_y = 0.0f;
        }

        // Check if the player is still on a climbable tile
        if (!scarecrow_player_is_on_climbable_tile())
        {
            scarecrow_player_is_climbing = false;
        }
    }
    else if (scarecrow_player_is_on_climbable_tile() && (scarecrow_input_is_up_pressed() || scarecrow_input_is_down_pressed()))
    {
        // Start climbing on a climbable tile
        scarecrow_player_set_position_to_climbable_tile();
        scarecrow_player_is_climbing = true;
        scarecrow_player_velocity_y = 0.0f;
    }
}

static void scarecrow_player_update_jump(void)
{
    // Jumping
    if (scarecrow_input_is_a_started() && (scarecrow_player_is_climbing || scarecrow_player_is_grounded))
    {
        scarecrow_player_velocity_y = -SCARECROW_CONSTANTS_PLAYER_JUMP_SPEED;
        scarecrow_player_is_grounded = false;
        scarecrow_player_is_climbing = false;
    }

    // Jumping (cutoff)
    if (scarecrow_input_is_a_canceled() && scarecrow_player_velocity_y < 0.0f)
    {
        scarecrow_player_velocity_y *= SCARECROW_CONSTANTS_PLAYER_JUMP_CUTOFF_SPEED;
    }
}

static void scarecrow_player_update_gravity(const float delta_time)
{
    if (!scarecrow_player_is_climbing && !scarecrow_player_is_grounded)
    {
        scarecrow_player_velocity_y += SCARECROW_CONSTANTS_PLAYER_GRAVITY * delta_time;

        if (scarecrow_player_velocity_y > SCARECROW_CONSTANTS_PLAYER_MAX_FALL_SPEED)
        {
            scarecrow_player_velocity_y = SCARECROW_CONSTANTS_PLAYER_MAX_FALL_SPEED;
        }
    }
}

static void scarecrow_player_move_horizontal(const float delta_time)
{
    float amount_x = scarecrow_player_velocity_x * delta_time;
    scarecrow_player_remainder_x += amount_x;

    int8_t motion_x = (int8_t)scarecrow_player_remainder_x;
    if (motion_x == 0)
    {
        return;
    }

    scarecrow_player_remainder_x -= motion_x;
    int8_t sign = motion_x > 0 ? 1 : -1;
    while (motion_x != 0)
    {
        int8_t next_x = scarecrow_player_x + sign;
        if (scarecrow_tilemap_is_collide_at_tile(next_x, scarecrow_player_y, scarecrow_player_width, scarecrow_player_height, 0))
        {
            scarecrow_player_velocity_x = 0.0f;
            break;
        }

        scarecrow_player_x += sign;
        motion_x -= sign;
    }
}

static void scarecrow_player_move_vertical(const float delta_time)
{
    float amount_y = scarecrow_player_velocity_y * delta_time;
    scarecrow_player_remainder_y += amount_y;

    int8_t motion_y = (int8_t)scarecrow_player_remainder_y;
    if (motion_y == 0)
    {
        return;
    }

    scarecrow_player_remainder_y -= motion_y;
    int8_t sign = motion_y > 0 ? 1 : -1;
    while (motion_y != 0)
    {
        int8_t next_y = scarecrow_player_y + sign;
        if (scarecrow_tilemap_is_collide_at_tile(scarecrow_player_x, next_y, scarecrow_player_width, scarecrow_player_height, sign))
        {
            if (scarecrow_player_velocity_y > 0.0f)
            {
                scarecrow_player_is_grounded = true;
            }

            scarecrow_player_velocity_y = 0.0f;
            break;
        }

        scarecrow_player_y += sign;
        motion_y -= sign;
    }
}

void scarecrow_player_update(const float delta_time)
{
    scarecrow_player_update_grounded();

    // INPUT
    if (!scarecrow_player_is_dead)
    {
        scarecrow_player_update_horizontal(delta_time);
        scarecrow_player_update_vertical();
        scarecrow_player_update_jump();
    }

    scarecrow_player_update_gravity(delta_time);

    // MOVEMENT
    scarecrow_player_move_horizontal(delta_time);
    scarecrow_player_move_vertical(delta_time);
}

void scarecrow_player_draw(SDL_Renderer* renderer)
{
    const SDL_FRect player_rect =
    {
        scarecrow_transition_is_active() ? scarecrow_transition_get_render_x() : (float)scarecrow_player_x,
        scarecrow_transition_is_active() ? scarecrow_transition_get_render_y() : (float)scarecrow_player_y,
        (float)scarecrow_player_width,
        (float)scarecrow_player_height
    };

    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);
}
