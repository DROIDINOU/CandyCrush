
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constante.h" // Assurez-vous que GrilleBonbons est défini avant d'utiliser la fonction
#define LONGUEURQ 300  // Taille de la queue

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
void ImprimerQueue(Queue *q);
#endif // QUEUE_H