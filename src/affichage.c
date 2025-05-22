#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "affichage.h"
#include "matrice.h"
#include "constante.h"
#include "erreur.h"

// Efface tout l'écran (CMD sous Windows ou "clear" sur Unix)
static void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

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
    int result;
    int c;

    do
    {
        // Affiche le message et la plage
        printf("%s %d : ",
               MESSAGESECHANGEBONBONS[index].message,
               MESSAGESECHANGEBONBONS[index].nombreLigneOuColonne);

        // 1) Lecture de l'entier
        result = scanf("%d", &choixUtilisateur);

        // 2) Vider le reste de la ligne (jusqu'au '\n' ou EOF)
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        // 3) Contrôle de la conversion et de la plage
        if (result != 1)
        {
            printf("Entrée invalide. Veuillez entrer un entier.\n");
            choixUtilisateur = -1;
        }
        else if (choixUtilisateur < 1 || choixUtilisateur > TAILLE)
        {
            printf("Erreur : veuillez entrer un nombre entre 1 et %d.\n", TAILLE);
            choixUtilisateur = -1;
        }

    } while (choixUtilisateur == -1);

    // on renvoie en 0-based
    return choixUtilisateur - 1;
}

bool EstPionAdjacent(int x1, int y1, int x2, int y2)
{
    // Vérifie si les pions sont adjacents (horizontalement ou verticalement)
    return (x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1);
}

// lit les coordonnees entrees par l utilisateur pour le swap de pions
bool LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ObtenirReponseAuMessage(1);
    printf("x1 = %d\n", *x1);

    *y1 = ObtenirReponseAuMessage(0);
    printf("y1 = %d\n", *y1);

    *x2 = ObtenirReponseAuMessage(1);
    printf("x2 = %d\n", *x2);

    *y2 = ObtenirReponseAuMessage(0);
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
        printf("lire coordonnees 1");
        printf("Coordonnees valides : (%d,%d) et (%d,%d)\n", *coordonneeXPremierPion, *coordonneeYPremierPion, *coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion);
        Actions action = {DEPLACEMENT, {*coordonneeXPremierPion, *coordonneeYPremierPion}, {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
        Enfiler(q, &action);
    }
    else
    {
        printf("Coordonnees invalides : (%d,%d) et (%d,%d)\n", *coordonneeXPremierPion, *coordonneeYPremierPion, *coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion);
        GererErreurNonFatale(ERREURDEPLACEMENT); // message d'erreur si les pions ne sont pas adjacents
        // on reenfile l action de lecture
        LireQuatreCoordonnees(coordonneeXPremierPion, coordonneeYPremierPion, coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
    }
}

/*____________________________________________________________________________________________________________________________
                                       **** FONCTION D'AFFICHAGE

-> Params : grille - queue
-> Afficher la grille des bonbons
-> Afficher la grille des gelatines
______________________________________________________________________________________________________________________________
*/
void afficherGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu)
{
    PAUSE(50);
    clearScreen();

    if (etatJeu->findepartie == 1)
    {
        printf(etatJeu->coupsepuises ? MESSAGEETATJEU[MESSAGECOUPSEPUISES] : MESSAGEETATJEU[MESSAGEFINJEU]);
        return;
    }

    if (etatJeu->niveausuivant == 1)
    {
        printf(MESSAGEETATJEU[MESSAGEFELICITATIONS]);
        return;
    }

    // En-tête colonnes
    printf("  ");
    for (int col = 0; col < grille->colonnes; col++)
        printf("  %2d", col + 1);
    printf("\n");

    // Affichage des lignes
    for (int row = 0; row < grille->lignes; row++)
    {
        printf("%2d ", row + 1);

        for (int col = 0; col < grille->colonnes; col++)
        {
            int pion = grille->tableau[row][col].pion;
            bool gelatine = grille->tableau[row][col].gelatine;

            if (pion == VIDE)
            {
                printf(" ⬚ ");
                continue;
            }

            char *emoji;
            switch ((CouleurBonbons)pion)
            {
            case JAUNE:
                emoji = "🟡";
                break;
            case VERT:
                emoji = "🟢";
                break;
            case BLEU:
                emoji = "🔵";
                break;
            case ROUGE:
                emoji = "🔴";
                break;
            case MAUVE:
                emoji = "🟣";
                break;
            case BLANC:
                emoji = "⚪";
                break;
            case GRIS:
                emoji = "⚫";
                break;
            case JAUNE_CLAIR:
                emoji = "🟨";
                break;
            case ROSE:
                emoji = "🌸";
                break;
            default:
                emoji = "❓";
                break;
            }

            if (gelatine)
                printf("\033[48;5;208m %s \033[0m", emoji); // fond orange vif
            else
                printf(" %s ", emoji);
        }

        printf("\n");
    }
}
