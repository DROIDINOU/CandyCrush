#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "constante.h"
#include "erreur.h"

int ObtenirReponseAuMessage(int index);
void afficherGrille(GrilleBonbons *grille, Queue *q);
bool EstPionAdjacent(int x1, int y1, int x2, int y2);
bool LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q);

#endif // AFFICHAGE_H