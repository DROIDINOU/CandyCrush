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

void LirePionsAChanger(GrilleBonbons *grille, int coordX1, int coordY1, int coordX2, int coordY2, Queue *q)
{
    Actions action = {"DEPLACEMENT", {coordX1, coordY1}, {coordX2, coordY2}, false};
    Enfiler(q, &action);
}

void afficher_grille(GrilleBonbons *grille, Texture2D *textures, Queue *q, Texture2D explosionTexture)
{
    int tailleCase = 50;
    int largeurFenetre = 1000;
    int hauteurFenetre = 1000;

    int grilleLargeur = grille->colonnes * tailleCase;
    int grilleHauteur = grille->lignes * tailleCase;
    int offsetX = (largeurFenetre - grilleLargeur) / 2;
    int offsetY = (hauteurFenetre - grilleHauteur) / 2;

    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            int x = offsetX + j * tailleCase;
            int y = offsetY + i * tailleCase;

            Texture2D bonbon = {0};
            switch (grille->tableau[i][j].pion)
            {
            case 'M':
                bonbon = textures[0];
                break;
            case 'R':
                bonbon = textures[1];
                break;
            case 'B':
                bonbon = textures[2];
                break;
            case 'V':
                bonbon = textures[3];
                break;
            case 'J':
                bonbon = textures[4];
                break;
            case 'W':
                bonbon = textures[5];
                break;
            case 'G':
                bonbon = textures[6];
                break;
            case 'Y':
                bonbon = textures[7];
                break;
            case 'P':
                bonbon = textures[8];
                break;
            default:
                continue;
            }

            if (bonbon.id != 0)
            {
                Rectangle src = {0, 0, bonbon.width, bonbon.height};
                Rectangle dst = {x, y, tailleCase, tailleCase};
                Vector2 origin = {0, 0};
                DrawTexturePro(bonbon, src, dst, origin, 0.0f, GRAY);
                DrawRectangleLines(x, y, tailleCase, tailleCase, RED);
            }
            else
            {
                DrawRectangle(x, y, tailleCase, tailleCase, BLACK);
                printf("Erreur : bonbon invalide en (%d, %d)\n", i, j);
            }

            if (grille->tableau[i][j].gelatine)
            {
                DrawRectangle(x, y, tailleCase, tailleCase, Fade(WHITE, 0.4f));
                DrawRectangleLinesEx((Rectangle){x, y, tailleCase, tailleCase}, 2, Fade(DARKGRAY, 0.7f));
            }
        }
    }

    if (strcmp(grille->lastAction, "SUPPRESSIONH") == 0 || strcmp(grille->lastAction, "SUPPRESSIONV") == 0)
    {
        printf("111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
        int x1 = grille->pion1Affiche.x;
        int y1 = grille->pion1Affiche.y;
        int x2 = grille->pion2Affiche.x;
        int y2 = grille->pion2Affiche.y;

        int xMin = (x1 < x2) ? x1 : x2;
        int xMax = (x1 > x2) ? x1 : x2;
        int yMin = (y1 < y2) ? y1 : y2;
        int yMax = (y1 > y2) ? y1 : y2;

        if (strcmp(grille->lastAction, "SUPPRESSIONH") == 0)
        {
            for (int y = yMin; y <= yMax; y++)
            {
                int px = offsetX + y * tailleCase;
                int py = offsetY + x1 * tailleCase;
                Rectangle src = {0, 0, explosionTexture.width, explosionTexture.height};
                Rectangle dst = {px, py, tailleCase, tailleCase}; // ← s’adapte pile à une case
                Vector2 origin = {0, 0};
                DrawTexturePro(explosionTexture, src, dst, origin, 0.0f, WHITE);
            }
        }
        else if (strcmp(grille->lastAction, "SUPPRESSIONV") == 0)
        {
            for (int x = xMin; x <= xMax; x++)
            {
                int px = offsetX + y1 * tailleCase;
                int py = offsetY + x * tailleCase;
                Rectangle src = {0, 0, explosionTexture.width, explosionTexture.height};
                Rectangle dst = {px, py, tailleCase, tailleCase}; // ← s’adapte pile à une case
                Vector2 origin = {0, 0};
                DrawTexturePro(explosionTexture, src, dst, origin, 0.0f, WHITE);
            }
        }
        else if (strcmp(grille->lastAction, "FINNIVEAU") == 0)
        {
            printf("rentre dans fiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
            DrawText("FIN NIVEAU", 350, 400, 40, DARKBLUE);
        }
    }

    if (strcmp(grille->lastAction, "AFFICHAGE") == 0)
    {
        printf("AFFICHAGE\n");
        Actions action = {"CALCUL", {0, 0}, {0, 0}, false};
        Enfiler(q, &action);
    }
}
