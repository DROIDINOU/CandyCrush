// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

// Déclaration d'une constante avec extern
#define TAILLE 20
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

extern const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR];
typedef struct {
    char pion;
    bool gelatine;
} Case;

typedef struct {
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
} GrilleBonbons;

#endif // CONSTANTS_H