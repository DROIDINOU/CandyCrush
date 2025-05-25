#include <stdio.h>
#include "constante.h"
#include "matrice.h"
#include "queue.h"
#include <stdlib.h>
#include <stdbool.h>
#include "affichage.h"
#include "generationaleatoire.h"
#include <string.h>

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTIONS D INITIALISER GRILLE

  *** Fonctions : initialiserBonbons - initialiserGelatines - aDeuxPionsAdjacents
  **  Sous fonctions d' initialiserGrille : initialiserBonbons - initialiserGelatines
  *   Sous fonction d' initialiserBonbons : aDeuxPionsAdjacents

  - > aDeuxPionsAdjacents : Verifie si les pions que l'utilisateur souhaite déplacer sont adjacents ou non

  -> initialiserBonbons : place les bonbons aleatoirement dans la grille en utilisant fonction de génération aléatoire
                          definie dans generationaleatoire.h et s'assure que la grille ne contient pas de lignes de victoire
                          (trois pions ou plus alignés horizontalement ou verticalement)
  -> initialiserGelatines : place aleatoirement les gelatines dans la grille en utilisant fonction de génération aléatoire
                            definie dans generationaleatoire.c


___________________________________________________________________________________________________________________
 */

/*---------Sous fonction d'initialiserBonbons*/
// Vérifie que les pions deplaces par joueur sont adjacents ou non
bool aDeuxPionsAdjacents(GrilleBonbons *grille, int position1, int position2)
{
    int couleur = grille->tableau[position1][position2].pion;
    //  deux pions directement à gauche
    if (position2 >= 2 &&
        grille->tableau[position1][position2 - 1].pion == couleur &&
        grille->tableau[position1][position2 - 2].pion == couleur)
    {
        return true;
    }
    //  deux pions directement au-dessus
    if (position1 >= 2 &&
        grille->tableau[position1 - 1][position2].pion == couleur &&
        grille->tableau[position1 - 2][position2].pion == couleur)
    {
        return true;
    }
    return false;
}

