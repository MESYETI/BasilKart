#include "app.h"

int main(int argc, char** argv) {
	UNUSED(argc);
	UNUSED(argv);

	App_Init();

	while (App_Instance()->running) {
		App_Update();
	}

	App_Free();
	return 0;
}
