#ifndef SCARECROW_TILEMAP_H
#define SCARECROW_TILEMAP_H

#include <stdbool.h>
#include <stdint.h>

typedef struct SDL_Renderer SDL_Renderer;

typedef enum scarecrow_tilemap_tile_type
{
    SCARECROW_TILEMAP_TILE_TYPE_NONE,
    SCARECROW_TILEMAP_TILE_TYPE_SOLID,
    SCARECROW_TILEMAP_TILE_TYPE_DOOR,
    SCARECROW_TILEMAP_TILE_TYPE_PLATFORM,
    SCARECROW_TILEMAP_TILE_TYPE_LADDER,
    SCARECROW_TILEMAP_TILE_TYPE_SCALABLE_PLATFORM,
} scarecrow_tilemap_tile_type;

bool scarecrow_tilemap_is_tile_solid(scarecrow_tilemap_tile_type value);
bool scarecrow_tilemap_is_tile_platform(scarecrow_tilemap_tile_type value);
bool scarecrow_tilemap_is_tile_climbable(scarecrow_tilemap_tile_type value);

uint8_t scarecrow_tilemap_get_tile_index(const uint8_t x, const uint8_t y);
bool scarecrow_tilemap_is_collide_at_tile(const int8_t x, const int8_t y, const uint8_t width, const uint8_t height, const int8_t motion_y);

scarecrow_tilemap_tile_type scarecrow_tilemap_get_tile_type(const uint8_t x, const uint8_t y);
void scarecrow_tilemap_set_tile_type(const uint8_t x, const uint8_t y, const scarecrow_tilemap_tile_type type);

void scarecrow_tilemap_clear(void);
void scarecrow_tilemap_draw(SDL_Renderer* renderer);

#endif // SCARECROW_TILEMAP_H