#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // lien vers la bibliothèque pour PlaySound
#include <stdio.h>
#include "constante.h"
#include "matrice.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "affichage.h"
#include "generationaleatoire.h"
#include <string.h>

// grilleestverifiee toujours utile??
// grilleaffiche toujours utile??
// grilleprete a commenter

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
// verifie l existance de lignes de victoires
// se limite a vérification à gauche et au dessus vu que la grille se voit attribuer des bonbons lors de l initialisation
// en partant de la case 0,0 jusqu'à la fin
bool aDeuxPionsAdjacents(GrilleBonbons *grille, int lignePion1, int lignePion2)
{
    int couleur = grille->tableau[lignePion1][lignePion2].pion;
    //  deux pions directement à gauche
    if (lignePion2 >= 2 &&
        grille->tableau[lignePion1][lignePion2 - 1].pion == couleur &&
        grille->tableau[lignePion1][lignePion2 - 2].pion == couleur)
    {
        return true;
    }
    //  deux pions directement au-dessus
    if (lignePion1 >= 2 &&
        grille->tableau[lignePion1 - 1][lignePion2].pion == couleur &&
        grille->tableau[lignePion1 - 2][lignePion2].pion == couleur)
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
    // printf("[BUGTRACK] initialiserGelatines appelée\n");
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
   même si on pourrait très bien directement passer à lecture. Calcul enverra directement à
   verification lors de l'initialisation de la grille)

 ****************************************************************************************************************************/

void initialiserGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu)
{
    // printf("[TRACE] initialiserGrille() appelée — compteurNiveau=%d, coupsJoués=%d\n",
    // NIVEAUX[0].compteurNiveau,
    // NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues);
    // Initialisation des éléments de la structure grille
    grille->lignes = TAILLE;
    grille->colonnes = TAILLE;
    grille->estVerifiee = 0;
    grille->calcX = 0;
    grille->calcY = 0;
    grille->deplacement = 0;
    grille->relancerDepuisDebut = false;
    etatJeu->niveausuivant = 0; // On initialise le niveau suivant à 0
    etatJeu->grillePrete = 1;
    initialiserBonbons(grille);                      // initialise la grille de bonbons
    initialiserGelatines(grille);                    // initialise la grille de gélatine
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
    grille->estVerifiee = 0; // On doit revérifier la grille après le déplacement
    grille->deplacement = 1; // on est en déplacement
    grille->calcX = 0;       // même si possiblement redondant on remet calcX a 0 par securite
    grille->calcY = 0;       // même si possiblement redondant on remet calcY a 0 par securite
    grille->affiche = 1;     // apres le deplacement on doit afficher

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
    if (pion != VIDE && pion != -1)
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
            // ajoute une action de suppression verticale à la queue
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
    if (pion != VIDE && pion != -1)
    {
        int compteur = 1;
        int yDebut = *y, yFin = *y;

        // Vérification vers la droite
        int j = *y + 1;
        while (j < grille->colonnes && grille->tableau[*x][j].pion == pion)
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
            // ajoute une action de suppression horizontale à la queue
            Actions supH = {SUPPRESSIONH, {*x, yDebut}, {*x, yFin}};
            Enfiler(q, &supH);
            return true;
        }
    }
    return false;
}

