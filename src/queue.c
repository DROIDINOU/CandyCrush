#include "queue.h"
#include "constante.h"  // Assurez-vous que GrilleBonbons est défini avant d'utiliser la fonction
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // Inclure pour utiliser strcpy et strcmp





void initialiser_queue(Queue *q) {
    printf("Initialisation de la queue\n");
    q->debut = 0;
    q->fin = -1;
    q->taille = 0;
}

bool est_vide(Queue *q) {
    printf("taille de la queue :");
    return q->taille == 0;
}

bool est_pleine(Queue *q) {
    return q->taille == LONGUEUR;
}

void enfiler(Queue *q, Actions action) {
    if (est_pleine(q)) {
        fprintf(stderr, "Erreur : la queue est pleine\n");
        exit(EXIT_FAILURE);
    }
    q->fin = (q->fin + 1) % LONGUEUR;
    q->elements[q->fin] = action;
    q->taille++;
}

Actions defiler(Queue *q) {
    if (q->taille == 0) {
        Actions emptyAction;
        strcpy(emptyAction.actionName, "QUEUE_VIDE");
        emptyAction.pion1.x = 0;
        emptyAction.pion1.y = 0;
        emptyAction.pion2.x = 0;
        emptyAction.pion2.y = 0;
        return emptyAction;
    }
    Actions action = q->elements[q->debut];
    q->debut = (q->debut + 1) % LONGUEUR;
    q->taille--;
    return action;
}

void imprimer_queue(Queue *q) {
    if (est_vide(q)) {
        printf("La queue est vide.\n");
        return;
    }

    //printf("Contenu de la queue :\n");
    int index = q->debut;
    for (int i = 0; i < q->taille; i++) {
        Actions action = q->elements[index];
        printf("Action: %s, Pion1: (%d, %d), Pion2: (%d, %d)\n",
               action.actionName,
               action.pion1.x, action.pion1.y,
               action.pion2.x, action.pion2.y);

        index = (index + 1) % LONGUEUR;
    }
}

/*
int main() {
   Queue q;
   return 0;
}
*/