// Initialise les bonbons dans la grille
// Remplissage aléatoire des cases avec des couleurs
// Vérifie que les bonbons ne sont pas adjacents (pas de lignes de victoire)
void initialiserBonbons(GrilleBonbons *grille)
{

    for (int ligne = 0; ligne < grille->lignes; ligne++)
    {
        for (int colonne = 0; colonne < grille->colonnes; colonne++)
        { // Remplissage aléatoire des cases avec des couleurs
            do
            {
                grille->tableau[ligne][colonne].pion = GenerationAleatoire(COULEURALEATOIRE, 1);
            } while (aDeuxPionsAdjacents(grille, ligne, colonne));
            // Vérifie que les bonbons ne sont pas adjacents (pas de lignes de victoire)
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
-> appel à la fonction calcul (remarque :on passe par calcul qui est la fonction centrale
   même si on pourrait très bien directement passer à lecture. Calcul enverra directement à verification)

 ****************************************************************************************************************************/

void initialiserGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu)
{
    // Initialisation des éléments de la structure grille
    grille->lignes = TAILLE;
    grille->colonnes = TAILLE;
    grille->estInitialisee = 0;
    grille->estVerifiee = 0;
    grille->calcX = 0;
    grille->calcY = 0;
    grille->deplacement = 0;
    etatJeu->niveausuivant = 0;                      // On initialise le niveau suivant à 0
    initialiserBonbons(grille);                      // initialise la grille de bonbons
    initialiserGelatines(grille);                    // initialise la grille de gélatine
    grille->estInitialisee = 1;                      // VERIFIER SI TOUJOURS UTILE
    Calcul(q, grille, NULL, NULL, NULL, NULL, true); // Appel à calcul avec flag initialisation à true
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

    NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues += 1;           // Incremente le nombre de coups joues
    Actions actionDeplacement = {CALCUL, {xPion1, yPion1}, {xPion2, yPion2}}; // On ajoute une action CALCUL avec les nouvelles coordonnées
    Enfiler(q, &actionDeplacement);
}

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
    if (pion != VIDE)
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
    if (pion != VIDE)
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

//
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
            int *x1, int *y1, int *x2, int *y2, bool initialisation)
{
    // si la grille vient d'être initialisee les calculs ne doivent pas être effectués (pas de lignes de victoires et gelatines
    //  toujours présente) On passe néanmoins par vérification pour respecter flux logique
    if (initialisation)
    {
        Actions aff = {AFFICHAGE, {0, 0}, {0, 0}};
        Enfiler(q, &aff);

        Actions verif = {VERIFICATION, {0, 0}, {0, 0}};
        Enfiler(q, &verif);
        return; // On ne doit pas calculer
    }
    // Récupère la cellule en cours
    int x = grille->calcX; // Variable interne utilisee pour verifier les alignements
    int y = grille->calcY; // // Variable interne utilisee pour verifier les alignements

    // Action affichage si on est pas en deplacement
    // if (grille->affiche && !grille->deplacement)
    //{
    // Actions aff = {AFFICHAGE, {0, 0}, {0, 0}}; // on utilise pas les coordonnees de l action
    // Enfiler(q, &aff);
    // grille->affiche = 0;
    // return;
    //}

    // Si deplacement on verifie si il y a des alignements. Si pas d alignement la grille est verifiee
    // on met estverifiee a 1, affiche a 1 et on reinitialise deplacement a 0 -> ACTION AFFICHAGE
    // AFFICHAGE relance CALCUL qui lance VERIFICATION
    // Si alignements, VerifierAlignements declenche une ACTION DE SUPPRESSION qui relance calcul a partir de 0
    // if (grille->deplacement)
    //{
    // if (!VerifierAlignements(x1, y1, grille, q) && !VerifierAlignements(x2, y2, grille, q))
    //{

    // grille->estVerifiee = 1;
    // grille->deplacement = 0;
    // Actions action = {AFFICHAGE, {0, 0}, {0, 0}};
    // Enfiler(q, &action);

    // return;
    //}
    // return;
    //}

    // Si la grille est verifiee on lance l action VERIFICATION
    // if (grille->estVerifiee == 1)
    //{
    // grille->estVerifiee = 0;
    // Actions verification = {VERIFICATION, {0, 0}, {0, 0}};
    // Enfiler(q, &verification);
    // return;
    //};

    // SI LORS DE LA VERIFICATION DES VARIABLES INTERNE DE CALCUL ON TROUVE UN ALIGNEMENT UNE ACTION DE
    //  SUPPRESSION EST LANCEE ET CETTE ACTION RELANCERA UNE ACTION DE CALCUL AVEC calcx et y remis a 0
    // D OU LA RAISON DU RETURN ICI
    if (VerifierAlignements(&x, &y, grille, q))
    {
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

    // Si on a atteint la fin, on peut enchaîner AFFICHAGE et verification
    if (grille->calcX >= TAILLE)
    {
        Actions affiche = {AFFICHAGE, {0, 0}, {0, 0}};
        Actions verif = {VERIFICATION, {0, 0}, {0, 0}};
        grille->calcX = 0;
        grille->calcY = 0;
        // Enfiler(q, &aff1);
        Enfiler(q, &affiche);

        Enfiler(q, &verif);
        return;
    }
    // debug
    // printf("DEBUG - calcX: %d, calcY: %d, deplacement: %d, estVerifiee: %d, affiche: %d\n",
    // grille->calcX, grille->calcY, grille->deplacement, grille->estVerifiee, grille->affiche);
    // Si on arrive ici on relance calcul (coordonnees ont été incrementees)
    Actions nextCalc = {CALCUL, {0, 0}, {0, 0}, false};
    Enfiler(q, &nextCalc);

    return;
}

/*________________________________________________________________________________________________________________
                                 **** FONCTION DE VERIFICATION

    -> Parametres : grille - queue
    -> Si le nombre de coups joués est égal au nombre de coups à jouer et qu'il reste de la gélatine
      On affiche un message de fin de partie coups epuises.
    -> Sinon On parcourt la grille et on vérifie si de la gélatine est toujours présente
    -> Si gelatine presente, ajoute une ACTION LECTURE a la queue
    -> Si pas de gelatine, affiche FIN NIVEAU (A ce stade la queue est vide et on sort de la boucle de main) et on passe
    au niveau suivant
____________________________________________________________________________________________________________________*/

void Verification(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu)
{

    printf("DEBUG - VERIFICATION\n");
    int gelatinepresente = 0;

    // Vérification des gélatines et des coups joués
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
            {
                gelatinepresente = 1;

                // Si les coups joués sont épuisés → fin de partie
                if (NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues ==
                    NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupAJouer)
                {
                    NIVEAUX[0].compteurNiveau = FINALNIVEAU;
                    etatJeu->findepartie = 1;  // etat servant à l affichage
                    etatJeu->coupsepuises = 1; // etat servant à l affichage

                    Actions actionAffichage = {AFFICHAGE, {0, 0}, {0, 0}};
                    Enfiler(q, &actionAffichage);
                    return;
                }

                etatJeu->niveausuivant = 0; // etat servant à l affichage
            }
        }
    }

    if (gelatinepresente == 1)
    {
        // Manche en cours → action de lecture
        Actions action = {LECTURE, {0, 0}, {0, 0}};
        Enfiler(q, &action);

        return;
    }
    // si plus de gelatine et niveau final atteint fin du jeu
    else if (NIVEAUX[0].compteurNiveau >= FINALNIVEAU)
    {
        etatJeu->findepartie = 1;

        Actions actionAffichage = {AFFICHAGE, {0, 0}, {0, 0}};
        Enfiler(q, &actionAffichage);
        return;
    }

    // Passage au niveau suivant
    etatJeu->niveausuivant = 1;

    Actions actionAffichage = {AFFICHAGE, {0, 0}, {0, 0}};
    Enfiler(q, &actionAffichage);
}

