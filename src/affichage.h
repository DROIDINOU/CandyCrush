#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "constante.h"

int ObtenirReponseAuMessage(int index);
void afficherGrille(GrilleBonbons *grille, Queue *q);
void LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q);

#endif // AFFICHAGE_H