#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"

/*____________________________________________________________________________________________________________________________


-> Params : grille - coordonnées pions (2) - queue
-> Lire coordonnées pions choisis par utilisateur
-> *** PLACER ACTION DEPLACEMENT DANS LA QUEUE avec les coordonnées des pions
______________________________________________________________________________________________________________________________
*/
void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion,
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion,
                       int coordonneeYDeuxiemePion, Queue *q)
{
    // ajouter action ici dans la queue;
    printf("Deplacement de (%d,%d) vers (%d,%d)\n", coordonneeXPremierPion, coordonneeYPremierPion, coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
    Actions action = {"DEPLACEMENT", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    enfiler(q, action);
}

/*____________________________________________________________________________________________________________________________
-> Params : grille - queue
-> Afficher la grille des bonbons
-> Afficher la grille des gelatines
-> **** PLACER ACTION CALCUL DANS LA QUEUE
______________________________________________________________________________________________________________________________
*/

void afficher_grille(GrilleBonbons *grille, Queue *q)
{
    printf("Grille des Pions:\n");
    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            printf(" %c ", grille->tableau[ligne][colonne].pion); // Affichage des cases avec bonbons
        }
        printf("\n");
    }

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
    enfiler(q, actionAffichage);
}