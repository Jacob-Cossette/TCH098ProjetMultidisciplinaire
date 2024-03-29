/*
	 __ ___  __
	|_   |  (_
	|__  |  __)

	MIT License

	Copyright (c) 2018	Ã‰cole de technologie supÃ©rieure

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify and/or merge copies of the Software, and to permit persons
	to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
*/
/**
	\file
	\brief Code source de fonctions qui pilotent directement du matÃ©riel

	\author Noms de membres de l'Ã©quipe
	\author idem
	\author idem
	\author idem
	\author idem
	\date date d'aujourd'hui
*/

/* ----------------------------------------------------------------------------
Includes
---------------------------------------------------------------------------- */

#include <avr/io.h>
#include "driver.h"


/* ----------------------------------------------------------------------------
Function definition
---------------------------------------------------------------------------- */

void adc_init(void){

	// 1-Configuration des broches du port A Ã  mettre en entrÃ©e
	DDRA = clear_bit(DDRA, PA0);  //DÃ©commenter pour utiliser
	DDRA = clear_bit(DDRA, PA1);  //DÃ©commenter pour utiliser
	DDRA = clear_bit(DDRA, PA2); //DÃ©commenter pour utiliser
	DDRA = clear_bit(DDRA, PA3); //DÃ©commenter pour utiliser
	DDRA = clear_bit(DDRA, PA4); //DÃ©commenter pour utiliser
	
	
	// 2-SÃ©lectionner la rÃ©fÃ©rence de tension: la tension d'alimentation
	ADMUX = clear_bit(ADMUX, REFS1);
	ADMUX = set_bit(ADMUX, REFS0);

	// 3-Choisir le format du rÃ©sultat de conversion: shift a gauche pour que
	// les 8 MSB se retrouvent dans le registre ADCH (ADLAR=1)
	ADMUX = set_bit(ADMUX, ADLAR);

	// 4-Choisir le facteur de division de l'horloge
	// ( L'horloge l'ADC ne doit pas dÃ©passer 200kHz. Avec une horloge de 8MHZ, Ã§a
	// prend une division d'horloge de min 40. Donc 64 ou 128) */
	ADCSRA = set_bit(ADCSRA, ADPS2);
	ADCSRA = set_bit(ADCSRA, ADPS1);
	ADCSRA = set_bit(ADCSRA, ADPS0);
	
	// 5-Activer le CAN
	ADCSRA = set_bit(ADCSRA, ADEN);
}

uint8_t adc_read(uint8_t canal){

// 1-SÃ©lection de l'entrÃ©e Ã  convertir (canal)
ADMUX = write_bits(ADMUX, 0b00000111, canal);

// 2-DÃ©marrage d'une conversion
ADCSRA = set_bit(ADCSRA, ADSC);

// 3-Attente de la fin de conversion
//TODO : Avec une boucle, regarder le bit ASC tant que ce n'es aps terminÃ©

uint8_t val=1;
while(val==1){
	val = read_bit(ADCSRA, ADSC);
}

// 4-Lecture et renvoi du rÃ©sultat
return ADCH;
	
}

void pwm0_init(void){

	// 1-Configuration des broches de sortie (PB4 et PB3)

	
	// 1.1-Mettre les broches de la modulation de largeur d'impulsion en sortie
//TODO_FB : Active PB1 et PB2 en sortie
	DDRB = set_bit(DDRB,PB4); // pb4 = 1
	DDRB = set_bit(DDRB,PB3); // pb3 = 1
	
	// PEUT ETRE AUTRE CHOSE 
	// 2-Initialisation du TIMER 0
	// 2.1- Mode de comparaison : "Toggle on compare match"

	
	TCCR0A = set_bit(TCCR0A,COM0A1);
	TCCR0A = set_bit(TCCR0A,COM0B1);
	TCCR0A = clear_bit(TCCR0A,COM0A0);
	TCCR0A = clear_bit(TCCR0A,COM0B0);
	
	
	
	// 2.2- Mode du compteur :  "PWM phase correct"
	
	TCCR0A = set_bit(TCCR0A,WGM00);
	TCCR0A = clear_bit(TCCR0A,WGM01);
	
	// 2.3- Fixer la valeur initiale du compteur 0 Ã  0
	
	 TCNT0 = 0;
	
	// 2.4- Facteur de division de frÃ©quence : 1
	
	TCCR0B = set_bit(TCCR0B,CS00);
	TCCR0B = clear_bit(TCCR0B,CS01);
	TCCR0B = clear_bit(TCCR0B,CS02);
}

