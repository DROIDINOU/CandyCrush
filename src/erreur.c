#include <stdio.h>
#include <stdlib.h>
#include "erreur.h"
#include "constante.h"

// ****************************             GESTION DES ERREURS                    **************************** //

/****** Gestion des erreurs <système> */
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

    case ECHECGENERATIONOBSTACLEALEATOIRE:
        printf("Erreur generation obstacle ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur

        break;

    case ECHECGENERATIONCOULEURALEATOIRE:
        printf("Erreur generation couleur ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur

        break;

    case TYPEINCONNU:
        printf("Type inconnu ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur

        break;

    default:
        printf("Erreur inconnue ... \n");
        exit(EXIT_FAILURE); // Sortie du programme avec un code d'erreur
        break;
    }
}

/****** Gestion des autres erreurs */

void GererErreurNonFatale(CodeErreur codeErreur)
{
    switch (codeErreur)
    {
    case ERREURDEPLACEMENT:
        printf(MESSAGEETATJEU[MESSAGEPIONSNONADJACENTS]); // Message d'erreur si les pions ne sont pas adjacents
        break;
    case ERREURENTREEINVALIDE:
        printf("Entrée invalide. Veuillez entrer un entier.\n");
        break;
    case ERREURNOMBREENTREEINCORRECTE:
        printf("Erreur : veuillez entrer un nombre entre 1 et %d.\n", TAILLE);
        break;
    default:
        printf("Erreur inconnue ... \n");
        break;
    }
}