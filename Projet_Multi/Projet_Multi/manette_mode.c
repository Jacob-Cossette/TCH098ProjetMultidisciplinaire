/*
 *
 *OBJECTIF: Conceoption des modes
 *
 * Projet_Multi
 *
 * Created: 3/26/2021 12:03:21 PM
 * Author : Jing Tong
 */
#if(1)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>    		 // sprintf
#include <util/delay.h>
#include "lcd.h"
#include "utils.h"				 // set_bit
#include "driver.h"
#include "uart.h"

void display_heartbeat(void);
void display_etat_bouton(void);
void display_pstn_joystick(void);

void display_mode(const uint8_t mode);
void read_mode (uint8_t*mode);
void mode_init();

int main(void)
{
	uint8_t mode = 1;
	

	mode_init();
	lcd_init();
	adc_init();
	uart_init(UART_0);
	
	
	sei();
	while(1){
		
		lcd_clear_display();
		
		display_heartbeat();
		
		read_mode(&mode);
		display_mode(mode); 
		
		_delay_ms(100);
		
		

	}
	
	
}

//Afficher mode
void display_mode(const uint8_t mode) {
	char str[40];
	
	switch (mode)
		{
			case 1:
			sprintf(str, "%d DEPLACEMENT", mode);
			break;
			
			case 2: // sw2
			sprintf(str, "%d ROTATION", mode);
			break;
			
			case 3: //sw3
			sprintf(str, "%d LANCER", mode);
			break;
		}
	
	lcd_set_cursor_position(0,0);
	lcd_write_string(str);
	
}


//lire mode
void read_mode (uint8_t*mode) {
	
	char str[40];
	
	//Lire etat switch 1,2,3
	uint8_t etat = read_bits(PIND, 0b11100000);
	
	if (etat != 0)
	{
		switch (etat)
			{
				case 0b01100000: // sw1
					*mode = 1;
					break;
					
				case 0b10100000: // sw2
					*mode = 2;
					break;
				
				case 0b11000000: //sw3
					*mode = 3;
					break;
			}
	}
	

}


//initialiser PD5, PD6, PD7 pour mode
void mode_init(){
		// PD7 entrée pour SW1
	DDRD=clear_bit(DDRD,PD7);
	PORTD=set_bit(PORTD,PD7);
	
	// PD6 entrée pour SW2
	DDRD=clear_bit(DDRD,PD6);
	PORTD=set_bit(PORTD,PD6);
	
	// PD5 entrée pour SW3
	DDRD=clear_bit(DDRD,PD5);
	PORTD=set_bit(PORTD,PD5);
	}

//Test
void display_heartbeat(void) {
	static uint8_t heartbeat = 'Z';

	heartbeat = (heartbeat == 'Z') ? ('A') : (heartbeat + 1);

	lcd_set_cursor_position(15, 1);
	lcd_write_char(heartbeat);
	//uart_put_byte(UART_0, heartbeat);

	
}

#endif