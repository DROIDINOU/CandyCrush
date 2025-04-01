#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
// #include <windows.h> Verifier pourquoi j utilisait cela
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
    int choixUtilisateur;
    do
    {
        printf("%s: %d  ", MESSAGESECHANGEBONBONS[index].message, MESSAGESECHANGEBONBONS[index].nombreLigneOuColonne); // Affiche le message
        int result = scanf(" %d", &choixUtilisateur);                                                                  // Lit un seul caractère
        while (getchar() != '\n')
            ;

        // Vérifie que la réponse est valide
        if (result != 1)
        {
            printf("Entrée invalide. Veuillez entrer un entier.\n");
            choixUtilisateur = -1; // Force la boucle à se répéter
        }
    } while (choixUtilisateur < 1 || choixUtilisateur > TAILLE + 1); // Si la réponse n'est pas valide, on redemande

    return choixUtilisateur - 1; // retourne index programmeur
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

    Actions action = {DEPLACEMENT, {*coordonneeXPremierPion, *coordonneeYPremierPion}, {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
    Enfiler(q, &action);
}

/*____________________________________________________________________________________________________________________________
                                       **** FONCTION D'AFFICHAGE

-> Params : grille - queue
-> Afficher la grille des bonbons
-> Afficher la grille des gelatines
-> Place une action calcul dans la queue (les coordonnees sont à 0)
______________________________________________________________________________________________________________________________
*/

// void afficherRegleManche() {} PAS ENCORE PRET POUR CELA FAUT AMELIORER MODULARITER VERIFICATIONSET SUPPRESSIONS

// affiche la grille des bonbons et la grille de la gelatine
void afficherGrille(GrilleBonbons *grille, Queue *q)
{
    printf("ON EST DANS LA FONCTION AFFICHAGE GRILLE\n");
    // SI LA GRILLE EST EN PHASE INITIALISATION ET N EST PAS VERIFIEE -> affichage de chargemenbt
    if (!grille->estVerifiee)
    {
        printf(MESSAGEETATJEU[5]);
    }

    else
    {
        // affiche la grille des bonbons
        printf("Grille des Pions:\n");
        for (int ligne = 0; ligne < grille->lignes; ligne++)
        {
            for (int colonne = 0; colonne < grille->colonnes; colonne++)
            {
                printf(" %d ", grille->tableau[ligne][colonne].pion); // Affichage des cases avec bonbons
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
    }

    grille->affiche = 0;
    Actions actionAffichage = {CALCUL, {0, 0}, {0, 0}};
    Enfiler(q, &actionAffichage);

    // mettre grille-> affiche a 0 ici et pas dans deplacement?
}