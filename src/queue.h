
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constante.h"
#define LONGUEURQ 10 // Taille de la queue (théoriquement je pourrait descendre jusque deux ENCORE A TESTER!!)

// Déclaration de la structure Coordonnees
typedef struct
{
    int x;
    int y;
} Coordonnees;

typedef enum
{
    ERREURQUEUEPLEINE, // 0 : Aucun problème
    ERREURQUEUEVIDE,   // 1 : Erreur liée à la mémoire
} CodeErreur;

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
} ActionType;

// Déclaration de la structure Actions
typedef struct
{
    ActionType actionName;
    Coordonnees pion1;
    Coordonnees pion2;
    CodeErreur erreur;
    // bool initialisation; // Indique si l'action est une initialisation

} Actions;

// Déclaration de la structure Queue
typedef struct
{
    Actions elements[LONGUEURQ]; // Tableau de structures Actions
    int debut;                   // Indice de début de la queue
    int fin;                     // Indice de fin de la queue
    int taille;                  // Taille actuelle de la queue
} Queue;

void InitialiserQueue(Queue *q);
bool EstVide(Queue *q);
bool EstPleine(Queue *q);
Actions Enfiler(Queue *q, Actions *action);
Actions Defiler(Queue *q);
void ImprimerQueue(Queue *q); // fonction de debugging a supprimer par après
#endif                        // QUEUE_H
