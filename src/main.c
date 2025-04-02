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
    Music currentMusic;
    bool musicChargee = false;
    srand(time(NULL));
    char buffer[50];
    InitWindow(1000, 1000, "Candy Crush Clone");
    InitAudioDevice();
    Music attenteMusic = LoadMusicStream("../assets/jellysplash_56f256e05113918.mp3");

    SetTargetFPS(60);

    bool jeuDemarre = false;
    while (!WindowShouldClose() && !jeuDemarre)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("CANDY CRUSH CLONE", 350, 400, 40, DARKBLUE);
        DrawText("Appuyez sur ESPACE pour commencer", 300, 500, 30, DARKGRAY);
        if (IsKeyPressed(KEY_SPACE))
            jeuDemarre = true;
        EndDrawing();
    }

    Texture2D textures[] = {
        [MAUVE] = LoadTexture("../candyimages/5cd560a569ed85884c879cb1da8e7d68.png"),
        [ROUGE] = LoadTexture("../candyimages/poignee_de_bonbons.png"),
        [BLEU] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-candy-crush-candy-crush-5ad0dcad6773e1.4200818515236374214238.png"),
        [VERT] = LoadTexture("../candyimages/580b57fcd9996e24bc43c517.png"),
        [JAUNE] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-lollipop-ga-sweet-cheats-for-candy-crush-saga-1-2-ipa-war4-5b67aab6d41443.6117650115335205668687.png"),
        [BLANC] = LoadTexture("../candyimages/293530-P7Q9H0-62.jpg"),
        [GRIS] = LoadTexture("../candyimages/pile-sweet-donuts.jpg"),
        [JAUNE_CLAIR] = LoadTexture("../candyimages/5939.jpg"),
        [ROSE] = LoadTexture("../candyimages/kisspng-spiral-circle-magenta-lollipop-5b2c7a0466c112.1881360715296414764209.png")};
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
    double dureeAttente = 0.3;

    bool etatFinNiveau = false;
    double tempsDebutFinNiveau = 0.0;
    double dureeFinNiveau = 2.5;

    bool etatFinJeu = false;
    double tempsDebutFinJeu = 0.0;
    double dureeFinJeu = 2.5;

    int niveauPrecedent = -1;

    while (!WindowShouldClose() && NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        if (niveauPrecedent != NIVEAUX[0].compteurNiveau)
        {
            char chemin[100];
            sprintf(chemin, "../assets/music_niveau_%d.mp3", NIVEAUX[0].compteurNiveau);

            if (musicChargee)
            {
                StopMusicStream(currentMusic);
                UnloadMusicStream(currentMusic);
                musicChargee = false;
            }

            currentMusic = LoadMusicStream(chemin);
            if (currentMusic.ctxData == NULL)
            {
                printf("⚠ Erreur chargement musique : %s\n", chemin);
            }
            else
            {
                PlayMusicStream(currentMusic);
                SetMusicVolume(currentMusic, 0.2f);
                musicChargee = true;
            }
            niveauPrecedent = NIVEAUX[0].compteurNiveau;
        }

        if (musicChargee)
            UpdateMusicStream(currentMusic);

        if (etatAttente)
        {
            UpdateMusicStream(attenteMusic);
            if (!IsMusicStreamPlaying(attenteMusic))
            {
                SetMusicVolume(attenteMusic, 1.0f);
                if (musicChargee)
                    SetMusicVolume(currentMusic, 0.0f);
                PlayMusicStream(attenteMusic);
            }
            if ((GetTime() - tempsDebutAttente) >= dureeAttente)
            {
                etatAttente = false;
                StopMusicStream(attenteMusic);
                if (musicChargee)
                    SetMusicVolume(currentMusic, 0.2f);
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
            sprintf(buffer, "FIN DU NIVEAU %d !", NIVEAUX[0].compteurNiveau);
            DrawRectangle(200, 350, 600, 150, BLACK);
            DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
            DrawText(buffer, 320, 390, 40, RAYWHITE);
            DrawText("Préparation du niveau suivant...", 260, 440, 25, GRAY);
        }

        if (etatFinJeu)
        {
            while (!WindowShouldClose())
            {
                BeginDrawing();
                ClearBackground(DARKBLUE);
                DrawRectangle(200, 350, 600, 150, BLACK);
                DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
                sprintf(buffer, "FIN DU JEU  FELICITATIONS!", NIVEAUX[0].compteurNiveau + 1);
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
    UnloadMusicStream(attenteMusic);

    if (musicChargee)
    {
        StopMusicStream(currentMusic);
        UnloadMusicStream(currentMusic);
    }

    CloseAudioDevice();
    CloseWindow();

    printf("FIN DU JEU\n");
    return 0;
}