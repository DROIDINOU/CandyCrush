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
#include <unistd.h>
// ouvrir via M2 BLEU
// cd /C/Users/32471/MySDL2

// $ gcc main.c matrice.c constante.c queue.c affichage.c -o mon_programme -lraylib
// mode fentre ajouter : -mwindows
// ./mon_programme.exe
// Ajoute RayGUI plus tard
extern void FreeConsole(void);
int main()
{
    FreeConsole(); // Cache la console sous Windows

    srand(time(NULL)); // Initialisation du RNG
    InitWindow(1000, 1000, "Candy Crush Clone");
    SetTargetFPS(60); // Limiter à 60 FPS

    // Chargement des textures
    Texture2D textures[5];
    textures[0] = LoadTexture("../candyimages/5cd560a569ed85884c879cb1da8e7d68.png");                                                                             // Violet
    textures[1] = LoadTexture("../candyimages/poignee_de_bonbons.png");                                                                                           // Rouge
    textures[2] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-candy-crush-candy-crush-5ad0dcad6773e1.4200818515236374214238.png"); // Bleu
    textures[3] = LoadTexture("../candyimages/580b57fcd9996e24bc43c517.png");                                                                                     // Vert
    textures[4] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-lollipop-ga-sweet-cheats-for-candy-crush-saga-1-2-ipa-war4-5b67aab6d41443.6117650115335205668687.png");

    for (int i = 0; i < 5; i++)
    {
        if (textures[i].id == 0)
        {
            printf("Erreur de chargement de la texture %d\n", i);
        }
    }

    // Un seul niveau pour l'exemple
    int niveau = 0;

    // Initialisation de la grille et de la queue
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    Queue q;
    initialiser_queue(&q);
    maGrille.estVerifiee = 0;

    // Ajout d'une action d'initialisation
    Actions initAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
    enfiler(&q, initAction);
    Actions action;
    // Boucle principale Raylib
    while (!WindowShouldClose() && niveau < 1)
    {
        // Traitement d'une action par frame (si la file n'est pas vide)
        if (q.taille > 0)
        {
            Actions action = defiler(&q);
            maGrille.suppressionsRestantes = 0;

            if (strcmp(action.actionName, "CALCUL") == 0)
            {
                printf("Traitement de CALCUL\n");
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                printf("Traitement de SUPPRESSIONV\n");
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                printf("Traitement de SUPPRESSIONH\n");
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                BeginDrawing();
                ClearBackground(RAYWHITE);
                afficher_grille(&maGrille, textures, &q);
                EndDrawing();
                usleep(200 * 1000);
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
                printf("Traitement de LECTURE\n");
                int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                printf("Initialisation\n");
                initialiser_grille(&maGrille);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "AFFICHAGE") == 0)
            {
                printf("Action : AFFICHAGE\n");
                BeginDrawing();
                ClearBackground(RAYWHITE);
                afficher_grille(&maGrille, textures, &q);
                EndDrawing();
                // Relancer une action de CALCUL après l'affichage
            }
        }

        // Vérification de fin du niveau (ex: plus de gélatine ?)
        bool gelatinePresente = false;
        for (int i = 0; i < maGrille.lignes; i++)
        {
            for (int j = 0; j < maGrille.colonnes; j++)
            {
                if (maGrille.tableau[i][j].gelatine)
                {
                    gelatinePresente = true;
                    break;
                }
            }
            if (gelatinePresente)
                break;
        }
        if (!gelatinePresente)
        {
            niveau++;
            printf("NIVEAU TERMINE\n");
        }
    }

    // Fermeture du jeu
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(textures[i]);
    }
    CloseWindow();
    printf("FIN DU JEU\n");

    return 0;
}
