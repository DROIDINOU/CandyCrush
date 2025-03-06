#include <stdio.h>
#include "constante.h"
#include <stdlib.h>
#include <stdbool.h>
#include "matrice.h"
#include "affichage.h"
#include <string.h>

// UNE ACTION A LA FOIS
// A FAIRE INITIALISER DOIT VERIFIER TOUT DANS CALCUL
// FAIRE FONCTION VERIFICATION
// BONUS FAIRE FONCTION CASCADE
// supprimer struct en double
// FAUDRA VERIFIER QUE COORDONNEES PAS DEJA DANS QUEUE

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTIONS D INITIALISER GRILLE

  *** Fonctions : initialiserBonbons - initialiserGelatines
  **   Sous fonctions de initialiserGrille
  -> Parametres : grille
  -> initialiserBonbons : place les bonbons aleatoirement dans la grille
  -> initialiserGelatines : place aleatoirement les gelatines dans la grille
___________________________________________________________________________________________________________________
 */

/* ====================================================================================================
Initialise les bonbons dans la grille
Remplissage aléatoire des cases avec des couleurs
========================================================================================================*/

void initialiserBonbons(GrilleBonbons *grille)
{

    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            grille->tableau[ligne][colonne].pion = couleurs[rand() % 5]; // Remplissage avec des couleurs
            grille->tableau[ligne][colonne].gelatine = false;            // Par défaut, pas de gelatine
        }
    }
}

/* ====================================================================================================
Fixe un nombre aléatoire de gélatines dans la grille
Initialise les gelatines a faux
Tant que gelatines inferieures au nombre fixe => placement des gelatines avec coordonnees aleatoires
========================================================================================================*/

void initialiserGelatines(GrilleBonbons *grille)
{
    int nombreGelatine = rand() % 5 + 6; // Nombre de gelatines aléatoire entre 1 et 5
    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            grille->tableau[ligne][colonne].gelatine = false; // Initialisation sans gelatine
        }
    }

    while (nombreGelatine > 0) // Tant qu'il reste des gelatines à placer
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

void initialiserGrille(GrilleBonbons *grille)
{
    // Initialisation des éléments de la structure grille
    grille->lignes = TAILLE;
    grille->colonnes = TAILLE;
    grille->estInitialisee = 0;
    grille->estVerifiee = 0;
    grille->calcX = 0; // mettre ca ici ???
    grille->calcY = 0; // mettre ca ici ???

    initialiserBonbons(grille);
    initialiserGelatines(grille);
}

/*_______________________________________________________________________________________________________________
                                     **** FONCTION DEPLACEMENT DES BONBONS
   -> Parametres : queue - grille - coordonnees x et y des pions
   -> Reinitialise element de la grille estVerifiee a 0 (il faut reverifier la grille apres le deplacement
   -> Reinitialise element de la grille estInitialisee a 1 (la grille a ete initialisee)
   -> Reinitialise element de la grille calcX et calcY a 0
   -> Echange des pions entre les deux coordonnees
_______________________________________________________________________________________________________________*/
void Deplacement(Queue *q, GrilleBonbons *grille, int xPion1,
                 int yPion1, int xPion2,
                 int yPion2)

{
    // Reinitialisation des elements de la grille
    grille->estVerifiee = 0;    // On doit re-vérifier la grille après le déplacement
    grille->estInitialisee = 1; // La grille a été initialisée
    grille->calcX = 0;
    grille->calcY = 0;

    // Echange des pions
    char temp = grille->tableau[xPion1][yPion1].pion;                            // echange des pions via variable temporaire qui stocke le pion 1
    grille->tableau[xPion1][yPion1].pion = grille->tableau[xPion2][yPion2].pion; // pion 1 devient pion 2
    grille->tableau[xPion2][yPion2].pion = temp;                                 // pion 2 devient pion 1

    Actions action = {"CALCUL", {xPion1, yPion1}, {xPion2, yPion2}}; // On ajoute une action CALCUL avec les nouvelles coordonnées
    enfiler(q, action);
}

