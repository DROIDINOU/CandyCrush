#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void InitialiserQueue(Queue *q)
{
    printf("Initialisation de la queue\n");
    q->debut = 0;
    q->fin = -1;
    q->taille = 0;
}

bool EstVide(Queue *q)
{
    printf("taille de la queue :");
    return q->taille == 0;
}

bool EstPleine(Queue *q)
{
    return q->taille == LONGUEURQ;
}

Actions Enfiler(Queue *q, Actions *action)
{
    if (EstPleine(q))
    {
        printf("Erreur : la queue est pleine\n");
        Actions fullAction;
        strcpy(fullAction.actionName, "QUEUE_VIDE");
        fullAction.pion1.x = 0;
        fullAction.pion1.y = 0;
        fullAction.pion2.x = 0;
        fullAction.pion2.y = 0;
        return fullAction;
    }
    q->fin = (q->fin + 1) % LONGUEURQ; // Incrementer la fin de la queue et revenir au debut si on est a la fin
    q->elements[q->fin] = *action;     // Ajouter l'action
    q->taille++;                       // Incrementer la taille de la queue
}

Actions Defiler(Queue *q)
{
    if (q->taille == 0)
    {
        Actions emptyAction;
        strcpy(emptyAction.actionName, "QUEUE_VIDE");
        emptyAction.pion1.x = 0;
        emptyAction.pion1.y = 0;
        emptyAction.pion2.x = 0;
        emptyAction.pion2.y = 0;
        return emptyAction;
    }
    Actions action = q->elements[q->debut];
    q->debut = (q->debut + 1) % LONGUEURQ;
    q->taille--;
    return action;
}

void ImprimerQueue(Queue *q)
{
    if (EstVide(q))
    {
        printf("La queue est vide.\n");
        return;
    }

    // printf("Contenu de la queue :\n");
    int index = q->debut;
    for (int i = 0; i < q->taille; i++)
    {
        Actions action = q->elements[index];
        printf("Action: %s, Pion1: (%d, %d), Pion2: (%d, %d)\n",
               action.actionName,
               action.pion1.x, action.pion1.y,
               action.pion2.x, action.pion2.y);

        index = (index + 1) % LONGUEURQ;
    }
}
