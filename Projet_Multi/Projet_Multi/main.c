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

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

void display_heartbeat(void) {
	static uint8_t heartbeat = 'Z';

	heartbeat = (heartbeat == 'Z') ? ('A') : (heartbeat + 1);

	lcd_set_cursor_position(15, 1);
	lcd_write_char(heartbeat);

}