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

void afficher_grille(GrilleBonbons *grille, Texture2D *textures, Queue *q, Texture2D explosionTexture, EtatJeu *etat)
{
    // cette idee est bonne mais pas encore trouvee
    /*if (!grille->estVerifiee)
    {
        // Cadre bleu semi-transparent qui couvre tout
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLUE, 0.6f));

        // Message au centre
        DrawRectangle(250, 430, 500, 100, DARKBLUE);
        DrawRectangleLinesEx((Rectangle){250, 430, 500, 100}, 2, WHITE);
        DrawText("Loading grille...", 370, 470, 25, WHITE);
    }*/
    DrawText(TextFormat("GetTime(): %.2f", GetTime()), 10, 10, 20, BLACK);

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

            Texture2D bonbon = textures[grille->tableau[i][j].pion];

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

    if (etat->explosionEnCours &&
        GetTime() - etat->tempsExplosion < etat->dureeExplosion &&
        (etat->typeExplosion == SUPPRESSIONH ||
         etat->typeExplosion == SUPPRESSIONV))
    {
        ActionType type = etat->typeExplosion;
        int l1 = etat->explosionP1.x;
        int c1 = etat->explosionP1.y;
        int l2 = etat->explosionP2.x;
        int c2 = etat->explosionP2.y;

        if (l1 < 0 || c1 < 0 || l2 < 0 || c2 < 0 ||
            l1 >= grille->lignes || l2 >= grille->lignes ||
            c1 >= grille->colonnes || c2 >= grille->colonnes)
        {
            return;
        }

        Rectangle src = {0, 0, explosionTexture.width, explosionTexture.height};

        if (type == SUPPRESSIONH)
        {
            int l = l1;
            int cMin = (c1 < c2) ? c1 : c2;
            int cMax = (c1 > c2) ? c1 : c2;

            for (int c = cMin; c <= cMax; c++)
            {
                int px = offsetX + c * tailleCase;
                int py = offsetY + l * tailleCase;
                Rectangle dst = {px, py, tailleCase, tailleCase};
                DrawTexturePro(explosionTexture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
        else if (type == SUPPRESSIONV)
        {
            int c = c1;
            int lMin = (l1 < l2) ? l1 : l2;
            int lMax = (l1 > l2) ? l1 : l2;

            for (int l = lMin; l <= lMax; l++)
            {
                int px = offsetX + c * tailleCase;
                int py = offsetY + l * tailleCase;
                Rectangle dst = {px, py, tailleCase, tailleCase};
                DrawTexturePro(explosionTexture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
    }

    if (grille->lastAction == AFFICHAGE)
    {
        printf("AFFICHAGE\n");
        Actions action = {CALCUL, {0, 0}, {0, 0}, false};
        Enfiler(q, &action);
    }

    if (!grille->estInitialisee)
    {
        // Recouvre toute la fenêtre avec un fond bleu opaque
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);

        // Affiche le texte par-dessus
        DrawText("Chargement de la grille...", 300, 480, 30, RAYWHITE);
    }
}