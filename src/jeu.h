#ifndef JEU_H
#define JEU_H
#include <stdbool.h>
#include "matrice.h"
#include "queue.h"

typedef struct
{
    // Gestion des clics
    bool attenteClics;
    int clicCompteur;
    int coordonneesClic[4];

    // Musique / attente entre actions
    bool etatAttente;
    double tempsDebutAttente;
    double dureeAttente;

    // Fin de niveau
    bool etatFinNiveau;
    double tempsDebutFinNiveau;
    double dureeFinNiveau;

    // Fin de jeu
    bool etatFinJeu;
    double tempsDebutFinJeu;
    double dureeFinJeu;

    // Explosion
    bool explosionEnCours;
    double tempsExplosion;
    double dureeExplosion;    // durée d'affichage (ex: 1.0 sec)
    ActionType typeExplosion; // "SUPPRESSIONH" ou "SUPPRESSIONV"

    // Niveau courant
    int niveauPrecedent;
    Coordonnees explosionP1;
    Coordonnees explosionP2;
    bool pretPourNiveauSuivant;

} EtatJeu;

void initialiserEtatJeu(EtatJeu *etat);
void afficherMenuAccueil(bool *jeuDemarre);
void verifierFinNiveau(EtatJeu *etat, Queue *q);
bool gererClics(GrilleBonbons *grille, int *clicCompteur, int coordonneesClic[], Queue *q);
void gererEtatTemporel(EtatJeu *etat, GrilleBonbons *grille, Queue *q);
void afficherEtatsEtFin(EtatJeu *etat, char *buffer, Texture2D *textures, GrilleBonbons *grille, Queue *q, Texture2D explosionTexture);
bool verifierFinJeu(bool enCours, double debut, double duree);

#endif // MAIN_H