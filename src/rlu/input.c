#include <rlu/rlu.h>

#define PLAYERS_MAX 8

#if defined(PLATFORM_RPI)
	#define XBOX360_NAME_ID            "Microsoft X-Box 360 pad"
	#define PS3_NAME_ID                "PLAYSTATION(R)3 Controller"
#else
	#define XBOX360_NAME_ID            "Xbox 360 Controller"
	#define XBOX360_LEGACY_NAME_ID     "Xbox Controller"
	#define PS3_NAME_ID                "PLAYSTATION(R)3 Controller"
#endif

typedef bool (* const key_cb)(int player, rlu_key_press press);
typedef float (* const axis_cb)(int player);

typedef struct key_lookup_tag {
	key_cb key_func;
} key_lookup;

typedef struct axis_lookup_tag {
	axis_cb axis_func;
} axis_lookup;

// keys
bool gamepad_dpad_up(int player, rlu_key_press press);
bool gamepad_dpad_right(int player, rlu_key_press press);
bool gamepad_dpad_down(int player, rlu_key_press press);
bool gamepad_dpad_left(int player, rlu_key_press press);
bool gamepad_button_up(int player, rlu_key_press press);
bool gamepad_button_right(int player, rlu_key_press press);
bool gamepad_button_down(int player, rlu_key_press press);
bool gamepad_button_left(int player, rlu_key_press press);
bool gamepad_bumper_right(int player, rlu_key_press press);
bool gamepad_bumper_left(int player, rlu_key_press press);
bool gamepad_button_select(int player, rlu_key_press press);
bool gamepad_button_start(int player, rlu_key_press press);
bool genericpad_dpad_up(int player, rlu_key_press press);
bool genericpad_dpad_right(int player, rlu_key_press press);
bool genericpad_dpad_down(int player, rlu_key_press press);
bool genericpad_dpad_left(int player, rlu_key_press press);
bool genericpad_button_up(int player, rlu_key_press press);
bool genericpad_button_right(int player, rlu_key_press press);
bool genericpad_button_down(int player, rlu_key_press press);
bool genericpad_button_left(int player, rlu_key_press press);
bool genericpad_bumper_right(int player, rlu_key_press press);
bool genericpad_bumper_left(int player, rlu_key_press press);
bool genericpad_button_select(int player, rlu_key_press press);
bool genericpad_button_start(int player, rlu_key_press press);
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
bool keyboard_button_start(int player, rlu_key_press press);

// axis
float gamepad_key_stick_left_x(int player);
float gamepad_key_stick_left_y(int player);
float gamepad_key_stick_right_x(int player);
float gamepad_key_stick_right_y(int player);
float gamepad_key_stick_trigger_right(int player);
float gamepad_key_stick_trigger_left(int player);
float genericpad_key_stick_left_x(int player);
float genericpad_key_stick_left_y(int player);
float genericpad_key_stick_right_x(int player);
float genericpad_key_stick_right_y(int player);
float genericpad_key_stick_trigger_right(int player);
float genericpad_key_stick_trigger_left(int player);
float keyboard_key_stick_left_x(int player);
float keyboard_key_stick_left_y(int player);
float keyboard_key_stick_right_x(int player);
float keyboard_key_stick_right_y(int player);
float keyboard_key_stick_trigger_right(int player);
float keyboard_key_stick_trigger_left(int player);

const static key_cb gamepad_key_lookup[] = {
	[RLU_KEY_DPAD_UP] = gamepad_dpad_up,
	[RLU_KEY_DPAD_RIGHT] = gamepad_dpad_right,
	[RLU_KEY_DPAD_DOWN] = gamepad_dpad_down,
	[RLU_KEY_DPAD_LEFT] = gamepad_dpad_left,
	[RLU_KEY_BUTTON_UP] = gamepad_button_up,
	[RLU_KEY_BUTTON_RIGHT] = gamepad_button_right,
	[RLU_KEY_BUTTON_DOWN] = gamepad_button_down,
	[RLU_KEY_BUTTON_LEFT] = gamepad_button_left,
	[RLU_KEY_BUMPER_RIGHT] = gamepad_bumper_right,
	[RLU_KEY_BUMPER_LEFT] = gamepad_bumper_left,
	[RLU_KEY_BUTTON_SELECT] = gamepad_button_select,
	[RLU_KEY_BUTTON_START] = gamepad_button_start,
};