// verifie si il y a une victoire horizontale ou verticale (pas les deuc en même temps)
bool VerifierAlignements(int *x, int *y, GrilleBonbons *grille, Queue *q)
{
    if (VerifierVerticale(x, y, grille, q))
        return true;

    if (VerifierHorizontale(x, y, grille, q))
        return true;

    return false;
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

    // permet de relancer calcul manuellement depuis le debut de la grille
    if (grille->relancerDepuisDebut)
    {
        grille->calcX = 0;
        grille->calcY = 0;
        grille->relancerDepuisDebut = false;
    }
    // en cas de deplacement, on utilise les coordonnées des pions échangés
    // si rien n est trouvé pas besoin de relancer calcul on passe a vérification (optimisation)
    // si non les chutes relanceront le calcul depuis le debut de la grille
    if (grille->deplacement)
    {
        grille->deplacement = 0; // On traite ce déplacement une seule fois

        bool alignementPion1 = VerifierAlignements(x1, y1, grille, q);

        if (!alignementPion1)
        {
            bool alignementPion2 = VerifierAlignements(x2, y2, grille, q);
            if (!alignementPion2)
            {
                Actions affiche = {AFFICHAGE, {0, 0}, {0, 0}};
                Actions verif = {VERIFICATION, {0, 0}, {0, 0}};
                grille->calcX = 0; // Réinitialisation des coordonnées de calcul
                grille->calcY = 0; // Réinitialisation des coordonnées de calcul
                Enfiler(q, &affiche);
                Enfiler(q, &verif);
            }
        }

        return; // On sort de la fonction car soit pas d alignement et on vérifie soit trouvé et calcul est relancé apres suppression
    }

    // Récupère la cellule en cours
    int x = grille->calcX; // Variable interne utilisee pour verifier les alignements
    int y = grille->calcY; // // Variable interne utilisee pour verifier les alignements

    // SI LORS DE LA VERIFICATION DES VARIABLES INTERNE DE CALCUL ON TROUVE UN ALIGNEMENT UNE ACTION DE
    //  SUPPRESSION EST LANCEE ET CETTE ACTION RELANCERA UNE ACTION DE CALCUL AVEC calcx et y remis a 0
    // d ou l importance du return ici
    if (VerifierAlignements(&x, &y, grille, q))
    {
        return;
    };

    // ─────────────────────────────────────────────────
    // Aucune suppression => On passe à la cellule suivante si on a pas fini la grille
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
        grille->calcX = 0; // Réinitialisation des coordonnées de calcul
        grille->calcY = 0; // Réinitialisation des coordonnées de calcul
        Enfiler(q, &affiche);
        Enfiler(q, &verif);
        return;
    }

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

    if (!etatJeu->grillePrete)
    {
        // printf("[DEBUG] 🛑 Verification ignorée : grillePrete = 0\n");
        return;
    }

    int gelatinePresente = 0;

    // Vérifier toute la grille
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            // Vérifier si la case contient de la gélatine
            if (grille->tableau[i][j].gelatine)
            {
                gelatinePresente = 1;
            }
            // verifie si la case est vide
            if (grille->tableau[i][j].pion == VIDE)
            {
                Enfiler(q, &(Actions){LECTURE, {0, 0}, {0, 0}, false});
                return;
            }
        }
    }

    // Si  Gélatine encore présente
    if (gelatinePresente)
    {

        //  mais que plus de coups disponibles
        if (NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues >=
            NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupAJouer)
        {
            // on met à jour l etat de jeu et on affiche le message de fin de partie
            NIVEAUX[0].compteurNiveau = FINALNIVEAU;
            etatJeu->findepartie = 1;
            etatJeu->coupsepuises = 1;
            Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
            return;
        }

        // si on arrive ici, il reste des coups a jouer et de la gelatine on continue la partie
        etatJeu->niveausuivant = 0;
        Enfiler(q, &(Actions){LECTURE, {0, 0}, {0, 0}, false});
        return;
    }

    // Plus de gélatines : le niveau est réussi

    // Si on est au dernier niveau
    if (NIVEAUX[0].compteurNiveau >= FINALNIVEAU - 1)
    {
        // indicateur de fin de partie et niveau mis à jour et on affiche le message de fin de partie
        NIVEAUX[0].compteurNiveau += 1;
        etatJeu->findepartie = 1;
        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
        return;
    }

    // Si on n'est pas au dernier niveau, on passe au niveau suivant
    NIVEAUX[0].compteurNiveau += 1;
    etatJeu->niveausuivant = 1;
    etatJeu->grillePrete = 0; // empêche la vérif prématurée

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
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

/*------------------------
CASCACADES LIGNE MATCH4
--------------------------*/

