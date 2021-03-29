#include <rlu/rlu.h>

typedef struct key_lookup_tag {
	bool (* const key_func)(int player, rlu_key key, rlu_key_press press);
} key_lookup;

typedef struct axis_lookup_tag {
	float (* const axis_func)(int player, rlu_axis key);
} axis_lookup;

// keys
bool xbox_dpad_up(int player, rlu_key_press press);
bool xbox_dpad_right(int player, rlu_key_press press);
bool xbox_dpad_down(int player, rlu_key_press press);
bool xbox_dpad_left(int player, rlu_key_press press);
bool xbox_button_up(int player, rlu_key_press press);
bool xbox_button_right(int player, rlu_key_press press);
bool xbox_button_down(int player, rlu_key_press press);
bool xbox_button_left(int player, rlu_key_press press);
bool xbox_bumper_right(int player, rlu_key_press press);
bool xbox_bumper_left(int player, rlu_key_press press);
bool xbox_button_select(int player, rlu_key_press press);
bool xbox_button_start(int player, rlu_key_press press);
bool ps3_dpad_up(int player, rlu_key_press press);
bool ps3_dpad_right(int player, rlu_key_press press);
bool ps3_dpad_down(int player, rlu_key_press press);
bool ps3_dpad_left(int player, rlu_key_press press);
bool ps3_button_up(int player, rlu_key_press press);
bool ps3_button_right(int player, rlu_key_press press);
bool ps3_button_down(int player, rlu_key_press press);
bool ps3_button_left(int player, rlu_key_press press);
bool ps3_bumper_right(int player, rlu_key_press press);
bool ps3_bumper_left(int player, rlu_key_press press);
bool ps3_button_select(int player, rlu_key_press press);
bool ps3_button_start(int player, rlu_key_press press);
bool keyboard_dpad_up(int player, rlu_key_press press);
bool keyboard_dpad_right(int player, rlu_key_press press);
bool keyboard_dpad_down(int player, rlu_key_press press);
bool keyboard_dpad_left(int player, rlu_key_press press);
bool keyboard_button_up(int player, rlu_key_press press);
bool keyboard_button_right(int player, rlu_key_press press);
bool keyboard_button_down(int player, rlu_key_press press);
bool keyboard_button_left(int player, rlu_key_press press);
bool keyboard_bumper_right(int player, rlu_key_press press);
bool keyboard_bumper_left(int player, rlu_key_press press);
bool keyboard_button_select(int player, rlu_key_press press);
bool keyboard_button_start(int player, rlu_key key, rlu_key_press press);

// axis
float xbox_key_stick_left_x(int player, rlu_axis key);
float xbox_key_stick_left_y(int player, rlu_axis key);
float xbox_key_stick_right_x(int player, rlu_axis key);
float xbox_key_stick_right_y(int player, rlu_axis key);
float xbox_key_stick_trigger_right(int player, rlu_axis key);
float xbox_key_stick_trigger_left(int player, rlu_axis key);
float ps3_key_stick_left_x(int player, rlu_axis key);
float ps3_key_stick_left_y(int player, rlu_axis key);
float ps3_key_stick_right_x(int player, rlu_axis key);
float ps3_key_stick_right_y(int player, rlu_axis key);
float ps3_key_stick_trigger_right(int player, rlu_axis key);
float ps3_key_stick_trigger_left(int player, rlu_axis key);
float keyboard_key_stick_left_x(int player, rlu_axis key);
float keyboard_key_stick_left_y(int player, rlu_axis key);
float keyboard_key_stick_right_x(int player, rlu_axis key);
float keyboard_key_stick_right_y(int player, rlu_axis key);
float keyboard_key_stick_trigger_right(int player, rlu_axis key);
float keyboard_key_stick_trigger_left(int player, rlu_axis key);

const static xbox_key_lookup[] = {
	[RLU_KEY_DPAD_UP] = xbox_dpad_up,
	[RLU_KEY_DPAD_RIGHT] = xbox_dpad_right,
	[RLU_KEY_DPAD_DOWN] = xbox_dpad_down,
	[RLU_KEY_DPAD_LEFT] = xbox_dpad_left,
	[RLU_KEY_BUTTON_UP] = xbox_button_up,
	[RLU_KEY_BUTTON_RIGHT] = xbox_button_right,
	[RLU_KEY_BUTTON_DOWN] = xbox_button_down,
	[RLU_KEY_BUTTON_LEFT] = xbox_button_left,
	[RLU_KEY_BUMPER_RIGHT] = xbox_bumper_right,
	[RLU_KEY_BUMPER_LEFT] = xbox_bumper_left,
	[RLU_KEY_BUTTON_SELECT] = xbox_button_select,
	[RLU_KEY_BUTTON_START] = xbox_button_start,
};

const static xbox_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = xbox_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = xbox_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = xbox_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = xbox_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = xbox_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = xbox_key_stick_trigger_left,
};

const static ps3_key_lookup[] = {
	[RLU_KEY_DPAD_UP] = ps3_dpad_up,
	[RLU_KEY_DPAD_RIGHT] = ps3_dpad_right,
	[RLU_KEY_DPAD_DOWN] = ps3_dpad_down,
	[RLU_KEY_DPAD_LEFT] = ps3_dpad_left,
	[RLU_KEY_BUTTON_UP] = ps3_button_up,
	[RLU_KEY_BUTTON_RIGHT] = ps3_button_right,
	[RLU_KEY_BUTTON_DOWN] = ps3_button_down,
	[RLU_KEY_BUTTON_LEFT] = ps3_button_left,
	[RLU_KEY_BUMPER_RIGHT] = ps3_bumper_right,
	[RLU_KEY_BUMPER_LEFT] = ps3_bumper_left,
	[RLU_KEY_BUTTON_SELECT] = ps3_button_select,
	[RLU_KEY_BUTTON_START] = ps3_button_start,
};

