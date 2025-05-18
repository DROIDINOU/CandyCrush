#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "matrice.h"
#include "constante.h"
#include "affichage.h"
#include "ressources.h"
#include "raylib.h"
#include <string.h>
#include "jeu.h"

void LirePionsAChanger(GrilleBonbons *grille, int coordX1, int coordY1, int coordX2, int coordY2, Queue *q)
{
    Actions action = {DEPLACEMENT, {coordX1, coordY1}, {coordX2, coordY2}, false};
    Enfiler(q, &action);
}
void afficher_grille(GrilleBonbons *grille,
                     Texture2D *textures,
                     Queue *q,
                     Texture2D explosionTexture,
                     EtatJeu *etat)
{
    // Debug : affichage du temps
    DrawText(TextFormat("GetTime(): %.2f", GetTime()), 10, 10, 20, BLACK);

    const int tailleCase = 50;
    const int largeurWnd = 1000;
    const int hauteurWnd = 1000;
    const int grilleW = grille->colonnes * tailleCase;
    const int grilleH = grille->lignes * tailleCase;
    const int offsetX = (largeurWnd - grilleW) / 2;
    const int offsetY = (hauteurWnd - grilleH) / 2;

    // 1) Dessin de la grille
    for (int i = 0; i < grille->lignes; i++)
    {
        for (int j = 0; j < grille->colonnes; j++)
        {
            int x = offsetX + j * tailleCase;
            int y = offsetY + i * tailleCase;
            char p = grille->tableau[i][j].pion;

            if (p != ' ')
            {
                // Conversion char→indice ; ici on suppose p est déjà un int valide
                Texture2D bonbon = textures[(int)p];
                Rectangle src = {0, 0, bonbon.width, bonbon.height};
                Rectangle dst = {x, y, tailleCase, tailleCase};
                DrawTexturePro(bonbon, src, dst, (Vector2){0, 0}, 0.0f, GRAY);
                DrawRectangleLines(x, y, tailleCase, tailleCase, RED);
            }
            else
            {
                // Case vide
                DrawRectangle(x, y, tailleCase, tailleCase, BLACK);
            }

            if (grille->tableau[i][j].gelatine)
            {
                DrawRectangle(x, y, tailleCase, tailleCase, Fade(WHITE, 0.4f));
                DrawRectangleLinesEx((Rectangle){x, y, tailleCase, tailleCase},
                                     2, Fade(DARKGRAY, 0.7f));
            }
        }
    }

    // 2) Animation d'explosion
    if (etat->explosionEnCours)
    {
        float elapsed = GetTime() - etat->tempsExplosion;
        if (elapsed < etat->dureeExplosion)
        {
            Rectangle src = {0, 0, explosionTexture.width, explosionTexture.height};

            if (etat->typeExplosion == SUPPRESSIONH)
            {
                int l = etat->explosionP1.x;
                int c1 = etat->explosionP1.y;
                int c2 = etat->explosionP2.y;
                int cMin = (c1 < c2 ? c1 : c2);
                int cMax = (c1 > c2 ? c1 : c2);

                for (int c = cMin; c <= cMax; c++)
                {
                    Rectangle dst = {
                        offsetX + c * tailleCase,
                        offsetY + l * tailleCase,
                        tailleCase, tailleCase};
                    DrawTexturePro(explosionTexture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
                }
            }
            else if (etat->typeExplosion == SUPPRESSIONV)
            {
                int c1 = etat->explosionP1.y;
                int l1 = etat->explosionP1.x;
                int l2 = etat->explosionP2.x;
                int lMin = (l1 < l2 ? l1 : l2);
                int lMax = (l1 > l2 ? l1 : l2);

                for (int l = lMin; l <= lMax; l++)
                {
                    Rectangle dst = {
                        offsetX + c1 * tailleCase,
                        offsetY + l * tailleCase,
                        tailleCase, tailleCase};
                    DrawTexturePro(explosionTexture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
                }
            }
        }
        else
        {
            etat->explosionEnCours = false;
        }
    }

    // 3) Enqueue d'un calcul après affichage
    if (grille->lastAction == AFFICHAGE)
    {
        Actions action = {CALCUL, {0, 0}, {0, 0}, false};
        Enfiler(q, &action);
    }

    // 4) Écran de chargement si nécessaire
    if (!grille->estInitialisee)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);
        DrawText("Chargement de la grille...", 300, 480, 30, RAYWHITE);
    }
}
