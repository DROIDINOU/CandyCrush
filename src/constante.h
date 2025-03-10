#include <stdbool.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define TAILLE 10
#define NOMBREMESSAGES 2
#define MAXLONGUEUR 100
#define LONGUEURMESSAGEETREPONSE 40

#define FINALNIVEAU 3
#define NOMBREMESSAGESJEU 5

extern const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR];
typedef struct
{
    char pion;
    bool gelatine;
} Case;

extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR];

typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int reponseChiffre;
} MessagesEtReponses;

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
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau;
} Niveaux;

extern MessagesEtReponses MessagesReponses[NOMBREMESSAGES];
extern Niveaux niveaux[FINALNIVEAU];
#endif