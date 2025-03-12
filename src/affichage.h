// affichage.h
#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "constante.h" // Assurez-vous que GrilleBonbons est défini avant d'utiliser la fonction

// Déclaration de la fonction afficher_grille
int ObtenirReponseAuMessage(int index);

void afficher_grille(GrilleBonbons *grille, Queue *q);
void LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q);

#endif // AFFICHAGE_H