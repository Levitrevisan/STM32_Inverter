#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define PWM_TIMERS_PRESCALER 0	
#define PWM_TIMERS_PERIOD 7900
#define PWM_TIMERS_INIT_PULSE 500


void delay(unsigned int nCount);
void InitializeTimer2();
void InitializeTimer3();
void InitializeTimer4IT();
void InitializePWMChannels();
void InitializePWMGPIO();
void InitializeLEDGPIO();
void changeRpositiveDutyCycle(int dutyCycle);
void changeRnegativeDutyCycle(int dutyCycle);
void changeSpositiveDutyCycle(int dutyCycle);
void changeSnegativeDutyCycle(int dutyCycle);
void changeTpositiveDutyCycle(int dutyCycle);
void changeTnegativeDutyCycle(int dutyCycle);

void setup(){
	InitializeLEDGPIO();
	InitializeTimer2();
	InitializeTimer3();
	InitializeTimer4IT();
	InitializePWMChannels();
	InitializePWMGPIO();
}


int main(){
	
	setup();
	
	changeRpositiveDutyCycle(1024);
	changeRnegativeDutyCycle(4095);
	changeSpositiveDutyCycle(1024);
	changeSnegativeDutyCycle(4095);
	changeTpositiveDutyCycle(1024);
	changeTnegativeDutyCycle(4095);	
	
	while(1){

	}
	

return 0;
}

void InitializeLEDGPIO(){
	// Enable clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_13; //Analog voltages to be measured are connected to pins 1 and 2 of port A, 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP; //Analog input mode for ADC
	GPIO_Init(GPIOC, &GPIO_InitStructure); // calls the function to actually configure the port A.
}

void InitializePWMGPIO(){
	// Enable clock for GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					//PA0
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					//PA1
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//PA2
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;					//PA3
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					//PA6
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;					//PA7
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
}

void InitializeTimer2(){
	// Enable clock for timer 2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	// Create and initialize TimeBase structure
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// Configure TimeBase structure
	TIM_TimeBaseStructure.TIM_Prescaler	= PWM_TIMERS_PRESCALER;//SystemCoreClock/8000 - 1; // considering 8MHz, 1ms
	TIM_TimeBaseStructure.TIM_Period = PWM_TIMERS_PERIOD; // 0..999
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);
	
}

void InitializeTimer3(){
	// Enable clock for timer 2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	// Create and initialize TimeBase structure
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// Configure TimeBase structure
	TIM_TimeBaseStructure.TIM_Prescaler	= PWM_TIMERS_PRESCALER;//SystemCoreClock/8 - 1; // considering 8MHz, 1ms
	TIM_TimeBaseStructure.TIM_Period = PWM_TIMERS_PERIOD; // 0..999
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);
	
}

void InitializePWMChannels(){
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM_TIMERS_INIT_PULSE;
	
	//TIM_OCx < X is the PWM channel number
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); //PA0
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); //PA1
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); //PA2
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //PA3
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); //PA6
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); //PA7

}

void delay(unsigned int nCount){
		unsigned int i, j;
 
		for (i = 0; i < nCount; i++)
				for (j = 0; j < 0x2AFF; j++);
}

void changeRpositiveDutyCycle(int dutyCycle){
TIM2->CCR1 = dutyCycle; //0 .. 8191
}

void changeRnegativeDutyCycle(int dutyCycle){
TIM2->CCR2 = dutyCycle; //0 .. 8191
}

void changeSpositiveDutyCycle(int dutyCycle){
TIM2->CCR3 = dutyCycle; //0 .. 8191
}

void changeSnegativeDutyCycle(int dutyCycle){
TIM2->CCR4 = dutyCycle; //0 .. 8191
}

void changeTpositiveDutyCycle(int dutyCycle){
TIM3->CCR1 = dutyCycle; //0 .. 8191
}

void changeTnegativeDutyCycle(int dutyCycle){
TIM3->CCR2 = dutyCycle; //0 .. 8191
}
void InitializeTimer4IT(){
	
	// Configure NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	// No StructInit call in API
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable clock for timer 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	// Create and initialize TimeBase structure
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// Configure TimeBase structure
	TIM_TimeBaseStructure.TIM_Prescaler	= 1; // considering 72MHz, 1ms
	TIM_TimeBaseStructure.TIM_Period = 35999; // each 1ms
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// Enable interrupt and starting timer 4
	TIM_ITConfig(TIM4 , TIM_IT_Update , ENABLE);
	TIM_Cmd(TIM4 , ENABLE);
	
}

void TIM4_IRQHandler(void){

	//toggle LED on pin PC13
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)){
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		} else {
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		
	TIM_ClearITPendingBit(TIM4 ,TIM_IT_Update);
}


