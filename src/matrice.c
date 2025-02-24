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

/***************************************************************************************************************************
                                                  INIT GRILLE

 ****************************************************************************************************************************/

void initialiser_grille(GrilleBonbons *grille)
{
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            grille->tableau[i][j].pion = couleurs[rand() % 5]; // Remplissage avec des couleurs
            grille->tableau[i][j].gelatine = false;            // Par défaut, pas de gelatine
        }
    }

    int nombreGelatine = rand() % 5 + 1; // Nombre de gelatines aléatoire entre 1 et 5
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            grille->tableau[i][j].gelatine = false; // Initialisation sans gelatine
        }
    }

    while (nombreGelatine > 0)
    {
        int x = rand() % TAILLE;
        int y = rand() % TAILLE;
        if (!grille->tableau[x][y].gelatine)
        {

            grille->tableau[x][y].gelatine = true; // Placement de la gelatine
            nombreGelatine--;
        }
    }
}

void Verif(Queue *q, GrilleBonbons *grille)
{
    printf("Début de la vérification séquentielle...\n");

    int nbCalculs = 0;

    // Vérification des alignements horizontaux
    for (int x = 0; x < TAILLE; x++)
    {
        for (int y = 0; y < TAILLE - 2; y++)
        { // -2 pour ne pas dépasser la grille
            Actions action = {"CALCUL", {x, y}, {x, y + 2}, false};
            enfiler(q, action);
            nbCalculs++;
        }
    }

    // Vérification des alignements verticaux
    for (int x = 0; x < TAILLE - 2; x++)
    { // -2 pour ne pas dépasser la grille
        for (int y = 0; y < TAILLE; y++)
        {
            Actions action = {"CALCUL", {x, y}, {x + 2, y}, false};
            enfiler(q, action);
            nbCalculs++;
        }
    }

    grille->calculsRestants = nbCalculs;
    printf("Vérification séquentielle terminée.\n");
}

int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index)
{
    int reponse;
    do
    {
        printf("%s", message[index][0]);     // Affiche le message
        int result = scanf(" %d", &reponse); // Lit un seul caractère
        while (getchar() != '\n')
            ;

        // Vérifie que la réponse est valide
        if (result == -1)
        {
            printf("Reponse invalide. Essayez encore.\n");
        }
    } while (reponse < 1 || reponse > 21); // Si la réponse n'est pas valide, on redemande

    return reponse - 1;
}

