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

// $ gcc main.c matrice.c constante.c queue.c affichage.c -o mon_programme -lraylib
// Ajoute RayGUI plus tard

int main() {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    InitWindow(800, 800, "Candy Crush Clone"); // 🖥️ Création de la fenêtre Raylib
    SetTargetFPS(60);

    // Chargement des textures
    Texture2D textures[5];
    textures[0] = LoadTexture("candyimages/candy-sprite.png");  // Violet (M)
    textures[1] = LoadTexture("candyimages/poignee_de_bonbons.png"); // Rouge (R)
    textures[2] = LoadTexture("candyimages/CANDYCANEPOLE.png"); // Bleu (B)
    textures[3] = LoadTexture("candyimages/poignee_de_bonbons.png"); // Vert (V)
    textures[4] = LoadTexture("candyimages/candy-sprite.png"); // Jaune (J)

    for (int i = 0; i < 5; i++) {
        if (textures[i].id == 0) {
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

    // Initialiser la grille avant la boucle
    initialiser_grille(&maGrille);

    while (!WindowShouldClose() && niveau < 1) { // Boucle principale Raylib
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Affichage de la grille
        afficher_grille(&maGrille, textures);

        Actions initialisationAction = {"INITIALISATION", {0, 0}, {0, 0}};
        enfiler(&q, initialisationAction);
        bool isVerificationInit = false;

        Actions action;
        while (q.taille > 0 && strcmp((action = defiler(&q)).actionName, "FINNIVEAU") != 0) {
            if (strcmp(action.actionName, "LECTURE") == 0) {
                int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            } else if (strcmp(action.actionName, "CALCUL") == 0) {
                Calcul(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &isVerificationInit);
            } else if (strcmp(action.actionName, "SUPPRESSIONV") == 0) {
                SuppressionV(&maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &q);
            } else if (strcmp(action.actionName, "SUPPRESSIONH") == 0) {
                SuppressionH(&maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &q);
            } else if (strcmp(action.actionName, "VERIFICATION") == 0) {
                Verification(&maGrille, &q);
            } else if (strcmp(action.actionName, "DEPLACEMENT") == 0) {
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            } else if (strcmp(action.actionName, "INITIALISATION") == 0) {
                initialiser_grille(&maGrille);
                VerificationInitit(&q, &maGrille, &isVerificationInit);
            }
        }

        EndDrawing(); // Fin du rendu graphique
        niveau++; // Passer au niveau suivant
    }

    // Décharger les textures après usage
    for (int i = 0; i < 5; i++) {
        UnloadTexture(textures[i]);
    }

    CloseWindow(); // 🛑 Fermer proprement Raylib
    printf("FIN DU JEU\n");
    return 0;
}
