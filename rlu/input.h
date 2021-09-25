#ifndef RLU_INPUT_H
#define RLU_INPUT_H

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
	RLU_KEY_BUTTON_THUMB_RIGHT,
	RLU_KEY_BUTTON_THUMB_LEFT,
} rlu_key;

typedef enum rlu_press_type {
	RLU_PRESS_PRESSED,
	RLU_PRESS_DOWN,
	RLU_PRESS_RELEASED,
	RLU_PRESS_UP,
} rlu_key_press;

// returns a float for movement amount
typedef enum rlu_axis_tag {
	RLU_KEY_STICK_LEFT_X, // -1 left, 0 middle, 1 right
	RLU_KEY_STICK_LEFT_Y, // -1 up, 0 middle, 1 down
	RLU_KEY_STICK_RIGHT_X,
	RLU_KEY_STICK_RIGHT_Y,
	RLU_KEY_TRIGGER_RIGHT, // -1 not pulled, 1 pulled all the way
	RLU_KEY_TRIGGER_LEFT,
	// auxiliary
	RLU_KEY_STICK_LEFT_X_ALT,
	RLU_KEY_STICK_LEFT_Y_ALT,
	RLU_KEY_STICK_RIGHT_X_ALT,
	RLU_KEY_STICK_RIGHT_Y_ALT,
} rlu_axis;

// before any key reads this frame, run this per player
void rlu_input_prescan(int player);
bool rlu_input_key(int player, rlu_key key, rlu_key_press press);
float rlu_input_axis(int player, rlu_axis axis);

#endif // RLU_INPUT_H
