#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define TAILLE 20  // Taille de la queue



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

void enfiler(Queue *q, Coordonnees c) {
    if (est_pleine(q)) {
        fprintf(stderr, "Erreur : la queue est pleine\n");
        exit(EXIT_FAILURE);
    }
    q->fin = (q->fin + 1) % TAILLE;
    q->elements[q->fin] = c;
    q->taille++;
}

Coordonnees defiler(Queue *q) {
    if (est_vide(q)) {
        fprintf(stderr, "Erreur : la queue est vide\n");
        exit(EXIT_FAILURE);
    }
    Coordonnees c = q->elements[q->debut];
    q->debut = (q->debut + 1) % TAILLE;
    q->taille--;
    return c;
}

int main() {
    Queue q;
    initialiser_queue(&q);

    Coordonnees c1 = {1, 2};
    Coordonnees c2 = {3, 4};
    Coordonnees c3 = {5, 6};

    enfiler(&q, c1);
    enfiler(&q, c2);
    enfiler(&q, c3);

    while (!est_vide(&q)) {
        Coordonnees c = defiler(&q);
        printf("Coordonnées : (%d, %d)\n", c.x, c.y);
    }

    return 0;
}
