/*
 * Projet_Multi_4X4.c
 *
 * Created: 4/15/2021 4:47:15 PM
 * Author : Jing Tong Chen et Jacob Cossette
 */ 

// Programme de embarquée:
//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "driver.h"
#include "utils.h"
#include "lcd.h"
#include "fifo.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>






int main(void){
	
	//*******Fonction******
	uart_init(UART_0);
	lcd_init();
	pwm0_init();
	pwm2_init();
	pwm6_
	setPuissanceMoteurRoue ();
	setRotation();

	sei();
	
	//*******Uint******
	uint8_t x = 0;			// Position Joystick x
	uint8_t y = 0;			// Position Joystick y
	uint8_t z = 0;			// Position potentiomètre
	uint8_t mode = 0;		// Variable selection des mondes 1: Déplacement 2: Tir sw1
	uint8_t moteurLanceur = 0;	// Activation du moteur de tir sw2
	uint8_t servoMoteurLanceur = 0;		// Activation du servo-moteur tir sw3
	uint8_t joystick = 0;	// Bouton JoyStick pour R et D
	
	
	//****Constante*****
	const uint8_t PUISSANCE_MAX = 255;
	
	//******Strings*****
	char message[40] = {'\0'};
	char str    [40] = {'\0'};
	
	
	
	DDRB = clear_bits(DDRB, 0b0000111);

	//Test pour vérifier l'affichage et le rafraîchissement
	
	
	
	while (1)
	{
		
// Lecture de donnee envoyer

		if (!uart_is_rx_buffer_empty(UART_0)){
			//if(uart_get_byte(UART_0) =='X'){
				lcd_clear_display();
				_delay_ms(20);
				uart_get_string(UART_0,message,151);

				sscanf(message,  "X%03dY%03dZ%03dm%dM%dL%dD%d\0", &x, &y, &z, &mode, &moteurLanceur, &servoMoteurLanceur, &joystick);
				//sprintf(str, "%s", message);
				
				sprintf(str, "X%03dY%03dZ%03dm%dM%dL%dD%d\0", x, y, z, mode, moteurLanceur, servoMoteurLanceur, joystick);
				lcd_set_cursor_position(0,0);

				lcd_write_string(str);
		
			
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
		if(joystick == 1){
			
				DDRB = clear_bits(DDRB, 0b0000011);
				//PB1 = 1 PB2 = 1 Alors la direction est vers l<avant 
				PORTB = set_bit(DDRB, 3);
				setPuissanceMoteurRoue(z);
		}
		else if (joystick == 0){
				DDRB = clear_bits(DDRB, 0b0000011);
				PORTB = set_bit(DDRB,0);
				setPuissanceMoteurRoue(z);
			}
			
		else{
				setPuissanceMoteurRoue(0);
			}
			break;
		}
	
	
		
	display_heartbeat();		
	}

}
	void setRotation(uint8_t valeur){	
	}
	
	
	void setPuissanceMoteurRoue (uint8_t valeur){
		pwm0_set_PB3(valeur);
		pwm0_set_PB4(valeur);
	}
	
	void display_heartbeat(void) {
		static uint8_t heartbeat = 'Z';
		heartbeat = (heartbeat == 'Z') ? ('A') : (heartbeat + 1);
		lcd_set_cursor_position(15, 1);
		lcd_write_char(heartbeat);
		//uart_put_byte(UART_0, heartbeat);
	}



/************************************************************************/
/*                 DRIVER LANCEUR 4X4                                   */
/************************************************************************/
/*
*Fonction Initialise les différents drivers utiliser dans la Fonction Lancer du 
*vehicule 4X4
*Fait appel au driver du moetru RI Servo Moteur et moteur d elevation
*
*Parametre uint8_t sw2 : Bouton sw2 circuit_mannette
*Parametre uint8_t sw3 : Bouton sw3 circuit_mannette
*Parametre uint8_t y   : Joystick y cicuit_mannette
*
*/
	void driveurLanceur(uint8_t sw2,uint8_t sw3, uint8_t y)    {
		driverMoteurElevation(y);
		driverServoMoteur(sw3);
		driverMoteurRoueInertie(sw2);
	}
	
	
/*
* Driver Pour le moteur d elevation
* 
* Strategie : L objectif est d activer le moteur d elevation de la platforme
* de tir. Le courant fournie est constant et est activer par l axe Y du joystick. 
* il est utilise comme une switch ou son etaps max et min est utilise pour 
* represente un 1 ( MAX ou MIN ) et un 0 lorsque Y = 127. Finalement la position 
* y = 255 fait elever la platform et Y = 0 fait descendre la platforme
*
* Parametre uint8_t y   : Joystick y cicuit_mannette
*/	
	void driverMoteurElevation(uint8_t y){
		if(y == 255){
			PORTB = clear_bits(DDRB,0b10000000);
			PORTD = set_bit(DDRD,6)
			PORTB = set_bit(DDRB,0);
			pwm6_set_PB0(PUISSANCE_MAX);
			
			}
		else if ( y ==0){
			PORTD = clear_bit(DDRD,6)
			PORTB = set_bit(DDRB,0);
			pwm6_set_PB0(PUISSANCE_MAX);
			
			}	
		else {
			pwm6_set_PB0(PUISSANCE_MAX);
		}		
		
			DDRB = clear_bits(DDRB, 0b0000011);
		PORTB = set_bit(DDRB, 2);
	}
	
/*
* Driver pour le Servo Moteur de poucer des disques   
*/
	
	void driverServoMoteur(uint8_t bouton){
		
	}

/*
*
*/
	void driverMoteurRoueInertie(uint8_t bouton){
		
	}
