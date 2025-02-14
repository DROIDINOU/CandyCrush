// affichage.h
#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include "constante.h"  // Assurez-vous que GrilleBonbons est défini avant d'utiliser la fonction
#include "raylib.h"  // Ajoute cette ligne pour inclure Raylib

// Déclaration de la fonction afficher_grille
void afficher_grille(GrilleBonbons *grille, Texture2D *textures);

#endif // AFFICHAGE_H