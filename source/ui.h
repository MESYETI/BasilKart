#ifndef BK_UI_H
#define BK_UI_H

#include <SDL2/SDL.h>
#include "util.h"
#include "types.h"
#include "common.h"
#include "gfx/canvas.h"

struct UI_Element;

typedef struct {
	struct UI_Element* elements;
	size_t            length;
	size_t            focused;
} UI_Manager;

struct UI_Element {
	UI_Manager* parent;
	GFX_Rect    rect;
	void*       data;

	FUNCTION_POINTER(void, free, struct UI_Element*);
	FUNCTION_POINTER(void, handleEvent, struct UI_Element*, SDL_Event*);
	FUNCTION_POINTER(void, render, GFX_Canvas*, struct UI_Element*);
};

typedef struct UI_Element UI_Element;

struct UI_TableEntry;

typedef struct {
	struct UI_TableEntry* entries;
	size_t                length;
	size_t                columns;
	size_t                selected;
	Vec2                  elementSize;
	Vec2                  slices;
	Vec2                  margin;
} UI_Table;

struct UI_TableEntry {
	UI_Table* parent;
	GFX_Rect  rect;
	void*     data;
	bool      selectable;
	size_t    thisIndex;

	FUNCTION_POINTER(void, free, struct UI_TableEntry*);
	FUNCTION_POINTER(void, handleEvent, struct UI_TableEntry*, SDL_Event*);
	FUNCTION_POINTER(void, onAction, struct UI_TableEntry*);
	FUNCTION_POINTER(void, render, GFX_Canvas*, struct UI_TableEntry*);
	FUNCTION_POINTER(int,  minSize, struct UI_TableEntry*);
};

typedef struct UI_TableEntry UI_TableEntry;

UI_Element  UI_NewTable(int x, int y, int w, int h, size_t columns);
void        UI_AddTableEntry(UI_Element* ptable, UI_TableEntry entry);
void        UI_SetTableElementSize(UI_Element* ptable, int width, int height);
void        UI_TableSelect(UI_Element* ptable, size_t column, size_t line);
Vec2        UI_GetTableSelected(UI_Element* ptable);
void        UI_SetTableMargin(UI_Element* ptable, int w, int h);
void        UI_UpdateTableHeight(UI_Element* ptable);
void        UI_InitTableSelection(UI_Element* ptable);
UI_Element* UI_GetLastElement(UI_Manager* ptable);
UI_Manager  UI_NewManager(void);
void        UI_FreeManager(UI_Manager* ui);
void        UI_AddManagerElement(UI_Manager* ui, UI_Element element);
void        UI_ManagerHandleEvent(UI_Manager* ui, SDL_Event* event);
void        UI_RenderManager(UI_Manager* ui, GFX_Canvas* canvas);

#endif
