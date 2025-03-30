// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define FINALNIVEAU 3
#include <stdbool.h>

// Déclaration d'une constante avec extern
#define TAILLE 10
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100
#define MAXCOULEUR 9

// PEUT ETRE METTRE CELA DANS MATRICE VU QU ON L UTILISE QUE LA
extern const char COULEURS[MAXCOULEUR];
extern const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR];

typedef struct
{
    int x;
    int y;
} Coordonnees;

typedef struct
{
    char actionName[20];
    Coordonnees pion1;
    Coordonnees pion2;
    bool initialisation; // Indicateur pour l'initialisation

} Actions;

typedef struct
{
    char pion;
    bool gelatine;
} Case;

typedef struct
{
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
    int calcX;           // Coordonnée X qu’on est en train de vérifier
    int calcY;           // Coordonnée Y qu’on est en train de vérifier
    int estVerifiee;     // Indique si la grille a déjà été vérifiée
    int calculsRestants; // Nombre d'actions CALCUL en attente
    int suppressionsRestantes;
    int affiche;
    int finniveau;
    char lastAction[20]; // Ajout pour mémoriser la dernière action
    Coordonnees pion1Affiche;
    Coordonnees pion2Affiche;

} GrilleBonbons;

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    int coupAJouer;
    int coupsJoues; // Le nombre de coups joués par le joueur
} Coups;

typedef struct
{
    char typeObstacle[20]; // EXEMPLE GELATINES MAIS ON VA EN METTRE D 'AUTRES PAR LA SUITE
    int randomObstacle;    // NOMBRE ALEATOIRE D OBSTACLE (A AMELIORER)
} Obstacles;

typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau; // Niveau actuel
    int randomColorModulo;
} Niveaux;

// YABLEAU DE STUCTURES NIVEAU AVEC COMME LONGUEUR LE NOMBRE DE NIVEAUX
extern Niveaux NIVEAUX[FINALNIVEAU];

#endif // CONSTANTS_H