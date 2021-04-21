/*
 * main.c
 *
 * Created: 2021-02-04 15:34:18
 * Author : Jing Tong Chen
 */ 



#if(1)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>    		 // sprintf
#include <util/delay.h>
#include "lcd.h"
#include "utils.h"			// set_bit
#include "driver.h"
#include "uart.h"

#include "manette.h"		//nouveau module cr��


int main(void)
{
	//D�claration des variables

	//Variables envoy�es au v�hicule
	uint8_t x;					//joystick en x
	uint8_t y;					//joystick en y
	uint8_t inclinaison;		//potentiom�tre lin�aire

	uint8_t mode = 0;			//(0 = D�PLACEMENT, 1 = ROTATION)
	uint8_t moteur = 0;			//(1 = en marche, 1 = ferm�)
	uint8_t lancer = 0;			//(1 = lancer munition, 0 = ne pas lancer munition)
	uint8_t joystick = 0;		//(0 = avancer, 1= reculer)
	
	//Variables interm�diaires servant aux calculs 
	uint8_t etat_mode = 0;
	uint8_t etat_moteur = 0;
	uint8_t etat_lance = 0;
	uint8_t etat_joystick = 0;
	
	
	//uint8_t del = 0b00011111; //Nombres de munitions (Max = 5 && 0b00011111 => 5)

	char str[40]; // Message envoy� au v�hicule
	
	
	//initialisation
	lcd_init();
	adc_init();
	uart_init(UART_0);
	
	sw_init();
	
	//DDRB=set_bits(DDRB,0b00011111);
	//PORTB=set_bits(PORTB,del);


	sei();

	//D�but de l'ex�cution
	while(1){
		
		lcd_clear_display();
		
		//D�terminer si v�hicule est en mode D�PLACEMENT ou ROTATION avec la SW1
		lire_mode(&mode, PD7, &etat_mode);
		
		//D�terminer si le moteur du frisbee est activ�
		//Le moteur du lance frisbee ne peut �tre activ� s'il n'y a plus de munitions
		//if (del){
			lire_mode(&moteur, PD6, &etat_moteur);
		//}
		//else{
			//moteur = 0;
			//etat_moteur = 0;
		//}
		
		lire_mode_joystick(&joystick, PA2, &etat_joystick);

		
		//joystick = read_bit(PINA, PA2);
		
		//Afficher sur LCD l'�tat du moteur de lance-frisbee et le mode (D�PLACEMENT ou ROTATION)
		display_mode(mode, moteur, joystick);
		
		//Test pour v�rifier l'affichage et le rafra�chissement
		display_heartbeat();
		
		//Lire et afficher l'inclinaison sur LCD
		lire_potentiometres(&x, &y, &inclinaison);
		afficher_potentiometres(x, y, inclinaison);
		
		//Lancer munition si le moteur du lance-frisbee est activ�
		if (moteur){
			lire_etat_lancer(&lancer, &etat_lance);

			//Afficher le nombre de munitions sur DEL
			//PORTB=clear_bits(PORTB,0b00011111);
			//PORTB=set_bits(PORTB,del);
		}
		
		//Message envoy� au v�hicule en string
		sprintf(str, "X%03dY%03dZ%03dm%dM%dL%dD%d\0", x, y, inclinaison, mode, moteur, lancer, joystick);
		

		uart_put_string(UART_0,str);
		
		//lcd_set_cursor_position(0,0);
		//lcd_write_string(str);
		
		_delay_ms(100);
		
		

	}
	
	
}

void display_heartbeat(void) {
	static uint8_t heartbeat = 'Z';

#endif