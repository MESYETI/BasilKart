#include "ui.h"
#include "app.h"
#include "safe.h"
#include "input.h"
#include "constants.h"

static void TableFree(UI_Element* element) {
	UI_Table* table = (UI_Table*) element->data;

	if (table->entries != NULL) {
		free(table->entries);
	}
	free(element->data);
}

static size_t GetFirstSelectable(UI_Table* table) {
	size_t row   = table->selected / table->columns;
	size_t start = row * table->columns;
	size_t end   = start + table->columns;

	for (size_t i = start; i < end; ++ i) {
		if (table->entries[i].selectable) {
			return i;
		}
	}

	assert(0);
}

static size_t GetLastSelectable(UI_Table* table) {
	ssize_t row   = (ssize_t) (table->selected / table->columns);
	ssize_t start = row * (ssize_t) table->columns;
	ssize_t end   = start + (size_t) table->columns;

	for (ssize_t i = end; i != start - 1; -- i) {
		if (i < 0) assert(0);
		if (table->entries[i].selectable) {
			return (size_t) i;
		}
	}

	assert(0);
}

static void TableHandleEvent(UI_Element* element, SDL_Event* event) {
	UI_Table* table = (UI_Table*) element->data;

	switch (event->type) {
		case SDL_KEYDOWN: {
			if (Input_ActionPressed(ACTION_TABLE_SELECT_UP, event)) {
				if (table->selected < table->columns) return;

				table->selected -= table->columns;

				if (!table->entries[table->selected].selectable) {
					table->selected = GetFirstSelectable(table);
				}
				break;
			}
			else if (Input_ActionPressed(ACTION_TABLE_SELECT_DOWN, event)) {
				if (table->selected >= table->length - table->columns) return;

				table->selected += table->columns;

				if (!table->entries[table->selected].selectable) {
					table->selected = GetFirstSelectable(table);
				}
				break;
			}
			else if (Input_ActionPressed(ACTION_TABLE_SELECT_LEFT, event)) {
				if (table->selected % table->columns == 0) return;

				size_t first = GetFirstSelectable(table);

				if (table->selected > first) {
					-- table->selected;
				}
				break;
			}
			else if (Input_ActionPressed(ACTION_TABLE_SELECT_RIGHT, event)) {
				if (table->selected % table->columns == table->columns - 1) return;

				size_t last = GetLastSelectable(table);

				if (table->selected < last) {
					++ table->selected;
				}
				break;
			}
			else if (Input_ActionPressed(ACTION_TABLE_SELECT, event)) {
				table->entries[table->selected].onAction(table->entries + table->selected);
				break;
			}
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
	App*      app   = App_Instance();
	int       w     = element->rect.w / APP_UI_SLICE_SIZE;
	int       h     = element->rect.h / APP_UI_SLICE_SIZE;

	for (int x = 0; x < w; ++ x) {
		for (int y = 0; y < h; ++ y) {
			GFX_Rect dest = (GFX_Rect) {
				(x * APP_UI_SLICE_SIZE) + element->rect.x,
				(y * APP_UI_SLICE_SIZE) + element->rect.y,
				APP_UI_SLICE_SIZE, APP_UI_SLICE_SIZE
			};

			GFX_Rect src = (GFX_Rect) {
				table->slices.x * APP_UI_SLICE_SIZE,
				table->slices.y * APP_UI_SLICE_SIZE,
				APP_UI_SLICE_SIZE, APP_UI_SLICE_SIZE
			};

			if ((y == 0) && (x == 0)) {
				src.x += 0;
				src.y += 0;
			}
			else if ((y == h - 1) && (x == 0)) {
				src.x += 0 * APP_UI_SLICE_SIZE;
				src.y += 2 * APP_UI_SLICE_SIZE;
			}
			else if ((y == 0) && (x == w - 1)) {
				src.x += 2 * APP_UI_SLICE_SIZE;
				src.y += 0 * APP_UI_SLICE_SIZE;
			}
			else if ((y == h - 1) && (x == w - 1)) {
				src.x += 2 * APP_UI_SLICE_SIZE;
				src.y += 2 * APP_UI_SLICE_SIZE;
			}
			else if (y == 0) {
				src.x += 1 * APP_UI_SLICE_SIZE;
				src.y += 0 * APP_UI_SLICE_SIZE;
			}
			else if (y == h - 1) {
				src.x += 1 * APP_UI_SLICE_SIZE;
				src.y += 2 * APP_UI_SLICE_SIZE;
			}
			else if (x == 0) {
				src.x += 0 * APP_UI_SLICE_SIZE;
				src.y += 1 * APP_UI_SLICE_SIZE;
			}
			else if (x == w - 1) {
				src.x += 2 * APP_UI_SLICE_SIZE;
				src.y += 1 * APP_UI_SLICE_SIZE;
			}
			else {
				src.x += 1 * APP_UI_SLICE_SIZE;
				src.y += 1 * APP_UI_SLICE_SIZE;
			}

			GFX_BlitCanvas(canvas, &app->uiTexture, &dest, &src);
		}
	}

	for (size_t i = 0; i < table->length; ++ i) {
		table->entries[i].render(canvas, table->entries + i);
	}
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
	table->slices   = (Vec2) {0, 0};
	table->margin   = (Vec2) {0, 0};

	return ret;
}

void UI_UpdateElementRects(UI_Element* ptable) {
	UI_Table* table = (UI_Table*) ptable->data;

	for (size_t i = 0; i < table->length; ++ i) {
		UI_TableEntry* thisEntry = table->entries + i;

		Vec2 tableStart = (Vec2) {
			ptable->rect.x + table->margin.x, ptable->rect.y + table->margin.y
		};
		Vec2 tableSize = (Vec2) {
			ptable->rect.w - (table->margin.x * 2), ptable->rect.h - (table->margin.y * 2)
		};

		thisEntry->rect.x =
			(((int) i % table->columns) *
			(tableSize.x / table->columns)) + tableStart.x;
		thisEntry->rect.y =
			((((int) i) / table->columns) * table->elementSize.y) + tableStart.y;
		thisEntry->rect.w    = tableSize.x / table->columns;
		thisEntry->rect.h    = table->elementSize.y;

		size_t rows = table->length / table->columns;
		if (table->length % table->columns != 0) {
			++ rows;
		}

		int col0Width = -1;

		// calculate width of column 0
		for (size_t j = 0; j < rows; ++ j) {
			col0Width = MAX(
				col0Width, table->entries[table->columns * j].minSize(
					&table->entries[table->columns * j]
				)
			);
		}

		int rowWidth = tableSize.x - col0Width;
		for (size_t j = 0; j < table->length; ++ j) {
			if (j % table->columns == 0) {
				table->entries[j].rect.w = col0Width;
			}
			else {
				UI_TableEntry* thisEntry = table->entries + j;

				thisEntry->rect.x =
					(((int) ((j - (int) (j / table->columns)) - 1) % (table->columns - 1)) *
					(rowWidth / (table->columns - 1))) + tableStart.x + col0Width;

				thisEntry->rect.w = rowWidth / (table->columns - 1);
			}
		}
	}
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
	*thisEntry               = entry;
	thisEntry->parent        = table;
	thisEntry->thisIndex     = table->length - 1;
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
	assert(table->selected != SIZE_MAX);

	return (Vec2) {
		table->selected % table->columns, table->selected / table->columns
	};
}

void UI_SetTableMargin(UI_Element* ptable, int w, int h) {
	UI_Table* table = (UI_Table*) ptable->data;
	table->margin   = (Vec2) {w, h};
}

void UI_UpdateTableHeight(UI_Element* ptable) {
	UI_Table*      table     = (UI_Table*) ptable->data;
	UI_TableEntry* lastEntry = table->entries + (table->length - 1);

	int rows = table->length / table->columns;
	if (table->length % table->columns != 0) {
		++ rows;
	}

	ptable->rect.h = (lastEntry->rect.h * rows) + (table->margin.y * 2);
}

void UI_InitTableSelection(UI_Element* ptable) {
	UI_Table* table = (UI_Table*) ptable->data;
	table->selected = GetFirstSelectable(table);
}

void UI_CenterElement(UI_Element* element) {
	element->rect.x = (APP_WIN_WIDTH / 2) - (element->rect.w / 2);
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
