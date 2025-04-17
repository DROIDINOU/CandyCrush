#include <stdio.h>
#include "constante.h"
#include <stdlib.h>
#include <stdbool.h>
#include "matrice.h"
#include "affichage.h"
#include "generationaleatoire.h"
#include <string.h>

// A FAIRE NE PAS SUPPRIMER GELATINES LORS DE L INITIALISATION
// A FAIRE GERER LES CAS OU L UTILISATEUR ECHANGE DES MAUVAIS PIONS
// DANS INITIALISER GRILLE FAUT LANCER L ACTION POUR LE MOMENT LANCEE A PARTIR DE MAIN
// LE CODE GERE LES ECHANGES DE PIONS OU QU ILS SOIENT !!!

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTIONS D INITIALISER GRILLE

  *** Fonctions : initialiserBonbons - initialiserGelatines
  **   Sous fonctions de initialiserGrille

  -> initialiserBonbons : place les bonbons aleatoirement dans la grille en utilisant fonction de génération aléatoire
                          definie dans generationaleatoire.c
  -> initialiserGelatines : place aleatoirement les gelatines dans la grille en utilisant fonction de génération aléatoire
                            definie dans generationaleatoire.c
___________________________________________________________________________________________________________________
 */

// Initialise les bonbons dans la grille
// Remplissage aléatoire des cases avec des couleurs
void initialiserBonbons(GrilleBonbons *grille)
{

    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {                                                                                    // Remplissage avec des couleurs aleatoires
            grille->tableau[ligne][colonne].pion = GenerationAleatoire(COULEURALEATOIRE, 1); // Appel de la fonction pour générer
            // une couleur aléatoire
            grille->tableau[ligne][colonne].gelatine = false; // Par défaut, pas de gelatine
        }
    }
}

// Fixe un nombre aléatoire de gélatines dans la grille
// Tant que gelatines inferieures au nombre de gélatines créées aléatoirement => placement des gelatines avec coordonnees aleatoires

void initialiserGelatines(GrilleBonbons *grille)
{
    int nombreGelatine = GenerationAleatoire(OBSTACLEALEATOIRE, 1); // Nombre aléatoire de gelatines à placer
    while (nombreGelatine > 0)                                      // Tant qu'il reste des gelatines à placer
    {
        int x = rand() % TAILLE;             // Coordonnées aléatoires
        int y = rand() % TAILLE;             // Coordonnées aléatoires
        if (!grille->tableau[x][y].gelatine) // Si pas déjà de gelatine
        {

            grille->tableau[x][y].gelatine = true; // Placement de la gelatine
            nombreGelatine--;                      // decrementer le nombre aleatoire de gelatines restantes
        }
    }
}

/***************************************************************************************************************************
                                 **** FONCTION CENTRALE INITIALISATION GRILLE

-> parametres : grille
-> initialise les élements de la structure grille
-> place les bonbons aléatoirement dans la grille (appel de la fonction initialiserBonbons)
-> place aleatoirement les gelatines dans la grille (appel de la fonction initialiserGelatines)

 ****************************************************************************************************************************/

void initialiser_grille(GrilleBonbons *grille)
{

    /*grille->lignes = TAILLE;
    grille->colonnes = TAILLE;
    grille->estInitialisee = 0;
    grille->estVerifiee = 0;
    grille->calcX = 0;
    grille->calcY = 0;
    grille->deplacement = 0;*/
    // Initialisation des éléments de la structure grille
    *grille = (GrilleBonbons){.lignes = TAILLE, .colonnes = TAILLE, .estInitialisee = 0, .estVerifiee = 0, .calcX = 0, .calcY = 0, .deplacement = 0};

    initialiserBonbons(grille);   // initialise la grille de bonbons
    initialiserGelatines(grille); // initialise la grille de gelatines
}

