/*
 *OBJECTIF: Concevoir et r�aliser un v�hicule t�l�command� pouvant lancer des petits
 *	disques sur des cibles ainsi que sa manette de contr�le dans un d�lai de 15 semaines. 
 *	Par la suite, vous ferez la d�monstration des performances de votre prototype lors d�
 *	une comp�tition organis�e durant la p�riode des examens finaux. La suite du cahier des 
 *	charges d�crit les conditions et contraintes auxquelles votre projet devra r�pondre.
 *
 * Projet_Multi.c
 *
 * Created: 3/4/2021 12:03:21 PM
 * Author : Jacob
 */ 


// affiche le message � DEL en fonction � sur l��cran LCD et �teint les DELs lorsque
// le bouton du � joystick � est rel�ch� et affiche le message � DEL hors fonction �.

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "utils.h"
#include "lcd.h"

int main(void){
		
	bool button_state; // Variable qui contiendra l'�tat du bouton
	
	// Mettre la broche du bouton du joystick en entr�e
	DDRA = clear_bit(DDRA, PINA2);
	
	// Activer la "pull-up" interne de la broche pour forcer un �tat haut
	// quand le bouton n'est pas enfonc�
	PORTA = set_bit(PORTA, PINA2);
	
	// Initialiser l'�cran LCD
	lcd_init();
	
	// Dans une boucle infinie
	while(1){
		
	indicateurLCDLED(button_state);
		
	}
}





void indicateurLCDLED(bool b){
	
lcd_clear_display(); // Effacer l'�cran LCD

	b = read_bit(PINA, PINA2); // R�cup�rer l'�tat du bouton

	// Si le bouton est enfonc� (il ne faut pas oublier qu'en appuyant
	// sur le bouton, on connecte la broche du microcontr�leur � la masse,
	// donc on g�n�re un 0 (ou FALSE))
	if(b == FALSE){
		lcd_write_string("DELs en fonction"); // �crire sur l'�cran LCD
	}

	// Si le bouton n'est pas enfonc�
	else{
		lcd_write_string("DELs hors fonction"); // �crire sur l'�cran LCD
	}

	_delay_ms(1000); // Petit d�lai pour ralentir un peu la boucle
	printf('frame');
}


