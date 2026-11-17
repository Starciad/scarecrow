#ifndef SCARECROW_WORLD_H
#define SCARECROW_WORLD_H

#include <stdint.h>

uint8_t scarecrow_world_get_current_chunk_x(void);
uint8_t scarecrow_world_get_current_chunk_y(void);
void scarecrow_world_set_current_chunk_x(const uint8_t value);
void scarecrow_world_set_current_chunk_y(const uint8_t value);
void scarecrow_world_load_chunk(void);

#endif // SCARECROW_WORLD_H