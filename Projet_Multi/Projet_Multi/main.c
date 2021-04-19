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

#include "manette.h"		//nouveau module créé


int main(void)
{
	//Déclaration des variables

	//Variables envoyées au véhicule
	uint8_t x;					//joystick en x
	uint8_t y;					//joystick en y
	uint8_t inclinaison;		//potentiomètre linéaire

	uint8_t mode = 0;			//(0 = DÉPLACEMENT, 1 = ROTATION)
	uint8_t moteur = 0;			//(1 = en marche, 1 = fermé)
	uint8_t lancer = 0;			//(1 = lancer munition, 0 = ne pas lancer munition)
	uint8_t joystick = 0;		//(0 = avancer, 1= reculer)
	
	//Variables intermédiaires servant aux calculs 
	uint8_t etat_mode = 0;
	uint8_t etat_moteur = 0;
	uint8_t etat_lance = 0;
	uint8_t etat_joystick = 0;
	
	
	//uint8_t del = 0b00011111; //Nombres de munitions (Max = 5 && 0b00011111 => 5)

	char str[40]; // Message envoyé au véhicule
	
	
	//initialisation
	lcd_init();
	adc_init();
	uart_init(UART_0);
	
	sw_init();
	
	//DDRB=set_bits(DDRB,0b00011111);
	//PORTB=set_bits(PORTB,del);


	sei();

	//Début de l'exécution
	while(1){
		
		lcd_clear_display();
		
		//Déterminer si véhicule est en mode DÉPLACEMENT ou ROTATION avec la SW1
		lire_mode(&mode, PD7, &etat_mode);
		
		//Déterminer si le moteur du frisbee est activé
		//Le moteur du lance frisbee ne peut être activé s'il n'y a plus de munitions
		//if (del){
			lire_mode(&moteur, PD6, &etat_moteur);
		//}
		//else{
			//moteur = 0;
			//etat_moteur = 0;
		//}
		
		lire_mode_joystick(&joystick, PA2, &etat_joystick);

		
		//joystick = read_bit(PINA, PA2);
		
		//Afficher sur LCD l'état du moteur de lance-frisbee et le mode (DÉPLACEMENT ou ROTATION)
		display_mode(mode, moteur, joystick);
		
		//Test pour vérifier l'affichage et le rafraîchissement
		display_heartbeat();
		
		//Lire et afficher l'inclinaison sur LCD
		lire_potentiometres(&x, &y, &inclinaison);
		afficher_potentiometres(x, y, inclinaison);
		
		//Lancer munition si le moteur du lance-frisbee est activé
		if (moteur){
			lire_etat_lancer(&lancer, &etat_lance);

			//Afficher le nombre de munitions sur DEL
			//PORTB=clear_bits(PORTB,0b00011111);
			//PORTB=set_bits(PORTB,del);
		}
		
		//Message envoyé au véhicule en string
		sprintf(str, "X%03dY%03dZ%03dm%dM%dL%dD%d\0", x, y, inclinaison, mode, moteur, lancer, joystick);
		

		uart_put_string(UART_0,str);
		
		//lcd_set_cursor_position(0,0);
		//lcd_write_string(str);
		
		_delay_ms(100);
		
		

	}
	
	
}


#endif