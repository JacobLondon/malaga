#ifndef RLU_INPUT_H
#define RLU_INPUT_H

typedef enum rlu_input_type {
	RLU_INPUT_TYPE_MOUSE,
	RLU_INPUT_TYPE_KEYBOARD,
	RLU_INPUT_TYPE_CONTROLLER,
} rlu_input;

// bool, is pressed or not
typedef enum rlu_key_tag {
	RLU_KEY_DPAD_UP,
	RLU_KEY_DPAD_RIGHT,
	RLU_KEY_DPAD_DOWN,
	RLU_KEY_DPAD_LEFT,
	RLU_KEY_BUTTON_UP,
	RLU_KEY_BUTTON_RIGHT,
	RLU_KEY_BUTTON_DOWN,
	RLU_KEY_BUTTON_LEFT,
	RLU_KEY_BUMPER_RIGHT,
	RLU_KEY_BUMPER_LEFT,
	RLU_KEY_BUTTON_SELECT,
	RLU_KEY_BUTTON_START,
	RLU_KEY_COUNT,

	// TODO: More aliases
	RLU_KEY_UP = RLU_KEY_DPAD_UP,
	RLU_KEY_RIGHT = RLU_KEY_DPAD_RIGHT,
	RLU_KEY_DOWN = RLU_KEY_DPAD_DOWN,
	RLU_KEY_LEFT = RLU_KEY_DPAD_LEFT,
} rlu_key;

typedef enum rlu_press_type {
	RLU_PRESS_PRESSED,
	RLU_PRESS_DOWN,
	RLU_PRESS_RELEASED,
	RLU_PRESS_UP,
} rlu_key_press;

// returns a float for movement amount
typedef enum rlu_axis_tag {
	RLU_KEY_STICK_LEFT_X,
	RLU_KEY_STICK_LEFT_Y,
	RLU_KEY_STICK_RIGHT_X,
	RLU_KEY_STICK_RIGHT_Y,
	RLU_KEY_TRIGGER_RIGHT,
	RLU_KEY_TRIGGER_LEFT,
	RLU_KEY_AXIS_COUNT
} rlu_axis;

// before any key reads this frame, run this per player
void rlu_input_prescan(int player);
bool rlu_input_key(int player, rlu_key key, rlu_key_press press);
float rlu_input_axis(int player, rlu_axis axis);

#endif // RLU_INPUT_H
