#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"
#include "affichage.h"
#include "raylib.h"
#include <string.h>

void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion,
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion,
                       int coordonneeYDeuxiemePion, Queue *q)
{
    // ajouter action ici dans la queue;
    Actions action = {"DEPLACEMENT", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    enfiler(q, action);
}

void afficher_grille(GrilleBonbons *grille, Texture2D *textures, Queue *q)
{

    int tailleCase = 50;
    int largeurFenetre = 1000;
    int hauteurFenetre = 1000;

    int grilleLargeur = grille->colonnes * tailleCase;
    int grilleHauteur = grille->lignes * tailleCase;
    int offsetX = (largeurFenetre - grilleLargeur) / 2;
    int offsetY = (hauteurFenetre - grilleHauteur) / 2;

    // Pas de BeginDrawing() ni EndDrawing() ici !

    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            int x = offsetX + j * tailleCase;
            int y = offsetY + i * tailleCase;

            Texture2D bonbonActuel = {0};

            switch (grille->tableau[i][j].pion)
            {
            case 'M':
                bonbonActuel = textures[0];
                break;
            case 'R':
                bonbonActuel = textures[1];
                break;
            case 'B':
                bonbonActuel = textures[2];
                break;
            case 'V':
                bonbonActuel = textures[3];
                break;
            case 'J':
                bonbonActuel = textures[4];
                break;
            default:
                continue;
            }

            if (bonbonActuel.id != 0)
            {
                Rectangle source = {0, 0, bonbonActuel.width, bonbonActuel.height};
                Rectangle dest = {x, y, tailleCase, tailleCase};
                Vector2 origin = {0, 0};

                DrawTexturePro(bonbonActuel, source, dest, origin, 0.0f, GRAY);
                DrawRectangleLines(x, y, tailleCase, tailleCase, RED);

                // Message de débogage pour l'affichage du bonbon
                // printf("Affichage bonbon à la position (%d, %d) avec type %c\n", x, y, grille->tableau[i][j].pion);
            }
            else
            {
                DrawRectangle(x, y, tailleCase, tailleCase, BLACK);
                printf("Erreur : type de bonbon invalide à la position (%d, %d)\n", i, j);
            }

            if (grille->tableau[i][j].gelatine)
            {
                DrawRectangle(x, y, tailleCase, tailleCase, Fade(WHITE, 0.4f));
                DrawRectangleLinesEx((Rectangle){x, y, tailleCase, tailleCase}, 2, Fade(DARKGRAY, 0.7f));
            }
        }
    }

    if (strcmp(grille->lastAction, "AFFICHAGE") == 0)
    {
        printf("AFFICHAGE\n");
        Actions actionAffichage = {"CALCUL", {0, 0}, {0, 0}, false};
        enfiler(q, actionAffichage);
    }
}
