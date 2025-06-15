
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constante.h"
#include "erreur.h"
#define LONGUEURQ 10

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
    ERREURACTION,
    CHUTECOLONNEENTIERE,
    CHUTELIGNEENTIERE,
    DEUXIEMECHUTECOLONNEPARTIELLE,
    RELANCERCALCUL,
    CHUTECOLONNEPARTIELLE,
    CHUTEHORIZONTALEPARTIELLE,
    GENERATIONHAUT,
    HORIZONTALDJOKER,

} ActionType;

// structure Actions
typedef struct
{
    ActionType actionName; // type d'action
    Coordonnees pion1;
    Coordonnees pion2;
    CodeErreur erreur;
    CouleurBonbons joker; //  Ajout pour transmettre un joker via la queue

} Actions;

// structure Queue
typedef struct
{
    Actions elements[LONGUEURQ]; // Tableau de structures Actions
    int debut;                   // Indice de début de la queue
    int fin;                     // Indice de fin de la queue
    int taille;                  // Taille actuelle de la queue
} Queue;

// Prototypes des fonctions de la queue
void InitialiserQueue(Queue *q);
bool EstVide(Queue *q);
bool EstPleine(Queue *q);
Actions Enfiler(Queue *q, Actions *action);
Actions Defiler(Queue *q);
void ImprimerQueue(Queue *q); // fonction de debugging a supprimer par après
#endif