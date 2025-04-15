#include "raylib.h"
#include "queue.h"
#include "constante.h"
#include "affichage.h"
#include "matrice.h"
#include <stdbool.h>

void afficherMenuAccueil(bool *jeuDemarre)
{
    while (!WindowShouldClose() && !(*jeuDemarre))
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("CANDY CRUSH CLONE", 350, 400, 40, DARKBLUE);
        DrawText("Appuyez sur ESPACE pour commencer", 300, 500, 30, DARKGRAY);
        if (IsKeyPressed(KEY_SPACE))
            *jeuDemarre = true;
        EndDrawing();
    }
}

bool verifierFinNiveau(bool etatFinNiveau, double tempsDebutFinNiveau, double dureeFinNiveau, Queue *q)
{
    if (etatFinNiveau && (GetTime() - tempsDebutFinNiveau >= dureeFinNiveau))
    {
        InitialiserQueue(q);
        Actions nouvelleAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
        Enfiler(q, &nouvelleAction);
        return false; // désactive l'état
    }
    return etatFinNiveau;
}

bool verifierFinJeu(bool etatFinJeu, double tempsDebutFinJeu, double dureeFinJeu)
{
    if (etatFinJeu && (GetTime() - tempsDebutFinJeu >= dureeFinJeu))
    {
        return false;
    }
    return etatFinJeu;
}

bool gererClics(GrilleBonbons *grille, int *clicCompteur, int coordonneesClic[], Queue *q)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 posSouris = GetMousePosition();
        int tailleCase = 50;
        int grilleLargeur = grille->colonnes * tailleCase;
        int grilleHauteur = grille->lignes * tailleCase;
        int offsetX = (1000 - grilleLargeur) / 2;
        int offsetY = (1000 - grilleHauteur) / 2;

        int xRel = posSouris.x - offsetX;
        int yRel = posSouris.y - offsetY;

        if (xRel >= 0 && xRel < grilleLargeur && yRel >= 0 && yRel < grilleHauteur)
        {
            int colonne = xRel / tailleCase;
            int ligne = yRel / tailleCase;
            coordonneesClic[*clicCompteur * 2] = ligne;
            coordonneesClic[*clicCompteur * 2 + 1] = colonne;
            (*clicCompteur)++;

            if (*clicCompteur == 2)
            {
                LirePionsAChanger(grille, coordonneesClic[0], coordonneesClic[1],
                                  coordonneesClic[2], coordonneesClic[3], q);
                return false; // on désactive attenteClics
            }
        }
    }
    return true;
}