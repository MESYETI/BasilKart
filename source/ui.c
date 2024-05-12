#include "ui.h"
#include "safe.h"

static void TableFree(UI_Element* element) {
	UI_Table* table = (UI_Table*) element->data;

	if (table->entries != NULL) {
		free(table->entries);
	}
	free(element->data);
}

static void TableHandleEvent(UI_Element* element, SDL_Event* event) {
	UI_Table* table = (UI_Table*) element->data;

	switch (event->type) {
		case SDL_KEYDOWN: {
			// TODO
		} // fall through
		default: {
			for (size_t i = 0; i < table->length; ++ i) {
				table->entries[i].handleEvent(table->entries + i, event);
			}
		}
	}
}

static void TableRender(GFX_Canvas* canvas, UI_Element* element) {
	UI_Table* table = (UI_Table*) element->data;

	for (size_t i = 0; i < table->length; ++ i) {
		table->entries[i].render(canvas, table->entries + i);
	}

	GFX_DrawRect(canvas, element->rect, 0xFFFFFFFF);
}

UI_Element UI_NewTable(int x, int y, int w, int h, size_t columns) {
	UI_Element ret;
	ret.data        = (void*) NEW(UI_Table);
	ret.free        = &TableFree;
	ret.handleEvent = &TableHandleEvent;
	ret.render      = &TableRender;
	ret.rect.x      = x;
	ret.rect.y      = y;
	ret.rect.w      = w;
	ret.rect.h      = h;

	UI_Table* table  = (UI_Table*) ret.data;
	table->entries  = NULL;
	table->length   = 0;
	table->columns  = columns;
	table->selected = 0;

	return ret;
}

void UI_AddTableEntry(UI_Element* ptable, UI_TableEntry entry) {
	UI_Table* table = (UI_Table*) ptable->data;

	++ table->length;

	if (table->entries == NULL) {
		table->entries = SafeMalloc(sizeof(UI_TableEntry));
	}
	else {
		table->entries = SafeRealloc(table->entries, sizeof(UI_TableEntry) * table->length);
	}

	UI_TableEntry* thisEntry = &table->entries[table->length - 1];

	*thisEntry        = entry;
	thisEntry->rect.x =
		(((int) (table->length - 1) % table->columns) *
		(ptable->rect.w / table->columns)) + ptable->rect.x;
	thisEntry->rect.y =
		((((int) table->length - 1) / table->columns) * table->elementSize.y) +
		ptable->rect.y;
	thisEntry->rect.w    = ptable->rect.w / table->columns;
	thisEntry->rect.h    = table->elementSize.y;
	thisEntry->parent    = table;
	thisEntry->thisIndex = table->length - 1;
}

void UI_SetTableElementSize(UI_Element* ptable, int width, int height) {
	UI_Table* table       = (UI_Table*) ptable->data;
	table->elementSize.y = height;
	table->elementSize.x = width;
}

void UI_TableSelect(UI_Element* ptable, size_t column, size_t line) {
	UI_Table* table = (UI_Table*) ptable->data;
	table->selected = (line * table->columns) + column;
	assert(table->selected < table->length);
}

Vec2 UI_GetTableSelected(UI_Element* ptable) {
	UI_Table* table = (UI_Table*) ptable->data;

	return (Vec2) {
		table->selected % table->columns, table->selected / table->columns
	};
}

UI_Element* UI_GetLastElement(UI_Manager* ui) {
	return &ui->elements[ui->length - 1];
}

UI_Manager UI_NewManager(void) {
	UI_Manager ret;
	ret.elements = NULL;
	ret.length   = 0;
	ret.focused  = 0;
	return ret;
}

void UI_FreeManager(UI_Manager* ui) {
	if (ui->elements != NULL) {
		for (size_t i = 0; i < ui->length; ++ i) {
			ui->elements[i].free(ui->elements + i);
		}

		free(ui->elements);
	}
}

void UI_AddManagerElement(UI_Manager* ui, UI_Element element) {
	++ ui->length;

	if (ui->elements == NULL) {
		ui->elements = SafeMalloc(sizeof(UI_Element));
	}
	else {
		ui->elements = SafeRealloc(ui->elements, sizeof(UI_Element) * ui->length);
	}

	ui->elements[ui->length - 1]        = element;
	ui->elements[ui->length - 1].parent = ui;
}

void UI_ManagerHandleEvent(UI_Manager* ui, SDL_Event* event) {
	for (size_t i = 0; i < ui->length; ++ i) {
		ui->elements[i].handleEvent(ui->elements + i, event);
	}
}

void UI_RenderManager(UI_Manager* ui, GFX_Canvas* canvas) {
	for (size_t i = 0; i < ui->length; ++ i) {
		ui->elements[i].render(canvas, ui->elements + i);
	}
}
