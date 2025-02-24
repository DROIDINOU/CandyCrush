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

// ouvrir via M2 BLEU
// cd /C/Users/32471/MySDL2

// $ gcc main.c matrice.c constante.c queue.c affichage.c -o mon_programme -lraylib
// ./mon_programme.exe
// Ajoute RayGUI plus tard

int main()
{
    srand(time(NULL));                           // Initialisation du générateur de nombres aléatoires
    InitWindow(1000, 1000, "Candy Crush Clone"); // Création de la fenêtre Raylib
    printf("Fenêtre initialisée\n");
    SetTargetFPS(60);

    // Chargement des textures
    Texture2D textures[5];
    textures[0] = LoadTexture("candyimages/candy-sprite.png");       // Violet (M)
    textures[1] = LoadTexture("candyimages/poignee_de_bonbons.png"); // Rouge (R)
    textures[2] = LoadTexture("candyimages/CANDYCANEPOLE.png");      // Bleu (B)
    textures[3] = LoadTexture("candyimages/poignee_de_bonbons.png"); // Vert (V)
    textures[4] = LoadTexture("candyimages/candy-sprite.png");       // Jaune (J)

    for (int i = 0; i < 5; i++)
    {
        if (textures[i].id == 0)
        {
            printf("Erreur de chargement de la texture %d\n", i);
        }
    }

    Niveaux NiveauJeu;
    int niveau = 0;
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    Queue q;
    initialiser_queue(&q);

    while (!WindowShouldClose() && niveau < 1)
    {
        maGrille.estVerifiee = 0;
        Actions initialisationAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
        enfiler(&q, initialisationAction);
        Actions action;

        while (q.taille > 0)
        {
            action = defiler(&q);
            maGrille.suppressionsRestantes = 0;

            if (strcmp(action.actionName, "AFFICHAGE") == 0)
            {
                printf("icccccccccccccccccccccccccccccccccccccccccccciaffffff");
                // Utilisation de Raylib pour afficher la grille
                BeginDrawing();
                ClearBackground(RAYWHITE);
                afficher_grille(&maGrille, textures);
                EndDrawing();
                WaitTime(0.02);
            }
            else if (strcmp(action.actionName, "CALCUL") == 0)
            {
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
            }
            else if (strcmp(action.actionName, "VERIFICATION") == 0)
            {
                Verification(&maGrille, &q);
                maGrille.estVerifiee = 1;
            }
            else if (strcmp(action.actionName, "DEPLACEMENT") == 0)
            {
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }
            else if (strcmp(action.actionName, "LECTURE") == 0)
            {
                int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiser_grille(&maGrille);
                Verif(&q, &maGrille);
            }
        }

        niveau++;
    }

    // Décharger les textures après usage
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(textures[i]);
    }

    CloseWindow(); // Fermer proprement Raylib
    printf("FIN DU JEU\n");

    return 0;
}
