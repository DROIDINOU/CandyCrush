#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*________________________________________________________________________________________________________________
                                 **** IMPLEMENTATION DES FONCTIONS DE LA QUEUE

  *** Fonctions : InitialiserQueue - EstVide - EstPleine - Enfiler - Defiler - ImprimerQueue
   ** Parametres : q - action (action seulement pour enfiler)
-> InitialiserQueue : Initialisation de la queue
-> EstVide : Verifie si la queue est vide
-> EstPleine : Verifie si la queue est pleine
> Enfiler :
       - Si la queue est pleine, affiche un message d'erreur et arrete le programme
       - sinon ajoute l'action a la fin de la queue
-> Defiler :
       - Si la queue est vide, retourne une action vide
       - sinon retourne l'action
-> ImprimerQueue : Affiche le contenu de la queue

___________________________________________________________________________________________________________________
 */

void InitialiserQueue(Queue *q)
{
    printf("Initialisation de la queue\n");
    q->debut = 0;
    q->fin = -1;
    q->taille = 0;
}

bool EstVide(Queue *q)
{
    return q->taille == 0;
}

bool EstPleine(Queue *q)
{
    return q->taille == LONGUEURQ;
}

void Enfiler(Queue *q, Actions *action)
{
    if (EstPleine(q))
    {
        fprintf(stderr, "Erreur : la queue est pleine\n");
        exit(EXIT_FAILURE);
    }
    q->fin = (q->fin + 1) % LONGUEURQ;
    q->elements[q->fin] = *action;
    q->taille++;
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