const static axis_cb gamepad_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = gamepad_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = gamepad_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = gamepad_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = gamepad_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = gamepad_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = gamepad_key_stick_trigger_left,
};

const static key_cb genericpad_key_lookup[] = {
	[RLU_KEY_DPAD_UP] = genericpad_dpad_up,
	[RLU_KEY_DPAD_RIGHT] = genericpad_dpad_right,
	[RLU_KEY_DPAD_DOWN] = genericpad_dpad_down,
	[RLU_KEY_DPAD_LEFT] = genericpad_dpad_left,
	[RLU_KEY_BUTTON_UP] = genericpad_button_up,
	[RLU_KEY_BUTTON_RIGHT] = genericpad_button_right,
	[RLU_KEY_BUTTON_DOWN] = genericpad_button_down,
	[RLU_KEY_BUTTON_LEFT] = genericpad_button_left,
	[RLU_KEY_BUMPER_RIGHT] = genericpad_bumper_right,
	[RLU_KEY_BUMPER_LEFT] = genericpad_bumper_left,
	[RLU_KEY_BUTTON_SELECT] = genericpad_button_select,
	[RLU_KEY_BUTTON_START] = genericpad_button_start,
};

const static axis_cb genericpad_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = genericpad_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = genericpad_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = genericpad_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = genericpad_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = genericpad_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = genericpad_key_stick_trigger_left,
};

const static key_cb keyboard_key_lookup[] = {
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

const static axis_cb keyboard_axis_lookup[] = {
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

typedef bool (* const gamepad_func)(int player, int button);
const static gamepad_func gamepad_press_lookup[] = {
	[RLU_PRESS_PRESSED] = IsGamepadButtonPressed,
	[RLU_PRESS_DOWN] = IsGamepadButtonDown,
	[RLU_PRESS_RELEASED] = IsGamepadButtonReleased,
	[RLU_PRESS_UP] = IsGamepadButtonUp,
};

static enum {
	XSTATUS_KEYBOARD = 0x00,
	XSTATUS_GAMEPAD = 0x01,
	XSTATUS_GENERICPAD = 0x02,
} player_statuses[PLAYERS_MAX];

void rlu_input_prescan(int player)
{
	assert(player >= 0);
	assert(player < PLAYERS_MAX);

	if (IsGamepadAvailable(player)) {
		if (IsGamepadName(0, XBOX360_NAME_ID) || IsGamepadName(0, XBOX360_LEGACY_NAME_ID) || IsGamepadName(0, PS3_NAME_ID))
		{
			player_statuses[player] = XSTATUS_GAMEPAD;
		}
		else
		{
			player_statuses[player] = XSTATUS_GENERICPAD;
		}
	}
	else {
		player_statuses[player] = XSTATUS_KEYBOARD;
	}
}

bool rlu_input_key(int player, rlu_key key, rlu_key_press press)
{
	if (player_statuses[player] & (XSTATUS_GAMEPAD | XSTATUS_GENERICPAD)) {
		return (gamepad_key_lookup[key])(player, press);
	}
	return keyboard_key_lookup[key](player, press);
}

float rlu_input_axis(int player, rlu_axis axis)
{
	if (player_statuses[player] & XSTATUS_GAMEPAD) {
		return gamepad_axis_lookup[axis](player);
	}
	else if (player_statuses[player] & XSTATUS_GENERICPAD) {
		return genericpad_axis_lookup[axis](player);
	}
	return keyboard_axis_lookup[axis](player);
}


bool gamepad_dpad_up(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_LEFT_FACE_UP);
}

bool gamepad_dpad_right(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
}

bool gamepad_dpad_down(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
}

bool gamepad_dpad_left(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
}

bool gamepad_button_up(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_RIGHT_FACE_UP);
}

