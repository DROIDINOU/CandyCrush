#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "affichage.h"
#include "matrice.h"
#include "constante.h"
#include "erreur.h"
#include <Windows.h>

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
    int caractereLu; // utilisé pour vider le tampon d'entrée après scanf;

    do
    {
        // Affiche le message et la plage
        printf("%s %d : ",
               MESSAGESECHANGEBONBONS[index].message,
               MESSAGESECHANGEBONBONS[index].nombreLigneOuColonne);

        // 1) Lecture de l'entier
        result = scanf("%d", &choixUtilisateur);

        // 2) Vider le reste de la ligne (jusqu'au '\n' ou EOF)
        while ((caractereLu = getchar()) != '\n' && caractereLu != EOF)
            ;

        // 3) Contrôle de la conversion et de la plage
        if (result != 1)
        {
            GererErreurNonFatale(ERREURENTREEINVALIDE);
            choixUtilisateur = -1;
        }
        else if (choixUtilisateur < 1 || choixUtilisateur > TAILLE)
        {
            GererErreurNonFatale(ERREURNOMBREENTREEINCORRECTE);
            choixUtilisateur = -1;
        }

    } while (choixUtilisateur == -1);

    // retourne le choix de l utilisateur utilisable pour le programmeur (-1)
    return choixUtilisateur - 1;
}

// Vérifie si les pions sont adjacents (horizontalement ou verticalement)
bool EstPionAdjacent(int x1, int y1, int x2, int y2)
{
    return (x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1);
}

// lit les coordonnees entrees par l utilisateur pour le swap de pions
bool LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ObtenirReponseAuMessage(1);

    *y1 = ObtenirReponseAuMessage(0);

    *x2 = ObtenirReponseAuMessage(1);

    *y2 = ObtenirReponseAuMessage(0);

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
        Actions action = {DEPLACEMENT, {*coordonneeXPremierPion, *coordonneeYPremierPion}, {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
        Enfiler(q, &action);
    }
    else
    {
        GererErreurNonFatale(ERREURDEPLACEMENT); // message d'erreur si les pions ne sont pas adjacents (pas gere via
        // affichage mais directement dans le prompt)
        LireQuatreCoordonnees(coordonneeXPremierPion, coordonneeYPremierPion, coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
    }
}

/*____________________________________________________________________________________________________________________________
                                       **** FONCTION D'AFFICHAGE

-> afficherMessagePleinEcran - afficherGrille
-> Params : grille - queue
-> Afficher la grille des bonbons
-> Afficher la grille des gelatines
______________________________________________________________________________________________________________________________
*/

void afficherMessagePleinEcran(const char *texte, int largeur, int hauteur)
{
    int len = strlen(texte); // longueur du texte a afficher
    int padding = (largeur - len) / 2;
    if (padding < 0)
        padding = 0;

    int ligneTexte = hauteur / 2;

    // Lignes vides avant le message
    for (int i = 0; i < ligneTexte; i++)
    {
        for (int j = 0; j < largeur; j++)
            printf(" ");
        printf("\n");
    }

    // Ligne contenant le message centré en orange
    for (int i = 0; i < padding; i++)
        printf(" ");
    printf("\033[38;5;208m%s\033[0m", texte);
    for (int i = 0; i < largeur - padding - len; i++)
        printf(" ");
    printf("\n");

    // Lignes vides après
    for (int i = ligneTexte + 1; i < hauteur; i++)
    {
        for (int j = 0; j < largeur; j++)
            printf(" ");
        printf("\n");
    }
}

void afficherGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu)
{
    PAUSE(200);
    clearScreen(); // permet d'avoir une grille fixe tout au long du jeu
    //       On commence par verifier l'état du jeux pour afficher les messages
    if (etatJeu->findepartie == 1) // affichage messages fin de partie
    {
        const char *texte = etatJeu->coupsepuises
                                ? MESSAGEETATJEU[MESSAGECOUPSEPUISES]
                                : MESSAGEETATJEU[MESSAGEFINJEU]; // pas encore vérifié que affichage est ok

        afficherMessagePleinEcran(texte, grille->colonnes * 4, grille->lignes);
        PAUSE(2000);
        return;
    }
    if (etatJeu->niveausuivant == 1)
    {
        afficherMessagePleinEcran(MESSAGEETATJEU[MESSAGEFELICITATIONS],
                                  grille->colonnes * 4,
                                  grille->lignes);
        PAUSE(2000);
        etatJeu->niveausuivant = 0;
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
                printf("  ⬚ ");
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
