#ifndef TYPES_H
#define TYPES_H
#include "erreur.h"
// structure Coordonnees
typedef struct
{
    int x;
    int y;
} Coordonnees;

// énumération ActionType
typedef enum
{
    INITIALISATION,
    AFFICHAGE,
    VERIFICATION,
    SUPPRESSIONH,
    SUPPRESSIONV,
    CALCUL,
    DEPLACEMENT,
    LECTURE,
    FINNIVEAU,
    FIN,
    AUCUNE_EXPLOSION,
    AUCUNE_ACTION,

} ActionType;

// structure Actions
typedef struct
{
    ActionType actionName;
    Coordonnees pion1;
    Coordonnees pion2;
    CodeErreur erreur;
    bool initialisation;

} Actions;

#endif
