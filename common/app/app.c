#include "app.h"
#include "tft.h"
#include "system.h"
#include "touch.h"
#include "screen_main.h"
#include "filesystem.h"

void app_init() {
	system_init();
	tft_init();
	touch_init();
	filesystem_init();

	gui_screen_navigate(get_screen_main());
}

//app event loop
void app_process() {
	
	system_process(); //Let the system handle it's pending events
	gui_screen_update(); //update the currently active screen
}


