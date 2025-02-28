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
    srand(time(NULL)); // Initialisation du RNG
    InitWindow(1000, 1000, "Candy Crush Clone");
    SetTargetFPS(1800);

    // Chargement des textures (exemple)
    Texture2D textures[5];
    textures[0] = LoadTexture("candyimages/candy-sprite.png");       // Violet
    textures[1] = LoadTexture("candyimages/poignee_de_bonbons.png"); // Rouge
    textures[2] = LoadTexture("candyimages/CANDYCANEPOLE.png");      // Bleu
    textures[3] = LoadTexture("candyimages/poignee_de_bonbons.png"); // Vert
    textures[4] = LoadTexture("candyimages/candy-sprite.png");       // Jaune

    for (int i = 0; i < 5; i++)
    {
        if (textures[i].id == 0)
        {
            printf("Erreur de chargement de la texture %d\n", i);
        }
    }

    // Un seul niveau pour l'exemple
    int niveau = 0;

    // Init de la grille et de la queue
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    Queue q;
    initialiser_queue(&q);

    // 1) On enfile une première action d'initialisation
    Actions initAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
    enfiler(&q, initAction);

    // ─────────────────────────────────────────────
    // 2) Boucle principale Raylib
    //    Reste jusqu’à ce que la fenêtre soit fermée
    //    ou qu’on ait fini notre "niveau < 1"
    // ─────────────────────────────────────────────
    while (!WindowShouldClose() && niveau < 1)
    {
        // A) Traitement d'UNE action par frame (si la file n'est pas vide)
        while (q.taille > 0)
        {

            // Récupérer l'action en haut de la queue
            Actions action = defiler(&q);
            maGrille.suppressionsRestantes = 0;
            if (strcmp(action.actionName, "AFFICHAGE") == 0)
            {
                // printf("on est dans affichage main ");
            }
            else if (strcmp(action.actionName, "CALCUL") == 0)
            {
                printf("ca sert a quoi cela ????: %d\n", action.pion1);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                // printf("decompte V dans pre main : %d ", maGrille.suppressionsRestantes);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                // printf("decompte H dans pre main : %d ", maGrille.suppressionsRestantes);
            }
            else if (strcmp(action.actionName, "VERIFICATION") == 0)
            {
                Verification(&maGrille, &q);
                maGrille.estVerifiee = 1;
            }
            else if (strcmp(action.actionName, "DEPLACEMENT") == 0)
            {
                maGrille.estVerifiee = 0;
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }

            else if (strcmp(action.actionName, "LECTURE") == 0)
            {
                maGrille.estVerifiee = 1;

                int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiser_grille(&maGrille);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
                printf("initialisation");
            }
        }

        // B) Rendu Raylib (affichage) à CHAQUE frame
        BeginDrawing();
        ClearBackground(RAYWHITE);

        afficher_grille(&maGrille, textures, &q);

        EndDrawing();

        // C) Condition de fin du niveau (ex: plus de gélatine ?)
        //    Ici, tu peux faire un test : si gélatine = 0 => niveau++
        //    Par exemple :

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
            // plus de gélatine => niveau fini
            niveau++;
            printf("NIVEAU TERMINE\n");
        }
    }

    // On sort de la boucle si la fenêtre est fermée ou si niveau >= 1
    // Déchargement des textures
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(textures[i]);
    }
    CloseWindow();

    printf("FIN DU JEU\n");
    return 0;
}
