#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"
#include "affichage.h"  // Inclure l'en-tête d'affichage
#include "raylib.h"  // Chemin relatif vers Raylibs


void afficher_grille(GrilleBonbons *grille) {
    int tailleCase = 40;
    int largeurFenetre = 1280;
    int hauteurFenetre = 1280;

    // Calcul du centrage de la grille
    int grilleLargeur = grille->colonnes * tailleCase;
    int grilleHauteur = grille->lignes * tailleCase;
    int offsetX = (largeurFenetre - grilleLargeur) / 1.5;
    int offsetY = (hauteurFenetre - grilleHauteur) / 2;

    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            int x = offsetX + j * tailleCase;
            int y = offsetY + i * tailleCase;

            Color couleurBonbon = WHITE; // Couleur par défaut

            // Déterminer la couleur du bonbon
            if (grille->tableau[i][j].pion == 'M') {
                couleurBonbon = PURPLE;
            } else if (grille->tableau[i][j].pion == 'R') {
                couleurBonbon = RED;
            } else if (grille->tableau[i][j].pion == 'B') {
                couleurBonbon = BLUE;
            } else if (grille->tableau[i][j].pion == 'V') {
                couleurBonbon = GREEN;
            } else if (grille->tableau[i][j].pion == 'J') {
                couleurBonbon = YELLOW;
            }

            // Dessiner le bonbon
            DrawRectangle(x, y, tailleCase, tailleCase, couleurBonbon);

            // Effet gélatine (si la case a de la gélatine)
            if (grille->tableau[i][j].gelatine) {
                // Ajout d'une couche semi-transparente
                DrawRectangle(x, y, tailleCase, tailleCase, Fade(couleurBonbon, 0.5f));

                // Reflet en haut à gauche pour donner un effet glossy
                DrawCircle(x + tailleCase / 4, y + tailleCase / 4, tailleCase / 6, Fade(WHITE, 0.6f));

                // Deuxième reflet plus grand, plus diffus
                DrawCircle(x + tailleCase / 3, y + tailleCase / 3, tailleCase / 4, Fade(WHITE, 0.3f));

                // Bordure douce pour mieux distinguer
                DrawRectangleLinesEx((Rectangle){x, y, tailleCase, tailleCase}, 2, Fade(DARKGRAY, 0.7f));
            }
        }
    }
}
