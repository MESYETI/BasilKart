#ifndef BK_INPUT_H
#define BK_INPUT_H

#include <SDL2/SDL.h>
#include "common.h"
#include "types.h"

#define INPUT_KEYBOARD 1
#define INPUT_MOUSE    2

enum {
	ACTION_CAMERA_FORWARD = 0,
	ACTION_CAMERA_LEFT,
	ACTION_CAMERA_RIGHT,
	ACTION_CAMERA_BACKWARDS,
	ACTION_CAMERA_LOOK_LEFT,
	ACTION_CAMERA_LOOK_RIGHT,
	ACTION_CAMERA_LOOK_UP,
	ACTION_CAMERA_LOOK_DOWN,

	ACTION_AMOUNT
};

typedef struct {
	uint8_t types;
} InputUser;

typedef struct {
	SDL_Scancode key[3];
	size_t       keyAmount;
} InputAction;

typedef struct {
	InputUser*  users;
	size_t      numUsers;
	InputAction actions[ACTION_AMOUNT];
} InputManager;

void Input_Init(void);
void Input_Free(void);
void Input_AddUser(InputUser user);
bool Input_KeyboardAvailable(void);
void Input_BindKey(size_t keybind, SDL_Scancode key);
bool Input_ActionActive(size_t actionIndex);

#endif
