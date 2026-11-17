#ifndef SCARECROW_PLAYER_H
#define SCARECROW_PLAYER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct SDL_Renderer SDL_Renderer;

float scarecrow_player_get_velocity_x(void);
float scarecrow_player_get_velocity_y(void);
int8_t scarecrow_player_get_x(void);
int8_t scarecrow_player_get_y(void);
uint8_t scarecrow_player_get_width(void);
uint8_t scarecrow_player_get_height(void);
bool scarecrow_player_get_is_climbing(void);
bool scarecrow_player_get_is_dead(void);
bool scarecrow_player_get_is_grounded(void);

void scarecrow_player_set_velocity_x(const float value);
void scarecrow_player_set_velocity_y(const float value);
void scarecrow_player_set_x(const int8_t value);
void scarecrow_player_set_y(const int8_t value);
void scarecrow_player_set_width(const uint8_t value);
void scarecrow_player_set_height(const uint8_t value);
void scarecrow_player_set_remainder_x(const float value);
void scarecrow_player_set_remainder_y(const float value);
void scarecrow_player_set_is_climbing(const bool value);
void scarecrow_player_set_is_dead(const bool value);
void scarecrow_player_set_is_grounded(const bool value);

void scarecrow_player_update(const float delta_time);
void scarecrow_player_draw(SDL_Renderer* renderer);

#endif // SCARECROW_PLAYER_H