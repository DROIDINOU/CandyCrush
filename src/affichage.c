#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"


void afficher_grille(GrilleBonbons *grille) {
    printf("Grille des Pions:\n");
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            printf(" %c ", grille->tableau[i][j].pion);  // Affichage de la grille des pions
        }
        printf("\n");
    }

    printf("Grille de la Gelatine:\n");
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
}