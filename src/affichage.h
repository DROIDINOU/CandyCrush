#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdbool.h>
#include "constante.h"
#include "erreur.h"
#include "queue.h"

void pause_ms(int ms); // ✅ Déclaration propre ici
int ObtenirReponseAuMessage(int index);
bool EstPionAdjacent(int x1, int y1, int x2, int y2);
void LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q);
void afficherMessagePleinEcran(const char *texte, int largeur, int hauteur);
void afficherGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu);

#endif