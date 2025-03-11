#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*________________________________________________________________________________________________________________
                                 **** IMPLEMENTATION DES FONCTIONS DE LA QUEUE

  *** Fonctions : InitialiserQueue - EstVide - EstPleine - Enfiler - Defiler - ImprimerQueue
   ** Parametres : sructure Queue - structure Actions
-> InitialiserQueue : Initialisation de la queue
-> EstVide : Verifie si la queue est vide
-> EstPleine : Verifie si la queue est pleine
> Enfiler :
       - Si la queue est pleine, affiche un message d'erreur
       - sinon ajoute l'action a la fin de la queue
-> Defiler :
       - Si la queue est vide, retourne une action vide
       - sinon retourne l'action en tete de la queue (debut)
-> ImprimerQueue : Affiche le contenu de la queue

___________________________________________________________________________________________________________________
 */

// Initialisation des elements de la queue (debut, fin et taille)
void InitialiserQueue(Queue *q)
{
    printf("Initialisation de la queue\n");
    q->debut = 0;
    q->fin = -1;
    q->taille = 0;
}

// Verifie si la queue est vide (taille = 0)
bool EstVide(Queue *q)
{
    return q->taille == 0;
}

// Verifie si la queue est pleine (taille = LONGUEURQ)
bool EstPleine(Queue *q)
{
    return q->taille == LONGUEURQ;
}

void Enfiler(Queue *q, Actions *action)
{
    if (EstPleine(q))
    {
        printf("Erreur : la queue est pleine\n");
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
    q->debut = (q->debut + 1) % LONGUEURQ; // Incrementer le debut de la queue et revenir au debut si on est a la fin
    q->taille--;                           // Decrementer la taille de la queue
    return action;
}

// fonction de debug a supprimer par apres
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
