#include "scarecrow/input.h"

#include <SDL3/SDL.h>

#include "scarecrow/constants.h"

static bool scarecrow_input_is_up_started_value;
static bool scarecrow_input_is_up_pressed_value;
static bool scarecrow_input_is_up_canceled_value;

static bool scarecrow_input_is_right_started_value;
static bool scarecrow_input_is_right_pressed_value;
static bool scarecrow_input_is_right_canceled_value;

static bool scarecrow_input_is_down_started_value;
static bool scarecrow_input_is_down_pressed_value;
static bool scarecrow_input_is_down_canceled_value;

static bool scarecrow_input_is_left_started_value;
static bool scarecrow_input_is_left_pressed_value;
static bool scarecrow_input_is_left_canceled_value;

static bool scarecrow_input_is_a_started_value;
static bool scarecrow_input_is_a_pressed_value;
static bool scarecrow_input_is_a_canceled_value;

static bool scarecrow_input_is_b_started_value;
static bool scarecrow_input_is_b_pressed_value;
static bool scarecrow_input_is_b_canceled_value;

static bool scarecrow_input_is_start_started_value;
static bool scarecrow_input_is_start_pressed_value;
static bool scarecrow_input_is_start_canceled_value;

static bool scarecrow_input_is_select_started_value;
static bool scarecrow_input_is_select_pressed_value;
static bool scarecrow_input_is_select_canceled_value;

bool scarecrow_input_is_up_started(void)
{
    return scarecrow_input_is_up_started_value;
}

bool scarecrow_input_is_up_pressed(void)
{
    return scarecrow_input_is_up_pressed_value;
}

bool scarecrow_input_is_up_canceled(void)
{
    return scarecrow_input_is_up_canceled_value;
}

bool scarecrow_input_is_right_started(void)
{
    return scarecrow_input_is_right_started_value;
}

bool scarecrow_input_is_right_pressed(void)
{
    return scarecrow_input_is_right_pressed_value;
}

bool scarecrow_input_is_right_canceled(void)
{
    return scarecrow_input_is_right_canceled_value;
}

bool scarecrow_input_is_down_started(void)
{
    return scarecrow_input_is_down_started_value;
}

bool scarecrow_input_is_down_pressed(void)
{
    return scarecrow_input_is_down_pressed_value;
}

bool scarecrow_input_is_down_canceled(void)
{
    return scarecrow_input_is_down_canceled_value;
}

bool scarecrow_input_is_left_started(void)
{
    return scarecrow_input_is_left_started_value;
}

bool scarecrow_input_is_left_pressed(void)
{
    return scarecrow_input_is_left_pressed_value;
}

bool scarecrow_input_is_left_canceled(void)
{
    return scarecrow_input_is_left_canceled_value;
}

bool scarecrow_input_is_a_started(void)
{
    return scarecrow_input_is_a_started_value;
}

bool scarecrow_input_is_a_pressed(void)
{
    return scarecrow_input_is_a_pressed_value;
}

bool scarecrow_input_is_a_canceled(void)
{
    return scarecrow_input_is_a_canceled_value;
}

bool scarecrow_input_is_b_started(void)
{
    return scarecrow_input_is_b_started_value;
}

bool scarecrow_input_is_b_pressed(void)
{
    return scarecrow_input_is_b_pressed_value;
}

bool scarecrow_input_is_b_canceled(void)
{
    return scarecrow_input_is_b_canceled_value;
}

bool scarecrow_input_is_start_started(void)
{
    return scarecrow_input_is_start_started_value;
}

bool scarecrow_input_is_start_pressed(void)
{
    return scarecrow_input_is_start_pressed_value;
}

bool scarecrow_input_is_start_canceled(void)
{
    return scarecrow_input_is_start_canceled_value;
}

bool scarecrow_input_is_select_started(void)
{
    return scarecrow_input_is_select_started_value;
}

bool scarecrow_input_is_select_pressed(void)
{
    return scarecrow_input_is_select_pressed_value;
}

bool scarecrow_input_is_select_canceled(void)
{
    return scarecrow_input_is_select_canceled_value;
}

static void scarecrow_input_update_button(bool *is_started, bool *is_pressed, bool *is_canceled, const bool is_pressed_state)
{
    if (is_pressed_state)
    {
        if (!*is_pressed)
        {
            *is_started = true;
            *is_pressed = true;
            *is_canceled = false;
        }
        else
        {
            *is_started = false;
            *is_pressed = true;
            *is_canceled = false;
        }
    }
    else
    {
        if (*is_pressed)
        {
            *is_started = false;
            *is_pressed = false;
            *is_canceled = true;
        }
        else
        {
            *is_started = false;
            *is_pressed = false;
            *is_canceled = false;
        }
    }
}

void scarecrow_input_update(void)
{
    const bool *ks = SDL_GetKeyboardState(NULL);

    scarecrow_input_update_button(&scarecrow_input_is_up_started_value, &scarecrow_input_is_up_pressed_value, &scarecrow_input_is_up_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_UP]);
    scarecrow_input_update_button(&scarecrow_input_is_right_started_value, &scarecrow_input_is_right_pressed_value, &scarecrow_input_is_right_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_RIGHT]);
    scarecrow_input_update_button(&scarecrow_input_is_down_started_value, &scarecrow_input_is_down_pressed_value, &scarecrow_input_is_down_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_DOWN]);
    scarecrow_input_update_button(&scarecrow_input_is_left_started_value, &scarecrow_input_is_left_pressed_value, &scarecrow_input_is_left_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_LEFT]);
    scarecrow_input_update_button(&scarecrow_input_is_a_started_value, &scarecrow_input_is_a_pressed_value, &scarecrow_input_is_a_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_A]);
    scarecrow_input_update_button(&scarecrow_input_is_b_started_value, &scarecrow_input_is_b_pressed_value, &scarecrow_input_is_b_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_B]);
    scarecrow_input_update_button(&scarecrow_input_is_start_started_value, &scarecrow_input_is_start_pressed_value, &scarecrow_input_is_start_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_START]);
    scarecrow_input_update_button(&scarecrow_input_is_select_started_value, &scarecrow_input_is_select_pressed_value, &scarecrow_input_is_select_canceled_value, ks[SCARECROW_CONSTANTS_INPUT_KEY_SELECT]);
}
