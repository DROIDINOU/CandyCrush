#include <stdio.h>
#include "constante.h"
#include "queue.h"
#include <stdlib.h>
#include <stdbool.h>
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

    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        { // Remplissage avec des couleurs aleatoires (le  nombre des couleurs/bonbons varient en fonction du niveau)
            grille->tableau[ligne][colonne].pion = COULEURS[rand() % NIVEAUX[NIVEAUX[0].compteurNiveau].randomColorModulo];
            grille->tableau[ligne][colonne].gelatine = false; // Par défaut, pas de gelatine
        }
    }
}

// Fixe un nombre aléatoire de gélatines dans la grille
// Initialise les gelatines a faux
// Tant que gelatines inferieures au nombre fixe => placement des gelatines avec coordonnees aleatoires

void initialiserGelatines(GrilleBonbons *grille)
{
    // a ameliorer trop compliqué
    int nombreGelatine = rand() % NIVEAUX[NIVEAUX[0].compteurNiveau].obstacleNiveau.randomObstacle + NIVEAUX[NIVEAUX[0].compteurNiveau].obstacleNiveau.randomObstacle; // Nombre de gelatines aléatoire entre 1 et 5
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

void initialiserGrille(GrilleBonbons *grille, Queue *q)
{
    // Initialisation des éléments de la structure grille
    grille->lignes = TAILLE;
    grille->colonnes = TAILLE;
    grille->estInitialisee = 0;
    grille->estVerifiee = 0;
    grille->calcX = 0;
    grille->calcY = 0;
    grille->deplacement = 0;

    initialiserBonbons(grille);                // initialise la grille de bonbons
    initialiserGelatines(grille);              // initialise la grille de gelatines
    Actions action = {CALCUL, {0, 0}, {0, 0}}; // On ajoute une action CALCUL avec les coordonnées de la première case
    Enfiler(q, &action);
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
    grille->estVerifiee = 0;    // On doit revérifier la grille après le déplacement
    grille->estInitialisee = 1; // La grille a été initialisée
    grille->deplacement = 1;    // on est en déplacement
    grille->calcX = 0;          // même si possiblement redondant on remet calcX a 0 par securite
    grille->calcY = 0;          // même si possiblement redondant on remet calcY a 0 par securite
    grille->affiche = 1;        // apres le deplacement on doit afficher

    // Echange des pions
    char temp = grille->tableau[xPion1][yPion1].pion;                            // echange des pions via variable temporaire qui stocke le pion 1
    grille->tableau[xPion1][yPion1].pion = grille->tableau[xPion2][yPion2].pion; // pion 1 devient pion 2
    grille->tableau[xPion2][yPion2].pion = temp;                                 // pion 2 devient pion 1

    Actions action = {CALCUL, {xPion1, yPion1}, {xPion2, yPion2}}; // On ajoute une action CALCUL avec les nouvelles coordonnées
    Enfiler(q, &action);
}

/*
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
}*/

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTION DE CALCUL - VERIFIER ALIGNEMENTS

  -> Parametres : pointeurs int x et y - pointeur structure grille - pointeur structure queue
  -> Verifie si un alignement vertical ou horizontal est present
  -> Si alignement present, ajoute une ACTION SUPPRESSIONV ou SUPPRESSIONH a la queue et retourne true
     sinon retourne false
___________________________________________________________________________________________________________________
 */

bool VerifierVerticale(int *x, int *y, GrilleBonbons *grille, Queue *q)
{
    char pion = grille->tableau[*x][*y].pion;
    if (pion != ' ')
    {
        int compteur = 1;
        int xDebut = *x, xFin = *x;

        // Vérification vers le bas
        int i = *x + 1;
        while (i < TAILLE && grille->tableau[i][*y].pion == pion)
        {
            compteur++;
            xFin = i;
            i++;
        }

        // Vérification vers le haut
        i = *x - 1;
        while (i >= 0 && grille->tableau[i][*y].pion == pion)
        {
            compteur++;
            xDebut = i;
            i--;
        }

        // Si victoire verticale détectée
        if (compteur >= 3)
        {
            Actions supV = {SUPPRESSIONV, {xDebut, *y}, {xFin, *y}};
            Enfiler(q, &supV);
            return true;
        }
    }
    return false;
}

bool VerifierHorizontale(int *x, int *y, GrilleBonbons *grille, Queue *q)
{
    char pion = grille->tableau[*x][*y].pion;
    if (pion != ' ')
    {
        int compteur = 1;
        int yDebut = *y, yFin = *y;

        // Vérification vers la droite
        int j = *y + 1;
        while (j < TAILLE && grille->tableau[*x][j].pion == pion)
        {
            compteur++;
            yFin = j;
            j++;
        }

        // Vérification vers la gauche
        j = *y - 1;
        while (j >= 0 && grille->tableau[*x][j].pion == pion)
        {
            compteur++;
            yDebut = j;
            j--;
        }

        // Si victoire horizontale détectée
        if (compteur >= 3)
        {
            Actions supH = {SUPPRESSIONH, {*x, yDebut}, {*x, yFin}};
            Enfiler(q, &supH);
            return true;
        }
    }
    return false;
}

bool VerifierAlignements(int *x, int *y, GrilleBonbons *grille, Queue *q)
{
    bool victoireVerticale = VerifierVerticale(x, y, grille, q);
    bool victoireHorizontale = VerifierHorizontale(x, y, grille, q);
    return victoireVerticale || victoireHorizontale;
}

/***************************************************************************************************************************
                         **** FONCTION CENTRALE DE CALCUL DES SUPPRESSIONS ET DES CASCADES

   - LOGIQUE GÉNÉRALE -

   1) Initialisation :
       - Si la fonction est appelée lors de l'initialisation (sans coordonnées spécifiques de pions en paramètre),
        une variable interne est utilisée pour parcourir la grille, de la première à la dernière case.
       - Si aucun alignement n'est détecté sur une case, la fonction passe à la suivante.
       - Si un alignement est trouvé, le parcours recommence à zéro.

   2) Déplacement :
       - Lorsque la fonction est appelée suite à un déplacement, elle utilise les coordonnées des pions échangés.
       - Si aucun alignement n'est détecté à ces coordonnées, cela signifie qu'aucun alignement nouveau n'a été créé
         (puisque la grille était déjà vérifiée avant le déplacement).
       - Si un alignement est trouvé, la vérification recommence depuis la première case.

   - FLUX DE TRAITEMENT -

  -> Paramètres : Pointeur structure Queue -pointeur structure GrilleBonbons
                  - pointeurs vers les coordonnées des pions échangés (x1, y1, x2, y2)

  -> Traitement :
      1) Si `affiche` est activé et qu'on n'est pas en déplacement, une action `AFFICHAGE` est ajoutée à la file.
      2) Si la grille a déjà été vérifiée, une action `VERIFICATION` est ajoutée à la file.
      3) Si la fonction est appelée suite à un déplacement, elle vérifie les alignements créés par l'échange :
         Si aucun alignement n'est trouvé, affiche est désactivé et estVerifiee est activé.
      4) Si un alignement est détecté, une action `SUPPRESSIONH` ou `SUPPRESSIONV` est ajoutée à la file,
         entraînant un recalcul depuis le début.
      5) Si aucun alignement n'est trouvé, on passe à la cellule suivante.
      6) Une fois la grille entièrement parcourue :
          - Une action `AFFICHAGE` est ajoutée à la file.
          - Les coordonnées de calcul sont réinitialisées.
          - La grille est marquée comme vérifiée (estVerifiee = 1).
 ***************************************************************************************************************************/

void Calcul(Queue *q, GrilleBonbons *grille,
            int *x1, int *y1, int *x2, int *y2)
{
    printf("ENTRE DANS CALCUL\n");
    // Récupère la cellule en cours
    int x = grille->calcX; // Variable interne utilisee pour verifier les alignements
    int y = grille->calcY; // // Variable interne utilisee pour verifier les alignements

    // Action affichage si on est pas en deplacement
    if (grille->affiche && !grille->deplacement)
    {
        printf("AFFICHAGE HORS DEPLACEMENT\n");
        Actions aff = {AFFICHAGE, {0, 0}, {0, 0}}; // on utilise pas les coordonnees de l action
        Enfiler(q, &aff);
        grille->affiche = 0;
        return;
    }

    // Si deplacement on verifie si il y a des alignements. Si pas d alignement la grille est verifiee
    // on met estverifiee a 1, affiche a 1 et on reinitialise deplacement a 0 -> ACTION AFFICHAGE
    // AFFICHAGE relance CALCUL qui lance VERIFICATION
    // Si alignements, VerifierAlignements declenche une ACTION DE SUPPRESSION qui relance calcul a partir de 0
    if (grille->deplacement)
    {
        printf("DEPLACEMENT \n");
        if (!VerifierAlignements(x1, y1, grille, q) && !VerifierAlignements(x2, y2, grille, q))
        {

            printf("On a rien trouvé dans déplacement \n");
            grille->estVerifiee = 1;
            grille->deplacement = 0;
            Actions action = {AFFICHAGE, {0, 0}, {0, 0}};
            Enfiler(q, &action);

            return;
        }
        printf("ici on a trouvé un alignement dans déplacement\n");
        return;
    }

    // Si la grille est verifiee on lance l action VERIFICATION
    if (grille->estVerifiee == 1)
    {
        printf("LA GRILLE EST VERIFIEE\n");
        // grille->estVerifiee = 0;
        Actions verification = {VERIFICATION, {0, 0}, {0, 0}};
        Enfiler(q, &verification);
        return;
    };

    // SI LORS DE LA VERIFICATION DES VARIABLES INTERNE DE CALCUL ON TROUVE UN ALIGNEMENT UNE ACTION DE
    //  SUPPRESSION EST LANCEE ET CETTE ACTION RELANCE UNE ACTION DE CALCUL AVEC LA VARIABLE INTERNE REMISE A 0
    if (VerifierAlignements(&x, &y, grille, q))
    {
        printf("ON A TROUVE UN ALIGNEMENT HORS DEPLACEMENT OU APRES UN DEPLACEMENT CONCLUANT \n");
        return;
    };

    // ─────────────────────────────────────────────────
    // Aucune suppression => On passe à la cellule suivante
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
        Actions aff = {AFFICHAGE, {0, 0}, {0, 0}};
        grille->calcX = 0;
        grille->calcY = 0;
        Enfiler(q, &aff);
        grille->estVerifiee = 1;
        return;
    }
    // debug
    // printf("DEBUG - calcX: %d, calcY: %d, deplacement: %d, estVerifiee: %d, affiche: %d\n",
    // grille->calcX, grille->calcY, grille->deplacement, grille->estVerifiee, grille->affiche);
    // Sinon, on continue avec la prochaine cellule
    Actions nextCalc = {CALCUL, {0, 0}, {0, 0}};
    Enfiler(q, &nextCalc);

    return;
}

