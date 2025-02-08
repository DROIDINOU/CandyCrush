
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TAILLE 20  // Taille de la queue

typedef struct {
    int x;
    int y;
} Coordonnees;

typedef struct{
    char actionName[20];
    Coordonnees pion1;
    Coordonnees pion2;

} Actions;



typedef struct {
    Actions elements[TAILLE];  // Tableau de structures Actions
    int debut;  // Indice de début de la queue
    int fin;    // Indice de fin de la queue
    int taille; // Taille actuelle de la queue
} Queue;

void initialiser_queue(Queue *q);
bool est_vide(Queue *q);
bool est_pleine(Queue *q);
void enfiler(Queue *q, Actions action);
Actions defiler(Queue *q);
#endif // QUEUE_H

