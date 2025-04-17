// verifier si pas de include non nécessaires !!!
#ifndef MATRICE_H
#define MATRICE_H
#include "queue.h"
#include <stdbool.h>
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

/***************************************************************************************************************************
                                                  Prototypes de fonctions
                                                -> InitialiserGrille
                                                -> initialiserGelatines
                                                -> ObtenirReponseAuMessage
                                                -> Calcul
                                                -> Verification
                                                -> SuppressionV
                                                -> SuppressionH
                                                -> Deplacement

 ****************************************************************************************************************************/

void initialiser_grille(GrilleBonbons *grille);
void initialiserGelatines(GrilleBonbons *grille);
int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index);
void Calcul(Queue *q, GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2);

Actions Verification(GrilleBonbons *grille, Queue *q);
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);
void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);
void Deplacement(Queue *q, GrilleBonbons *grille, int coordonneeXPremierPion,
                 int coordonneeYPremierPion, int coordonneeXDeuxiemePion,
                 int coordonneeYDeuxiemePion);

#endif // QUEUE_H