/*________________________________________________________________________________________________________________
                                 **** FONCTION DE VERIFICATION

    -> Parametres : grille - queue
    -> Si le nombre de coups joués est égal au nombre de coups à jouer et qu'il reste de la gélatine
      On affiche un message de fin de partie. Niveau est au niveau Max
    -> Sinon On parcourt la grille et on vérifie si de la gélatine est toujours présente
    -> Si gelatine presente, ajoute une ACTION LECTURE a la queue
    -> Si pas de gelatine, affiche FIN NIVEAU (A ce stade la queue est vide et on sort de la boucle de main)
____________________________________________________________________________________________________________________*/

// transformer en bool et laisser calcul gerer l action supprmier parametre queue
void Verification(GrilleBonbons *grille, Queue *q)
{

    // VERIFICATION DES GELATINES ET DES COUPS JOUES
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
            {
                // si les coups joues sont epuises fin de partie
                if (NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues == NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupAJouer)
                {
                    NIVEAUX[0].compteurNiveau = FINALNIVEAU;
                    printf(MESSAGEETATJEU[0]);
                    return;
                }
                // si non on place une action de LECTURE pour continuer la manche
                printf("Bonbon gelatine toujours present\n");
                Actions action = {LECTURE, {0, 0}, {0, 0}};
                Enfiler(q, &action);
                return;
            }
        }
    }

    // on passe au niveu suivant
    printf(MESSAGEETATJEU[1]);

    return;
}