// eventuellement supprimer
bool actionExiste(Queue *q, const char *nom, int x1, int y1, int x2, int y2)
{
    for (int i = q->debut; i != q->fin; i = (i + 1) % LONGUEUR)
    {
        if (strcmp(q->elements[i].actionName, nom) == 0 && // Vérification du nom
            q->elements[i].pion1.x == x1 && q->elements[i].pion1.y == y1 &&
            q->elements[i].pion2.x == x2 && q->elements[i].pion2.y == y2)
        {
            return true; // Action trouvée
        }
    }
    return false; // Action non trouvée
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
bool VerifierAlignements(int *x, int *y, GrilleBonbons *grille, Queue *q)
{

    char pion = grille->tableau[*x][*y].pion;
    if (pion != ' ') // si pas déjà supprimé
    {
        // Vérification verticale
        int compteur = 1;
        int xDebut = *x, xFin = *x;

        // vers le bas
        int i = *x + 1;
        while (i < TAILLE && grille->tableau[i][*y].pion == pion)
        {
            compteur++;
            xFin = i;
            i++;
        }
        // vers le haut
        i = *x - 1;
        while (i >= 0 && grille->tableau[i][*y].pion == pion)
        {
            compteur++;
            xDebut = i;
            i--;
        }

        if (compteur >= 3)
        {
            Actions supV = {"SUPPRESSIONV", {xDebut, *y}, {xFin, *y}, false};
            enfiler(q, supV);
            return true; // On s'arrête (une seule action)
        }

        // Vérification horizontale
        compteur = 1;
        int yDebut = *y, yFin = *y;

        // vers la droite
        int j = *y + 1;
        while (j < TAILLE && grille->tableau[*x][j].pion == pion)
        {
            compteur++;
            yFin = j;
            j++;
        }
        // vers la gauche
        j = *y - 1;
        while (j >= 0 && grille->tableau[*x][j].pion == pion)
        {
            compteur++;
            yDebut = j;
            j--;
        }

        if (compteur >= 3)
        {
            Actions supH = {"SUPPRESSIONH", {*x, yDebut}, {*x, yFin}, false};
            enfiler(q, supH);
            return true;
        }
    }
    return false;
}

void Calcul(Queue *q, GrilleBonbons *grille,
            int *x1, int *y1, int *x2, int *y2)
{
    printf("ENTRE DANS CALCULE");
    // 1) Récupérer la cellule en cours
    int x = grille->calcX;
    int y = grille->calcY;
    if (grille->estVerifiee == 1)
    {
        printf("Verification déjà faite\n");
        Actions verification = {"VERIFICATION", {0, 0}, {0, 0}, false};
        enfiler(q, verification);
        return;
    };

    // Si on est déjà hors limites, c'est que tout est vérifié
    if (x >= TAILLE)
    {
        printf("Terminé : pas d'alignement trouvé dans toute la grille.\n");
        // On peut enchaîner AFFICHAGE ou LECTURE ou rien, selon ta logique :
        Actions aff = {"AFFICHAGE", {0, 0}, {0, 0}, false};
        enfiler(q, aff);
        return;
    }

    // ─────────────────────────────────────────────────
    // 2) Même logique que ton code actuel pour détecter
    //    un alignement vertical/horizontal sur la case (x,y).
    // ─────────────────────────────────────────────────
    if (grille->affiche)
    {
        Actions aff = {"AFFICHAGE", {0, 0}, {0, 0}, false};
        enfiler(q, aff);
        grille->affiche = 0;
        return;
    }

    if (VerifierAlignements(&x, &y, grille, q))
    {
        return;
    };

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
        Actions aff = {"AFFICHAGE", {0, 0}, {0, 0}, false};
        grille->calcX = 0;
        grille->calcY = 0;
        enfiler(q, aff);
        grille->estVerifiee = 1;
        return;
    }

    // Sinon, on continue avec la prochaine cellule
    Actions nextCalc = {"CALCUL", {0, 0}, {0, 0}, false};
    enfiler(q, nextCalc);

    return;
}