/*________________________________________________________________________________________________________________
                                 **** FONCTION DE SUPRESSIONS VERTICALES ET HORIZONTALES
/**
  GESTION DES SUPPRESSIONS HORIZONTALES ET VERTICALES

  Cette série de fonctions prend en charge la détection et la suppression
  de groupes de 4 bonbons consécutifs (ou plus) sur une même ligne ou
  colonne, ainsi que la cascade et le réapprovisionnement en bonbons, puis
  déclenche le recalcul de l’état de la grille.

  Paramètres généraux (SuppressionH / SuppressionV) :
    - grille : pointeur vers la structure de la grille de bonbons.
    - x1, y1 : coordonnées de la première extrémité du segment à supprimer.
    - x2, y2 : coordonnées de la seconde extrémité du segment.
    - q      : file d’actions pour enchaîner l’opération de recalcul.

  Étapes de traitement :
  1) Détection d’une séquence de 4 bonbons ou plus :
     • Horizontal (SuppressionH) : si y2 – y1 + 1 ≥ 4 → suppression de toute la ligne x1.
     • Vertical   (SuppressionV) : si x2 – x1 + 1 ≥ 4 → suppression de toute la colonne y1.
  2) Suppression partielle sinon :
     • Effacement du seul segment [x1..x2]×[y1..y2].
  3) Nettoyage :
     • Remplacement des cases supprimées par des espaces vides.
     • Suppression éventuelle des gelatines pour les grilles déjà initialisées.
  4) Cascade :
     • Les bonbons au-dessus dégringolent pour combler les vides.
     • Si aucun bonbon n’est disponible, génération aléatoire de nouveaux bonbons
       en haut de chaque colonne concernée.
  5) Enchaînement du recalcul :
     • Réinitialisation des indicateurs calcX, calcY, affiche et deplacement.
     • Enfilement d’une action CALCUL dans la file q.

 ____________________________________________________________________________________*/

/*------------------------------------------------------------------------------------------------------------------------
                                        Sous fonctions de détection des victoires 4+
--------------------------------------------------------------------------------------------------------------------------/*/

// detections des victoires horizontales 4+
bool QuatreALaSuiteHorizontale(GrilleBonbons *grille, int *y1, int *y2)
{
    return ((*y2 - *y1 + 1) >= 4);
}

// detections des victoires verticales 4+
bool QuatreALaSuiteVerticale(GrilleBonbons *grille, int *x1, int *x2)
{
    return ((*x2 - *x1 + 1) >= 4);
}

/*------------------------------------------------------------------------------------------------------------------------
                                        Sous fonctions de cascades pour victoires 4+
--------------------------------------------------------------------------------------------------------------------------/*/

//-------- CASCADES COLONNES

void SupprimerColonne(GrilleBonbons *grille, int col, Queue *q)
{
    // toutes les cases de la colonne sont <videes>
    for (int row = 0; row < grille->lignes; row++)
    {
        grille->tableau[row][col].pion = VIDE;
        // if (grille->estInitialisee)
        grille->tableau[row][col].gelatine = false; // si des gélatines sont présentes elles sont supprimées
    }

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false}); // afficher la suppression

    Enfiler(q, &(Actions){CHUTECOLONNEENTIERE, {
                                                   grille->lignes - 1,
                                                   col,
                                               },
                          {0, 0},
                          false}); // lance AppliquerChuteColonne
    grille->calcX = 0;
    grille->calcY = 0;
    Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false}); // relance calcul
}

// remplace les bonbons par d 'autres bonbons (sens pas important vu que tout est affiché d'un coup)
void AppliquerChuteColonne(GrilleBonbons *grille, int col, Queue *q)
{
    int lignes = grille->lignes;
    for (int row = lignes - 1; row >= 0; row--)
    {
        int indiceCouleur = GenerationAleatoire(COULEURALEATOIRE, 1); // generation couleur aleatoire
        grille->tableau[row][col].pion = COULEURS[indiceCouleur];     // attribution nouvelle couleur a case
        grille->tableau[row][col].gelatine = false;                   // suppression des gelatines
    }

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
}

//-------- CASCADES LIGNES

