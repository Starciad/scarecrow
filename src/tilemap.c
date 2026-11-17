#include "scarecrow/tilemap.h"

#include <SDL3/SDL.h>

#include "scarecrow/constants.h"
#include "scarecrow/graphics.h"
#include "scarecrow/mathematics.h"
#include "scarecrow/tilemap.h"

static scarecrow_tilemap_tile_type scarecrow_tilemap[SCARECROW_CONSTANTS_WORLD_CHUNK_SIZE] = {0};

bool scarecrow_tilemap_is_tile_solid(scarecrow_tilemap_tile_type value)
{
    switch (value)
    {
    case SCARECROW_TILEMAP_TILE_TYPE_SOLID:
    case SCARECROW_TILEMAP_TILE_TYPE_DOOR:
        return true;

    default:
        return false;
    }
}

bool scarecrow_tilemap_is_tile_platform(scarecrow_tilemap_tile_type value)
{
    switch (value)
    {
    case SCARECROW_TILEMAP_TILE_TYPE_PLATFORM:
    case SCARECROW_TILEMAP_TILE_TYPE_SCALABLE_PLATFORM:
        return true;

    default:
        return false;
    }
}

bool scarecrow_tilemap_is_tile_climbable(scarecrow_tilemap_tile_type value)
{
    switch (value)
    {
    case SCARECROW_TILEMAP_TILE_TYPE_LADDER:
    case SCARECROW_TILEMAP_TILE_TYPE_SCALABLE_PLATFORM:
        return true;

    default:
        return false;
    }
}

bool scarecrow_tilemap_is_tile_hazard(scarecrow_tilemap_tile_type value)
{
    switch (value)
    {
    case SCARECROW_TILEMAP_TILE_TYPE_THORN:
        return true;

    default:
        return false;
    }
}

uint8_t scarecrow_tilemap_get_tile_index(const uint8_t x, const uint8_t y)
{
    return scarecrow_mathematics_flatten_2d_index(x, y, SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH);
}

scarecrow_tilemap_tile_type scarecrow_tilemap_get_tile_type(const uint8_t x, const uint8_t y)
{
    return scarecrow_tilemap[scarecrow_tilemap_get_tile_index(x, y)];
}

void scarecrow_tilemap_set_tile_type(const uint8_t x, const uint8_t y, const scarecrow_tilemap_tile_type type)
{
    scarecrow_tilemap[scarecrow_tilemap_get_tile_index(x, y)] = type;
}

