#ifndef SCARECROW_GRAPHICS_H
#define SCARECROW_GRAPHICS_H

#include <stdbool.h>

typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

SDL_Texture* scarecrow_graphics_get_tileset_texture(void);

bool scarecrow_graphics_try_load(SDL_Renderer* renderer);
void scarecrow_graphics_unload(void);

#endif // SCARECROW_GRAPHICS_H