
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"
// RESTE A FAIRE VERIFIER TOUS LES INCLUDE RESTE EST OK

/*
 ____________________________________________________________________________________________________________________________

                        **** FONCTIONS DE LECTURE DES ENTREES UTILISATEUR

    -> Fonctions : ObtenirReponseAuMessage - LireQuatreCoordonnees - LirePionsAChanger
    -> Parametres ObtenirReponseAuMessage  : int index
    -> Parametres LireQuatreCoordonnees : int *x1 - int *y1 - int *x2 - int *y2
    -> Parametres LirePionsAChanger : GrilleBonbons *grille - int *coordonneeXPremierPion -
                                     int *coordonneeYPremierPion - int *coordonneeXDeuxiemePion -
                                     int *coordonneeYDeuxiemePion - Queue *q
    -> ObtenirReponseAuMessage : Affiche le message et lit la reponse de l'utilisateur
    -> LireQuatreCoordonnees : Lit les coordonnees des pions a changer
    -> LirePionsAChanger : Lit les coordonnees des pions a changer et ajoute l'action DEPLACEMENT dans la queue
                           avec les coordonnees des bonbons
___________________________________________________________________________________________________________________________
*/

// Affiche un message et lit la réponse de l'utilisateur
// index est l'index du message dans le tableau MessagesReponses
int ObtenirReponseAuMessage(int index)
{
    int reponse;
    do
    {
        printf("%s: %d  ", MESSAGESREPONSES[index].message, MESSAGESREPONSES[index].reponseChiffre); // Affiche le message
        int result = scanf(" %d", &reponse);                                                         // Lit un seul caractère
        while (getchar() != '\n')
            ;

        // Vérifie que la réponse est valide
        if (result == -1)
        {
            // printf("Reponse invalide. Essayez encore.\n");
        }
    } while (reponse < 1 || reponse > TAILLE + 1); // Si la réponse n'est pas valide, on redemande

    return reponse - 1; // retourne index programmeur
}

// lit les coordonnees entrees par l utilisateur pour le swap de pions
void LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ObtenirReponseAuMessage(0);
    printf("x1 = %d\n", *x1);

    *y1 = ObtenirReponseAuMessage(1);
    printf("y1 = %d\n", *y1);

    *x2 = ObtenirReponseAuMessage(0);
    printf("x2 = %d\n", *x2);

    *y2 = ObtenirReponseAuMessage(1);
    printf("y2 = %d\n", *y2);
}

// lit les coordonnees des pions a changer et ajoute l'action DEPLACEMENT dans la queue
// avec les coordonnees des bonbons
void LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q)
{
    LireQuatreCoordonnees(coordonneeXPremierPion, coordonneeYPremierPion, coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);

    // ajouter action ici dans la queue;
    Actions action = {"DEPLACEMENT", {*coordonneeXPremierPion, *coordonneeYPremierPion}, {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
    Enfiler(q, &action);
}

/*____________________________________________________________________________________________________________________________
                                       **** FONCTION D'AFFICHAGE

-> Params : grille - queue
-> Afficher la grille des bonbons
-> Afficher la grille des gelatines
-> Place une action calcul dans la queue
______________________________________________________________________________________________________________________________
*/

// affiche la grille des bonbons et la grille de la gelatine
void afficher_grille(GrilleBonbons *grille, Queue *q)
{
    // affiche la grille des bonbons
    printf("Grille des Pions:\n");
    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            printf(" %c ", grille->tableau[ligne][colonne].pion); // Affichage des cases avec bonbons
        }
        printf("\n");
    }

    // affiche la grille des gelatines
    printf("Grille de la Gelatine:\n");
    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            if (grille->tableau[ligne][colonne].gelatine)
            {
                printf(" %c ", 'G'); // Affichage des cases avec gélatine
            }
            else
            {
                printf(" %c ", '.'); // Affichage des cases sans gélatine
            }
        }
        printf("\n");
    }
    Actions actionAffichage = {"CALCUL", {0, 0}, {0, 0}};
    Enfiler(q, &actionAffichage);
}