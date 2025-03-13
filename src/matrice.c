#include <stdio.h>
#include "constante.h"
#include <stdlib.h>
#include <stdbool.h>
#include "matrice.h"
#include "affichage.h"
#include <string.h>

// A FAIRE GERER LES CAS OU L UTILISATEUR ECHANGE DES MAUVAIS PIONS
// LE CODE GERE LES ECHANGES DE PIONS OU QU ILS SOIENT !!!

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTIONS D INITIALISER GRILLE

  *** Fonctions : initialiserBonbons - initialiserGelatines
  **   Sous fonctions de initialiserGrille
  -> Parametres : grille
  -> initialiserBonbons : place les bonbons aleatoirement dans la grille
  -> initialiserGelatines : place aleatoirement les gelatines dans la grille
___________________________________________________________________________________________________________________
 */

// Initialise les bonbons dans la grille
// Remplissage aléatoire des cases avec des couleurs
void initialiserBonbons(GrilleBonbons *grille)
{

    char couleurs[] = {'J', 'V', 'B', 'R', 'M'}; // on pourrait deplacer ca dans grille struct
    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        {
            grille->tableau[ligne][colonne].pion = couleurs[rand() % 5]; // Remplissage avec des couleurs
            grille->tableau[ligne][colonne].gelatine = false;            // Par défaut, pas de gelatine
        }
    }
}

// Fixe un nombre aléatoire de gélatines dans la grille
// Initialise les gelatines a faux
// Tant que gelatines inferieures au nombre fixe => placement des gelatines avec coordonnees aleatoires

void initialiserGelatines(GrilleBonbons *grille)
{

    int nombreGelatine = rand() % NIVEAUX[NIVEAUX[0].compteurNiveau].obstacleNiveau.randomGelatine + NIVEAUX[NIVEAUX[0].compteurNiveau].obstacleNiveau.randomGelatine; // Nombre de gelatines aléatoire entre 1 et 5
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
    grille->calcX = 0;
    grille->calcY = 0;
    grille->gelatinePresente = true;
    grille->deplacement = 0;

    initialiserBonbons(grille);   // initialise la grille de bonbons
    initialiserGelatines(grille); // initialise la grille de gelatines
}

/*_______________________________________________________________________________________________________________
                                     **** FONCTION DEPLACEMENT DES BONBONS
   -> Parametres : queue - grille - coordonnees x et y des pions
   -> Reinitialise element de la grille estVerifiee a 0 (il faut reverifier la grille apres le deplacement
   -> Reinitialise element de la grille estInitialisee a 1 (la grille a ete initialisee)
   -> Reinitialise element de la grille calcX et calcY a 0
   -> Echange des pions entre les deux coordonnees
_______________________________________________________________________________________________________________*/

// VA FALLOIR PRENDRE EN COMPTE LE CAS DES ERREURS DE DEPLACEMENT
void Deplacement(Queue *q, GrilleBonbons *grille, int xPion1,
                 int yPion1, int xPion2,
                 int yPion2)

{
    // Reinitialisation des elements de la grille
    grille->estVerifiee = 0;    // On doit re-vérifier la grille après le déplacement
    grille->estInitialisee = 1; // La grille a été initialisée
    grille->deplacement = 1;    // on est en déplacement
    grille->calcX = 0;          // même si possiblement redondant on remet calcX a 0 par securite
    grille->calcY = 0;          // même si possiblement redondant on remet calcX a 0 par securite
    grille->affiche = 1;        // apres le deplacement on doit afficher

    // Echange des pions
    char temp = grille->tableau[xPion1][yPion1].pion;                            // echange des pions via variable temporaire qui stocke le pion 1
    grille->tableau[xPion1][yPion1].pion = grille->tableau[xPion2][yPion2].pion; // pion 1 devient pion 2
    grille->tableau[xPion2][yPion2].pion = temp;                                 // pion 2 devient pion 1

    Actions action = {"CALCUL", {xPion1, yPion1}, {xPion2, yPion2}}; // On ajoute une action CALCUL avec les nouvelles coordonnées
    Enfiler(q, &action);
}

// eventuellement supprimer
bool actionExiste(Queue *q, const char *nom, int x1, int y1, int x2, int y2)
{
    for (int i = q->debut; i != q->fin; i = (i + 1) % LONGUEURQ)
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

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTION DE CALCUL

  -> Parametres : x et y - grille - queue
  -> Verifie si un alignement est present
  -> Si alignement present, ajoute une ACTION SUPPRESSIONV ou SUPPRESSIONH a la queue
___________________________________________________________________________________________________________________
 */

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
            Enfiler(q, &supV);
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
            Enfiler(q, &supH);
            return true;
        }
    }
    return false;
}

