#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"
#include "affichage.h"  // Inclure l'en-tête d'affichage
#include "raylib.h"  // Chemin relatif vers Raylibs


/*void afficher_grille(GrilleBonbons *grille) {
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            int x = j * 64;
            int y = i * 64;
            
            // Afficher un bonbon (par exemple, un rectangle coloré)
            if (grille->tableau[i][j].pion == 'M') {
                DrawRectangle(x, y, 64, 64, RED);  // Bonbon rouge
            } else {
                DrawRectangle(x, y, 64, 64, BLUE); // Autre bonbon
            }

            // Afficher la gélatine (rectangle violet)
            if (grille->tableau[i][j].gelatine) {
                DrawRectangle(x, y, 64, 64, PURPLE);
            }
        }
    }
}
*/


void afficher_grille(GrilleBonbons *grille) {
    int tailleCase = 40;
    int largeurFenetre = 1280; // Change selon ta fenêtre
    int hauteurFenetre = 1280; // Change selon ta fenêtre

    // Calcul de la taille totale de la grille
    int grilleLargeur = grille->colonnes * tailleCase;
    int grilleHauteur = grille->lignes * tailleCase;

    // Calcul du décalage pour centrer
    int offsetX = (largeurFenetre - grilleLargeur) / 1.5;
    int offsetY = (hauteurFenetre - grilleHauteur) / 2;

    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            int x = offsetX + j * tailleCase;
            int y = offsetY + i * tailleCase;

            if (grille->tableau[i][j].pion == 'M') {
                DrawRectangle(x, y, tailleCase, tailleCase, PURPLE);
            } else if (grille->tableau[i][j].pion == 'R') {
                DrawRectangle(x, y, tailleCase, tailleCase, RED);
            } else if (grille->tableau[i][j].pion == 'B') {
                DrawRectangle(x, y, tailleCase, tailleCase, BLUE);
            }
            else if (grille->tableau[i][j].pion == 'V') {
                DrawRectangle(x, y, tailleCase, tailleCase, GREEN);
            }
            else if (grille->tableau[i][j].pion == 'J') {
                DrawRectangle(x, y, tailleCase, tailleCase, YELLOW);
            }
            
        }
    }
}

    /*printf("Grille de la Gelatine:\n");
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            if (grille->tableau[i][j].gelatine) {
                printf(" %c ", 'G');  // Affichage des cases avec gélatine
            } else {
                printf(" %c ", '.');  // Affichage des cases sans gélatine
            }
        }
        printf("\n");
    }
}*/