#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
// #include <windows.h> Verifier pourquoi j utilisait cela
#include <locale.h>
#include "matrice.h"
#include "constante.h"
#include "erreur.h"

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

bool EstPionAdjacent(int x1, int y1, int x2, int y2)
{
    // Vérifie si les pions sont adjacents (horizontalement ou verticalement)
    return (x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1);
}

// lit les coordonnees entrees par l utilisateur pour le swap de pions
bool LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ObtenirReponseAuMessage(0);
    printf("x1 = %d\n", *x1);

    *y1 = ObtenirReponseAuMessage(1);
    printf("y1 = %d\n", *y1);

    *x2 = ObtenirReponseAuMessage(0);
    printf("x2 = %d\n", *x2);

    *y2 = ObtenirReponseAuMessage(1);
    printf("y2 = %d\n", *y2);

    return EstPionAdjacent(*x1, *y1, *x2, *y2); // Vérifie si les pions sont adjacents
}

// lit les coordonnees des pions a changer et ajoute l'action DEPLACEMENT dans la queue
// avec les coordonnees des bonbons
void LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q)
{
    if (LireQuatreCoordonnees(coordonneeXPremierPion, coordonneeYPremierPion, coordonneeXDeuxiemePion, coordonneeYDeuxiemePion))
    {
        printf("Coordonnees valides : (%d,%d) et (%d,%d)\n", *coordonneeXPremierPion, *coordonneeYPremierPion, *coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion);
        Actions action = {DEPLACEMENT, {*coordonneeXPremierPion, *coordonneeYPremierPion}, {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
        Enfiler(q, &action);
    }
    else
    {
        printf("Coordonnees invalides : (%d,%d) et (%d,%d)\n", *coordonneeXPremierPion, *coordonneeYPremierPion, *coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion);
        GererErreurNonFatale(ERREURDEPLACEMENT);
        Actions action = {LECTURE, {*coordonneeXPremierPion, *coordonneeYPremierPion}, {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
        Enfiler(q, &action); // Si les coordonnées ne sont pas valides, on gère l'erreur
    }
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
// Afficher la grille
// Afficher la grille
// Afficher la grille
void afficherGrille(GrilleBonbons *grille, Queue *q)
{
    printf("ON EST DANS LA FONCTION AFFICHAGE GRILLE\n");

    // Si la grille n'est pas encore vérifiée
    if (!grille->estVerifiee)
    {
        printf(MESSAGEETATJEU[MESSAGE_CHARGEMENT]);
    }
    else
    {
        // Affichage de la première ligne (numéros de colonnes)
        printf("   ");
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            if (colonne + 1 < 10)  // Si c'est un chiffre à un seul chiffre
                printf("  %d", colonne + 1);  // Un espace avant le chiffre
            else  // Si c'est un chiffre à deux chiffres
                printf(" %d", colonne + 1);  // Pas d'espace nécessaire pour les deux chiffres
        }
        printf("\n");

        // Affichage de la grille avec les numéros de lignes
        for (int ligne = 0; ligne < grille->lignes; ligne++)
        {
            // Afficher le numéro de la ligne (première colonne)
            printf("%2d ", ligne + 1); // Affiche les numéros de ligne au début de chaque ligne

            for (int colonne = 0; colonne < grille->colonnes; colonne++)
            {
                int pion = grille->tableau[ligne][colonne].pion;   // Le bonbon
                bool gelatine = grille->tableau[ligne][colonne].gelatine; // La gélatine

                // Si la case contient de la gélatine
                if (gelatine)
                {
                    // Affichage avec fond gris clair et couleur principale pour le bonbon
                    switch ((CouleurBonbons)pion)
                    {
                        case JAUNE: 
                            printf("\033[48;5;235m\033[38;5;226m%2d \033[0m", pion); break; // Gélatine sur fond Jaune Clair avec gris
                        case VERT: 
                            printf("\033[48;5;235m\033[38;5;82m%2d \033[0m", pion); break;   // Gélatine sur fond Vert avec gris
                        case BLEU: 
                            printf("\033[48;5;235m\033[38;5;39m%2d \033[0m", pion); break;   // Gélatine sur fond Bleu avec gris
                        case ROUGE: 
                            printf("\033[48;5;235m\033[38;5;160m%2d \033[0m", pion); break;   // Gélatine sur fond Rouge avec gris
                        case MAUVE: 
                            printf("\033[48;5;235m\033[38;5;93m%2d \033[0m", pion); break; // Gélatine sur fond Mauve avec gris
                        case BLANC: 
                            printf("\033[48;5;235m\033[38;5;15m%2d \033[0m", pion); break;  // Gélatine sur fond Blanc avec gris
                        case GRIS: 
                            printf("\033[48;5;235m\033[38;5;235m%2d \033[0m", pion); break;  // Gélatine sur fond Gris avec gris
                        case JAUNE_CLAIR: 
                            printf("\033[48;5;235m\033[38;5;220m%2d \033[0m", pion); break;  // Gélatine sur fond Jaune Clair avec gris
                        case ROSE: 
                            printf("\033[48;5;235m\033[38;5;213m%2d \033[0m", pion); break;   // Gélatine sur fond Rose avec gris
                        default: 
                            printf("\033[48;5;235m\033[38;5;8m%2d \033[0m", pion); break; // Gélatine sur fond Gris par défaut avec gris
                    }
                }
                else
                {
                    // Affichage des cases de bonbons (sans gélatine)
                    switch ((CouleurBonbons)pion)
                    {
                        case JAUNE: 
                            printf("\033[43m\033[30m%2d \033[0m", pion); break;  // Fond Jaune pour le bonbon
                        case VERT: 
                            printf("\033[42m\033[30m%2d \033[0m", pion); break;   // Fond Vert pour le bonbon
                        case BLEU: 
                            printf("\033[44m\033[30m%2d \033[0m", pion); break;   // Fond Bleu pour le bonbon
                        case ROUGE: 
                            printf("\033[41m\033[30m%2d \033[0m", pion); break;  // Fond Rouge pour le bonbon
                        case MAUVE: 
                            printf("\033[45m\033[30m%2d \033[0m", pion); break;  // Fond Mauve pour le bonbon
                        case BLANC: 
                            printf("\033[47m\033[30m%2d \033[0m", pion); break;  // Fond Blanc pour le bonbon
                        case GRIS: 
                            printf("\033[48;5;235m\033[30m%2d \033[0m", pion); break;  // Fond Gris pour le bonbon
                        case JAUNE_CLAIR: 
                            printf("\033[48;5;220m\033[30m%2d \033[0m", pion); break;  // Fond Jaune Clair pour le bonbon
                        case ROSE: 
                            printf("\033[48;5;213m\033[30m%2d \033[0m", pion); break;   // Fond Rose pour le bonbon
                        default: 
                            printf("\033[47m\033[30m%2d \033[0m", pion); break;  // Fond par défaut pour le bonbon
                    }
                }
            }
            printf("\n");
        }
    }

    grille->affiche = 0;
    Actions actionAffichage = {CALCUL, {0, 0}, {0, 0}}; // Mettre à jour l'action
    Enfiler(q, &actionAffichage); // Ajoute l'action dans la queue
}