void pwm0_set_PB3(uint8_t duty){

	// Fixer le rapport cyclique Ã  la valeur de duty
	OCR0A = duty; 
	
}


void pwm0_set_PB4(uint8_t duty){

	// Fixer le rapport cyclique Ã  la valeur de duty
	OCR0B = duty;
}

void pwm1_init(uint16_t top){	
	
	// broches de PWM en sortie
	DDRD = set_bit(DDRD, PD4);
	DDRD = set_bit(DDRD, PD5);
	
	//Clear OCnA/OCnB on Compare Match, set OCnA/OCnB at BOTTOM (non-inverting mode)
	TCCR1A=set_bit(TCCR1A,COM1A1);
	TCCR1A=clear_bit(TCCR1A,COM1A0);
	TCCR1A=set_bit(TCCR1A,COM1B1);
	TCCR1A=clear_bit(TCCR1A,COM1B0);
	
	// Fast PWM mode (avec valeur TOP fixÃ© par ICR1)
	TCCR1B=set_bit(TCCR1B,WGM13);
	TCCR1B=set_bit(TCCR1B,WGM12);
	TCCR1A=set_bit(TCCR1A,WGM11);
	TCCR1A=clear_bit(TCCR1A,WGM10);

	// valeur initiale du compteur Ã  0
	TCNT1=0;

	// fixer la valeur maximale (TOP) du compteur 2	
	ICR1 = top;			
	
	// activer l'horloge avec facteur de division par 8
	TCCR1B=clear_bit(TCCR1B,CS12);
	TCCR1B=set_bit(TCCR1B,CS11);
	TCCR1B=clear_bit(TCCR1B,CS10);
}

void pwm1_set_PD5(uint16_t limit){ 
	OCR1A = limit;
}
void pwm1_set_PD4(uint16_t limit){
	OCR1B = limit;
}

void pwm2_init(){
	// broches de PWM en sortie
//TODO_FB : Active PB0 en sortie
	DDRD = set_bit(DDRD, PD6);
	DDRD = set_bit(DDRD, PD7);
	
	//Set OC2A/B on Compare Match when up-counting. Clear OC2A on	Compare Match when down-counting.
	
	// PEUT ETRE AUTRE CHOSE
	// 2-Initialisation du TIMER 0
	// 2.1- Mode de comparaison : "Toggle on compare match"

	
	TCCR2A = set_bit(TCCR2A,COM0A1);
	TCCR2A = set_bit(TCCR2A,COM0B1);
	TCCR2A = clear_bit(TCCR2A,COM0A0);
	TCCR2A = clear_bit(TCCR2A,COM0B0);
	
	
	
	// 2.2- Mode du compteur :  "PWM phase correct"
	
	TCCR2A = set_bit(TCCR2A,WGM00);
	TCCR2A = clear_bit(TCCR2A,WGM01);
	
	// 2.3- Fixer la valeur initiale du compteur 0 Ã  0
	
	TCNT2 = 0;
	
	// 2.4- Facteur de division de frÃ©quence : 1
	
	TCCR2B = set_bit(TCCR2B,CS00);
	TCCR2B = clear_bit(TCCR2B,CS01);
	TCCR2B = clear_bit(TCCR2B,CS02);
}
	


void pwm2_set_PD7(uint8_t limit){
	// Fixer le rapport cyclique Ã  la valeur de duty
	OCR2A = limit;	
}

void pwm2_set_PD6(uint8_t limit){
	// Fixer le rapport cyclique Ã  la valeur de duty
	OCR2B = limit;
}

