#include "safe.h"
#include "input.h"

static InputManager input;

void Input_Init(void) {
	input.users    = NULL;
	input.numUsers = 0;

	if (SDL_GetDefaultCursor() == NULL) {
		Input_AddUser((InputUser) {INPUT_KEYBOARD});
	}
	else {
		Input_AddUser((InputUser) {INPUT_KEYBOARD | INPUT_MOUSE});
	}
}

void Input_Free(void) {
	if (input.users == NULL) {
		free(input.users);
	}
}

void Input_AddUser(InputUser user) {
	if (input.users == NULL) {
		input.users = NEW(InputUser);
	}
	else {
		input.users = SafeRealloc(input.users, input.numUsers + 1);
	}

	input.users[input.numUsers] = user;
	++ input.numUsers;
}

bool Input_KeyboardAvailable(void) {
	return true;
}

void Input_BindKey(size_t keybind, SDL_Scancode key) {
	input.actions[keybind].key[0]    = key;
	input.actions[keybind].keyAmount = 1;
}

bool Input_ActionActive(size_t actionIndex) {
	if (Input_KeyboardAvailable()) {
		const uint8_t* keys   = SDL_GetKeyboardState(NULL);
		InputAction*   action = input.actions + actionIndex;

		for (size_t i = 0; i < action->keyAmount; ++ i) {
			if (!keys[action->key[i]]) return false;
		}

		return true;
	}
	else return false;
}
