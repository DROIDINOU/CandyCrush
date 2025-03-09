#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

#define TAILLE 10
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100
#define FINALNIVEAU 3
#define NOMBREMESSAGESJEU 4

extern const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR];
typedef struct
{
    char pion;
    bool gelatine;
} Case;

extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR];

typedef struct
{
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
    int calcX; // Coordonnée X qu’on est en train de vérifier
    int calcY; // Coordonnée Y qu’on est en train de vérifier
    int estVerifiee;
    int estInitialisee;  // Indique si la grille a déjà été vérifiée
    int calculsRestants; // Nombre d'actions CALCUL en attente
    int affiche;
    bool gelatinePresente;

} GrilleBonbons;

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
    Coups coupsNiveau;        //  Juste UN seul coup par niveau
    Obstacles obstacleNiveau; //  Juste UN seul obstacle par niveau
    int compteurNiveau;       //  Niveau actuel
} Niveaux;

extern Niveaux niveaux[FINALNIVEAU];
#endif // CONSTANTS_H