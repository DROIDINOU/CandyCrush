
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constante.h"
#define LONGUEURQ 300 // Taille de la queue

/*________________________________________________________________________________________________________________
                                 **** STRUTURE ET PROTOTYPES DE LA QUEUE

   -> Structure ErreurQueue : contient les types d'erreurs possibles (queue vide, queue pleine)
   -> Structure Actions : Contient les informations sur l'action à effectuer (type d'action, coordonnées des pions, enum erreur))
   -> Structure Queue : Contient les éléments de la queue (actions), ainsi que les indices de début, fin et taille
   -> Prototypes de fonctions : InitialiserQueue, EstVide, EstPleine, Enfiler, Defiler, ImprimerQueue
___________________________________________________________________________________________________________________
 */

typedef struct
{
    ActionType actionName;
    Coordonnees pion1;
    Coordonnees pion2;
    bool initialisation; // Indicateur pour l'initialisation

} Actions;

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
