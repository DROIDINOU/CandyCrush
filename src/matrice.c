#include <stdio.h>
#include "constante.h"
#include "matrice.h"
#include "queue.h"
#include <stdlib.h>
#include <stdbool.h>
#include "affichage.h"
#include "generationaleatoire.h"
#include <string.h>
#define MAX_VICTOIRE 100

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
    printf("[BUGTRACK] 🎯 initialiserGelatines appelée\n");
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
    printf("[TRACE] initialiserGrille() appelée — compteurNiveau=%d, coupsJoués=%d\n",
           NIVEAUX[0].compteurNiveau,
           NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues);
    // Initialisation des éléments de la structure grille
    grille->lignes = TAILLE;
    grille->colonnes = TAILLE;
    grille->estInitialisee = 0;
    grille->estVerifiee = 0;

    grille->calcX = 0;
    grille->calcY = 0;
    grille->deplacement = 0;
    etatJeu->niveausuivant = 0;   // On initialise le niveau suivant à 0
    initialiserBonbons(grille);   // initialise la grille de bonbons
    initialiserGelatines(grille); // initialise la grille de gélatine
    for (int i = 0; i < TAILLE; i++)
        grille->casesAGenerer[i] = false;
    etatJeu->grillePrete = 1;
    printf("[DEBUG] ✅ Grille prête → grillePrete = 1\n");
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
            printf("[DEBUG] ✅ Victoire verticale détectée à (%d,%d) → [%d][%d] à [%d][%d], couleur = %d, compteur = %d\n",
                   *x, *y, *x, xDebut, *x, xFin, pion, compteur);
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

        // ✅ Affichage uniquement en cas de vraie victoire
        if (compteur >= 3)
        {
            printf("[DEBUG] ✅ Victoire horizontale détectée à (%d,%d) → [%d][%d] à [%d][%d], couleur = %d, compteur = %d\n",
                   *x, *y, *x, yDebut, *x, yFin, pion, compteur);

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
        printf("initilialisation dans calcul");
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
    printf("on est ici????????????????????");
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
    printf("[DEBUG] 📍 Verification() appelée — compteurNiveau=%d, coupsJoués=%d\n",
           NIVEAUX[0].compteurNiveau,
           NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues);
    if (!etatJeu->grillePrete)
    {
        printf("[DEBUG] 🛑 Verification ignorée : grillePrete = 0\n");
        return;
    }

    int gelatinePresente = 0;

    // 🔍 Vérifie toute la grille
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].gelatine)
                gelatinePresente = 1;

            if (grille->tableau[i][j].pion == VIDE)
            {
                printf("[DEBUG] ⚠️ Case vide [%d][%d] → relance LECTURE\n", i, j);
                Enfiler(q, &(Actions){LECTURE, {0, 0}, {0, 0}, false});
                return;
            }
        }
    }

    // 🧊 Gélatine encore présente
    if (gelatinePresente)
    {
        printf("[DEBUG] 🧊 Gélatine encore présente\n");

        // ❌ Plus de coups disponibles
        if (NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues >=
            NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupAJouer)
        {
            printf("[DEBUG] ❌ Coups épuisés → fin de partie\n");
            NIVEAUX[0].compteurNiveau = FINALNIVEAU;
            etatJeu->findepartie = 1;
            etatJeu->coupsepuises = 1;
            Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
            return;
        }

        // Partie encore en cours
        printf("[DEBUG] 🔁 Partie en cours — pas de changement de niveau\n");
        etatJeu->niveausuivant = 0;
        Enfiler(q, &(Actions){LECTURE, {0, 0}, {0, 0}, false});
        return;
    }

    // ✅ Plus de gélatines : niveau réussi
    printf("[DEBUG] ✅ Plus de gélatines\n");

    // 🏁 Dernier niveau ?
    if (NIVEAUX[0].compteurNiveau >= FINALNIVEAU)
    {
        printf("[DEBUG] 🏁 Dernier niveau atteint → fin de partie\n");
        etatJeu->findepartie = 1;
        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
        return;
    }

    // 🎉 Passage au niveau suivant
    NIVEAUX[0].compteurNiveau += 1;
    etatJeu->niveausuivant = 1;

    printf("[DEBUG] 🚀 Passage au niveau %d\n", NIVEAUX[0].compteurNiveau);
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

