#include "app.h"

int main(void) {
	App_Init();

	while (App_Instance()->running) {
		App_Update();
	}

	App_Free();
	return 0;
}
