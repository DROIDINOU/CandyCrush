#ifndef ERREUR_H
#define ERREUR_H
#include "constante.h" // Pour les constantes définies dans constante.c

typedef enum
{
    ERREURQUEUEPLEINE, // 0 : Aucun problème
    ERREURQUEUEVIDE,   // 1 : Erreur liée à la mémoire
    ERREURDEPLACEMENT, // 2 : Erreur liée au déplacement des pions par l'utilisateur
} CodeErreur;

#endif
