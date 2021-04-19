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
#include "vehicule.h"

void display_heartbeat();

//extern uint16_t clock;

int main(void){
	
	//*******Fonction******
	uart_init(UART_0);
	lcd_init();
	pwm0_init();
	pwm1_init(20000);
	pwm2_init();


	sei();
	
	//*******Uint******
	uint8_t x = 0;						// Position Joystick X
	uint8_t y = 1;						// Position Joystick Y
	uint8_t z = 0;						// Position potentiomètre
	uint8_t mode = 0;					// Variable selection des mondes 1: Déplacement 2: Tir SW1
	uint8_t moteurLanceur = 0;			// Activation du moteur de tir SW2
	uint8_t servoMoteurLanceur = 0;		// Activation du servo-moteur tir SW3
	uint8_t joystick = 0;				// Bouton JoyStick pour R et D
	
	
	//******Strings*****
	char message[40] = {'\0'};
	char str    [40] = {'\0'};
	
	
	
	
	DDRB = clear_bits(DDRB, 0b0001111);
	DDRB = set_bits(DDRB, 0b0001111);

	//DDRD = clear_bits(DDRD, 0b1111111);
	//DDRD = set_bits(DDRD, 0b1111111);
	//Test pour vérifier l'affichage et le rafraîchissement
	
	
	
	while (1)
	{
		
		// Lecture des données reçues par le véhicule
		if (!uart_is_rx_buffer_empty(UART_0)){

				lcd_clear_display();
				_delay_ms(20);
				uart_get_string(UART_0,message,151);

				sscanf(message,  "X%03dY%03dZ%03dm%dM%dL%dD%d", &x, &y, &z, &mode, &moteurLanceur, &servoMoteurLanceur, &joystick);
				//sprintf(str, "%s", message);
				
				sprintf(str, "X%03dY%03dZ%03dm%dM%dL%dD%d", x, y, z, mode, moteurLanceur, servoMoteurLanceur, joystick);
				lcd_set_cursor_position(0,0);

				lcd_write_string(str);
						
				//Mode rotation (le véhicule tourne sur lui-même par la gauche ou par la droite)
				if(mode == 1){
					setRotation(x,z);	
					driverLanceur(moteurLanceur,servoMoteurLanceur,y);	
                    pwm1_set_PD5(1250);		
				}					
				//Mode déplacement (le véhicule peut avancer, reculer, tourner)
				else if(mode == 0){
					setDeplacement(joystick, x, z);	
					driverLanceur(moteurLanceur,servoMoteurLanceur,y);
                    pwm1_set_PD5(2300);	
					}
				//Si problème alors le véhicule arrête de bouger
				else {
					setPuissanceMoteurRoue(0);
				}
		}	
	display_heartbeat();		
	}
}

	
