/*
 * manette.c
 *
 * Created: 2021-04-02 13:35:10
 *  Author: Jing Tong Chen
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>    		 // sprintf
#include <util/delay.h>
#include "lcd.h"
#include "utils.h"			// set_bit
#include "driver.h"
#include "uart.h"




//initialiser PD5, PD6, PD7 pour SW3,2,1
void sw_init() {
	// PD7 entrée pour SW1
	DDRD = clear_bit(DDRD, PD7);
	PORTD = set_bit(PORTD, PD7);

	// PD6 entrée pour SW2
	DDRD = clear_bit(DDRD, PD6);
	PORTD = set_bit(PORTD, PD6);

	// PD5 entrée pour SW3
	DDRD = clear_bit(DDRD, PD5);
	PORTD = set_bit(PORTD, PD5);
}


//Lire la valeur de la position des potentiomètres
void lire_potentiometres(uint8_t*x, uint8_t*y, uint8_t*inclinaison){

	*x = adc_read(0);
	*y = adc_read(1);
	
	*inclinaison = 255-adc_read(3);  //0 => bas, 255 => en haut
	
}


//Afficher la valeur de la position des potentimètres sur LCD
void afficher_potentiometres(const uint8_t x, const uint8_t y, const uint8_t inclinaison) {
	char str[40];

	sprintf(str, "X%03d Y%03d Z%03d\n", x, y, inclinaison);

	lcd_set_cursor_position(0, 1); //2e ligne
	lcd_write_string(str);

}




//Lire le mode d'une switch de la manette et alterné entre deux modes à chaque clic
void lire_mode (uint8_t*mode, const uint8_t bit, uint8_t*etat){
	uint8_t b = read_bit(PIND, bit);

	//La switch n'est pas influencé par le temps d'appui
	if (*etat != b) {	
			*etat = b;
			if(*mode==0 && *etat==0){
				*mode=1;
			}
			else if (*mode==1 && *etat==0){
				*mode=0;
			}
	}
}



//Afficher le mode et l'état du moteur de lance_frisbee de la manette
void display_mode(const uint8_t mode, const uint8_t moteur) {
	char str[40];

	//Affichage mode du véhicule
	if (mode){
		sprintf(str, "2 TURN\n");
	}
	else{
			sprintf(str, "1 MOVE\n");
	}
	lcd_set_cursor_position(0,0);
	lcd_write_string(str);
	
	//Affichage état moteur
	if (moteur == 1){
			lcd_set_cursor_position(10,0);
			lcd_write_string("PRET\n");
	}
}


//Fonction permettant de déterminer le nombres de munitions restants et lire la SW3 pour faire un lancer 
void lire_etat_lancer(uint8_t* lancer, uint8_t* etat, uint8_t* del) {
	uint8_t b = !read_bit(PIND, PA5);

	//Si la SW est activée
	if (*etat == 0 && b == 1) {
		*lancer = 1;
		*del /= 2; //le nombre de del allumé indique le nombre de munitions qui reste
	}

	//Si la SW n'est pas activée
	else {
		*lancer = 0;
	}
	*etat = b;
}



//Test pour vérifier l'affichage et le rafraîchissement
void display_heartbeat(void) {
	static uint8_t heartbeat = 'Z';

	heartbeat = (heartbeat == 'Z') ? ('A') : (heartbeat + 1);

	lcd_set_cursor_position(15, 1);
	lcd_write_char(heartbeat);
	//uart_put_byte(UART_0, heartbeat);

	
}

