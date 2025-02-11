// queue.h (ou le nom de ton fichier d'en-tête)
#ifndef MATRICE_H
#define MATRICE_H
#include "queue.h"
#include <stdbool.h>

#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100
#define TAILLE 20

/***************************************************************************************************************************
                                                  INIT GRILLE
 ****************************************************************************************************************************/

typedef struct {
    char pion;
    bool gelatine;
} Case;

typedef struct {
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
} GrilleBonbons;

void initialiser_grille(GrilleBonbons *grille);
void initialiserGrilleGelatine(GrilleBonbons *grille);
int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index);
void Calcul(Queue *q, GrilleBonbons *grille, int x1, int y1, int x2, int y2);
void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion, Queue *q );
bool Verification(GrilleBonbons *grille);
void VerificationInitit(Queue *q, GrilleBonbons *grille);
void SuppressionV(GrilleBonbons *grille, int x1, int y1, int x2, int y2);
void SuppressionH(GrilleBonbons *grille, int x1, int y1, int x2, int y2);




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