/*________________________________________________________________________________________________________________
                                 **** FONCTION DE SUPRESSIONS VERTICALES ET HORIZONTALES

    -> Parametres : Pointeur vers struct grille - pointeurs vers coordonnes des extremites de la ligen a supprimer
    -> Supprimer les lignes de victoires et remplace par un espace vide
    -> Si grille initialisee on supprime les gelatines éventuelles (en phase d'initialisation on les conserve)
    -> faire tomber les bonbons en cascade
       - si il y a des bonbons disponible faire tomber les bonbons
       - si pas de bonbons disponibles générer aléatoirement de nouveaux bonbons

    -> Vu qu'il y a des suppressions on recalcule a partir du début de la grille
       (tous les élement de la grille sont remis a 0 sauf affichage qui est a 1)

    _____________________________________________________________________________________*/

void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    printf(" SUPPRESSIONV  (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);

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
                int index = rand() % NIVEAUX[NIVEAUX[0].compteurNiveau].randomColorModulo;
                grille->tableau[i][*y1].pion = COULEURS[index];
                if (grille->estInitialisee)
                {
                    grille->tableau[i][*y1].gelatine = false;
                }
            }
        }
    }

    // Ajouter une action de recalcul dans la queue
    Actions action = {CALCUL, {0, 0}, {0, 0}};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    grille->deplacement = 0;

    Enfiler(q, &action);
    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}

void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    printf(" SUPPRESSIONH  (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
    // 1️ SUPPRESSION DES BONBONS HORIZONTAUX (remplacement par un espace vide et suppression de la gélatine)
    for (int j = *y1; j <= *y2; j++)
    { // On parcourt la ligne horizontale
        for (int i = *x1; i <= *x2; i++)
        {                                     // Supprimer les bonbons alignés horizontalement
            grille->tableau[i][j].pion = ' '; // Remplacement par un espace vide
            if (grille->estInitialisee)
            {
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
        grille->tableau[0][j].pion = COULEURS[index];
        if (grille->estInitialisee)
        {
            grille->tableau[0][j].gelatine = false; // Par défaut, pas de gélatine
        }
    }

    // Ajouter une action de recalcul dans la queue
    Actions action = {CALCUL, {0, 0}, {0, 0}};
    grille->calcX = 0;
    grille->calcY = 0;
    grille->affiche = 1;
    grille->deplacement = 0;
    Enfiler(q, &action);
    // printf("\n Ajout de l'action CALCUL dans la file d'attente pour (%d,%d) -> (%d,%d)\n", *x1, *y1, *x2, *y2);
}