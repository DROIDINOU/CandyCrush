#ifndef JEU_H
#define JEU_H
#include <stdbool.h>
#include "matrice.h"
#include "queue.h"

void afficherMenuAccueil(bool *jeuDemarre);
bool verifierFinNiveau(bool etatFinNiveau, double tempsDebut, double duree, Queue *q);
bool verifierFinJeu(bool etatFinJeu, double tempsDebut, double duree);
bool gererClics(GrilleBonbons *grille, int *clicCompteur, int coordonneesClic[], Queue *q);

#endif // MAIN_H