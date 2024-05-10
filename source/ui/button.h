#ifndef BK_UI_BUTTON_H
#define BK_UI_BUTTON_H

#include "../ui.h"

typedef void (*UIButtonAction)(UITableEntry* button);

typedef struct {
	const char*    label;
	UIButtonAction onClick;
} UIButton;

UITableEntry UI_NewButton(const char* label, UIButtonAction onClick);

#endif
