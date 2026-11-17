#include "scarecrow/graphics.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static SDL_Texture* scarecrow_graphics_tileset_texture = NULL;

SDL_Texture* scarecrow_graphics_get_tileset_texture(void)
{
    return scarecrow_graphics_tileset_texture;
}

bool scarecrow_graphics_try_load(SDL_Renderer* renderer)
{
    scarecrow_graphics_tileset_texture = IMG_LoadTexture(renderer, "res/textures/tileset.png");

    if (!scarecrow_graphics_tileset_texture)
    {
        SDL_Log("Failed to load tileset texture! SDL_image error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetTextureScaleMode(scarecrow_graphics_tileset_texture, SDL_SCALEMODE_NEAREST);

    return true;
}

void scarecrow_graphics_unload(void)
{
    if (scarecrow_graphics_tileset_texture)
    {
        SDL_DestroyTexture(scarecrow_graphics_tileset_texture);
        scarecrow_graphics_tileset_texture = NULL;
    }
}