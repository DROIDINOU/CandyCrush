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
bool aDeuxPionsAdjacents(GrilleBonbons *grille, int lignePion1, int lignePion2);
int GenerationAleatoire(TypeElementRandom type, int maxTentatives);
void initialiserGelatines(GrilleBonbons *grille);
void initialiserBonbons(GrilleBonbons *grille);
void GenererJoker(GrilleBonbons *grille);

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
void AppliquerChuteColonneEntiere(GrilleBonbons *grille, int row, int col, Queue *q);
void SupprimerLigne(GrilleBonbons *grille, int row, Queue *q);
void AppliquerChuteLigneEntiere(GrilleBonbons *grille, int row, Queue *q);
void AppliquerChuteColonnePartielle(GrilleBonbons *grille, int x1, int x2, int col, Queue *q);
void AppliquerChuteHorizontalePartielle(GrilleBonbons *grille, int destRow, int col, Queue *q);
void AppliquerGenerationHaut(GrilleBonbons *grille, int row, int col, Queue *q);

// JOKERS
CouleurBonbons VictoireHorizontaleAJoker(GrilleBonbons *grille, int *ligne, int *colonne1, int *colonne2);
void ActiverJokerHorizontalVictoire(GrilleBonbons *grille, CouleurBonbons joker, Queue *q);

/*_____________________________________________________________________________________________________________________________*/

#endif // QUEUE_H