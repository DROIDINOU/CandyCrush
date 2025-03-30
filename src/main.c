

// === Compilation ===
// cd /c/Users/32471/MySDL2
// gcc main.c matrice.c constante.c queue.c affichage.c -o mon_programme -lraylib
// ./mon_programme.exe

#include "raylib.h"
#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    srand(time(NULL));
    InitWindow(1000, 1000, "Candy Crush Clone");
    InitAudioDevice();

    Sound sound = LoadSound("../assets/gaming-music-8-bit-console-play-background-intro-theme-278382.mp3");
    Music music = LoadMusicStream("../assets/gaming-music-8-bit-console-play-background-intro-theme-278382.mp3");
    Music attenteMusic = LoadMusicStream("../assets/jellysplash_56f256e05113918.mp3");

    SetMusicVolume(music, 0.2f);
    PlayMusicStream(music);
    SetTargetFPS(60);

    bool jeuDemarre = false;
    while (!WindowShouldClose() && !jeuDemarre)
    {
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("CANDY CRUSH CLONE", 350, 400, 40, DARKBLUE);
        DrawText("Appuyez sur ESPACE pour commencer", 300, 500, 30, DARKGRAY);
        if (IsKeyPressed(KEY_SPACE))
            jeuDemarre = true;
        EndDrawing();
    }

    Texture2D textures[5] = {
        LoadTexture("../candyimages/5cd560a569ed85884c879cb1da8e7d68.png"),
        LoadTexture("../candyimages/poignee_de_bonbons.png"),
        LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-candy-crush-candy-crush-5ad0dcad6773e1.4200818515236374214238.png"),
        LoadTexture("../candyimages/580b57fcd9996e24bc43c517.png"),
        LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-lollipop-ga-sweet-cheats-for-candy-crush-saga-1-2-ipa-war4-5b67aab6d41443.6117650115335205668687.png")};
    Texture2D explosionTexture = LoadTexture("../candyimages/circle_01.png");

    int niveau = 0;
    GrilleBonbons maGrille = {.lignes = TAILLE, .colonnes = TAILLE, .estVerifiee = 0, .finniveau = 0};
    Queue q;
    InitialiserQueue(&q);
    Actions initAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
    Enfiler(&q, &initAction);

    bool attenteClics = false;
    int clicCompteur = 0;
    int coordonneesClic[4] = {-1, -1, -1, -1};

    bool etatAttente = false;
    double tempsDebutAttente = 0.0;
    double dureeAttente = 0.6;

    bool etatFinNiveau = false;
    double tempsDebutFinNiveau = 0.0;
    double dureeFinNiveau = 2.5;

    while (!WindowShouldClose() && NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        UpdateMusicStream(music);

        if (etatAttente)
        {
            UpdateMusicStream(attenteMusic);
            if (!IsMusicStreamPlaying(attenteMusic))
            {
                SetMusicVolume(attenteMusic, 1.0f);
                SetMusicVolume(music, 0.0f);
                PlayMusicStream(attenteMusic);
            }
            if ((GetTime() - tempsDebutAttente) >= dureeAttente)
            {
                etatAttente = false;
                StopMusicStream(attenteMusic);
                SetMusicVolume(music, 0.2f);
            }
        }
        else if (q.taille > 0)
        {
            Actions action = Defiler(&q);
            maGrille.suppressionsRestantes = 0;
            strcpy(maGrille.lastAction, action.actionName);
            maGrille.pion1Affiche = action.pion1;
            maGrille.pion2Affiche = action.pion2;

            if (strcmp(action.actionName, "CALCUL") == 0)
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                etatAttente = true;
                tempsDebutAttente = GetTime();
            }
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                etatAttente = true;
                tempsDebutAttente = GetTime();
            }
            else if (strcmp(action.actionName, "VERIFICATION") == 0)
            {
                Verification(&maGrille, &q);
                maGrille.estVerifiee = 1;
            }
            else if (strcmp(action.actionName, "DEPLACEMENT") == 0)
            {
                maGrille.estVerifiee = 0;
                NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues++;
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }
            else if (strcmp(action.actionName, "LECTURE") == 0)
            {
                attenteClics = true;
                clicCompteur = 0;
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiser_grille(&maGrille);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "FINNIVEAU") == 0)
            {
                etatFinNiveau = true;
                tempsDebutFinNiveau = GetTime();
            }
        }

        if (etatFinNiveau && (GetTime() - tempsDebutFinNiveau >= dureeFinNiveau))
        {
            etatFinNiveau = false;
            InitialiserQueue(&q);
            Actions nouvelleAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
            Enfiler(&q, &nouvelleAction);
        }

        if (attenteClics)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 posSouris = GetMousePosition();
                int tailleCase = 50;
                int grilleLargeur = maGrille.colonnes * tailleCase;
                int grilleHauteur = maGrille.lignes * tailleCase;
                int offsetX = (1000 - grilleLargeur) / 2;
                int offsetY = (1000 - grilleHauteur) / 2;

                int xRel = posSouris.x - offsetX;
                int yRel = posSouris.y - offsetY;

                if (xRel >= 0 && xRel < grilleLargeur && yRel >= 0 && yRel < grilleHauteur)
                {
                    int colonne = xRel / tailleCase;
                    int ligne = yRel / tailleCase;
                    coordonneesClic[clicCompteur * 2] = ligne;
                    coordonneesClic[clicCompteur * 2 + 1] = colonne;
                    clicCompteur++;

                    if (clicCompteur == 2)
                    {
                        LirePionsAChanger(&maGrille, coordonneesClic[0], coordonneesClic[1],
                                          coordonneesClic[2], coordonneesClic[3], &q);
                        attenteClics = false;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        afficher_grille(&maGrille, textures, &q, explosionTexture);

        if (etatFinNiveau)
        {
            DrawText("FIN DU NIVEAU !", 300, 400, 40, RED);
            DrawText("Préparation du niveau suivant...", 250, 460, 30, DARKGRAY);
        }

        EndDrawing();
    }

    for (int i = 0; i < 5; i++)
        UnloadTexture(textures[i]);
    UnloadTexture(explosionTexture);
    StopMusicStream(attenteMusic);
    StopMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    printf("FIN DU JEU\n");
    return 0;
}
