/*
 * CFile1.c
 *
 * Created: 2021-04-16 10:54:56
 * 
 *
 *
 * Projet_Multi_4X4.c
 *
 * Created: 4/15/2021 4:47:15 PM
 * Author : Jing Tong Chen et Jacob Cossette
 */ 

// Programme de embarquée:
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>    		 // sprintf
#include <util/delay.h>
#include "lcd.h"
#include "utils.h"			// set_bit
#include "driver.h"
#include "uart.h"

#include "vehicule.h"

//****Constante*****
const static uint8_t PUISSANCE_MAX = 255;
const static uint8_t PUISSANCE_MOTEUR_ROUE_INERTIE = 200;
const static uint8_t PUISSANCE_MOTEUR_ELEVATION = 130;
const static uint8_t DELAIS_DEPART_ROUE_INERTIE = 20;
const static uint8_t RESET_CLOCK = 0;

//*******int********
static uint16_t clock2 = 0;
static uint16_t clock = 0;




//mode rotation
void setRotation (uint8_t x, uint8_t z){
			if (x >= 200){
				DDRB = clear_bits(DDRB, 0b0000110);
				PORTB = set_bit(DDRB, 2);
				setPuissanceMoteurRoue(z);
			}
			else if (x <= 100){
				DDRB = clear_bits(DDRB, 0b0000110);
				PORTB = set_bit(DDRB, 1);
				setPuissanceMoteurRoue(z);
			}
			
			else{
				setPuissanceMoteurRoue(0);	
			}
			
}



//Set puissance des roues
void setPuissanceMoteurRoue (uint8_t valeur){
	pwm0_set_PB3(valeur);
	pwm0_set_PB4(valeur);
}



//Variation de vitesse pour tourner vers la gauche
void setPuissance_gauche (uint8_t x, uint8_t z){
	double rapport = 1;
	double vitesse = z;

	char str1 [40] = {'\0'};

	rapport = 0.0056*(double)x + 0.2;
	vitesse = (int)(rapport * z);
	pwm0_set_PB3(vitesse);
	pwm0_set_PB4(z);
				
	sprintf(str1, "%03f", vitesse);
	lcd_set_cursor_position(10,1);
	lcd_write_string(str1);
}


//Variation de vitesse pour tourner vers la droite
void setPuissance_droite (uint8_t x, uint8_t z){
	double rapport = 1;
	double vitesse = z;
	
	char str1 [40] = {'\0'};

	rapport = -0.0056*(double)x + 1.728;
	vitesse =  (int)(rapport * z);
	pwm0_set_PB3(z);
	pwm0_set_PB4(vitesse);
	
	sprintf(str1, "%03f", vitesse);
	lcd_set_cursor_position(10,1);
	lcd_write_string(str1);
}



//Variation de vitesse pour tourner (droite et gauche)
void setPuissance_tourner (uint8_t x, uint8_t z){
	if (x <= 125){
		setPuissance_gauche (x,z);
	}
	else if (x >= 130){
		setPuissance_droite (x,z);
	}
	else {
		setPuissanceMoteurRoue(z);
	}
}



//fonction pour déplacer le véhicule (tourner et reculer/avancer)
void setDeplacement(uint8_t joystick, uint8_t x, uint8_t z){
		if (joystick == 1){
			PORTB = clear_bits(DDRB, 0b0000110);
			setPuissance_tourner (x,z);
		}
		else if(joystick == 0){
			PORTB = set_bits(DDRB, 0b0000110);
			setPuissance_tourner (x,z);
		}
				
		else{
			setPuissanceMoteurRoue(0);
		}
}



//Test 
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
	* Driver Pour le moteur d elevation
	*
	* Strategie : L objectif est d activer le moteur d elevation de la platforme
	* de tir. Le courant fournie est constant et est activer par l axe Y du joystick.
	* il est utilise comme une switch ou son etaps max et min est utilise pour
	* represente un 1 ( MAX ou MIN ) et un 0 lorsque Y = 127. Finalement la position
	* y = 255 fait elever la platform et Y = 0 fait descendre la platforme.
	*
	* Parametre uint8_t y   : Joystick y cicuit_mannette
	*/

	void driverMoteurElevation(uint8_t y){
		lcd_set_cursor_position(8, 1);		
		if(y == 255){ 		
			PORTB = clear_bit(PORTB, 0);
			pwm2_set_PD6(255);		
		}
		else if ( y ==0){		
			PORTB = set_bit(PORTB, 0);
			pwm2_set_PD6(255);	
		}
		else {		
			pwm2_set_PD6(0);
		}
	}
	
	/*
	* Driver pour le Servo Moteur de poucer des disques
	*
	* Strategie : Controle du servomoteur, Active une sequence du de tir
	* du disque courant. la methode doit mettre en place une securite
	* qui empeche l utilisateur d effectuer une demande de tirs tant que
	* le tirs n est pas effectue et que la roue d inertie n est pas a
	* vitesse nominal.
	*
	* interogation : est-il possible de mettre un decompte sur la mannette
	* ou  de continuer a utiliser le 4X4 durant le temps d attente.
	*
	* Parametre uint8_t sw3 : Bouton sw3 circuit_mannette
	* init servo : 2300 
    * init servo : 
	*/
	
	void driverServoMoteur(uint8_t bouton){
	/*if (bouton)
	{
		pwm1_set_PD5(9999);
		pwm1_set_PD5(9999);
	    pwm1_set_PD5(9999);
	}
	else
		pwm1_set_PD5(2300);	*/
	}

	/*
	*
	*/
	void driverMoteurRoueInertie(uint8_t bouton){

		if (bouton){
			if(clock2 < DELAIS_DEPART_ROUE_INERTIE){
			pwm2_set_PD7(PUISSANCE_MAX);
			clock2++;
			}
			else if(clock2 >= DELAIS_DEPART_ROUE_INERTIE){
			pwm2_set_PD7(PUISSANCE_MOTEUR_ROUE_INERTIE);
			clock++;
			}
		}
		else {
			pwm2_set_PD7(0);
			clock = RESET_CLOCK;
			clock2 = RESET_CLOCK;
			
		}
	}

	/*
	* Fonction Initialise les différents drivers utiliser dans la Fonction Lancer du
	* vehicule 4X4
	*
	* Fait appel au driver du moetru RI Servo Moteur et moteur d elevation.
	*
	*Parametre uint8_t sw2 : Bouton sw2 circuit_mannette
	*Parametre uint8_t sw3 : Bouton sw3 circuit_mannette
	*Parametre uint8_t y   : Joystick y cicuit_mannette
	*
	*/
	void driverLanceur(uint8_t sw2,uint8_t sw3, uint8_t y){

		driverMoteurElevation(y);
		driverServoMoteur(sw3);
		driverMoteurRoueInertie(sw2);
	}