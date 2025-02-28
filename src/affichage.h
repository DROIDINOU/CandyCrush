// affichage.h
#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "raylib.h"    // Ajout de cette ligne ✅
#include "constante.h" // Assurez-vous que GrilleBonbons est défini avant d'utiliser la fonction

// Déclaration de la fonction afficher_grille
void afficher_grille(GrilleBonbons *grille, Texture2D *textures, Queue *q);
void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion,
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion,
                       int coordonneeYDeuxiemePion, Queue *q);
#endif // AFFICHAGE_H