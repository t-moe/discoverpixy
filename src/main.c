#include "stm32f4_discovery.h"


int main(void) {
	int state=0;
	const int APB1_CLK=42E6; //42mhz APB1 Takt
	TIM_TimeBaseInitTypeDef tinit;

	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOff(LED6);
	

	TIM_TimeBaseStructInit(&tinit);

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6EN,ENABLE);
	//RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	tinit.TIM_Prescaler = APB1_CLK/1000 -1;
	tinit.TIM_Period = 249;
	TIM_TimeBaseInit(TIM6,&tinit);
	TIM_Cmd(TIM6,ENABLE);


	/*TIM6->PSC = APB1_CLK/1000 -1;
	TIM6->DIER |=TIM_DIER_UIE;
	TIM6->ARR = 249;
	TIM6->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
	TIM6->EGR = TIM_EGR_UG;
	TIM6->SR &= ~TIM_SR_UIF;*/


	while(1) {	

		//while((TIM6->SR&0x01)==0);
		//TIM6->SR=0;

		while(TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)!=SET);
		TIM_ClearFlag(TIM6,TIM_FLAG_Update);

		state++;
		state%=4;
		
		switch(state) {
			case 0:
				STM_EVAL_LEDOff(LED5);
				STM_EVAL_LEDOn(LED3);
			break;
			case 1:
				STM_EVAL_LEDOff(LED3);
				STM_EVAL_LEDOn(LED4);
			break;
			case 2:
				STM_EVAL_LEDOff(LED4);
				STM_EVAL_LEDOn(LED6);
			break;
			case 3:
				STM_EVAL_LEDOff(LED6);
				STM_EVAL_LEDOn(LED5);
			break;

		}		
	}
}
