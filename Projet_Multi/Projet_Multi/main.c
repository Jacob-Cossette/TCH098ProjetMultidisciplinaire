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


// affiche le message « DEL en fonction » sur l’écran LCD et éteint les DELs lorsque
// le bouton du « joystick » est relâché et affiche le message « DEL hors fonction ».

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "utils.h"
#include "lcd.h"

int main(void){
		
	bool button_state; // Variable qui contiendra l'état du bouton
	
	// Mettre la broche du bouton du joystick en entrée
	DDRA = clear_bit(DDRA, PINA2);
	
	// Activer la "pull-up" interne de la broche pour forcer un état haut
	// quand le bouton n'est pas enfoncé
	PORTA = set_bit(PORTA, PINA2);
	
	// Initialiser l'écran LCD
	lcd_init();
	
	// Dans une boucle infinie
	while(1){
		
	indicateurLCDLED(button_state);
		
	}
}





void indicateurLCDLED(bool b){
	
lcd_clear_display(); // Effacer l'écran LCD

	b = read_bit(PINA, PINA2); // Récupérer l'état du bouton

	// Si le bouton est enfoncé (il ne faut pas oublier qu'en appuyant
	// sur le bouton, on connecte la broche du microcontrôleur à la masse,
	// donc on génère un 0 (ou FALSE))
	if(b == FALSE){
		lcd_write_string("DELs en fonction"); // Écrire sur l'écran LCD
	}

	// Si le bouton n'est pas enfoncé
	else{
		lcd_write_string("DELs hors fonction"); // Écrire sur l'écran LCD
	}

	_delay_ms(1000); // Petit délai pour ralentir un peu la boucle
	printf('frame');
}


