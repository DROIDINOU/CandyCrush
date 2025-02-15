// queue.h (ou le nom de ton fichier d'en-tête)
#ifndef MATRICE_H
#define MATRICE_H
#include "queue.h"
#include <stdbool.h>
#include "constante.h"
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

/***************************************************************************************************************************
                                                  INIT GRILLE
 ****************************************************************************************************************************/




void initialiser_grille(GrilleBonbons *grille);
void initialiserGrilleGelatine(GrilleBonbons *grille);
int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index);
void Calcul(Queue *q, GrilleBonbons *grille, int x1, int y1, int x2, int y2, bool *isVerificationInit);
void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion, Queue *q );
Actions Verification(GrilleBonbons *grille,Queue *q);
void VerificationInitit(Queue *q, GrilleBonbons *grille, int *currentX, int *currentY, bool *isVerificationInit);
void SuppressionV(GrilleBonbons *grille, int x1, int y1, int x2, int y2,Queue *q);
void SuppressionH(GrilleBonbons *grille, int x1, int y1, int x2, int y2,Queue *q);
void Deplacement (Queue *q,GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion);




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
