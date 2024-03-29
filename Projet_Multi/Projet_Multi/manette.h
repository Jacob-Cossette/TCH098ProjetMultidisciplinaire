/*
 * manette.h
 *
 * Created: 2021-04-02 13:34:53
 *  Author: AR17930
 */ 


 
 //initialiser PD5, PD6, PD7 pour les trois switch
void sw_init();


 //Afficher //Lire la valeur de la position des potentiom�tres (joystick (x,y) && le potentiom�tre lin�aire (inclinaison))
void afficher_potentiometres(const uint8_t x, const uint8_t y, const uint8_t inclinaison);

//Lire le mode d'une switch de la manette et altern� entre deux modes � chaque clic
void lire_mode(uint8_t* mode, const uint8_t bit, uint8_t* etat1);


//Afficher le mode et l'�tat du moteur de lance_frisbee de la manette
void display_mode(const uint8_t mode, const uint8_t moteur );


//Fonction permettant de d�terminer le nombres de munitions restants et lire la SW3 pour faire un lancer
void lire_etat_lancer(uint8_t* lancer, uint8_t* etat, uint8_t* del);


//Test pour v�rifier l'affichage et le rafra�chissement
void display_heartbeat(void);