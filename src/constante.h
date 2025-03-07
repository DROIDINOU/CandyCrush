#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

#define TAILLE 10
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

extern const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR];
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
    int calcX; // Coordonnée X qu’on est en train de vérifier
    int calcY; // Coordonnée Y qu’on est en train de vérifier
    int estVerifiee;
    int estInitialisee;  // Indique si la grille a déjà été vérifiée
    int calculsRestants; // Nombre d'actions CALCUL en attente
    int affiche;

} GrilleBonbons;

#endif // CONSTANTS_H