#ifndef MATRICE_H
#define MATRICE_H
#include "queue.h"
#include <stdbool.h>
#include "constante.h"

/***************************************************************************************************************************
                                                  INITIALISATION GRILLE
 ****************************************************************************************************************************/
void initialiserGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu);

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTION DE Initialiser grille
___________________________________________________________________________________________________________________
 */
bool aDeuxPionsAdjacents(GrilleBonbons *grille, int position1, int position2);
int GenerationAleatoire(TypeElementRandom type, int maxTentatives);
void initialiserGelatines(GrilleBonbons *grille);
void initialiserBonbons(GrilleBonbons *grille);

/***************************************************************************************************************************
                                                  DEPLACEMENT
 ****************************************************************************************************************************/

void Deplacement(Queue *q, GrilleBonbons *grille, int xPion1, int yPion1, int xPion2, int yPion2);

/***************************************************************************************************************************
                                                  CALCUL
 ****************************************************************************************************************************/

void Calcul(Queue *q, GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, bool initialisation);

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTION DE CALCUL - VERIFIER ALIGNEMENTS
___________________________________________________________________________________________________________________
 */

bool VerifierVerticale(int *x, int *y, GrilleBonbons *grille, Queue *q);
bool VerifierHorizontale(int *x, int *y, GrilleBonbons *grille, Queue *q);
bool VerifierAlignements(int *x, int *y, GrilleBonbons *grille, Queue *q);
void Verification(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu);

/***************************************************************************************************************************
                                                  SUPPRESSIONS H ET V
 ****************************************************************************************************************************/
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);
void SuppressionH(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);

/*________________________________________________________________________________________________________________
                                 **** SOUS FONCTION DE SUPPRESSION H ET V (cascades)
___________________________________________________________________________________________________________________
 */
bool QuatreALaSuiteHorizontale(GrilleBonbons *grille, int *y1, int *y2);
bool QuatreALaSuiteVerticale(GrilleBonbons *grille, int *x1, int *x2);
void SupprimerColonne(GrilleBonbons *grille, int row, Queue *q);
void AppliquerChuteColonne(GrilleBonbons *grille, int row, int col, Queue *q);
void SupprimerLigne(GrilleBonbons *grille, int row, Queue *q);
void AppliquerChuteLigne(GrilleBonbons *grille, int row, Queue *q);
// void AppliquerChutePartielle(GrilleBonbons *grille, int row, int col, Queue *q);
void AppliquerChuteVerticalePartielle(GrilleBonbons *grille, int destRow, int col, int limite, Queue *q);
void SuppressionH(GrilleBonbons *grille,
                  int *x1, int *y1, // début
                  int *y2, int *x2, // fin
                  Queue *q);
void SuppressionV(GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2, Queue *q);
void RemplirPremiereLigne(GrilleBonbons *grille, Queue *q);
void LancerCascadeVerticale(GrilleBonbons *grille, int x1, int x2, int col, Queue *q);
void AppliquerChuteVerticaleDepuisH(GrilleBonbons *grille, int destRow, int col, Queue *q);
void AppliquerGenerationHaut(GrilleBonbons *grille, int row, int col, Queue *q);
void GenererCasesMarquees(GrilleBonbons *grille, Queue *q);
void AppliquerSuppressions(GrilleBonbons *grille, Queue *q);
void LancerCascadeHorizontale(Queue *q, int ligne, int y1, int y2);

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