// suppression de la ligne entière
void SupprimerColonne(GrilleBonbons *grille, int col, Queue *q)
{
    for (int row = 0; row < grille->lignes; row++)
    {
        grille->tableau[row][col].pion = VIDE;      // suppression du pion
        grille->tableau[row][col].gelatine = false; // suppression de la gelatine
    }

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false}); // Affiche la colonne vide avant chute

    // Lancer la cascade pion par pion en partant du bas
    Enfiler(q, &(Actions){CHUTECOLONNEENTIERE, {grille->lignes - 1, col}, {0, 0}, false});
    // On relance le calcul depuis le début avec calcx et calcy remis à 0
    grille->relancerDepuisDebut = true;
    Enfiler(q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
}

// remplace les bonbons par d 'autres bonbons (sens pas important vu que tout est affiché d'un coup)
void AppliquerChuteColonneEntiere(GrilleBonbons *grille, int row, int col, Queue *q)
{
    if (row < 0)
        return; // fin de la cascade

    int indiceCouleur = GenerationAleatoire(COULEURALEATOIRE, 1); // Génération aléatoire d'une couleur
    grille->tableau[row][col].pion = COULEURS[indiceCouleur];     // Remplacement du pion par un nouveau bonbon
    grille->tableau[row][col].gelatine = false;                   // pas de gelatine pour les nouveaux bonbons

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});                   // Affiche la grille après le remplacement
    Enfiler(q, &(Actions){CHUTECOLONNEENTIERE, {row - 1, col}, {0, 0}, false}); // relance la chute de la colonne pour ligne précédente
}

/*------------------------
CASCACADES COLONNE MATCH4
--------------------------*/
// suppression de la colonne entière
void SupprimerLigne(GrilleBonbons *grille, int row, Queue *q)
{

    for (int col = 0; col < grille->colonnes; col++)
    {
        grille->tableau[row][col].pion = VIDE;      //  Suppression du pion
        grille->tableau[row][col].gelatine = false; // Suppression de la gelatine
    }

    //  Affiche la ligne vide AVANT toute chute
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});

    // déclenche la chute des pions superieurs en un seul coup
    Enfiler(q, &(Actions){CHUTELIGNEENTIERE, {0, row}, {0, 0}, false});
    // On relance le calcul depuis le début avec calcx et calcy remis à 0
    grille->relancerDepuisDebut = true;
    Enfiler(q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
}