//-------- CASCADES COLONNES
void SupprimerColonne(GrilleBonbons *grille, int col, Queue *q)
{
    for (int row = 0; row < grille->lignes; row++)
    {
        grille->tableau[row][col].pion = VIDE;
        grille->tableau[row][col].gelatine = false;
    }

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});

    // Lancer la cascade pion par pion en partant du bas
    Enfiler(q, &(Actions){CHUTECOLONNEENTIERE, {grille->lignes - 1, col}, {0, 0}, false});
}

// remplace les bonbons par d 'autres bonbons (sens pas important vu que tout est affiché d'un coup)
void AppliquerChuteColonne(GrilleBonbons *grille, int row, int col, Queue *q)
{
    if (row < 0)
        return; // fin de la cascade

    int indiceCouleur = GenerationAleatoire(COULEURALEATOIRE, 1);
    grille->tableau[row][col].pion = COULEURS[indiceCouleur];
    grille->tableau[row][col].gelatine = false;

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    Enfiler(q, &(Actions){CHUTECOLONNEENTIERE, {row - 1, col}, {0, 0}, false});

    if (row == 0)
    {
        Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false});
    }
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

/*****************************************************************************************************************************
                   FONCTIONS CENTRALES DE SUPPRESSION VERTICALES ET HORIZONTALE DES LIGNES DE VICTOIRE 3 ET 4+

*****************************************************************************************************************************/

// suppression des victoires verticales (3 et 4+)

void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q)
{
    printf("[DEBUG] SuppressionV de [%d][%d] à [%d][%d]\n", *x1, *y1, *x2, *y2);

    if (QuatreALaSuiteVerticale(grille, x1, x2) && grille->estInitialisee)
    {
        SupprimerColonne(grille, *y1, q);
        return;
    }

    for (int i = *x1; i <= *x2; i++)
    {
        grille->tableau[i][*y1].pion = -1; // ✅ Marquage temporaire pour ne pas re-détecter
        grille->tableau[i][*y1].gelatine = false;

        if (i == 0)
            grille->casesAGenerer[*y1] = true;

        printf("[DEBUG] Suppression manuelle [%d][%d]\n", i, *y1);
    }

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    Enfiler(q, &(Actions){CHUTEVERTICALE, {*x1, *y1}, {*x2, *y2}, false});
}

void LancerCascadeVerticale(GrilleBonbons *grille, int x1, int x2, int col, Queue *q)
{
    int match = x2 - x1 + 1;
    printf("[DEBUG] Cascade VERTICALE x1=%d x2=%d col=%d (nb=%d)\n", x1, x2, col, match);

    // Étape 1 : faire descendre les pions du haut vers le bas
    for (int ligne = x1 - 1; ligne >= 0; ligne--)
    {
        int ligneDest = ligne + match;
        if (ligneDest >= grille->lignes)
            continue; // sécurité

        grille->tableau[ligneDest][col].pion = grille->tableau[ligne][col].pion;
        grille->tableau[ligneDest][col].gelatine = false; // toujours faux

        // grille->tableau[ligneDest][col].gelatine = grille->tableau[ligne][col].gelatine;

        grille->tableau[ligne][col].pion = VIDE;
        grille->tableau[ligne][col].gelatine = false;

        printf("[ChutePartielle] 📦 Copie de [%d][%d] → [%d][%d] = %d\n",
               ligne, col, ligneDest, col, grille->tableau[ligneDest][col].pion);

        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
        PAUSE(100);
    }

    // générer les nouveaux pions en haut dans l espace libéré
    for (int i = match - 1; i >= 0; i--)
    {
        int ligne_cible = i; // ligne cible pour le nouveau bonbon

        // On enfile une action CHUTEPARTIELLE avec une source "virtuelle" au-dessus (ligne -1)
        Enfiler(q, &(Actions){
                       CHUTEPARTIELLE,
                       {ligne_cible, col},
                       {-1, 0}, // -1 → signal que c’est une génération (inutilisé dans la fonction on va supprimer ca)
                       false});
    }
}