void Deplacement(Queue *q, GrilleBonbons *grille, int coordonneeXPremierPion,
                 int coordonneeYPremierPion, int coordonneeXDeuxiemePion,
                 int coordonneeYDeuxiemePion)
{
    char temp = grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion;
    grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion = grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion;
    grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion = temp;
    grille->calcX = 0;
    grille->calcY = 0;
    Actions action = {CALCUL, {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    Enfiler(q, &action);
}

/*______________________________________________________________________________________________________________________________

Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes

/*_____________________________________________________________________________________________________________________________*/

void Calcul(Queue *q, GrilleBonbons *grille,
            int *x1, int *y1, int *x2, int *y2)
{
    printf("ENTRE DANS CALCULLLLLL");
    // 1) Récupérer la cellule en cours
    int x = grille->calcX;
    int y = grille->calcY;
    if (grille->estVerifiee == 1)
    {
        printf("ne dis pas qu on est ici lol");
        Actions verification = {VERIFICATION, {0, 0}, {0, 0}, false};
        Enfiler(q, &verification);
        return;
    };

    // Si on est déjà hors limites, c'est que tout est vérifié
    if (x >= TAILLE)
    {
        printf("Terminé : pas d'alignement trouvé dans toute la grille.\n");
        // On peut enchaîner AFFICHAGE ou LECTURE ou rien, selon ta logique :
        Actions aff = {AFFICHAGE, {0, 0}, {0, 0}, false};
        Enfiler(q, &aff);
        grille->estVerifiee = true;
        return;
    }

    // ─────────────────────────────────────────────────
    // 2) Même logique que ton code actuel pour détecter
    //    un alignement vertical/horizontal sur la case (x,y).
    // ─────────────────────────────────────────────────
    if (grille->affiche)
    {
        Actions aff = {AFFICHAGE, {0, 0}, {0, 0}, false};
        Enfiler(q, &aff);
        grille->affiche = 0;
        return;
    }

    char pion = grille->tableau[x][y].pion;
    if (pion != ' ') // si pas déjà supprimé
    {
        // Vérification verticale
        int compteur = 1;
        int xDebut = x, xFin = x;

        // vers le bas
        int i = x + 1;
        while (i < TAILLE && grille->tableau[i][y].pion == pion)
        {
            compteur++;
            xFin = i;
            i++;
        }
        // vers le haut
        i = x - 1;
        while (i >= 0 && grille->tableau[i][y].pion == pion)
        {
            compteur++;
            xDebut = i;
            i--;
        }

        if (compteur >= 3)
        {
            Actions supV = {SUPPRESSIONV, {xDebut, y}, {xFin, y}, false};
            Enfiler(q, &supV);
            return; // On s'arrête (une seule action)
        }

        // Vérification horizontale
        compteur = 1;
        int yDebut = y, yFin = y;

        // vers la droite
        int j = y + 1;
        while (j < TAILLE && grille->tableau[x][j].pion == pion)
        {
            compteur++;
            yFin = j;
            j++;
        }
        // vers la gauche
        j = y - 1;
        while (j >= 0 && grille->tableau[x][j].pion == pion)
        {
            compteur++;
            yDebut = j;
            j--;
        }

        if (compteur >= 3)
        {
            Actions supH = {SUPPRESSIONH, {x, yDebut}, {x, yFin}, false};
            Enfiler(q, &supH);
            return;
        }
    }

    // ─────────────────────────────────────────────────
    // 3) Aucune suppression => On passe à la cellule suivante
    // ─────────────────────────────────────────────────
    grille->calcY++;
    if (grille->calcY >= TAILLE)
    {
        grille->calcX++;
        grille->calcY = 0;
    }

    // Si on a atteint la fin, on peut enchaîner AFFICHAGE
    if (grille->calcX >= TAILLE)
    {
        printf("Fin du parcours : pas d'alignement trouvé.\n");
        Actions aff = {AFFICHAGE, {0, 0}, {0, 0}, false};
        grille->calcX = 0;
        grille->calcY = 0;
        Enfiler(q, &aff);
        grille->estVerifiee = true;
        return;
    }

    // Sinon, on continue avec la prochaine cellule
    Actions nextCalc = {CALCUL, {0, 0}, {0, 0}, false};
    Enfiler(q, &nextCalc);

    return;
}

// transformer en bool et laisser calcul gerer l action supprmier parametre queue
Actions Verification(GrilleBonbons *grille, Queue *q)
{
    grille->estVerifiee = 1;
    bool gelatinePresente = false; // On suppose qu'il n'y a pas de gélatine au début

    ImprimerQueue(q);
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
            {
                printf("Bonbon gélatine toujours présent\n");

                Actions action = {LECTURE, {0, 0}, {0, 0}, false};
                Enfiler(q, &action);
                // printf("oooooooooooooooooooo\n");
                gelatinePresente = true; // On détecte de la gélatine
                return action;           // On quitte immédiatement la fonction
            }
        }
    }

    // Si on arrive ici, c'est qu'il n'y avait pas de gélatine
    if (!gelatinePresente && NIVEAUX[0].compteurNiveau != FINALNIVEAU - 1)
    {
        printf("FIN NIVEAU\n");
        NIVEAUX[0].compteurNiveau += 1;
        printf("compteur niveau : %d", NIVEAUX[0].compteurNiveau);

        Actions action = {FINNIVEAU, {0, 0}, {0, 0}, false};
        Enfiler(q, &action);
        return action;
    }
    Actions action = {FIN, {0, 0}, {0, 0}, false};
    Enfiler(q, &action);
    return action;
}

