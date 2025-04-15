#include <stdio.h>
#include <stdlib.h>
#include "erreur.h"
#include "constante.h"

void GererErreurFatale(CodeErreur codeErreur)
{
    switch (codeErreur)
    {
    case ERREURQUEUEPLEINE:
        printf("la queue est pleine ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur
        break;

    case ERREURQUEUEVIDE:
        printf("la queue est vide ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur
        break;

    case TYPEINCONNU:
        printf("Type inconnu ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur

        break;

    case ECHECGENERATIONOBSTACLEALEATOIRE:
        printf("Erreur generation obstacle ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur

        break;

    case ECHECGENERATIONCOULEURALEATOIRE:
        printf("Erreur generation couleur ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur

        break;

    default:
        printf("Erreur inconnue ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur
        break;
    }
}

void GererErreurNonFatale(CodeErreur codeErreur)
{
    switch (codeErreur)
    {
    case ERREURDEPLACEMENT:
        printf(MESSAGEETATJEU[MESSAGE_PIONS_NON_ADJACENTS]); // Message d'erreur si les pions ne sont pas adjacents
        break;
    default:
        printf("Erreur inconnue ... \n");
        break;
    }
}

// Reste à définir les autres fonctions d'erreur