void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *y2, int *x2, Queue *q)
{
    printf("[DEBUG] SuppressionH de [%d][%d] à [%d][%d]\n", *x1, *y1, *x2, *y2);

    // Cas Match-4 ou +
    if (QuatreALaSuiteHorizontale(grille, y1, y2) && grille->estInitialisee)
    {
        SupprimerLigne(grille, *x1, q);
        return;
    }

    int x = *x1;
    int y_start = *y1;
    int y_end = *y2;

    // Étape 1 : Marquer les bonbons supprimés
    for (int y = y_start; y <= y_end; y++)
    {
        grille->tableau[x][y].pion = -1;
        grille->tableau[x][y].gelatine = false;
        printf("[DEBUG] Suppression Match-3 [%d][%d]\n", x, y);
    }

    // Étape 2 : Affichage
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(50);

    // Étape 3 : Chute ou génération
    for (int y = y_start; y <= y_end; y++)
    {
        if (x > 1)
        {
            // Chute vers ligne 1
            Enfiler(q, &(Actions){
                           CHUTEVERTICALEHORIZONTALE,
                           {x, y},
                           {0, 0},
                           false});

            // Anticipation génération ligne 1 après chute
            grille->casesAGenerer[y] = true;
        }
        else
        {
            // Ligne 0 ou 1 : demande génération immédiate
            grille->casesAGenerer[y] = true;

            Enfiler(q, &(Actions){
                           GENERATIONHAUT,
                           {x, y},
                           {0, 0},
                           false});
        }
    }
}

void LancerCascadeHorizontale(Queue *q, int ligne, int y1, int y2)
{
    printf("[DEBUG] 🚀 LancerCascadeHorizontale ligne=%d de y1=%d à y2=%d\n", ligne, y1, y2);

    for (int col = y1; col <= y2; col++)
    {
        for (int d = 0; d < 3; d++) // seulement 3 lignes au-dessus
        {
            int dest = ligne - d;
            int src = dest - 1;

            if (dest < 0)
                continue;

            if (src >= 0)
            {
                printf("[DEBUG] Enfiler CHUTEPARTIELLE pour [%d][%d] (dest), source = [%d][%d]\n", dest, col, src, col);
                Enfiler(q, &(Actions){CHUTEPARTIELLE, {dest, col}, {ligne - 3, 0}, false});
            }
            else
            {
                printf("[DEBUG] ⬆️ Ligne 0 vide → GENERATIONHAUT [%d][%d]\n", dest, col);
                Enfiler(q, &(Actions){GENERATIONHAUT, {dest, col}, {0, 0}, false});
            }
        }
    }
}

void AppliquerChuteVerticalePartielle(GrilleBonbons *grille, int destRow, int col, int limite, Queue *q)
{
    if (destRow < limite)
    {
        printf("[DEBUG] ❌ destRow=%d < limite=%d → on stoppe la chute\n", destRow, limite);
        return;
    }

    int searchRow = destRow - 1;

    // Cherche un pion au-dessus
    while (searchRow >= 0 && grille->tableau[searchRow][col].pion == VIDE)
    {
        searchRow--;
    }

    if (searchRow < 0)
    {
        // Rien trouvé, génération
        int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
        grille->tableau[destRow][col].pion = couleur;
        grille->tableau[destRow][col].gelatine = false;

        printf("[DEBUG] 🆕 Génération en haut [%d][%d] = %d\n", destRow, col, couleur);
        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
        PAUSE(100);
        return;
    }

    // Copier le pion trouvé
    grille->tableau[destRow][col].pion = grille->tableau[searchRow][col].pion;
    grille->tableau[destRow][col].gelatine = false;

    grille->tableau[searchRow][col].pion = VIDE;
    grille->tableau[searchRow][col].gelatine = false;

    printf("[DEBUG] 📦 Copie de [%d][%d] → [%d][%d] = %d\n", searchRow, col, destRow, col, grille->tableau[destRow][col].pion);

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(100);

    // Si on peut, on prépare une chute partielle en-dessous
    if (searchRow >= limite && searchRow > 0)
    {
        printf("[DEBUG] 🔁 Nouvelle chute partielle en [%d][%d] (limite=%d)\n", destRow - 1, col, limite);
        Enfiler(q, &(Actions){CHUTEPARTIELLE, {destRow - 1, col}, {limite, 0}, false});
    }
    else
    {
        printf("[DEBUG] ✅ Chute arrêtée : searchRow=%d, limite=%d\n", searchRow, limite);
    }
}

