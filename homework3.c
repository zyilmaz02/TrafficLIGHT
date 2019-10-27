#include "tm4c123gh6pm.h"


void EnableInterrupts(void); // Enabling the Interrrupts

void PORTB_init(void); //port B is going to be used for extarnal SWitches 
void PORTA_init(void); // Port A is going to be used for External LEDs

void PORTA_data1(void); 
void delay(int tm);
void PORTA_data2(void);
void PORTA_data3(void);

int on = 1;      // this parameter will allow us to see if the system is on or off
int rain = 0;

uint8_t Data = 0x22; // just an initialization

uint8_t OFF = 0;
uint8_t NORMAL = 1;
uint8_t RAIN = 2;

uint8_t STATE = 0;

int main() {
	
	PORTA_init();
	PORTB_init();
	Data = GPIO_PORTB_DATA_R;

	while (1) {		
		if(STATE == OFF){
			// No LED will be Blinking 
			PORTA_data2();
		}
		
		if(STATE == NORMAL){
			// it will  work in normal State
			PORTA_data1();
		}
		
		if(STATE == RAIN){
			// it will work in RAIN State.
			PORTA_data3();
		}


	}


}


void PORTB_init(void) {

	SYSCTL_RCGCGPIO_R |= 0x02; // 1) activate Port B
	GPIO_PORTB_DIR_R &= ~0x03; // input PB0 and PB1 are used for inpurt
	GPIO_PORTB_DEN_R |= 0x03;  // digital enabled
	
		// USED Interrupt
	GPIO_PORTB_IS_R &= ~0x03; 
	GPIO_PORTB_IBE_R &= ~0x03; 
	GPIO_PORTB_IEV_R |= 0x03; 
	GPIO_PORTB_ICR_R = 0x03; 
	GPIO_PORTB_IM_R |= 0x03; 
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000;; 
	NVIC_EN0_R = 0x00000002; 
	
	EnableInterrupts();
}

void PORTA_init(void) {
	SYSCTL_RCGCGPIO_R |= 0x01;
	GPIO_PORTA_DIR_R |= 0xFF;  // 
	GPIO_PORTA_DEN_R |= 0xFF;
}

void PORTA_data1(void) {  // NORMAL MODE this function will be called during normad state.
	GPIO_PORTA_DATA_R = 0x30;
	delay(10000000);
	GPIO_PORTA_DATA_R = 0x50;
	delay(2500000);
	GPIO_PORTA_DATA_R = 0x84;
	delay(10000000);
	GPIO_PORTA_DATA_R = 0x88;
	delay(2500000);
}

void PORTA_data2(void) {   // OFF MODE this function will be called in the OFF Mode
	GPIO_PORTA_DATA_R = 0x00;
}
void PORTA_data3(void) {  // RAIN Mode . This function will be called for the Rain STATE
	GPIO_PORTA_DATA_R = 0x50;
	delay(3000000);
	GPIO_PORTA_DATA_R = 0x00;
	delay(3000000);
}

void GPIOPortB_Handler(void){
	GPIO_PORTB_ICR_R = 0x03;
	Data = GPIO_PORTB_DATA_R;
	
	if(STATE == OFF){
		if(Data == 0x01){
			STATE = NORMAL;
		}
	}else	if(STATE == NORMAL){
		
		if(Data == 0x01){
			STATE = OFF;
		}else if(Data == 0x02){
			STATE = RAIN;
		}
	}else	if(STATE == RAIN){
		if(Data == 0x02){
			STATE = NORMAL;
		}
		if(Data == 0x01){
			STATE = OFF;
		}		
	}
}
	
	

void delay(int tm) {
	int i = tm;
	while (i > 0) {
		i--;
	}
}