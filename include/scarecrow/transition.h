#ifndef SCARECROW_TRANSITION_H
#define SCARECROW_TRANSITION_H

#include <stdbool.h>
#include <stdint.h>

typedef struct SDL_Renderer SDL_Renderer;

bool scarecrow_transition_is_active(void);

float scarecrow_transition_get_render_x(void);
float scarecrow_transition_get_render_y(void);

void scarecrow_transition_start_chunk(const int8_t direction_x, const int8_t direction_y);
void scarecrow_transition_update_chunk(void);
void scarecrow_transition_update(const float delta_time);

void scarecrow_transition_draw_overlay(SDL_Renderer* renderer);

#endif // SCARECROW_TRANSITION_H