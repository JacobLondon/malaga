#include "rlu.h"

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
bool gamepad_button_thumb_right(int player, rlu_key_press press);
bool gamepad_button_thumb_left(int player, rlu_key_press press);
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
bool keyboard_button_thumb_right(int player, rlu_key_press press);
bool keyboard_button_thumb_left(int player, rlu_key_press press);

// axis
float gamepad_key_stick_left_x(int player);
float gamepad_key_stick_left_y(int player);
float gamepad_key_stick_right_x(int player);
float gamepad_key_stick_right_y(int player);
float gamepad_key_stick_trigger_right(int player);
float gamepad_key_stick_trigger_left(int player);
float keyboard_key_stick_left_x(int player);
float keyboard_key_stick_left_y(int player);
float keyboard_key_stick_right_x(int player);
float keyboard_key_stick_right_y(int player);
float keyboard_key_stick_trigger_right(int player);
float keyboard_key_stick_trigger_left(int player);

static const key_cb gamepad_key_lookup[] = {
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
	[RLU_KEY_BUTTON_THUMB_RIGHT] = gamepad_button_thumb_right,
	[RLU_KEY_BUTTON_THUMB_LEFT] = gamepad_button_thumb_left,
};

static const axis_cb gamepad_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = gamepad_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = gamepad_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = gamepad_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = gamepad_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = gamepad_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = gamepad_key_stick_trigger_left,
};

static int gamepad_key_map[] = {
	[RLU_KEY_DPAD_UP] = GAMEPAD_BUTTON_LEFT_FACE_UP,
	[RLU_KEY_DPAD_RIGHT] = GAMEPAD_BUTTON_LEFT_FACE_RIGHT,
	[RLU_KEY_DPAD_DOWN] = GAMEPAD_BUTTON_LEFT_FACE_DOWN,
	[RLU_KEY_DPAD_LEFT] = GAMEPAD_BUTTON_LEFT_FACE_LEFT,
	[RLU_KEY_BUTTON_UP] = GAMEPAD_BUTTON_RIGHT_FACE_UP,
	[RLU_KEY_BUTTON_RIGHT] = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
	[RLU_KEY_BUTTON_DOWN] = GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
	[RLU_KEY_BUTTON_LEFT] = GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
	[RLU_KEY_BUMPER_RIGHT] = GAMEPAD_BUTTON_RIGHT_TRIGGER_1,
	[RLU_KEY_BUMPER_LEFT] = GAMEPAD_BUTTON_LEFT_TRIGGER_1,
	[RLU_KEY_BUTTON_SELECT] = GAMEPAD_BUTTON_MIDDLE_LEFT,
	[RLU_KEY_BUTTON_START] = GAMEPAD_BUTTON_MIDDLE_RIGHT,
	[RLU_KEY_BUTTON_THUMB_RIGHT] = GAMEPAD_BUTTON_RIGHT_THUMB,
	[RLU_KEY_BUTTON_THUMB_LEFT] = GAMEPAD_BUTTON_LEFT_THUMB,
};

static int gamepad_axis_map[] = {
	[RLU_KEY_STICK_LEFT_X] = GAMEPAD_AXIS_LEFT_X,
	[RLU_KEY_STICK_LEFT_Y] = GAMEPAD_AXIS_LEFT_Y,
	[RLU_KEY_STICK_RIGHT_X] = GAMEPAD_AXIS_RIGHT_X,
	[RLU_KEY_STICK_RIGHT_Y] = GAMEPAD_AXIS_RIGHT_Y,
	[RLU_KEY_TRIGGER_RIGHT] = GAMEPAD_AXIS_RIGHT_TRIGGER,
	[RLU_KEY_TRIGGER_LEFT] = GAMEPAD_AXIS_LEFT_TRIGGER,
};

static const key_cb keyboard_key_lookup[] = {
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
	[RLU_KEY_BUTTON_THUMB_RIGHT] = keyboard_button_thumb_right,
	[RLU_KEY_BUTTON_THUMB_LEFT] = keyboard_button_thumb_left,
};

static const axis_cb keyboard_axis_lookup[] = {
	[RLU_KEY_STICK_LEFT_X] = keyboard_key_stick_left_x,
	[RLU_KEY_STICK_LEFT_Y] = keyboard_key_stick_left_y,
	[RLU_KEY_STICK_RIGHT_X] = keyboard_key_stick_right_x,
	[RLU_KEY_STICK_RIGHT_Y] = keyboard_key_stick_right_y,
	[RLU_KEY_TRIGGER_RIGHT] = keyboard_key_stick_trigger_right,
	[RLU_KEY_TRIGGER_LEFT] = keyboard_key_stick_trigger_left,
};

