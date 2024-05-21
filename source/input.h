#ifndef BK_INPUT_H
#define BK_INPUT_H

#include "common.h"
#include "types.h"

#define INPUT_KEYBOARD 1
#define INPUT_MOUSE    2

enum {
	// map preview for level builder
	ACTION_CAMERA_FORWARD = 0,
	ACTION_CAMERA_LEFT,
	ACTION_CAMERA_RIGHT,
	ACTION_CAMERA_BACKWARDS,
	ACTION_CAMERA_LOOK_LEFT,
	ACTION_CAMERA_LOOK_RIGHT,
	ACTION_CAMERA_LOOK_UP,
	ACTION_CAMERA_LOOK_DOWN,

	// table UIs
	ACTION_TABLE_SELECT_LEFT,
	ACTION_TABLE_SELECT_RIGHT,
	ACTION_TABLE_SELECT_UP,
	ACTION_TABLE_SELECT_DOWN,
	ACTION_TABLE_SELECT,

	ACTION_AMOUNT
};

typedef struct {
	uint8_t types;

	// mouse
	Vec2 mousePos;
} InputUser;

typedef struct {
	SDL_Scancode key[3]; // first is primary key, next ones are modifiers
	size_t       keyAmount;
} InputAction;

typedef struct {
	InputUser*  users;
	size_t      numUsers;
	InputAction actions[ACTION_AMOUNT];
} InputManager;

void       Input_Init(void);
void       Input_Free(void);
void       Input_AddUser(InputUser user);
bool       Input_KeyboardAvailable(void);
void       Input_BindKey(size_t keybind, SDL_Scancode key);
bool       Input_ActionActive(size_t actionIndex);
bool       Input_ActionPressed(size_t actionIndex, SDL_Event* e);
void       Input_HandleEvent(SDL_Event* e);
InputUser* Input_GetUser(size_t which);

#endif
