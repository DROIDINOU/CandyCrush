// queue.h (ou le nom de ton fichier d'en-tête)
#ifndef MATRICE_H
#define MATRICE_H
#include "queue.h"
#include <stdbool.h>
#include "constante.h"

/***************************************************************************************************************************
                                                  INIT GRILLE
 ****************************************************************************************************************************/
int GenerationAleatoire(TypeElementRandom type, int maxTentatives);
void initialiserGrille(GrilleBonbons *grille, Queue *q);
void initialiserGelatines(GrilleBonbons *grille);
void initialiserBonbons(GrilleBonbons *grille);
void Calcul(Queue *q, GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2);
bool VerifierVerticale(int *x, int *y, GrilleBonbons *grille, Queue *q);
bool VerifierHorizontale(int *x, int *y, GrilleBonbons *grille, Queue *q);
bool VerifierAlignements(int *x, int *y, GrilleBonbons *grille, Queue *q);
void Verification(GrilleBonbons *grille, Queue *q);
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);
void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);
void Deplacement(Queue *q, GrilleBonbons *grille, int xPion1,
                 int yPion1, int xPion2,
                 int yPion2);

/*Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes

/*_____________________________________________________________________________________________________________________________*/

/***************************************************************************************************************************
                                                  MAIN
 ****************************************************************************************************************************/

#endif // QUEUE_H