bool scarecrow_tilemap_is_collide_at_tile(const int8_t x, const int8_t y, const uint8_t width, const uint8_t height, const int8_t motion_y)
{
    const int16_t previous_y = y - motion_y;
    const int16_t next_right = x + width;
    const int16_t next_bottom = y + height;
    const int16_t previous_bottom = previous_y + height;

    // Calculate the start and end tile coordinates based on the given position and size
    int8_t start_x = x / SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;
    int8_t end_x = (next_right - 1) / SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;
    int8_t start_y = y / SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;
    int8_t end_y = (next_bottom - 1) / SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;

    // Check if the start and end coordinates are outside the tilemap bounds
    if (end_x < 0 || end_y < 0 || start_x > SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH - 1 || start_y > SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT - 1)
    {
        return false;
    }

    // Clamp the start and end coordinates to the tilemap bounds
    start_x = start_x < 0 ? 0 : start_x;
    start_y = start_y < 0 ? 0 : start_y;
    end_x = end_x > SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH - 1 ? SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH - 1 : end_x;
    end_y = end_y > SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT - 1 ? SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT - 1 : end_y;

    for (uint8_t tile_y = start_y; tile_y <= end_y; ++tile_y)
    {
        for (uint8_t tile_x = start_x; tile_x <= end_x; ++tile_x)
        {
            const scarecrow_tilemap_tile_type t = scarecrow_tilemap[scarecrow_tilemap_get_tile_index(tile_x, tile_y)];

            if (scarecrow_tilemap_is_tile_solid(t))
            {
                return true;
            }

            if (scarecrow_tilemap_is_tile_platform(t))
            {
                const int8_t tile_left = tile_x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;
                const int8_t tile_top = tile_y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;
                const int8_t tile_right = tile_left + SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH;

                const bool is_horizontal_overlap = next_right > tile_left && x < tile_right;
                const bool is_vertical_overlap = next_bottom > tile_top && y < tile_top + SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT;
                const bool was_above_tile = previous_bottom <= tile_top;

                if (is_horizontal_overlap && is_vertical_overlap && motion_y > 0 && was_above_tile)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void scarecrow_tilemap_clear(void)
{
    for (uint8_t tile_y = 0; tile_y < SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT; ++tile_y)
    {
        for (uint8_t tile_x = 0; tile_x < SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH; ++tile_x)
        {
            scarecrow_tilemap[scarecrow_tilemap_get_tile_index(tile_x, tile_y)] = SCARECROW_TILEMAP_TILE_TYPE_NONE;
        }
    }
}

static void scarecrow_tilemap_draw_solid(SDL_Renderer* renderer, const uint8_t x, const uint8_t y)
{
    const SDL_FRect source_rect =
    {
        0,
        0,
        16,
        16,
    };

    const SDL_FRect destination_rect =
    {
        (float)(x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH),
        (float)(y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT),
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH,
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 56, 128, 4, 255);
    SDL_RenderTexture(renderer, scarecrow_graphics_get_tileset_texture(), &source_rect, &destination_rect);
}

static void scarecrow_tilemap_draw_door(SDL_Renderer* renderer, const uint8_t x, const uint8_t y)
{
    const SDL_FRect tile_rect =
    {
        (float)(x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH),
        (float)(y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT),
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH,
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 100, 255, 150, 255);
    SDL_RenderFillRect(renderer, &tile_rect);
}

static void scarecrow_tilemap_draw_platform(SDL_Renderer* renderer, const uint8_t x, const uint8_t y)
{
    const SDL_FRect tile_rect =
    {
        (float)(x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH),
        (float)(y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT),
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH,
        (float)(SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT / 2.0f)
    };

    SDL_SetRenderDrawColor(renderer, 165, 42, 42, 255);
    SDL_RenderFillRect(renderer, &tile_rect);
}

static void scarecrow_tilemap_draw_ladder(SDL_Renderer* renderer, const uint8_t x, const uint8_t y)
{
    const SDL_FRect tile_rect =
    {
        (float)(x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH),
        (float)(y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT),
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH,
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderFillRect(renderer, &tile_rect);
}

static void scarecrow_tilemap_draw_scalable_platform(SDL_Renderer* renderer, const uint8_t x, const uint8_t y)
{
    const SDL_FRect tile_rect =
    {
        (float)(x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH),
        (float)(y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT),
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH,
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &tile_rect);
}

static void scarecrow_tilemap_draw_thorn(SDL_Renderer* renderer, const uint8_t x, const uint8_t y)
{
    const SDL_FRect tile_rect =
    {
        (float)(x * SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH),
        (float)(y * SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT),
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_WIDTH,
        (float)SCARECROW_CONSTANTS_TILEMAP_TILE_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &tile_rect);
}

void scarecrow_tilemap_draw(SDL_Renderer* renderer)
{
    for (size_t y = 0; y < SCARECROW_CONSTANTS_WORLD_CHUNK_HEIGHT; ++y)
    {
        for (size_t x = 0; x < SCARECROW_CONSTANTS_WORLD_CHUNK_WIDTH; ++x)
        {
            const scarecrow_tilemap_tile_type type = scarecrow_tilemap[scarecrow_tilemap_get_tile_index(x, y)];

            switch (type)
            {
            case SCARECROW_TILEMAP_TILE_TYPE_NONE:
                break;

            case SCARECROW_TILEMAP_TILE_TYPE_SOLID:
                scarecrow_tilemap_draw_solid(renderer, x, y);
                break;

            case SCARECROW_TILEMAP_TILE_TYPE_DOOR:
                scarecrow_tilemap_draw_door(renderer, x, y);
                break;

            case SCARECROW_TILEMAP_TILE_TYPE_PLATFORM:
                scarecrow_tilemap_draw_platform(renderer, x, y);
                break;

            case SCARECROW_TILEMAP_TILE_TYPE_LADDER:
                scarecrow_tilemap_draw_ladder(renderer, x, y);
                break;

            case SCARECROW_TILEMAP_TILE_TYPE_SCALABLE_PLATFORM:
                scarecrow_tilemap_draw_scalable_platform(renderer, x, y);
                break;

            case SCARECROW_TILEMAP_TILE_TYPE_THORN:
                scarecrow_tilemap_draw_thorn(renderer, x, y);
                break;

            default:
                break;
            }
        }
    }
}