static int keyboard_key_map[] = {
	[RLU_KEY_DPAD_UP] = KEY_UP,
	[RLU_KEY_DPAD_RIGHT] = KEY_RIGHT,
	[RLU_KEY_DPAD_DOWN] = KEY_DOWN,
	[RLU_KEY_DPAD_LEFT] = KEY_LEFT,
	[RLU_KEY_BUTTON_UP] = KEY_Q,
	[RLU_KEY_BUTTON_RIGHT] = KEY_LEFT_SHIFT,
	[RLU_KEY_BUTTON_DOWN] = KEY_SPACE,
	[RLU_KEY_BUTTON_LEFT] = KEY_E,
	[RLU_KEY_BUMPER_RIGHT] = KEY_Z,
	[RLU_KEY_BUMPER_LEFT] = KEY_X,
	[RLU_KEY_BUTTON_SELECT] = KEY_ENTER,
	[RLU_KEY_BUTTON_START] = KEY_ENTER,
	[RLU_KEY_BUTTON_THUMB_RIGHT] = KEY_F,
	[RLU_KEY_BUTTON_THUMB_LEFT] = KEY_G,
};

static int keyboard_axis_map[] = {
	[RLU_KEY_STICK_LEFT_X] = KEY_A,
	[RLU_KEY_STICK_LEFT_X_ALT] = KEY_D,
	[RLU_KEY_STICK_LEFT_Y] = KEY_W,
	[RLU_KEY_STICK_LEFT_Y_ALT] = KEY_S,
	[RLU_KEY_STICK_RIGHT_X] = KEY_LEFT,
	[RLU_KEY_STICK_RIGHT_X_ALT] = KEY_RIGHT,
	[RLU_KEY_STICK_RIGHT_Y] = KEY_UP,
	[RLU_KEY_STICK_RIGHT_Y_ALT] = KEY_DOWN,
	[RLU_KEY_TRIGGER_RIGHT] = KEY_SPACE,
	[RLU_KEY_TRIGGER_LEFT] = KEY_LEFT_CONTROL,
};

typedef bool (* const keyboard_func)(int key);
static const keyboard_func keyboard_press_lookup[] = {
	[RLU_PRESS_PRESSED] = IsKeyPressed,
	[RLU_PRESS_DOWN] = IsKeyDown,
	[RLU_PRESS_RELEASED] = IsKeyReleased,
	[RLU_PRESS_UP] = IsKeyUp,
};

typedef bool (* const gamepad_func)(int player, int button);
static const gamepad_func gamepad_press_lookup[] = {
	[RLU_PRESS_PRESSED] = IsGamepadButtonPressed,
	[RLU_PRESS_DOWN] = IsGamepadButtonDown,
	[RLU_PRESS_RELEASED] = IsGamepadButtonReleased,
	[RLU_PRESS_UP] = IsGamepadButtonUp,
};

static enum {
	XSTATUS_KEYBOARD = 0x00,
	XSTATUS_GAMEPAD = 0x01,
} player_statuses[PLAYERS_MAX];

void rlu_input_prescan(int player)
{
	assert(player >= 0);
	assert(player < PLAYERS_MAX);

	if (IsGamepadAvailable(player)) {
		player_statuses[player] = XSTATUS_GAMEPAD;
	}
	else {
		player_statuses[player] = XSTATUS_KEYBOARD;
	}
}

bool rlu_input_key(int player, rlu_key key, rlu_key_press press)
{
	if (player_statuses[player] & XSTATUS_GAMEPAD) {
		return (gamepad_key_lookup[key])(player, press);
	}
	return keyboard_key_lookup[key](player, press);
}

float rlu_input_axis(int player, rlu_axis axis)
{
	if (player_statuses[player] & XSTATUS_GAMEPAD) {
		return gamepad_axis_lookup[axis](player);
	}
	return keyboard_axis_lookup[axis](player);
}


bool gamepad_dpad_up(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_DPAD_UP]);
}

bool gamepad_dpad_right(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_DPAD_RIGHT]);
}

bool gamepad_dpad_down(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_DPAD_DOWN]);
}

