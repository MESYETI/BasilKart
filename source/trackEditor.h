#ifndef BK_TRACK_EDITOR_H
#define BK_TRACK_EDITOR_H

#include "ui.h"
#include "map.h"
#include "game.h"

enum {
	TRACKEDITOR_VIEW_2D,
	TRACKEDITOR_VIEW_3D
};

typedef struct {
	Map         map;
	Camera2D    camera;
	UI_Manager  ui;
	UI_Table*   table;
	UI_Element* tableElem;
	int         view;
	uint16_t    tile;
} TrackEditor;

Scene TrackEditorScene(void);

#endif
