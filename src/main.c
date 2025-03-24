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

// === Compilation ===
// gcc main.c matrice.c constante.c queue.c affichage.c -o mon_programme -lraylib
// ./mon_programme.exe

int main()
{
    srand(time(NULL));
    InitWindow(1000, 1000, "Candy Crush Clone");
    InitAudioDevice();

    Sound sound = LoadSound("../assets/gaming-music-8-bit-console-play-background-intro-theme-278382.mp3");
    Music music = LoadMusicStream("../assets/gaming-music-8-bit-console-play-background-intro-theme-278382.mp3");
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

    Texture2D textures[5];
    textures[0] = LoadTexture("../candyimages/5cd560a569ed85884c879cb1da8e7d68.png");
    textures[1] = LoadTexture("../candyimages/poignee_de_bonbons.png");
    textures[2] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-candy-crush-candy-crush-5ad0dcad6773e1.4200818515236374214238.png");
    textures[3] = LoadTexture("../candyimages/580b57fcd9996e24bc43c517.png");
    textures[4] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-lollipop-ga-sweet-cheats-for-candy-crush-saga-1-2-ipa-war4-5b67aab6d41443.6117650115335205668687.png");

    for (int i = 0; i < 5; i++)
    {
        if (textures[i].id == 0)
            printf("Erreur de chargement de la texture %d\n", i);
    }

    int niveau = 0;
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;
    maGrille.estVerifiee = 0;

    Queue q;
    initialiser_queue(&q);
    Actions initAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
    enfiler(&q, initAction);

    bool attenteClics = false;
    int clicCompteur = 0;
    int coordonneesClic[4] = {-1, -1, -1, -1};

    // Timer d'attente
    bool etatAttente = false;
    double tempsDebutAttente = 0.0;
    double dureeAttente = 0.5; // 0.5 secondes

    while (!WindowShouldClose() && niveau < 1)
    {
        UpdateMusicStream(music);

        // Si on est en attente d'une pause (remplaçant usleep)
        if (etatAttente)
        {
            if ((GetTime() - tempsDebutAttente) >= dureeAttente)
            {
                etatAttente = false; // fin de l'attente, traitement peut reprendre
            }
        }
        else if (q.taille > 0)
        {
            Actions action = defiler(&q);
            maGrille.suppressionsRestantes = 0;
            strcpy(maGrille.lastAction, action.actionName);

            if (strcmp(action.actionName, "CALCUL") == 0)
            {
                printf("Traitement de CALCUL\n");
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                printf("Traitement de SUPPRESSIONV\n");
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                etatAttente = true;
                tempsDebutAttente = GetTime();
            }
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                printf("Traitement de SUPPRESSIONH\n");
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                etatAttente = true;
                tempsDebutAttente = GetTime();
            }
            else if (strcmp(action.actionName, "VERIFICATION") == 0)
            {
                printf("Traitement de VERIFICATION\n");
                Verification(&maGrille, &q);
                maGrille.estVerifiee = 1;
            }
            else if (strcmp(action.actionName, "DEPLACEMENT") == 0)
            {
                printf("Traitement de DEPLACEMENT\n");
                maGrille.estVerifiee = 0;
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }
            else if (strcmp(action.actionName, "LECTURE") == 0)
            {
                printf("Traitement de LECTURE via clics de souris\n");
                attenteClics = true;
                clicCompteur = 0;
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                printf("Initialisation\n");
                initialiser_grille(&maGrille);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
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
                    printf("Clic %d: ligne=%d, colonne=%d\n", clicCompteur, ligne, colonne);

                    if (clicCompteur == 2)
                    {
                        LirePionsAChanger(&maGrille, coordonneesClic[0], coordonneesClic[1],
                                          coordonneesClic[2], coordonneesClic[3], &q);
                        attenteClics = false;
                    }
                }
                else
                {
                    printf("Clic hors de la grille, ignoré\n");
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        afficher_grille(&maGrille, textures, &q);
        EndDrawing();

        bool gelatinePresente = false;
        for (int i = 0; i < maGrille.lignes && !gelatinePresente; i++)
        {
            for (int j = 0; j < maGrille.colonnes; j++)
            {
                if (maGrille.tableau[i][j].gelatine)
                {
                    gelatinePresente = true;
                    break;
                }
            }
        }

        if (!gelatinePresente)
        {
            niveau++;
            printf("NIVEAU TERMINE\n");
        }
    }

    for (int i = 0; i < 5; i++)
        UnloadTexture(textures[i]);

    StopMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    printf("FIN DU JEU\n");
    return 0;
}
