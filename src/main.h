
#ifndef MAIN_H
#define MAIN_H
#define NIVEAU1 0
#define NIVEAU2 1
#define NIVEAU3 2
// Inclure les autres fichiers d'en-tête nécessaires
#include "affichage.h" // Pour afficher_grille
#include "matrice.h"   // Pour initialiser_grille et GrilleBonbons
#include "queue.h"     // Pour Queue et fonctions associées
#include "constante.h" // Pour les constantes définies dans constante.c

int ObtenirReponseAuMessage(int index);
void LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2);

/* CECI EST PEUT ETRE MIEUX typedef struct {
    int coupAJouer;   // Nombre total de coups possibles
    int coupsJoues;   // Nombre de coups effectués
} Coups;

typedef struct {
    Coups coups;                  // Infos sur les coups pour ce niveau
    bool verificationFaite;        // Indique si la vérification a été faite
} Niveaux;/*/

#endif // MAIN_H