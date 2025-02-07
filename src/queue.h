#include <stdio.h>
#include <stdlib.h>
#define TAILLE 20  // Taille de la queue





typedef struct {
    int x;
    int y;
} Coordonnees;

typedef struct {
    Coordonnees elements[TAILLE];  // Tableau de structures Coordonnees
    int debut;  // Indice de début de la queue
    int fin;    // Indice de fin de la queue
    int taille; // Taille actuelle de la queue
} Queue;

void initialiser_queue(Queue *q) {
   
}

bool est_vide(Queue *q) {
    
}

bool est_pleine(Queue *q) {
}

void enfiler(Queue *q, Coordonnees c) {
   
}

Coordonnees defiler(Queue *q) {

}


