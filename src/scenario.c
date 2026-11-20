#include "scarecrow/scenario.h"

#include <SDL3/SDL.h>

#include "scarecrow/constants.h"

void scarecrow_scenario_draw_background(SDL_Renderer* renderer)
{
    const SDL_FRect screen_rect = { 0.0f, 0.0f, (float)SCARECROW_CONSTANTS_GAME_LOGICAL_WIDTH, (float)SCARECROW_CONSTANTS_GAME_LOGICAL_HEIGHT };

    SDL_SetRenderDrawColor(renderer, 41, 173, 255, 255);
    SDL_RenderFillRect(renderer, &screen_rect);
}
