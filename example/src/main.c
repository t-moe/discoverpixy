#include "stm32f4_discovery.h"
#include <stdbool.h>


int main(void) {
	int state=0;
	bool oldButtonState;
	bool newButtonState;

	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	STM_EVAL_PBInit(BUTTON_USER,BUTTON_MODE_GPIO);

	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOff(LED6);
	
	while(1) {	

		newButtonState=STM_EVAL_PBGetState(BUTTON_USER);
		if((newButtonState^oldButtonState) && newButtonState) {
			state++;
			state%=4;
		}		
		oldButtonState=newButtonState;
		
		switch(state) {
			case 0:
				STM_EVAL_LEDOff(LED6);
				STM_EVAL_LEDOn(LED3);
			break;
			case 1:
				STM_EVAL_LEDOff(LED3);
				STM_EVAL_LEDOn(LED4);
			break;
			case 2:
				STM_EVAL_LEDOff(LED4);
				STM_EVAL_LEDOn(LED5);
			break;
			case 3:
				STM_EVAL_LEDOff(LED5);
				STM_EVAL_LEDOn(LED6);
			break;

		}		
	}
}