const static ps3_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = ps3_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = ps3_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = ps3_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = ps3_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = ps3_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = ps3_key_stick_trigger_left,
};

const static keyboard_key_lookup[] = {
	[RLU_KEY_DPAD_UP] = keyboard_dpad_up,
	[RLU_KEY_DPAD_RIGHT] = keyboard_dpad_right,
	[RLU_KEY_DPAD_DOWN] = keyboard_dpad_down,
	[RLU_KEY_DPAD_LEFT] = keyboard_dpad_left,
	[RLU_KEY_BUTTON_UP] = keyboard_button_up,
	[RLU_KEY_BUTTON_RIGHT] = keyboard_button_right,
	[RLU_KEY_BUTTON_DOWN] = keyboard_button_down,
	[RLU_KEY_BUTTON_LEFT] = keyboard_button_left,
	[RLU_KEY_BUMPER_RIGHT] = keyboard_bumper_right,
	[RLU_KEY_BUMPER_LEFT] = keyboard_bumper_left,
	[RLU_KEY_BUTTON_SELECT] = keyboard_button_select,
	[RLU_KEY_BUTTON_START] = keyboard_button_start,
};

const static keyboard_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = keyboard_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = keyboard_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = keyboard_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = keyboard_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = keyboard_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = keyboard_key_stick_trigger_left,
};

typedef bool (* const keyboard_func)(int key);
const static keyboard_func keyboard_press_lookup[] = {
	[RLU_PRESS_PRESSED] = IsKeyPressed,
	[RLU_PRESS_DOWN] = IsKeyDown,
	[RLU_PRESS_RELEASED] = IsKeyReleased,
	[RLU_PRESS_UP] = IsKeyUp,
};

bool rlu_input_key(int player, rlu_key key, rlu_key_press press)
{

}

float rlu_input_axis(int player, rlu_axis axis)
{

}


bool xbox_dpad_up(int player, rlu_key_press press)
{

}

bool xbox_dpad_right(int player, rlu_key_press press)
{

}

bool xbox_dpad_down(int player, rlu_key_press press)
{

}

bool xbox_dpad_left(int player, rlu_key_press press)
{

}

bool xbox_button_up(int player, rlu_key_press press)
{

}

bool xbox_button_right(int player, rlu_key_press press)
{

}

bool xbox_button_down(int player, rlu_key_press press)
{

}

bool xbox_button_left(int player, rlu_key_press press)
{

}

bool xbox_bumper_right(int player, rlu_key_press press)
{

}

bool xbox_bumper_left(int player, rlu_key_press press)
{

}

bool xbox_button_select(int player, rlu_key_press press)
{

}

bool xbox_button_start(int player, rlu_key_press press)
{

}

bool ps3_dpad_up(int player, rlu_key_press press)
{

}

bool ps3_dpad_right(int player, rlu_key_press press)
{

}

bool ps3_dpad_down(int player, rlu_key_press press)
{

}

bool ps3_dpad_left(int player, rlu_key_press press)
{

}

bool ps3_button_up(int player, rlu_key_press press)
{

}

bool ps3_button_right(int player, rlu_key_press press)
{

}

bool ps3_button_down(int player, rlu_key_press press)
{

}

bool ps3_button_left(int player, rlu_key_press press)
{

}

bool ps3_bumper_right(int player, rlu_key_press press)
{

}

bool ps3_bumper_left(int player, rlu_key_press press)
{

}

bool ps3_button_select(int player, rlu_key_press press)
{

}

bool ps3_button_start(int player, rlu_key_press press)
{

}

bool keyboard_dpad_up(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_UP);
}

bool keyboard_dpad_right(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_RIGHT);
}

bool keyboard_dpad_down(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_DOWN);
}

bool keyboard_dpad_left(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_LEFT);
}

bool keyboard_button_up(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_Q);
}

bool keyboard_button_right(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_SHIFT);
}

bool keyboard_button_down(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_SPACE);
}

bool keyboard_button_left(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_E);
}

bool keyboard_bumper_right(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_E);
}

bool keyboard_bumper_left(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_Q);
}

bool keyboard_button_select(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_ENTER);
}

bool keyboard_button_start(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_ENTER);
}

float xbox_key_stick_left_x(int player, rlu_axis key)
{

}

float xbox_key_stick_left_y(int player, rlu_axis key)
{

}

float xbox_key_stick_right_x(int player, rlu_axis key)
{

}

float xbox_key_stick_right_y(int player, rlu_axis key)
{

}

float xbox_key_stick_trigger_right(int player, rlu_axis key)
{

}

float xbox_key_stick_trigger_left(int player, rlu_axis key)
{

}

float ps3_key_stick_left_x(int player, rlu_axis key)
{

}

float ps3_key_stick_left_y(int player, rlu_axis key)
{

}

float ps3_key_stick_right_x(int player, rlu_axis key)
{

}

float ps3_key_stick_right_y(int player, rlu_axis key)
{

}

float ps3_key_stick_trigger_right(int player, rlu_axis key)
{

}

float ps3_key_stick_trigger_left(int player, rlu_axis key)
{

}

float keyboard_key_stick_left_x(int player, rlu_axis key)
{

}

float keyboard_key_stick_left_y(int player, rlu_axis key)
{

}

float keyboard_key_stick_right_x(int player, rlu_axis key)
{

}

float keyboard_key_stick_right_y(int player, rlu_axis key)
{

}

float keyboard_key_stick_trigger_right(int player, rlu_axis key)
{

}

float keyboard_key_stick_trigger_left(int player, rlu_axis key)
{

}