void AppliquerChuteVerticaleDepuisH(GrilleBonbons *grille, int destRow, int col, Queue *q)
{
    if (destRow < 0 || col < 0 || col >= grille->colonnes)
        return;

    if (grille->tableau[destRow][col].pion != VIDE)
        return;

    // Ligne 0 vide → génération directe
    if (destRow == 0)
    {
        printf("[DEBUG] Ligne 0 vide → Enfiler GENERATIONHAUT [%d][%d]\n", 0, col);
        Enfiler(q, &(Actions){
                       GENERATIONHAUT,
                       {0, col},
                       {0, 0},
                       false});
        return;
    }

    // Chercher un pion au-dessus
    int searchRow = destRow - 1;
    while (searchRow >= 0 && grille->tableau[searchRow][col].pion == VIDE)
        searchRow--;

    if (searchRow < 0)
    {
        printf("[DEBUG] Aucun pion à copier pour [%d][%d] → Enfiler GENERATIONHAUT\n", destRow, col);
        Enfiler(q, &(Actions){
                       GENERATIONHAUT,
                       {0, col},
                       {0, 0},
                       false});
        return;
    }

    // Effectuer la chute
    grille->tableau[destRow][col].pion = grille->tableau[searchRow][col].pion;
    grille->tableau[destRow][col].gelatine = false; // ✅ on écrase toute propagation de gélatine
    grille->tableau[searchRow][col].pion = VIDE;
    grille->tableau[searchRow][col].gelatine = false;

    printf("[ChuteDepuisH] 📦 %d,%d → %d,%d : %d\n", searchRow, col, destRow, col, grille->tableau[destRow][col].pion);

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(50);

    Enfiler(q, &(Actions){
                   CHUTEVERTICALEHORIZONTALE,
                   {searchRow, col},
                   {0, 0},
                   false});
}

