#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"
#include "affichage.h"
#include "raylib.h"



void afficher_grille(GrilleBonbons *grille, Texture2D *textures) {
    int tailleCase = 50;
    int largeurFenetre = 800;
    int hauteurFenetre = 800;

    int grilleLargeur = grille->colonnes * tailleCase;
    int grilleHauteur = grille->lignes * tailleCase;
    int offsetX = (largeurFenetre - grilleLargeur) / 2;
    int offsetY = (hauteurFenetre - grilleHauteur) / 2;

    BeginDrawing();
    ClearBackground(DARKGRAY);

    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            int x = offsetX + j * tailleCase;
            int y = offsetY + i * tailleCase;

            Texture2D bonbonActuel = { 0 };

            switch (grille->tableau[i][j].pion) {
                case 'M': bonbonActuel = textures[0]; break;
                case 'R': bonbonActuel = textures[1]; break;
                case 'B': bonbonActuel = textures[2]; break;
                case 'V': bonbonActuel = textures[3]; break;
                case 'J': bonbonActuel = textures[4]; break;
                default: continue;
            }

            if (bonbonActuel.id != 0) {
                Rectangle source = { 0, 0, bonbonActuel.width, bonbonActuel.height };
                Rectangle dest = { x, y, tailleCase, tailleCase };
                Vector2 origin = { 0, 0 };

                DrawTexturePro(bonbonActuel, source, dest, origin, 0.0f, WHITE);
                DrawRectangleLines(x, y, tailleCase, tailleCase, RED);
            } else {
                DrawRectangle(x, y, tailleCase, tailleCase, BLACK);
            }

            if (grille->tableau[i][j].gelatine) {
                DrawRectangle(x, y, tailleCase, tailleCase, Fade(WHITE, 0.4f));
                DrawRectangleLinesEx((Rectangle){ x, y, tailleCase, tailleCase }, 2, Fade(DARKGRAY, 0.7f));
            }
        }
    }

    EndDrawing();
}
