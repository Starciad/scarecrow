#ifndef SCARECROW_INPUT_H
#define SCARECROW_INPUT_H

#include <stdbool.h>

bool scarecrow_input_is_up_started(void);
bool scarecrow_input_is_up_pressed(void);
bool scarecrow_input_is_up_canceled(void);

bool scarecrow_input_is_right_started(void);
bool scarecrow_input_is_right_pressed(void);
bool scarecrow_input_is_right_canceled(void);

bool scarecrow_input_is_down_started(void);
bool scarecrow_input_is_down_pressed(void);
bool scarecrow_input_is_down_canceled(void);

bool scarecrow_input_is_left_started(void);
bool scarecrow_input_is_left_pressed(void);
bool scarecrow_input_is_left_canceled(void);

bool scarecrow_input_is_a_started(void);
bool scarecrow_input_is_a_pressed(void);
bool scarecrow_input_is_a_canceled(void);

bool scarecrow_input_is_b_started(void);
bool scarecrow_input_is_b_pressed(void);
bool scarecrow_input_is_b_canceled(void);

bool scarecrow_input_is_start_started(void);
bool scarecrow_input_is_start_pressed(void);
bool scarecrow_input_is_start_canceled(void);

bool scarecrow_input_is_select_started(void);
bool scarecrow_input_is_select_pressed(void);
bool scarecrow_input_is_select_canceled(void);

void scarecrow_input_update(void);

#endif // SCARECROW_INPUT_H