void SupprimerLigne(GrilleBonbons *grille, int row, Queue *q)
{
    for (int col = 0; col < grille->colonnes; col++)
    {
        grille->tableau[row][col].pion = VIDE;
        if (grille->estInitialisee)
            grille->tableau[row][col].gelatine = false;
    }

    //  Affiche la ligne vide AVANT toute chute
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});

    //  Nouvelle action : déclenche la chute ensuite
    Enfiler(q, &(Actions){CHUTELIGNEENTIERE, {0, row}, {0, 0}, false});
    grille->calcX = 0;
    grille->calcY = 0;
    Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false}); // une fois suppression effectuee on relance calcul
}

// remplace la ligne d'un coup
void AppliquerChuteLigne(GrilleBonbons *grille, int row, Queue *q)
{
    for (int col = 0; col < grille->colonnes; col++)
    {
        for (int i = row - 1; i >= 0; i--)
        {
            grille->tableau[i + 1][col].pion = grille->tableau[i][col].pion;
        }
        int indiceCouleur = GenerationAleatoire(COULEURALEATOIRE, 1);
        grille->tableau[0][col].pion = COULEURS[indiceCouleur];
        grille->tableau[0][col].gelatine = false;
    }

    // Affiche la grille après que tout soit tombé
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
}

/*------------------------------------------------------------------------------------------------------------------------
                                        Sous fonctions de cascades pour victoires 3
--------------------------------------------------------------------------------------------------------------------------/*/

// Cascade Appliquée au victoires 3 (suppression partielle)
// copie la case précédente de la colonne et remonte jusqu'à la case tout au dessus (cascade)
// la case du dessus se voit attribuer une nouvelle couleur aléatoire (en fonction du niveau)
void AppliquerChutePartielle(GrilleBonbons *grille, int row, int col, Queue *q)
{
    // remonter vers la case tout au dessus
    for (int i = row; i > 0; i--)
    {
        grille->tableau[i][col].pion = grille->tableau[i - 1][col].pion; // attribuer valeur de la case située au dessus
    }

    int indiceCouleur = GenerationAleatoire(COULEURALEATOIRE, 1); // indice de la couleur en fonction du niveau
    grille->tableau[0][col].pion = COULEURS[indiceCouleur];       // attribue une nouvelle couleur a la case située tout au dessus
    grille->tableau[0][col].gelatine = false;                     // pas de gélatine

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false}); // enfile une action d'affichage
}

/*****************************************************************************************************************************
                   FONCTIONS CENTRALES DE SUPPRESSION VERTICALES ET HORIZONTALE DES LIGNES DE VICTOIRE 3 ET 4+

*****************************************************************************************************************************/

// suppression des victoires verticales (3 et 4+)
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    // Si victoire verticale avec 4 pions ou plus
    if (QuatreALaSuiteVerticale(grille, x1, x2) && grille->estInitialisee)
    {
        SupprimerColonne(grille, *y1, q); // supprime la colonne et remplace par de nouveaux pions
    }
    else // Cas d'une combinaison verticale de 3
    {
        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});

        // Calcul du nombre de pions à supprimer (ligne de début à ligne de fin, inclus)
        int nbPionsASupprimer = *x2 - *x1 + 1;

        // Suppression des pions et de la gélatine dans la colonne concernée
        for (int i = *x1; i <= *x2; i++)
        {
            grille->tableau[i][*y1].pion = VIDE; // pion vide
            if (grille->estInitialisee)
                grille->tableau[i][*y1].gelatine = false; // suppression des gelatines
        }

        // Enfiler autant de chutes que de pions supprimés
        for (int i = 0; i < nbPionsASupprimer; i++)
        {
            Enfiler(q, &(Actions){CHUTEPARTIELLE, {
                                                      *x2,
                                                      *y1,
                                                  },
                                  {0, 0},
                                  false});
        }
        grille->calcX = 0;
        grille->calcY = 0;
        Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false}); // suppression terminee on relance calcul
    }
}

// suppression des victoires horizontales (3 et 4+)
void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{ // si victoires horizontales 4+
    if (QuatreALaSuiteHorizontale(grille, y1, y2) && grille->estInitialisee)
    {
        SupprimerLigne(grille, *x1, q);
    }
    else // // Cas d'une combinaison horizontale de 3
    {
        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});

        // Suppression des pions et de la gélatine dans la ligne concernée
        for (int j = *y1; j <= *y2; j++)
        {
            grille->tableau[*x1][j].pion = VIDE; // pion vide
            if (grille->estInitialisee)
                grille->tableau[*x1][j].gelatine = false; // suppression des gélatines

            // Faire descendre tout ce qui est au-dessus de la ligne (x1)
            Enfiler(q, &(Actions){CHUTEPARTIELLE, {*x1, j}, {0, 0}, false});
        }
        grille->calcX = 0;
        grille->calcY = 0;
        Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false});
    }
}