// fait tomber les bonbons du dessus d un coup et remplace la premiere ligne par des nouveaux bonbons
void AppliquerChuteLigneEntiere(GrilleBonbons *grille, int row, Queue *q)
{
    // itere sur les lignes superieures et fait tomber pions sur ligne inferieur
    for (int col = 0; col < grille->colonnes; col++)
    {
        for (int i = row - 1; i >= 0; i--)
        {
            grille->tableau[i + 1][col].pion = grille->tableau[i][col].pion;
        }
        // attribue de nouveaux bonbons sur la premiere ligne
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

void AppliquerChuteColonnePartielle(GrilleBonbons *grille, int ligneDebutMatch, int ligneFinMatch, int colonne, Queue *q)
{
    int hauteurMatch = ligneFinMatch - ligneDebutMatch + 1; // Taille du match
    // printf("[DEBUG] Cascade VERTICALE ligneDebut=%d ligneFin=%d col=%d (taille=%d)\n",
    // ligneDebutMatch, ligneFinMatch, colonne, hauteurMatch);

    // Vérification des bornes et de la taille du match
    if (hauteurMatch < 3 || ligneDebutMatch < 0 || ligneFinMatch >= grille->lignes || colonne < 0 || colonne >= grille->colonnes)
    {
        printf("[ERREUR] Paramètres invalides : ligneDebut=%d, ligneFin=%d, col=%d\n", ligneDebutMatch, ligneFinMatch, colonne);
        GererErreurFatale(TYPEINCONNU);
    }

    // 🔽 Faire tomber les pions du dessus
    for (int ligne = ligneDebutMatch - 1; ligne >= 0; ligne--)
    {
        int ligneDest = ligne + hauteurMatch;
        if (ligneDest >= grille->lignes)
            continue;

        grille->tableau[ligneDest][colonne].pion = grille->tableau[ligne][colonne].pion;
        grille->tableau[ligne][colonne].pion = VIDE;

        printf("[ChutePartielle]  Copie de [%d][%d] → [%d][%d] = %d\n",
               ligne, colonne, ligneDest, colonne, grille->tableau[ligneDest][colonne].pion);

        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
        PAUSE(100);
    }

    // Générer les nouveaux pions en haut
    for (int i = hauteurMatch - 1; i >= 0; i--)
    {
        int ligneCible = i;
        int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
        grille->tableau[ligneCible][colonne].pion = couleur;
        grille->tableau[ligneCible][colonne].gelatine = false;

        printf("[Génération] Nouveau bonbon en [%d][%d] = %d\n", ligneCible, colonne, couleur);

        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
        PAUSE(100);
    }
}

void AppliquerChuteHorizontalePartielle(GrilleBonbons *grille, int destRow, int col, Queue *q)
{
    // Vérification des bornes
    if (destRow < 0 || col < 0 || col >= grille->colonnes)
        return;

    if (grille->tableau[destRow][col].pion != VIDE)
        return;

    // Ligne 0 vide → génération directe et enfile action
    if (destRow == 0)
    {
        // printf("[DEBUG] Ligne 0 vide → Enfiler GENERATIONHAUT [%d][%d]\n", 0, col);
        Enfiler(q, &(Actions){
                       GENERATIONHAUT,
                       {0, col},
                       {0, 0},
                       false});
        return;
    }

    // Chercher le premier pion au-dessus de la ligne de destination
    int searchRow = destRow - 1;
    while (searchRow >= 0 && grille->tableau[searchRow][col].pion == VIDE)
        searchRow--;

    if (searchRow < 0)
    {
        // printf("[DEBUG] Aucun pion à copier pour [%d][%d] → Enfiler GENERATIONHAUT\n", destRow, col);
        Enfiler(q, &(Actions){
                       GENERATIONHAUT,
                       {0, col},
                       {0, 0},
                       false});
        return;
    }

    // Effectuer la chute
    grille->tableau[destRow][col].pion = grille->tableau[searchRow][col].pion;
    // grille->tableau[destRow][col].gelatine = false; // ✅ on écrase toute propagation de gélatine
    grille->tableau[searchRow][col].pion = VIDE;
    // grille->tableau[searchRow][col].gelatine = false;

    printf("[ChuteDepuisH] 📦 %d,%d → %d,%d : %d\n", searchRow, col, destRow, col, grille->tableau[destRow][col].pion);

    Enfiler(q, &(Actions){
                   CHUTEHORIZONTALEPARTIELLE,
                   {searchRow, col},
                   {0, 0},
                   false});
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(50);
}

void AppliquerGenerationHaut(GrilleBonbons *grille, int row, int col, Queue *q)
{
    int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
    grille->tableau[row][col].pion = couleur;
    grille->tableau[row][col].gelatine = false;

    printf("[GENERATIONHAUT] [%d][%d] = %d\n", row, col, couleur);

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(100);
    // Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false});
}

/*****************************************************************************************************************************
                   FONCTIONS CENTRALES DE SUPPRESSION VERTICALES ET HORIZONTALE DES LIGNES DE VICTOIRE 3 ET 4+

*****************************************************************************************************************************/

// suppression des victoires verticales (3 et 4+)
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    PlaySound(CHEMINSMUSIQUESCHUTES[NIVEAUX[0].compteurNiveau], NULL, SND_FILENAME | SND_ASYNC);
    Sleep(200); // attendre que le son soit entendu (ajuste en fonction de la longueur du son)
    PlaySound(CHEMINSMUSIQUES[NIVEAUX[0].compteurNiveau], NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    // printf("[DEBUG] SuppressionV de [%d][%d] à [%d][%d]\n", *x1, *y1, *x2, *y2);
    //  Cas Match4 ou +
    if (QuatreALaSuiteVerticale(grille, x1, x2))
    { // supprime la colonne entiere et fais appel aux sous fonctions de cascades
        SupprimerColonne(grille, *y1, q);
        return;
    }
    // Cas Match3
    // coordonnees colonne victoire et ligne de debut et fin
    int colonne = *y1;
    int ligneDebut = *x1;
    int ligneFin = *x2;
    // supprime les bonbons de la partie de colonne et met a VIDE et supprime la gelatine
    for (int ligne = ligneDebut; ligne <= ligneFin; ligne++)
    {
        grille->tableau[ligne][colonne].pion = VIDE;
        grille->tableau[ligne][colonne].gelatine = false;
        // printf("[DEBUG] Suppression manuelle [%d][%d]\n", ligne, colonne);
    }
    // Affiche la partie de colonne vide avant la chute
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    // Fait tomber les trois pions au dessus de la partie de colonne supprimee
    Enfiler(q, &(Actions){CHUTECOLONNEPARTIELLE, {ligneDebut, colonne}, {ligneFin, colonne}, false});
    // On relance le calcul depuis le début avec calcx et calcy remis à 0
    grille->relancerDepuisDebut = true;
    Enfiler(q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
}

// suppression des victoires horizontales (3 et 4+)
void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    PlaySound(CHEMINSMUSIQUESCHUTES[NIVEAUX[0].compteurNiveau], NULL, SND_FILENAME | SND_ASYNC);
    Sleep(200); // attendre que le son soit entendu (ajuste en fonction de la longueur du son)
    PlaySound(CHEMINSMUSIQUES[NIVEAUX[0].compteurNiveau], NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    // printf("[DEBUG] SuppressionH de [%d][%d] à [%d][%d]\n", *x1, *y1, *x2, *y2);

    // Cas Match4 ou +
    if (QuatreALaSuiteHorizontale(grille, y1, y2)) //&& grille->estInitialisee)
    {
        // supprime la ligne entiere et fais appel aux sous fonctions de cascades
        SupprimerLigne(grille, *x1, q);
        return;
    }
    // Cas Match3
    // coordonnees ligne victoire et colonnes de debut et fin
    int ligne = *x1;
    int colonneDebut = *y1;
    int colonneFin = *y2;

    // supprime les bonbons de la partie de ligne et met a VIDE et supprime la gelatine
    for (int coordonneeColonneCible = colonneDebut; coordonneeColonneCible <= colonneFin; coordonneeColonneCible++)
    {
        grille->tableau[ligne][coordonneeColonneCible].pion = VIDE;
        grille->tableau[ligne][coordonneeColonneCible].gelatine = false;
        // printf("[DEBUG] Suppression Match-3 [%d][%d]\n", ligne, coordonneeColonneCible);
    }

    // Affiche la partie de ligne vide avant la chute
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(50);

    // Fait tomber les pions supérieurs
    for (int coordonneeColonneCible = colonneDebut; coordonneeColonneCible <= colonneFin; coordonneeColonneCible++)
    {
        if (ligne > 0)
        {
            // On fait tomber les pions au-dessus de la ligne supprimée
            Enfiler(q, &(Actions){
                           CHUTEHORIZONTALEPARTIELLE,
                           {ligne, coordonneeColonneCible},
                           {0, 0},
                           false});
        }
        else
        {
            // Si on est sur la première ligne, on génère de nouveaux bonbons en haut
            Enfiler(q, &(Actions){
                           GENERATIONHAUT,
                           {ligne, coordonneeColonneCible},
                           {0, 0},
                           false});
        }
    }
    // On relance le calcul depuis le début avec calcx et calcy remis à 0
    grille->relancerDepuisDebut = true;
    Enfiler(q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
}