/***************************************************************************************************************************
                                 **** FONCTION CENTRALE DE CALCUL DES SUPPRESSIONS ET DES CASCADES

-> parametres : queue - grille - coordonnees des pions
-> Récupere la cellule en cours (x,y sont a 0 si calcul declenche a l initialisation si non on recupere les coordonnees des pions echanges)
-> Si la grille est déjà vérifiée, on envoie une ACTION VERIFICATION
-> Si element de la grille affiche est a 1 on envoie une ACTION AFFICHAGE et on remet directement affiche a 0
   -> Affichage est mis a 1 a chaque fois qu'une suppression est effectuee
   -> Affichage lance une ACTION CALCUL avec coordonnees a 0;
-> Si on trouve un alignement on retourne true -> une ACTION SUPPRESSIONH OU SUPPRESSIONV est ajoutée à la queue
   -> SuppressionV et suppressionH ajoute une ACTION CALCUL à la queue en recommencant a 0
-> Si pas d'alignement on incremente les coordonnees x et y et on ajoute une ACTION CALCUL à la queue
-> Si on a atteint la fin de la grille on envoie une action AFFICHAGE
   -> on remet les coordonnees a 0
   -> on remet estverifiee a 1
   -> Affichage lance une ACTION CALCUL


 ****************************************************************************************************************************/

void Calcul(Queue *q, GrilleBonbons *grille,
            int *x1, int *y1, int *x2, int *y2)
{
    printf("ENTRE DANS CALCUL\n");
    // 1) Récupérer la cellule en cours
    int x = grille->calcX;
    int y = grille->calcY;

    if (grille->affiche && !grille->deplacement)
    {
        Actions aff = {"AFFICHAGE", {0, 0}, {0, 0}, false};
        Enfiler(q, &aff);
        grille->affiche = 0;
        return;
    }

    if (grille->estVerifiee == 1)
    {
        printf("Verification deje faite\n");
        grille->estVerifiee = 0;
        Actions verification = {"VERIFICATION", {0, 0}, {0, 0}, false};
        Enfiler(q, &verification);
        return;
    };

    if (grille->deplacement)
    {
        if (!VerifierAlignements(x1, y1, grille, q) && !VerifierAlignements(x2, y2, grille, q))
        {
            grille->affiche = 0;
            grille->estVerifiee = 1;
        }
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
        printf("Fin du parcours : pas d alignement trouve.\n");
        Actions aff = {"AFFICHAGE", {0, 0}, {0, 0}, false};
        grille->calcX = 0;
        grille->calcY = 0;
        Enfiler(q, &aff);
        grille->estVerifiee = 1;
        return;
    }

    // Sinon, on continue avec la prochaine cellule
    Actions nextCalc = {"CALCUL", {0, 0}, {0, 0}, false};
    Enfiler(q, &nextCalc);

    return;
}

/*________________________________________________________________________________________________________________
                                 **** FONCTION DE VERIFICATION

    -> Parametres : grille - queue
    -> Si le nombre de coups joués est égal au nombre de coups à jouer et qu'il reste de la gélatine
      On affiche un message de fin de partie
    -> Sinon On parcourt la grille et on vérifie si de la gélatine est toujours présente
    -> Si gelatine presente, ajoute une ACTION LECTURE a la queue
    -> Si pas de gelatine, affiche FIN NIVEAU


___________________________________________________________________________________________________________________*/
// transformer en bool et laisser calcul gerer l action supprmier parametre queue
void Verification(GrilleBonbons *grille, Queue *q)
{
    grille->estVerifiee = 1;

    ImprimerQueue(q);

    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
            {
                if (NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues == NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupAJouer)
                {
                    NIVEAUX[0].compteurNiveau = FINALNIVEAU;
                    printf(MESSAGEETATJEU[0]);
                    return;
                }
                printf("Bonbon gelatine toujours present\n");

                Actions action = {"LECTURE", {0, 0}, {0, 0}, false};
                Enfiler(q, &action);
                // supprimer plus pas utile
                return;
            }
        }
    }
    grille->gelatinePresente = false;

    // Si on arrive ici, c'est qu'il n'y avait pas de gélatine
    printf(MESSAGEETATJEU[1]);

    return;
}

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
    printf("\n Apres suppression et chute des bonbons :\n");
    // afficher_grille(grille);

    // Vérification pour éviter la suppression horizontale en double
    int yDebut = *y1, yFin = *y1;

    // Ajouter une action de recalcul dans la queue
    Actions action = {"CALCUL", {0, 0}, {0, 0}, false};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    grille->deplacement = 0;

    Enfiler(q, &action);
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
    Actions action = {"CALCUL", {0, 0}, {0, 0}, false};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    grille->deplacement = 0;
    Enfiler(q, &action);
    // afficher_grille(grille);

    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}