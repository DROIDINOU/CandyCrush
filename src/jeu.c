#include "raylib.h"
#include "queue.h"
#include "constante.h"
#include "affichage.h"
#include "matrice.h"
#include "jeu.h"
#include <stdbool.h>
#include <string.h>

void initialiserEtatJeu(EtatJeu *etat)
{
    etat->attenteClics = false;
    etat->clicCompteur = 0;
    for (int i = 0; i < 4; i++)
        etat->coordonneesClic[i] = -1;

    etat->etatAttente = false;
    etat->tempsDebutAttente = 0.0;
    etat->dureeAttente = 0.3;

    etat->etatFinNiveau = false;
    etat->tempsDebutFinNiveau = 0.0;
    etat->dureeFinNiveau = 2.5;

    etat->etatFinJeu = false;
    etat->tempsDebutFinJeu = 0.0;
    etat->dureeFinJeu = 2.5;

    etat->niveauPrecedent = -1;

    etat->explosionEnCours = false;
    etat->tempsExplosion = 0.0;
    etat->dureeExplosion = 1.0;             // ✅ explosion visible pendant 1 seconde
    etat->typeExplosion = AUCUNE_EXPLOSION; // vide au début
    etat->pretPourNiveauSuivant = false;
}

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

void verifierFinNiveau(EtatJeu *etat, Queue *q)
{
    // Étape 1 : attendre la fin du timer d'affichage du message
    if (etat->etatFinNiveau &&
        !etat->pretPourNiveauSuivant &&
        (GetTime() - etat->tempsDebutFinNiveau >= etat->dureeFinNiveau))
    {
        printf("⏳ Fin niveau affiché, prêt pour niveau suivant\n");
        etat->pretPourNiveauSuivant = true;
        return; // ← Laisser le message s'afficher encore une frame
    }

    // Étape 2 : déclencher le vrai changement de niveau
    if (etat->pretPourNiveauSuivant)
    {
        printf("➡️ Passage au niveau suivant (compteur = %d)\n", NIVEAUX[0].compteurNiveau);
        InitialiserQueue(q);

        Actions nouvelleAction = {INITIALISATION, {0, 0}, {0, 0}, true};
        Enfiler(q, &nouvelleAction);

        // Reset des flags
        etat->etatFinNiveau = false;
        etat->pretPourNiveauSuivant = false;
    }
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

void gererEtatTemporel(EtatJeu *etat, GrilleBonbons *grille, Queue *q)
{
    if (etat->attenteClics)
    {
        etat->attenteClics = gererClics(grille, &etat->clicCompteur, etat->coordonneesClic, q);
    }

    if (etat->explosionEnCours && GetTime() - etat->tempsExplosion >= etat->dureeExplosion)
    {
        etat->explosionEnCours = false;
    }
}

void afficherEtatsEtFin(EtatJeu *etat, char *buffer, Texture2D *textures, GrilleBonbons *grille, Queue *q, Texture2D explosionTexture)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    afficher_grille(grille, textures, q, explosionTexture, etat);

    if (etat->etatFinNiveau)
    {
        sprintf(buffer, "FIN DU NIVEAU %d !", NIVEAUX[0].compteurNiveau);
        DrawRectangle(200, 350, 600, 150, BLACK);
        DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
        DrawText(buffer, 320, 390, 40, RAYWHITE);
        DrawText("Préparation du niveau suivant...", 260, 440, 25, GRAY);
    }

    if (etat->etatFinJeu)
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawRectangle(200, 350, 600, 150, BLACK);
            DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
            sprintf(buffer, "FIN DU JEU  FELICITATIONS!");
            DrawText(buffer, 220, 390, 30, RAYWHITE);
            DrawText("Appuyez sur [ECHAP] pour quitter", 270, 440, 20, GRAY);
            EndDrawing();

            if (IsKeyPressed(KEY_ESCAPE))
            {
                NIVEAUX[0].compteurNiveau += 1;
                break;
            }
        }
    }

    EndDrawing();
}
