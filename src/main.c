#include "raylib.h"
#include "matrice.h"
#include "main.h"
#include "jeu.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include "ressources.h"
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
    initialiserFenetre();
    initialiserAudio();
    Music attenteMusic = chargerMusiqueAttente();

    bool jeuDemarre = false;
    afficherMenuAccueil(&jeuDemarre);

    Texture2D textures[NB_COULEURS]; // ← Assure-toi que NB_COULEURS est bien défini dans constante.h
    chargerTextures(textures);
    Texture2D explosionTexture = chargerTextureExplosion();
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
        gererMusiqueParNiveau(&niveauPrecedent, &currentMusic, &musicChargee);

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

        etatFinNiveau = verifierFinNiveau(etatFinNiveau, tempsDebutFinNiveau, dureeFinNiveau, &q);
        etatFinJeu = verifierFinJeu(etatFinJeu, tempsDebutFinJeu, dureeFinJeu);
        if (attenteClics)
        {
            attenteClics = gererClics(&maGrille, &clicCompteur, coordonneesClic, &q);
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

    libererTextures(textures);
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