bool gamepad_dpad_left(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_DPAD_LEFT]);
}

bool gamepad_button_up(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_UP]);
}

bool gamepad_button_right(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_RIGHT]);
}

bool gamepad_button_down(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_DOWN]);
}

bool gamepad_button_left(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_LEFT]);
}

bool gamepad_bumper_right(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUMPER_RIGHT]);
}

bool gamepad_bumper_left(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUMPER_LEFT]);
}

bool gamepad_button_select(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_SELECT]);
}

bool gamepad_button_start(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_START]);
}

bool gamepad_button_thumb_right(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_THUMB_RIGHT]);
}

bool gamepad_button_thumb_left(int player, rlu_key_press press)
{
	return gamepad_press_lookup[press](player, gamepad_key_map[RLU_KEY_BUTTON_THUMB_LEFT]);
}

bool keyboard_dpad_up(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_DPAD_UP]);
}

bool keyboard_dpad_right(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_DPAD_RIGHT]);
}

bool keyboard_dpad_down(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_DPAD_DOWN]);
}

bool keyboard_dpad_left(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_DPAD_LEFT]);
}

bool keyboard_button_up(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_UP]);
}

bool keyboard_button_right(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_RIGHT]);
}

bool keyboard_button_down(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_DOWN]);
}

bool keyboard_button_left(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_LEFT]);
}

bool keyboard_bumper_right(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUMPER_RIGHT]);
}

bool keyboard_bumper_left(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUMPER_LEFT]);
}

bool keyboard_button_select(int player, rlu_key_press press)
{
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_SELECT]);
	(void)player;
}

bool keyboard_button_start(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_START]);
}

bool keyboard_button_thumb_right(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_THUMB_RIGHT]);
}

bool keyboard_button_thumb_left(int player, rlu_key_press press)
{
	(void)player;
	return keyboard_press_lookup[press](keyboard_key_map[RLU_KEY_BUTTON_THUMB_LEFT]);
}

float gamepad_key_stick_left_x(int player)
{
	return GetGamepadAxisMovement(player, gamepad_axis_map[RLU_KEY_STICK_LEFT_X]);
}

float gamepad_key_stick_left_y(int player)
{
	return GetGamepadAxisMovement(player, gamepad_axis_map[RLU_KEY_STICK_LEFT_Y]);
}

float gamepad_key_stick_right_x(int player)
{
	return GetGamepadAxisMovement(player, gamepad_axis_map[RLU_KEY_STICK_RIGHT_X]);
}

float gamepad_key_stick_right_y(int player)
{
	return GetGamepadAxisMovement(player, gamepad_axis_map[RLU_KEY_STICK_RIGHT_Y]);
}

float gamepad_key_stick_trigger_right(int player)
{
	return GetGamepadAxisMovement(player, gamepad_axis_map[RLU_KEY_TRIGGER_RIGHT]);
}

float gamepad_key_stick_trigger_left(int player)
{
	return GetGamepadAxisMovement(player, gamepad_axis_map[RLU_KEY_TRIGGER_LEFT]);
}

float keyboard_key_stick_left_x(int player)
{
	(void)player;
	if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_LEFT_X])) {
		return -1.0f;
	}
	else if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_LEFT_X_ALT])) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_left_y(int player)
{
	(void)player;
	if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_LEFT_Y])) {
		return -1.0f;
	}
	else if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_LEFT_Y_ALT])) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_right_x(int player)
{
	(void)player;
	if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_RIGHT_X])) {
		return -1.0f;
	}
	else if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_RIGHT_X_ALT])) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_right_y(int player)
{
	(void)player;
	if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_RIGHT_Y])) {
		return -1.0f;
	}
	else if (IsKeyDown(keyboard_axis_map[RLU_KEY_STICK_RIGHT_Y_ALT])) {
		return 1.0f;
	}
	return 0.0f;
}

float keyboard_key_stick_trigger_right(int player)
{
	(void)player;
	if (IsKeyDown(keyboard_axis_map[RLU_KEY_TRIGGER_RIGHT])) {
		return 1.0f;
	}
	return -1.0f;
}

float keyboard_key_stick_trigger_left(int player)
{
	(void)player;
	if (IsKeyDown(keyboard_axis_map[RLU_KEY_TRIGGER_LEFT])) {
		return 1.0f;
	}
	return -1.0f;
}
