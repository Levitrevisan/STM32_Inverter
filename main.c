#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_tim.h"

#define PWM_TIMERS_PRESCALER 0	
#define PWM_TIMERS_PERIOD 3550
#define PWM_TIMERS_INIT_PULSE 500
#define NUMBER_OF_POINTS_IN_ONE_CYCLE 60

// Senoid Pattern generated in MatLab Script on subfolder

int senoid_pwm[NUMBER_OF_POINTS_IN_ONE_CYCLE] = {1775 ,1961 ,2144 ,2324 ,2497 ,2663 ,2818 ,2963 ,3094 ,3211 ,3312 ,3397 ,3463 ,3511 ,3540 ,3550 ,3540 ,3511 ,3463 ,3397 ,3312 ,3211 ,3094 ,2963 ,2818 ,2663 ,2497 ,2324 ,2144 ,1961 ,1775 ,1589 ,1406 ,1226 ,1053 ,888 ,732 ,587 ,456 ,339 ,238 ,153 ,87 ,39 ,10 ,0 ,10 ,39 ,87 ,153 ,238 ,339 ,456 ,587 ,732 ,887 ,1053 ,1226 ,1406 ,1589};

// State Control Variables

int t_senoid_R = 0;
int t_senoid_S = NUMBER_OF_POINTS_IN_ONE_CYCLE/3;
int t_senoid_T = NUMBER_OF_POINTS_IN_ONE_CYCLE*2/3;

// Functions Prototypes										

void delay(unsigned int nCount);
void InitializeTimer2(void);
void InitializeTimer3(void);
void InitializeTimer4IT(void);
void InitializeGPIOInterruptionOnPB6(void);
void InitializePWMChannels(void);
void InitializePWMGPIO(void);
void InitializeLEDGPIO(void);
void toggle_led_PC13(void);
void changeRpositiveDutyCycle(int dutyCycle);
void changeRnegativeDutyCycle(int dutyCycle);
void changeSpositiveDutyCycle(int dutyCycle);
void changeSnegativeDutyCycle(int dutyCycle);
void changeTpositiveDutyCycle(int dutyCycle);
void changeTnegativeDutyCycle(int dutyCycle);
void updatePhaseR(void);
void updatePhaseS(void);
void updatePhaseT(void);

void setup(){
	InitializeLEDGPIO();
	InitializeTimer2();
	InitializeTimer3();
	InitializeTimer4IT();
	InitializePWMChannels();
	InitializePWMGPIO();
	InitializeGPIOInterruptionOnPB6();
}


int main(){
	
	setup();
	
	while(1){

	}
	

return 0;
}

void InitializeLEDGPIO(){
	// Enable clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO_StructInit(&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_13; //Analog voltages to be measured are connected to pins 1 and 2 of port A, 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP; //Output push-pull mode 
	GPIO_Init(GPIOC, &GPIO_InitStructure); // calls the function to actually configure the port A.
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	
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
	
	// Configuration for the positive parte (non-denied PWM channles)
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM_TIMERS_INIT_PULSE;
	
	//TIM_OCx < X is the PWM channel number
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); //PA0 - R positive
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); //PA2 - S positive
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); //PA6 - T positive
	
	// Configuration for the negative parte (denied PWM channles)
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); //PA1 - R negative
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //PA3 - S negative
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); //PA7 - T negative
}

void delay(unsigned int nCount){
		unsigned int i, j;
 
		for (i = 0; i < nCount; i++)
				for (j = 0; j < 0x2AFF; j++);
}

void changeRpositiveDutyCycle(int dutyCycle){
	TIM2->CCR1 = dutyCycle;
}

void changeRnegativeDutyCycle(int dutyCycle){
	TIM2->CCR2 = dutyCycle;
}

void changeSpositiveDutyCycle(int dutyCycle){
	TIM2->CCR3 = dutyCycle;
}

void changeSnegativeDutyCycle(int dutyCycle){
	TIM2->CCR4 = dutyCycle;
}

