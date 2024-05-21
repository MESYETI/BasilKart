#ifdef __3DS__
	#include <3ds.h>
#endif

#include "platform.h"

void Platform_Init(void) {
	#ifdef __3DS__
		gfxInitDefault();
		consoleInit(GFX_BOTTOM, NULL);
	#endif
}

void Platform_Free(void) {
	#ifdef __3DS__
		gfxExit();
	#endif
}
