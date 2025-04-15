#ifndef JEU_H
#define JEU_H
#include <stdbool.h>
#include "matrice.h"
#include "queue.h"

typedef struct
{
    bool attenteClics;
    int clicCompteur;
    int coordonneesClic[4];

    bool etatAttente;
    double tempsDebutAttente;
    double dureeAttente;

    bool etatFinNiveau;
    double tempsDebutFinNiveau;
    double dureeFinNiveau;

    bool etatFinJeu;
    double tempsDebutFinJeu;
    double dureeFinJeu;

    int niveauPrecedent;
} EtatJeu;

void initialiserEtatJeu(EtatJeu *etat);
void afficherMenuAccueil(bool *jeuDemarre);
bool verifierFinNiveau(bool etatFinNiveau, double tempsDebut, double duree, Queue *q);
bool verifierFinJeu(bool etatFinJeu, double tempsDebut, double duree);
bool gererClics(GrilleBonbons *grille, int *clicCompteur, int coordonneesClic[], Queue *q);

#endif // MAIN_H