#ifndef BK_UI_H
#define BK_UI_H

#include <SDL2/SDL.h>
#include "util.h"
#include "types.h"
#include "common.h"
#include "gfx/canvas.h"

struct UIElement;

typedef struct {
	struct UIElement* elements;
	size_t            length;
	size_t            focused;
} UIManager;

struct UIElement {
	UIManager* parent;
	Vec2       pos;
	void*      data;

	FUNCTION_POINTER(void, free, struct UIElement*);
	FUNCTION_POINTER(void, handleEvent, struct UIElement*, SDL_Event*);
	FUNCTION_POINTER(void, render, GFX_Canvas*, struct UIElement*);
};

typedef struct UIElement UIElement;

struct UITableEntry;

typedef struct {
	struct UITableEntry* entries;
	size_t               length;
	size_t               selected;
	Vec2                 elementSize;
} UITable;

struct UITableEntry {
	UITable* parent;
	GFX_Rect rect;
	void*    data;

	FUNCTION_POINTER(void, free, struct UITableEntry*);
	FUNCTION_POINTER(void, handleEvent, struct UITableEntry*, SDL_Event*);
	FUNCTION_POINTER(void, onAction, struct UITableEntry*);
	FUNCTION_POINTER(void, render, GFX_Canvas*, struct UITableEntry*);
};

typedef struct UITableEntry UITableEntry;

UIElement UI_NewTable(void);
void      UI_AddTableEntry(UIElement* ptable, UITableEntry entry);
void      UI_SetTableElementSize(UIElement* ptable, int width, int height);
UIManager UI_NewManager(void);
void      UI_FreeManager(UIManager* ui);
void      UI_AddManagerElement(UIManager* ui, UIElement element);
void      UI_ManagerHandleEvent(UIManager* ui, SDL_Event* event);
void      UI_RenderManager(UIManager* ui, GFX_Canvas* canvas);

#endif
