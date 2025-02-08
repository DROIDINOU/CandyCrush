#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define TAILLE 20  // Taille de la queue


typedef struct {
    int x;
    int y;
} Coordonnees;

typedef struct {
    char actionName[20];
    Coordonnees pion1;
    Coordonnees pion2;
} Actions;

typedef struct {
    Actions elements[TAILLE];  // Tableau d'éléments de type Actions
    int debut;
    int fin;
    int taille;
} Queue;

void initialiser_queue(Queue *q) {
    q->debut = 0;
    q->fin = -1;
    q->taille = 0;
}

bool est_vide(Queue *q) {
    return q->taille == 0;
}

bool est_pleine(Queue *q) {
    return q->taille == TAILLE;
}

void enfiler(Queue *q, Actions action) {
    if (est_pleine(q)) {
        fprintf(stderr, "Erreur : la queue est pleine\n");
        exit(EXIT_FAILURE);
    }
    q->fin = (q->fin + 1) % TAILLE;
    q->elements[q->fin] = action;
    q->taille++;
}

Actions defiler(Queue *q) {
    if (est_vide(q)) {
        fprintf(stderr, "Erreur : la queue est vide\n");
        exit(EXIT_FAILURE);
    }
    Actions action = q->elements[q->debut];
    q->debut = (q->debut + 1) % TAILLE;
    q->taille--;
    return action;
}

int main() {
   Queue q;
   return 0;
}
