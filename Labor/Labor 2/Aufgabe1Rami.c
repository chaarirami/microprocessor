#include <stdio.h>
#include <stdint.h>
#include "tm4c1294ncpdt.h"

void wait (unsigned int waitTime){
	for (int i=0; i< waitTime; i++);
}

int main(void){

	SYSCTL_RCGCPIO_R |= ((1 << 10) | (1 << 11)); //clock enable port M and L
	GPIO_PORTL_DEN_R = 0x01; // activate L(0)
	GPIO_PORTM_DEN_R = 0xFF; // activate port M
	GPIO_PORTL_DIR_R = 0x00; // define L(0) as input
	GPIO_PORTM_DIR_R = 0xFF; // define port M as output
	
	unsigned char oldValue, newValue = 0x00; // used to compare the old value of the input with new one
 	
	while(1){
		newValue = GPIO_PORTL_DATA_R, // read input value
		if (oldValue != newValue){
			GPIO_PORTM_DATA_R = 0xFF; //LEDs on
			wait(10000);			  //wait until signal reaches LED
		} else {
			GPIO_PORTM_DATA_R = 0x00; //LED off
			wait(10000);
		}
		oldValue = newValue;
	}
	return 0;
}