#include "ui.h"
#include "safe.h"

static void TableFree(UIElement* element) {
	UITable* table = (UITable*) element->data;

	if (table->entries != NULL) {
		free(table->entries);
	}
	free(element->data);
}

static void TableHandleEvent(UIElement* element, SDL_Event* event) {
	UITable* table = (UITable*) element->data;

	for (size_t i = 0; i < table->length; ++ i) {
		table->entries[i].handleEvent(table->entries + i, event);
	}
}

static void TableRender(GFX_Canvas* canvas, UIElement* element) {
	UITable* table = (UITable*) element->data;

	for (size_t i = 0; i < table->length; ++ i) {
		table->entries[i].render(canvas, table->entries + i);
	}
}

UIElement UI_NewTable(void) {
	UIElement ret;
	ret.data        = (void*) NEW(UITable);
	ret.free        = &TableFree;
	ret.handleEvent = &TableHandleEvent;
	ret.render      = &TableRender;

	UITable* table  = (UITable*) &ret.data;
	table->entries  = NULL;
	table->length   = 0;
	table->selected = 0;

	return ret;
}

void UI_AddTableEntry(UIElement* ptable, UITableEntry entry) {
	UITable* table = (UITable*) ptable->data;

	++ table->length;

	if (table->entries == NULL) {
		table->entries = SafeMalloc(sizeof(UITableEntry));
	}
	else {
		table->entries = SafeRealloc(table->entries, sizeof(UITableEntry) * table->length);
	}

	table->entries[table->length - 1]        = entry;
	table->entries[table->length - 1].rect.x = ptable->pos.x;
	table->entries[table->length - 1].rect.y =
		ptable->pos.y + ((table->length - 1) * table->elementSize.y);
	table->entries[table->length - 1].rect.w = table->elementSize.x;
	table->entries[table->length - 1].rect.h = table->elementSize.y;
	table->entries[table->length - 1].parent = table;
}

void UI_SetTableElementSize(UIElement* ptable, int width, int height) {
	UITable* table       = (UITable*) ptable->data;
	table->elementSize.y = height;
	table->elementSize.x = width;
}

UIManager UI_NewManager(void) {
	UIManager ret;
	ret.elements = NULL;
	ret.length   = 0;
	ret.focused  = 0;
	return ret;
}

void UI_FreeManager(UIManager* ui) {
	if (ui->elements != NULL) {
		for (size_t i = 0; i < ui->length; ++ i) {
			ui->elements[i].free(ui->elements + i);
		}

		free(ui->elements);
	}
}

void UI_AddManagerElement(UIManager* ui, UIElement element) {
	++ ui->length;

	if (ui->elements == NULL) {
		ui->elements = SafeMalloc(sizeof(UIElement));
	}
	else {
		ui->elements = SafeRealloc(ui->elements, sizeof(UIElement) * ui->length);
	}

	ui->elements[ui->length - 1]        = element;
	ui->elements[ui->length - 1].parent = ui;
}

void UI_ManagerHandleEvent(UIManager* ui, SDL_Event* event) {
	for (size_t i = 0; i < ui->length; ++ i) {
		ui->elements[i].handleEvent(ui->elements + i, event);
	}
}

void UI_RenderManager(UIManager* ui, GFX_Canvas* canvas) {
	for (size_t i = 0; i < ui->length; ++ i) {
		ui->elements[i].render(canvas, ui->elements + i);
	}
}
