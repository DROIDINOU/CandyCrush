
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constante.h" // Assurez-vous que GrilleBonbons est défini avant d'utiliser la fonction
#define LONGUEURQ 300  // Taille de la queue

// Déclaration de la structure Coordonnees
typedef struct
{
    int x;
    int y;
} Coordonnees;

// Déclaration de la structure Actions
typedef struct
{
    char actionName[20];
    Coordonnees pion1;
    Coordonnees pion2;
    bool initialisation; // Indique si l'action est une initialisation

} Actions;

typedef struct
{
    Actions elements[LONGUEURQ]; // Tableau de structures Actions
    int debut;                   // Indice de début de la queue
    int fin;                     // Indice de fin de la queue
    int taille;                  // Taille actuelle de la queue
} Queue;

void initialiser_queue(Queue *q);
bool est_vide(Queue *q);
bool est_pleine(Queue *q);
void enfiler(Queue *q, Actions action);
Actions defiler(Queue *q);
void imprimer_queue(Queue *q);
#endif // QUEUE_H