void changeTpositiveDutyCycle(int dutyCycle){
	TIM3->CCR1 = dutyCycle;
}

void changeTnegativeDutyCycle(int dutyCycle){
	TIM3->CCR2 = dutyCycle;
}
void InitializeTimer4IT(){
	
	// Configure NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	// No StructInit call in API
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable clock for timer 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	// Create and initialize TimeBase structure
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// Configure TimeBase structure
	TIM_TimeBaseStructure.TIM_Prescaler	= 0; // considering 72MHz, 1ms
	TIM_TimeBaseStructure.TIM_Period = 19999; // each 277,777777778 us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// Enable interrupt and starting timer 4
	TIM_ITConfig(TIM4 , TIM_IT_Update , ENABLE);
	TIM_Cmd(TIM4 , ENABLE);
	
}

void InitializeGPIOInterruptionOnPB6(){
	// acording to https://scienceprog.com/interrupt-based-button-read-on-stm32f103zet6-board/
	// Interruption pin = PB1
	// Ext. Interruption Line = 0

	/* Enable clock for GPIOB */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	/* Set pin 6 as input Pull Down*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Connect PB6 to External Interrupt Event Controller line 0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	
	//EXTI_StructInit(&EXTI_InitStruct);
	// PD1 is connected to EXTI_Line1 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, DISABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_Init(&NVIC_InitStructure);
	//select NVIC channel to configure
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  //set priority to lowest
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  //set subpriority to lowest
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  //enable IRQ channel
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //update NVIC registers
  NVIC_Init(&NVIC_InitStructure);
	
	
	
	//NVIC_EnableIRQ(EXTI0_IRQn);
	
	
	
}

//EXTIn_IRQHandler - where n is the line which is also the pin
void EXTI1_IRQHandler(void){
	//Check if EXTI_Line0 is asserted
  if(EXTI_GetITStatus(EXTI_Line1) != RESET){
		toggle_led_PC13();
		
  }
	
  //we need to clear line pending bit manually
  EXTI_ClearITPendingBit(EXTI_Line1);
}

void TIM4_IRQHandler(void){
	// Update phases SPWMs
	updatePhaseR();
	updatePhaseS();
	updatePhaseT();
	// Update Interruption bit state
	TIM_ClearITPendingBit(TIM4 ,TIM_IT_Update);
}

void updatePhaseR(){
	if(t_senoid_R<60){
		changeRpositiveDutyCycle(senoid_pwm[t_senoid_R]);
		changeRnegativeDutyCycle(senoid_pwm[t_senoid_R]);
		t_senoid_R++;
	} else {
		t_senoid_R = 0;
		changeRpositiveDutyCycle(senoid_pwm[t_senoid_R]);
		changeRnegativeDutyCycle(senoid_pwm[t_senoid_R]);
	}
}

void updatePhaseS(){
	if(t_senoid_S<60){
		changeSpositiveDutyCycle(senoid_pwm[t_senoid_S]);
		changeSnegativeDutyCycle(senoid_pwm[t_senoid_S]);
		t_senoid_S++;
	} else {
		t_senoid_S = 0;
		changeSpositiveDutyCycle(senoid_pwm[t_senoid_S]);
		changeSnegativeDutyCycle(senoid_pwm[t_senoid_S]);
	}
}

void updatePhaseT(){
	if(t_senoid_T<60){
		changeTpositiveDutyCycle(senoid_pwm[t_senoid_T]);
		changeTnegativeDutyCycle(senoid_pwm[t_senoid_T]);
		t_senoid_T++;
	} else {
		t_senoid_T = 0;
		changeTpositiveDutyCycle(senoid_pwm[t_senoid_T]);
		changeTnegativeDutyCycle(senoid_pwm[t_senoid_T]);
	}
}


void toggle_led_PC13(){
	//toggle LED on pin PC13
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)){
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		} else {
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}

}