void Deplacement(Queue *q, GrilleBonbons *grille, int coordonneeXPremierPion,
                 int coordonneeYPremierPion, int coordonneeXDeuxiemePion,
                 int coordonneeYDeuxiemePion)
{
    afficher_grille(grille);
    char temp = grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion;
    grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion = grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion;
    grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion = temp;
    Actions action = {"CALCUL", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    enfiler(q, action);
}

/*Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes

/*_____________________________________________________________________________________________________________________________*/

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

void Calcul(Queue *q, GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2)
{
    int compteur;
    char pion;
    int i, j;

    // Vérification pour le pion en (*x2, *y2)
    pion = grille->tableau[*x2][*y2].pion;

    // Vérification verticale (colonne)
    compteur = 1;
    int xDebut = *x2, xFin = *x2;
    i = *x2 + 1;
    while (i < TAILLE && grille->tableau[i][*y2].pion == pion)
    {
        compteur++;
        xFin = i;
        i++;
    }
    i = *x2 - 1;
    while (i >= 0 && grille->tableau[i][*y2].pion == pion)
    {
        compteur++;
        xDebut = i;
        i--;
    }

    if (compteur >= 3)
    {
        Actions action = {"SUPPRESSIONV", {xDebut, *y2}, {xFin, *y2}, false};
        enfiler(q, action);
        grille->suppressionsRestantes += 1;
        printf("Ajout suppressions restantes :%d", grille->suppressionsRestantes);
        return;
    }

    // Vérification horizontale (ligne)
    compteur = 1;
    int yDebut = *y2, yFin = *y2;
    j = *y2 + 1;
    while (j < TAILLE && grille->tableau[*x2][j].pion == pion)
    {
        compteur++;
        yFin = j;
        j++;
    }
    j = *y2 - 1;
    while (j >= 0 && grille->tableau[*x2][j].pion == pion)
    {
        compteur++;
        yDebut = j;
        j--;
    }

    if (compteur >= 3)
    {
        Actions action = {"SUPPRESSIONH", {*x2, yDebut}, {*x2, yFin}, false};
        enfiler(q, action);
        grille->suppressionsRestantes += 1;
        printf("Ajout suppressions restantes :%d", grille->suppressionsRestantes);
        return;
    }

    // Vérification pour le pion en (*x1, *y1)
    pion = grille->tableau[*x1][*y1].pion;

    // Vérification verticale (colonne)
    compteur = 1;
    xDebut = *x1, xFin = *x1;
    i = *x1 + 1;
    while (i < TAILLE && grille->tableau[i][*y1].pion == pion)
    {
        compteur++;
        xFin = i;
        i++;
    }
    i = *x1 - 1;
    while (i >= 0 && grille->tableau[i][*y1].pion == pion)
    {
        compteur++;
        xDebut = i;
        i--;
    }

    if (compteur >= 3)
    {
        Actions action = {"SUPPRESSIONV", {xDebut, *y1}, {xFin, *y1}, false};
        enfiler(q, action);
        grille->suppressionsRestantes += 1;
        printf("decompteV : %d", grille->suppressionsRestantes);
        return;
    }

    // Vérification horizontale (ligne)
    compteur = 1;
    yDebut = *y1, yFin = *y1;
    j = *y1 + 1;
    while (j < TAILLE && grille->tableau[*x1][j].pion == pion)
    {
        compteur++;
        yFin = j;
        j++;
    }
    j = *y1 - 1;
    while (j >= 0 && grille->tableau[*x1][j].pion == pion)
    {
        compteur++;
        yDebut = j;
        j--;
    }

    if (compteur >= 3)
    {
        Actions action = {"SUPPRESSIONH", {*x1, yDebut}, {*x1, yFin}, false};
        enfiler(q, action);
        grille->suppressionsRestantes += 1;
        // printf("Ajout suppressions restantes :%d", grille->suppressionsRestantes);
        return;
    }

    printf("decompte avant decrementation : %d", grille->suppressionsRestantes);
    grille->calculsRestants--;

    printf("decompte : %d", grille->suppressionsRestantes);
    if (grille->calculsRestants <= 0)
    {
        printf("suppressions restantes :%d", grille->suppressionsRestantes);

        switch (est_vide(q))
        {
        case 1: // Si la queue est vide
            Verif(q, grille);
            {
                Actions action = {"VERIFICATION", {0, 0}, {0, 0}, false};
                enfiler(q, action);
            }
            // printf("queue vide calcul");

            // printf("les suppressions restantes si queue vide devraient normalement être ici : %d", grille->suppressionsRestantes);
            break;

        case 0: // Si la queue n'est pas vide
        {
            Actions actionAffichage = {"AFFICHAGE", {0, 0}, {0, 0}, false};
            enfiler(q, actionAffichage);
        }
            // printf("STADE POST CALCUL");
            // printf("les suppressions restantes devraient normalement être ici !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! : %d", grille->suppressionsRestantes);
            imprimer_queue(q);
            return;
        }
    }
}

/*if (grille->calculsRestants <= 0) {
    grille->estVerifiee = 1;

    // Enfile l'action AFFICHAGE
    Actions actionAffichage = {"AFFICHAGE", {0, 0}, {0, 0}, false};
    enfiler(q, actionAffichage);

    // Vérifie si la queue est vide après affichage
    if (est_vide(q)) {
        // Enfile une action de vérification seulement si la queue est vide
        Actions actionVerif = {"VERIFICATION", {0, 0}, {0, 0}, false};
        enfiler(q, actionVerif);
    }

    // Débogage / Affichage des actions restantes
    printf("STADE POST CALCUL\n");
    imprimer_queue(q);

    return;
}

*/
// transformer en bool et laisser calcul gerer l action supprmier parametre queue
Actions Verification(GrilleBonbons *grille, Queue *q)
{
    bool gelatinePresente = false; // On suppose qu'il n'y a pas de gélatine au début

    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
            {
                printf("Bonbon gélatine toujours présent\n");

                Actions action = {"LECTURE", {0, 0}, {0, 0}, false};
                enfiler(q, action);
                printf("oooooooooooooooooooo\n");
                gelatinePresente = true; // On détecte de la gélatine
                return action;           // On quitte immédiatement la fonction
            }
        }
    }

    // Si on arrive ici, c'est qu'il n'y avait pas de gélatine
    if (!gelatinePresente)
    {
        printf("FIN NIVEAU\n");
        Actions action = {"FINNIVEAU", {0, 0}, {0, 0}, false};
        enfiler(q, action);
        return action;
    }
    afficher_grille(grille);
    Actions action = {"FIN", {0, 0}, {0, 0}, false};
    return action;
}

// T AS OUBLIE DE SUPPRIMER LA GELATINE ICI
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};

    // 1 SUPPRESSION DES BONBONS (remplacement par un espace vide et suppression de la gélatine)
    for (int i = *x1; i <= *x2; i++)
    {
        grille->tableau[i][*y1].pion = ' '; // Suppression en mettant un espace vide
        printf("grille est verif?????????????????????????????? %d", grille->estVerifiee);

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
                grille->tableau[i][*y1].pion = couleurs[index];
                grille->tableau[i][*y1].gelatine = false; // Par défaut, pas de gélatine
            }
        }
    }
    afficher_grille(grille);
    // Ajouter une action de recalcul dans la queue pour continuer les vérifications
    Actions action = {"CALCUL", {*x1, *y1}, {*x2, *y2}, false};
    enfiler(q, action);
}

void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};

    // 1️ SUPPRESSION DES BONBONS HORIZONTAUX (remplacement par un espace vide et suppression de la gélatine)
    for (int j = *y1; j <= *y2; j++)
    { // On parcourt la ligne horizontale
        for (int i = *x1; i <= *x2; i++)
        { // Supprimer les bonbons alignés horizontalement
            grille->tableau[i][j].pion = ' ';
            printf("grille est verif?????????????????????????????? %d", grille->estVerifiee);
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
        grille->tableau[0][j].pion = couleurs[index];
        grille->tableau[0][j].gelatine = false; // Par défaut, pas de gélatine
    }
    Actions action = {"CALCUL", {*x1, *y1}, {*x2, *y2}, false};
    enfiler(q, action);
    afficher_grille(grille);
}

// Créer l'action "CALCUL"

// === Ajout de la vérification ===

// === Ajout de la vérification ===

/***************************************************************************************************************************
                                                  MAIN

 ****************************************************************************************************************************/