// transformer en bool et laisser calcul gerer l action supprmier parametre queue
void Verification(GrilleBonbons *grille, Queue *q)
{
    grille->estVerifiee = 1;
    bool gelatinePresente = false; // On suppose qu'il n'y a pas de gélatine au début

    imprimer_queue(q);
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
            {
                printf("Bonbon gélatine toujours présent\n");

                Actions action = {"LECTURE", {0, 0}, {0, 0}, false};
                enfiler(q, action);
                // printf("oooooooooooooooooooo\n");
                gelatinePresente = true; // On détecte de la gélatine
                return;
            }
        }
    }

    // Si on arrive ici, c'est qu'il n'y avait pas de gélatine
    if (!gelatinePresente)
    {
        printf("FIN NIVEAU\n");
    }
    return;
}

// T AS OUBLIE DE SUPPRIMER LA GELATINE ICI
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    printf("avant");
    printf(" SUPPRESSIONV  (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);

    // afficher_grille(grille);
    //  1 SUPPRESSION DES BONBONS (remplacement par un espace vide et suppression de la gélatine)
    for (int i = *x1; i <= *x2; i++)
    {
        grille->tableau[i][*y1].pion = ' '; // Suppression en mettant un espace vide
        // printf("grille est verif?????????????????????????????? %d", grille->estVerifiee);

        if (grille->estInitialisee)
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
                if (grille->estInitialisee)
                {
                    grille->tableau[i][*y1].gelatine = grille->tableau[j][*y1].gelatine;
                } // Copie aussi l'état de la gélatine
                grille->tableau[j][*y1].pion = ' '; // Efface la case d'origine
            }
            else
            {
                // Si aucun bonbon trouvé, on génère un nouveau bonbon en haut
                int index = rand() % 5;
                grille->tableau[i][*y1].pion = couleurs[index];
                if (grille->estInitialisee)
                {
                    grille->tableau[i][*y1].gelatine = false;
                }
            }
        }
    }
    printf("\n Après suppression et chute des bonbons :\n");
    // afficher_grille(grille);

    // Vérification pour éviter la suppression horizontale en double
    int yDebut = *y1, yFin = *y1;

    // Ajouter une action de recalcul dans la queue
    Actions action = {"CALCUL", {*x1, *y1}, {*x2, *y2}, false};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    enfiler(q, action);
    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}

void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    printf("entre dans suppression H");
    printf(" SUPPRESSIONH  (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
    // 1️ SUPPRESSION DES BONBONS HORIZONTAUX (remplacement par un espace vide et suppression de la gélatine)
    for (int j = *y1; j <= *y2; j++)
    { // On parcourt la ligne horizontale
        for (int i = *x1; i <= *x2; i++)
        { // Supprimer les bonbons alignés horizontalement
            grille->tableau[i][j].pion = ' ';
            // printf("grille est verif?????????????????????????????? %d", grille->estVerifiee);
            if (grille->estInitialisee)
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
            if (grille->estInitialisee)
            {

                grille->tableau[i][j].gelatine = grille->tableau[i - 1][j].gelatine; // Descendre la gélatine aussi
            }
        }
        // Générer un nouveau bonbon en haut
        int index = rand() % 5;
        grille->tableau[0][j].pion = couleurs[index];
        if (grille->estInitialisee)
        {
            grille->tableau[0][j].gelatine = false; // Par défaut, pas de gélatine
        }
    }
    int yDebut = *y1, yFin = *y1;

    // Ajouter une action de recalcul dans la queue
    Actions action = {"CALCUL", {*x1, *y1}, {*x2, *y2}, false};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    enfiler(q, action);
    // afficher_grille(grille);

    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}