bool gamepad_button_right(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
}

bool gamepad_button_down(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
}

bool gamepad_button_left(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
}

bool gamepad_bumper_right(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
}

bool gamepad_bumper_left(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
}

bool gamepad_button_select(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_MIDDLE_LEFT);
}

bool gamepad_button_start(int player, rlu_key_press press)
{
	return gamepad_func[press](player, GAMEPAD_BUTTON_MIDDLE_RIGHT);
}

bool genericpad_dpad_up(int player, rlu_key_press press)
{

}

bool genericpad_dpad_right(int player, rlu_key_press press)
{

}

bool genericpad_dpad_down(int player, rlu_key_press press)
{

}

bool genericpad_dpad_left(int player, rlu_key_press press)
{

}

bool genericpad_button_up(int player, rlu_key_press press)
{

}

bool genericpad_button_right(int player, rlu_key_press press)
{

}

bool genericpad_button_down(int player, rlu_key_press press)
{

}

bool genericpad_button_left(int player, rlu_key_press press)
{

}

bool genericpad_bumper_right(int player, rlu_key_press press)
{

}

bool genericpad_bumper_left(int player, rlu_key_press press)
{

}

bool genericpad_button_select(int player, rlu_key_press press)
{

}

bool genericpad_button_start(int player, rlu_key_press press)
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
	return keyboard_press_lookup[press](KEY_LEFT_SHIFT);
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
	return keyboard_press_lookup[press](KEY_Z);
}

bool keyboard_bumper_left(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_X);
}

bool keyboard_button_select(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_ENTER);
}

bool keyboard_button_start(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](KEY_ENTER);
}

float gamepad_key_stick_left_x(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_LEFT_X);
}

float gamepad_key_stick_left_y(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_LEFT_Y);
}

float gamepad_key_stick_right_x(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_RIGHT_X);
}

float gamepad_key_stick_right_y(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_RIGHT_Y);
}

float gamepad_key_stick_trigger_right(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_RIGHT_TRIGGER);
}

float gamepad_key_stick_trigger_left(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_LEFT_TRIGGER);
}

float genericpad_key_stick_left_x(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_LEFT_X);
}

float genericpad_key_stick_left_y(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_LEFT_Y);
}

float genericpad_key_stick_right_x(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_RIGHT_X);
}

float genericpad_key_stick_right_y(int player)
{
	return GetGamepadAxisMovement(player, GAMEPAD_AXIS_RIGHT_Y);
}

float genericpad_key_stick_trigger_right(int player)
{
	//return (float)(GetGamepadButtonPressed() == GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
	return (float)IsGamepadButtonDown(player, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
}

float genericpad_key_stick_trigger_left(int player)
{
	//return (float)(GetGamepadButtonPressed() == GAMEPAD_BUTTON_LEFT_TRIGGER_2);
	return (float)IsGamepadButtonDown(player, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
}

float keyboard_key_stick_left_x(int player)
{
	if (IsKeyDown(KEY_A)) {
		return -1.0f;
	}
	else if (IsKeyDown(KEY_D)) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_left_y(int player)
{
	if (IsKeyDown(KEY_W)) {
		return -1.0f;
	}
	else if (IsKeyDown(KEY_S)) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_right_x(int player)
{
	if (IsKeyDown(KEY_LEFT)) {
		return -1.0f;
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_right_y(int player)
{
	if (IsKeyDown(KEY_UP)) {
		return -1.0f;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_trigger_right(int player)
{
	if (IsKeyDown(KEY_SPACE)) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_trigger_left(int player)
{
	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		return 1.0f;
	}
	return 0.0f;
}