// T AS OUBLIE DE SUPPRIMER LA GELATINE ICI
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    printf("avant");
    printf(" SUPPRESSIONV  (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);

    // afficher_grille(grille);
    //  1 SUPPRESSION DES BONBONS (remplacement par un espace vide et suppression de la gélatine)
    for (int i = *x1; i <= *x2; i++)
    {
        grille->tableau[i][*y1].pion = ' '; // Suppression en mettant un espace vide
        // printf("grille est verif?????????????????????????????? %d", grille->estVerifiee);

        if (grille->estVerifiee)
        {
            grille->tableau[i][*y1].gelatine = false;
        } // Suppression de la gélatine
    }

    // 2 FAIRE TOMBER LES BONBONS
    for (int i = *x2; i >= 0; i--)
    {
        // Si la case actuelle est vide, on "fait tomber" un bonbon
        if (grille->tableau[i][*y1].pion == ' ')
        {
            // Trouver la première case occupée au-dessus de cette case vide
            int j = i - 1;
            while (j >= 0 && grille->tableau[j][*y1].pion == ' ')
            {
                j--; // Cherche un bonbon au-dessus
            }

            // Si un bonbon a été trouvé, on le déplace dans la case vide
            if (j >= 0)
            {
                grille->tableau[i][*y1].pion = grille->tableau[j][*y1].pion;
                grille->tableau[i][*y1].gelatine = grille->tableau[j][*y1].gelatine; // Copie aussi l'état de la gélatine
                grille->tableau[j][*y1].pion = ' ';                                  // Efface la case d'origine
            }
            else
            {
                // Si aucun bonbon trouvé, on génère un nouveau bonbon en haut
                int index = rand() % 5;
                grille->tableau[i][*y1].pion = COULEURS[index];
                grille->tableau[i][*y1].gelatine = false; // Par défaut, pas de gélatine
            }
        }
    }
    printf("\n Après suppression et chute des bonbons :\n");
    // afficher_grille(grille);

    // Vérification pour éviter la suppression horizontale en double
    int yDebut = *y1, yFin = *y1;

    // Ajouter une action de recalcul dans la queue
    Actions action = {CALCUL, {*x1, *y1}, {*x2, *y2}, false};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    Enfiler(q, &action);
    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}

void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    printf("entre dans suppression H");
    printf(" SUPPRESSIONH  (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
    // 1️ SUPPRESSION DES BONBONS HORIZONTAUX (remplacement par un espace vide et suppression de la gélatine)
    for (int j = *y1; j <= *y2; j++)
    { // On parcourt la ligne horizontale
        for (int i = *x1; i <= *x2; i++)
        { // Supprimer les bonbons alignés horizontalement
            grille->tableau[i][j].pion = ' ';
            // printf("grille est verif?????????????????????????????? %d", grille->estVerifiee);
            if (grille->estVerifiee)
            {                                           // Remplacement par un espace vide
                grille->tableau[i][j].gelatine = false; // Suppression de la gélatine
            }
        }
    }

    // 2️ FAIRE TOMBER LES BONBONS
    for (int j = *y1; j <= *y2; j++)
    { // Parcourir la ligne horizontale
        for (int i = *x1; i > 0; i--)
        { // Faire descendre les bonbons au-dessus
            grille->tableau[i][j].pion = grille->tableau[i - 1][j].pion;
            grille->tableau[i][j].gelatine = grille->tableau[i - 1][j].gelatine; // Descendre la gélatine aussi
        }
        // Générer un nouveau bonbon en haut
        int index = rand() % 5;
        grille->tableau[0][j].pion = COULEURS[index];
        grille->tableau[0][j].gelatine = false; // Par défaut, pas de gélatine
    }
    int yDebut = *y1, yFin = *y1;

    // Ajouter une action de recalcul dans la queue
    Actions action = {CALCUL, {*x1, *y1}, {*x2, *y2}, false};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    Enfiler(q, &action);
    // afficher_grille(grille);

    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}