/*
 * Projet_Multi_4X4.c
 *
 * Created: 4/15/2021 4:47:15 PM
 * Author : Jing Tong et Jacob
 */ 

// Programme d'initiation :
// Programme qui allume les DELs lorsque le bouton du « joystick » est appuyé et
// affiche le message « DEL en fonction » sur l’écran LCD et éteint les DELs lorsque
// le bouton du « joystick » est relâché et affiche le message « DEL hors fonction ».

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // a faire je me rappelle plus les comande 
#include "driver.h"
#include "utils.h"
#include "lcd.h"
#include "fifo.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>




int main(void){
	
	uart_init(UART_0);
	lcd_init();
	pwm0_init();
	pwm2_init();

	sei();
	
	//*******Uint******
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;
	uint8_t mode = 0;
	uint8_t direction = 0;
	uint8_t lancer = 0;
	uint8_t joystick = 0;
	
	
	//******Strings*****
	char message[40] = {'\0'};
	char str    [40] = {'\0'};
	
	
	
	DDRB = clear_bits(DDRB, 0b0000111);

	//Test pour vérifier l'affichage et le rafraîchissement
	void display_heartbeat(void) {
		static uint8_t heartbeat = 'Z';

		heartbeat = (heartbeat == 'Z') ? ('A') : (heartbeat + 1);

		lcd_set_cursor_position(15, 1);
		lcd_write_char(heartbeat);
		//uart_put_byte(UART_0, heartbeat);

		
	}
	
	
	while (1)
	{
		
		
		
		

		if (!uart_is_rx_buffer_empty(UART_0)){
			//if(uart_get_byte(UART_0) =='X'){
				lcd_clear_display();
				_delay_ms(20);
				uart_get_string(UART_0,message,151);

				sscanf(message,  "X%03dY%03dZ%03dm%dM%dL%dD%d\0", &x, &y, &z, &mode, &direction, &lancer, &joystick);
				//sprintf(str, "%s", message);
				
				sprintf(str, "X%03dY%03dZ%03dm%dM%dL%dD%d\0", x, y, z, mode, direction, lancer, joystick);
				lcd_set_cursor_position(0,0);

				lcd_write_string(str);
			//} 
			
		}
		
		switch (mode)
		{case 1:
			
			if (x >= 200){
				DDRB = clear_bits(DDRB, 0b0000011);
				PORTB = set_bit(DDRB, 2);
				pwm0_set_PB3(z);
				pwm0_set_PB4(z);
			}
			else if (x <= 100){
				DDRB = clear_bits(DDRB, 0b0000011);
				PORTB = set_bit(DDRB, 1);
				pwm0_set_PB3(z);
				pwm0_set_PB4(z);
			}
			
			else{
				pwm0_set_PB3(0);
				pwm0_set_PB4(0);
				}
				break;
		case 0:
		
		// changer la variable moteur pour diration R --> arrier D --> Avant
		// NOTE :  ceci est un brouillon
		if(direction == "G")
			
				DDRB = clear_bits(DDRB, 0b0000011);
				//PB1 = 1 PB2 = 1 Alors la direction est vers l<avant 
				PORTB = set_bit(DDRB, 3);
				pwm0_set_PB3(z);
				pwm0_set_PB4(z);
			
		else if (direction == "D"){
				DDRB = clear_bits(DDRB, 0b0000011);
				PORTB = set_bit(DDRB,0);
				pwm0_set_PB3(z);
				pwm0_set_PB4(z);
			}
			
		else{
				pwm0_set_PB3(0);
				pwm0_set_PB4(0);
			}
			break;
		}
	
	
		
	display_heartbeat();
		
	}
	
	
}
