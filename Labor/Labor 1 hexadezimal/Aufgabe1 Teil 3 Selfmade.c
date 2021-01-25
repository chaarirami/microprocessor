#include "tm4c1294ncpdt.h"              //Einbinden der MC-Header
#include "stdio.h"

#define FOUR 4
#define TWENTY 20
#define DELAY 300000					// Um die Aufgabe ordnungsgemäß zu erfüllen muss DELAY mit WARTEN ersetzt werden. Wenn WARTEN verwendet wird,
#define WARTEN 1600000					// wird nach der Eingabe eine Zeit von etwa 1s bis zur nächsten Eingabe gewartet. Mit DELAY ist die
#define FEHLERT 500000					// Ansprechzeit (mit ~0,2s Verzögerung) der Tastatur angenehmer.
#define PORTBESCHR 1

void warte(unsigned long);
void Tastaturabfrage(void);

int main(){

	SYSCTL_RCGCGPIO_R |= 0x800;	    				// Port M clock ini
	while ((SYSCTL_PRGPIO_R & 0x00000800) ==0);		// Port M ready ?
	GPIO_PORTM_DEN_R  = 0xFF;
	GPIO_PORTM_DIR_R  = 0x0F;
	GPIO_PORTM_DATA_R = 0xFF;
	while(1){
		Tastaturabfrage();
	}
}		

void Tastaturabfrage(void){
		unsigned int i = 0;
		for(i= 0;i<FOUR;i++){
			switch (i){
			case 0:
				GPIO_PORTM_DATA_R = 0xFE;
				break;
			case 1:
				GPIO_PORTM_DATA_R = 0xFD;
				break;
			case 2:
				GPIO_PORTM_DATA_R = 0xFB;
				break;
			case 3:
				GPIO_PORTM_DATA_R = 0xF7;
				break;
			}
			warte(PORTBESCHR);
			switch(GPIO_PORTM_DATA_R){
				case 0xEE :
					printf("1\n");
					warte(DELAY);
				break;
				case 0xDE:
					printf("4\n");
					warte(DELAY);
				break;
				case 0xBE :
					printf("7\n");
					warte(DELAY);
				break;
				case 0x7E :
					printf("A\n");
					warte(DELAY);
				break;
				case 0xED :
					printf("2\n");
					warte(DELAY);
				break;
				case 0xDD :
					printf("5\n");
					warte(DELAY);
				break;
				case 0xBD :
					printf("8\n");
					warte(DELAY);
				break;
				case 0x7D :
					printf("0\n");
					warte(DELAY);
				break;
				case 0xEB :
					printf("3\n");
					warte(DELAY);
				break;
				case 0xDB :
					printf("6\n");
					warte(DELAY);
				break;
				case 0xBB :
					printf("9\n");
					warte(DELAY);
					break;
				case 0x7B :
					printf("B\n");
					warte(DELAY);
				break;
				case 0xE7 :
					printf("F\n");
					warte(DELAY);
				break;
				case 0xD7 :
					printf("E\n");
					warte(DELAY);
				break;
				case 0xB7 :
					printf("D\n");
					warte(DELAY);
				break;
				case 0x77 :
					printf("C\n");
					warte(DELAY);
				break;
				case 0xFE:
				break;
				case 0xFD:
				break;
				case 0xFB:
				break;
				case 0xF7 :
				break;
				default :
					printf("Fehler\n");
					warte(FEHLERT);
				break;
			}

		}
}

void warte (unsigned long zeit){        //Wartefunktion
	unsigned long j ;
	for (j = 0 ;  j < zeit ;  j++ )  ;
}	
