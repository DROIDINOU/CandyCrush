
#ifndef MAIN_H
#define MAIN_H
#define NIVEAU1 0
#define NIVEAU2 1
#define NIVEAU3 2
#define MaxLongueurNiveau 3
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100
// Inclure les autres fichiers d'en-tête nécessaires
#include "affichage.h"  // Pour afficher_grille
#include "matrice.h"    // Pour initialiser_grille et GrilleBonbons
#include "queue.h"      // Pour Queue et fonctions associées
#include "constante.h"  // Pour les constantes définies dans constante.c

typedef struct {
    int coupAJouer;
    int coupsJoues;   // Le nombre de coups joués par le joueur
} Coups;

typedef struct {
    Coups tableau[MaxLongueurNiveau];
}  Niveaux;




#endif // MAIN_H