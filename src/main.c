

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
    char buffer[50];
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

    Texture2D textures[9] = {
        LoadTexture("../candyimages/5cd560a569ed85884c879cb1da8e7d68.png"),
        LoadTexture("../candyimages/poignee_de_bonbons.png"),
        LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-candy-crush-candy-crush-5ad0dcad6773e1.4200818515236374214238.png"),
        LoadTexture("../candyimages/580b57fcd9996e24bc43c517.png"),
        LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-lollipop-ga-sweet-cheats-for-candy-crush-saga-1-2-ipa-war4-5b67aab6d41443.6117650115335205668687.png"),
        LoadTexture("../candyimages/293530-P7Q9H0-62.jpg"),
        LoadTexture("../candyimages/pile-sweet-donuts.jpg"),
        LoadTexture("../candyimages/299018-P85W91-91.jpg"),
        LoadTexture("../candyimages/2202_w020_n001_1260_gameicons_p15_1260.jpg")};
    Texture2D explosionTexture = LoadTexture("../candyimages/circle_01.png");
    GrilleBonbons maGrille;

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

    bool etatFinJeu = false;
    double tempsDebutFinJeu = 0.0;
    double dureeFinJeu = 2.5;

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
                maGrille.estVerifiee = 0;
            }
            else if (strcmp(action.actionName, "FIN") == 0)
            {
                etatFinJeu = true;
                tempsDebutFinJeu = GetTime();
            }
        }

        if (etatFinNiveau && (GetTime() - tempsDebutFinNiveau >= dureeFinNiveau))
        {
            etatFinNiveau = false;
            InitialiserQueue(&q);
            Actions nouvelleAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
            Enfiler(&q, &nouvelleAction);
        }

        if (etatFinJeu && (GetTime() - tempsDebutFinJeu >= dureeFinJeu))
        {
            etatFinJeu = false;
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
            // va falloir faire un "niveau user"
            sprintf(buffer, "FIN DU NIVEAU %d !", NIVEAUX[0].compteurNiveau);
            // Cadre noir (rectangle de fond du message)
            DrawRectangle(200, 350, 600, 150, BLACK); // Facultatif ici (fond déjà noir, mais pour structure)

            // Contour du cadre (bordure visible)
            DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE); // Bord blanc pour bien voir

            // Texte centré dans le cadre
            DrawText(buffer, 320, 390, 40, RAYWHITE);
            DrawText("Préparation du niveau suivant...", 260, 440, 25, GRAY);
        }

        if (etatFinJeu)
        {
            while (!WindowShouldClose())
            {
                BeginDrawing();
                ClearBackground(DARKBLUE); // un fond plus sombre et lisible
                DrawRectangle(200, 350, 600, 150, BLACK);
                DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
                sprintf(buffer, "FIN DU NIVEAU FINAL %d \n      - FELICITATIONS !\n", NIVEAUX[0].compteurNiveau);
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
