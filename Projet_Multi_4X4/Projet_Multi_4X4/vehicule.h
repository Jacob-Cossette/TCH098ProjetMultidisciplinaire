/*
 * vehicule.h
 *
 * Created: 2021-04-16 10:55:16
 * 
 */ 
#ifndef __EQUIPE43_VEHICULE__
#define __EQUIPE43_VEHICULE__

//Mode rotation
void driverRotation (uint8_t x, uint8_t z);

//Set puissance moteur
void setPuissanceMoteurRoue (uint8_t valeur);

//Variation de vitesse pour tourner vers la gauche
void setPuissance_gauche (uint8_t x, uint8_t z);

//Variation de vitesse pour tourner vers la droite
void setPuissance_droite (uint8_t x, uint8_t z);

//Variation de vitesse pour tourner (droite et gauche)
void setPuissance_tourner (uint8_t x, uint8_t z);

//Fonction pour déplacer le véhicule (tourner et reculer/avancer)
void driverDeplacement(uint8_t joystick, uint8_t x, uint8_t z);

/*Fonction Initialise les différents drivers utiliser dans la Fonction Lancer du
vehicule 4X4*/
void driverLanceur(uint8_t sw2,uint8_t sw3, uint8_t y);

//Driver Pour le moteur d elevation
void driverMoteurElevation(uint8_t y);

//Driver pour le Servo Moteur de poucer des disques
void driverServoMoteur(uint8_t bouton);

//Driver Roue Inertie
void driverMoteurRoueInertie(uint8_t bouton);

#endif