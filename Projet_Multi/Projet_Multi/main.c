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

//*******Uint******
	static uint8_t INTENSITE_TOTAL_ROUE = 100;
	static uint8_t INTENSITE_ARRET = 0;
	static uint8_t INTENSITE_MIN_VIRAGE = 10;
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
	
	
	//*******BOOL******
	bool button_state; // bool ok
	bool direction = TRUE; 
	
	 

	while(1){
		button_state = read_bit (PINA, PINA2);
			if ( button_state == FALSE){
			set_all_wheel(INTENSITE_TOTAL_ROUE);
			_delay_ms(500);
			}
			if (button_state == TRUE){
			set_all_wheel(INTENSITE_ARRET);

			}
		}
    }
}


/*
* Methode: Setter pour les quatre moteur roue du v/hicule  
* Created: 3/4/2021 
* Author : Jacob
*/
void set_all_wheel(uint8_t vitesse){
	set_back_wheel(vitesse);
	set_front_wheel(vitesse);
}

/*
* Methode: Setter pour les 2 moteur roue avant du v/hicule
* Created: 3/4/2021 
* Author : Jacob
*/

void set_front_wheel(uint8_t vitesse){ //vitesse sur 100
	pwm1_set_PD4(vitesse);
	pwm2_set_PD6(vitesse);
}


/*
* Methode: Setter pour les 2 moteur roue arrier du v/hicule
* Created: 3/4/2021 
* Author : Jacob
*/

void set_back_wheel(uint8_t vitesse){ //vitesse sur 100
	pwm1_set_PD5(vitesse);
	pwm2_set_PD7(vitesse);
}


/*
* Methode: Setter pour un virage vers la gauche lorsque 
* le v/hicule est en mouvement.
* Created: 3/4/2021 
* Author : Jacob
*/

void set_virage_gauche_avant (uint8_t intensite){
	if (intensite >= 95){
		pwm1_set_PD4(INTENSITE_MIN_VIRAGE);
		pwm1_set_PD5(INTENSITE_MIN_VIRAGE);
		pwm2_set_PD6(intensite);
		pwm2_set_PD7(intensite);			
	}
	else{
		pwm1_set_PD4(INTENSITE_TOTAL_ROUE-intensite);		
		pwm1_set_PD5(INTENSITE_TOTAL_ROUE-intensite);
		pwm2_set_PD6(intensite);
		pwm2_set_PD7(intensite);
	}
}

/*
* Methode: Setter pour un virage vers la doite lorsque
* le v/hicule est en mouvement.
* Created: 3/4/2021 
* Author : Jacob
*/

void set_virage_droite_avant (uint8_t intensite){
	if (intensite >= 95){
		pwm1_set_PD4(intensite);
		pwm1_set_PD5(intensite);
		pwm2_set_PD6(INTENSITE_MIN_VIRAGE);
		pwm2_set_PD7(INTENSITE_MIN_VIRAGE);
	}
	else{
		pwm1_set_PD4(intensite);
		pwm1_set_PD5(intensite);
		pwm2_set_PD6(INTENSITE_TOTAL_ROUE-intensite);
		pwm2_set_PD7(INTENSITE_TOTAL_ROUE-intensite);
	}

/*
* Methode: Setter pour un virage vers la droite static
* 
* Created: 3/4/2021
* Author : Jacob
*/	
	
void set_virage_static_droite(uint8_t intensite){
			pwm1_set_PD4(intensite);
			pwm1_set_PD5(intensite);
			pwm2_set_PD6(-(intensite));
			pwm2_set_PD7(-(intensite));
	
	}

/*
* Methode: Setter pour un virage vers la gauche static
*
* Created: 3/4/2021
* Author : Jacob
*/

void set_virage_static_gauche(uint8_t intensite){
	pwm1_set_PD4(-(intensite))
	pwm1_set_PD5(-(intensite));
	pwm2_set_PD6(intensite);;
	pwm2_set_PD7(intensite);
	
	}	
	
}

