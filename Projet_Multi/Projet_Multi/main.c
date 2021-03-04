/*
 *OBJECTIF: Concevoir et réaliser un véhicule télécommandé pouvant lancer des petits
 *	disques sur des cibles ainsi que sa manette de contrôle dans un délai de 15 semaines. 
 *	Par la suite, vous ferez la démonstration des performances de votre prototype lors d’
 *	une compétition organisée durant la période des examens finaux. La suite du cahier des 
 *	charges décrit les conditions et contraintes auxquelles votre projet devra répondre.
 *
 * Projet_Multi.c
 *
 * Created: 3/4/2021 12:03:21 PM
 * Author : Jacob
 */ 

#include <avr/io.h>



/*          --_______________--
*      PD4 |  |             |  | PD6
*          |  |             |  |
*           --|             |--
*             |             |
*             |				|
*             |				|
*             |				|
*           --|             |--
*      PD5 |  |             |  | PD7
*          |  |_____________|  |
*           --               --
*
*/
int main(void)
{
    /* Replace with your application code */

		
	// Configuration des broche de sortie	
	pwm0_init();
	pwm1_init(100); // TOP = 100 DUTY = pourcentage d<activation
	pwm2_init(100); // TOP = 100 DUTY = pourcentage d<activation
	
	DDRA = clear_bit(DDRA, PINA2);// udeclarer entrer
	PORTA = set_bit(PORTA, PINA2); // definition utiliter
	DDRB = set_bits(DDRB, 0b00011111);
	
	//*******Uint******
	static uint8_t INTENSITE_1 = 25;
	static uint8_t INTENSITE_ARRET = 0;
	
	//*******BOOL******
	bool button_state; // bool ok
	bool direction = TRUE; 
	
	 

	while(1){
		button_state = read_bit (PINA, PINA2);
		if ( button_state == FALSE){
		pwm0_set_PB3(intensite);
		pwm0_set_PB4(INTENSITE_1);
		INTENSITE_1 = INTENSITE_1+10;
		intensite = intensite +10;
		_delay_ms(500);
		}
		if (button_state == TRUE){
		pwm0_set_PB3(0);
		pwm0_set_PB4(0);
		intensite = 0;
		intensite1 = 0;
		}
	}
    }
}

void set_roue_avant(uint8_t vitesse){ //vitesse sur 100
	pwm1_set_PD4(vitesse);
	pwm2_set_PD6(vitesse);
}

void set_roue_arriere(uint8_t vitesse){ //vitesse sur 100
	pwm1_set_PD5(vitesse);
	pwm2_set_PD7(vitesse);
}