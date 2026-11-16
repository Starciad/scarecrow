#include "scarecrow/mathematics.h"

uint8_t scarecrow_mathematics_flatten_2d_index(const uint8_t x, const uint8_t y, const uint8_t width)
{
    return (y * width) + x;
}
