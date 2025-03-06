
#ifndef MAIN_H
#define MAIN_H
#define NIVEAU1 0
#define NIVEAU2 1
#define NIVEAU3 2
#define MaxLongueurNiveau 3
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100
// Inclure les autres fichiers d'en-tête nécessaires
#include "affichage.h" // Pour afficher_grille
#include "matrice.h"   // Pour initialiser_grille et GrilleBonbons
#include "queue.h"     // Pour Queue et fonctions associées
#include "constante.h" // Pour les constantes définies dans constante.c

int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index);

typedef struct
{
    int coupAJouer;
    int coupsJoues; // Le nombre de coups joués par le joueur
} Coups;

typedef struct
{
    char typeObstacle[20];
    int randomGelatine;

} Obstacles;

typedef struct
{
    Coups coupsNiveau[MaxLongueurNiveau];
    Obstacles obstacleNiveau[MaxLongueurNiveau]; // bool verificationFaite;  // a rajouter pour se passer du boolean en params dans verificationinit et calcul

} Niveaux;

/* CECI EST PEUT ETRE MIEUX typedef struct {
    int coupAJouer;   // Nombre total de coups possibles
    int coupsJoues;   // Nombre de coups effectués
} Coups;

typedef struct {
    Coups coups;                  // Infos sur les coups pour ce niveau
    bool verificationFaite;        // Indique si la vérification a été faite
} Niveaux;/*/

#endif // MAIN_H