void RemplirPremiereLigne(GrilleBonbons *grille, Queue *q)
{
    for (int col = 0; col < TAILLE; col++)
    {
        if (grille->tableau[0][col].pion == VIDE)
        {
            int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
            grille->tableau[0][col].pion = couleur;
            grille->tableau[0][col].gelatine = false;

            printf("[REMPLIRPREMIERELIGNE] 🌟 [%d][%d] = %d\n", 0, col, couleur);
        }
    }

    // Enchaîne un affichage pour visualiser
    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(100);

    // Et relance le calcul pour poursuivre normalement
    Enfiler(q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
}

void AppliquerGenerationHaut(GrilleBonbons *grille, int row, int col, Queue *q)
{
    int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
    grille->tableau[row][col].pion = couleur;
    grille->tableau[row][col].gelatine = false;

    printf("[GENERATIONHAUT] 🎁 [%d][%d] = %d\n", row, col, couleur);

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(100);
    // Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false});
}

void AppliquerChuteHorizontaleParCase(GrilleBonbons *grille, int destRow, int col, Queue *q)
{
    int srcRow = destRow - 1;

    // Si on est tout en haut (aucun pion au-dessus)
    if (srcRow < 0)
    {
        int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
        grille->tableau[destRow][col].pion = couleur;
        grille->tableau[destRow][col].gelatine = false;

        printf("[ChuteHorizontale] 🌈 Génération directe en haut [%d][%d] = %d\n", destRow, col, couleur);
        return;
    }

    // On cherche un pion valide au-dessus
    while (srcRow >= 0 && grille->tableau[srcRow][col].pion == VIDE)
        srcRow--;

    if (srcRow < 0)
    {
        // Aucun pion au-dessus → on génère un bonbon et on ne continue pas
        int couleur = COULEURS[GenerationAleatoire(COULEURALEATOIRE, 1)];
        grille->tableau[destRow][col].pion = couleur;
        grille->tableau[destRow][col].gelatine = false;

        printf("[ChuteHorizontale] ⚠️ Aucun pion trouvé → Génération en [%d][%d] = %d\n", destRow, col, couleur);
        return;
    }

    // Copie du pion trouvé
    grille->tableau[destRow][col].pion = grille->tableau[srcRow][col].pion;
    grille->tableau[destRow][col].gelatine = grille->tableau[srcRow][col].gelatine;

    grille->tableau[srcRow][col].pion = VIDE;
    grille->tableau[srcRow][col].gelatine = false;

    printf("[ChuteHorizontale] 📦 Copie de [%d][%d] → [%d][%d] = %d\n",
           srcRow, col, destRow, col, grille->tableau[destRow][col].pion);

    // ✅ Ne continue que si la source est encore au-dessus de 0
    if (srcRow > 0)
    {
        Enfiler(q, &(Actions){
                       CHUTEHORIZONTALE,
                       {srcRow, col},
                       {0, 0},
                       false});
    }

    Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    PAUSE(100);
}

void GenererCasesMarquees(GrilleBonbons *grille, Queue *q)
{
    for (int col = 0; col < grille->colonnes; col++)
    {
        if (grille->casesAGenerer[col])
        {
            grille->casesAGenerer[col] = false;
            printf("[DEBUG] ✅ Génération déclenchée pour [0][%d]\n", col);

            // 🔍 Vérifie si une gélatine est encore présente en haut (bug potentiel)
            if (grille->tableau[0][col].gelatine)
            {
                printf("[BUG] ❌ Gelatine encore présente en [0][%d] lors de la génération !\n", col);
            }

            Enfiler(q, &(Actions){
                           GENERATIONHAUT,
                           {0, col},
                           {0, 0},
                           false});
        }
    }
}

void AppliquerSuppressions(GrilleBonbons *grille, Queue *q)
{
    bool suppressionEffectuee = false;

    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            int couleur = grille->tableau[i][j].pion;

            if (couleur == VIDE || couleur == -1) // ⛔️ Ne détecte pas sur cases vides ou déjà marquées
                continue;

            // 🔍 Détection VERTICALE 3+
            int x1 = i, x2 = i;
            while (x2 + 1 < grille->lignes && grille->tableau[x2 + 1][j].pion == couleur)
                x2++;

            if ((x2 - x1 + 1) >= 3)
            {
                printf("[DEBUG] ✅ Victoire VERTICALE [%d][%d] → [%d][%d], couleur=%d\n", x1, j, x2, j, couleur);
                SuppressionV(grille, &x1, &j, &x2, &j, q);
                suppressionEffectuee = true;
                i = x2; // saute les lignes déjà modifiées
                continue;
            }

            // 🔍 Détection HORIZONTALE 3+
            int y1 = j, y2 = j;
            while (y2 + 1 < grille->colonnes && grille->tableau[i][y2 + 1].pion == couleur)
                y2++;

            if ((y2 - y1 + 1) >= 3)
            {
                printf("[DEBUG] ✅ Victoire HORIZONTALE [%d][%d] → [%d][%d], couleur=%d\n", i, y1, i, y2, couleur);
                SuppressionH(grille, &i, &y1, &y2, &i, q);
                suppressionEffectuee = true;
                j = y2;   // saute les colonnes déjà modifiées
                continue; // 🔥 AJOUTÉ : empêche les bugs liés au j++ trop tôt
            }
        }
    }

    // ✅ Nettoyage global de tous les marqueurs -1
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].pion == -1)
                grille->tableau[i][j].pion = VIDE;
            grille->tableau[i][j].gelatine = false; // nettoie systématiquement
        }
    }

    // ✅ Génère les bonbons nécessaires (ligne 0)
    GenererCasesMarquees(grille, q);

    // ✅ Vérifie s’il reste des cases vides pour relancer
    bool casesVides = false;
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            if (grille->tableau[i][j].pion == VIDE)
                casesVides = true;
        }
    }

    if (casesVides || suppressionEffectuee)
    {
        Enfiler(q, &(Actions){RELANCERCALCUL, {0, 0}, {0, 0}, false});
    }
    else
    {
        // Aucun calcul → affichage final forcé
        Enfiler(q, &(Actions){AFFICHAGE, {0, 0}, {0, 0}, false});
    }
}
