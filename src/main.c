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
int main() {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    InitWindow(1280, 1280, "Candy Crush Clone"); // 🖥️ Création de la fenêtre Raylib
    SetTargetFPS(60);

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

       

        Actions initialisationAction = {"INITIALISATION", {0, 0}, {0, 0}};
        enfiler(&q, initialisationAction);
        bool isVerificationInit = false; 

        Actions action;
        while (q.taille > 0 && strcmp((action = defiler(&q)).actionName, "FINNIVEAU") != 0) {
            if (strcmp(action.actionName, "AFFICHAGE") == 0) {
            } else if (strcmp(action.actionName, "LECTURE") == 0) {
                int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            } else if (strcmp(action.actionName, "CALCUL") == 0) {
                Calcul(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &isVerificationInit);
            } else if (strcmp(action.actionName, "SUPPRESSIONV") == 0) {
                printf("entre dans le SUPPRESSIONV\n");
                SuppressionV(&maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &q);
            } else if (strcmp(action.actionName, "SUPPRESSIONH") == 0) {
                SuppressionH(&maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &q);
            } else if (strcmp(action.actionName, "VERIFICATION") == 0) {
                printf("entre dans la verification\n");
                Verification(&maGrille, &q);
                BeginDrawing();  // Commencer le rendu graphique
                ClearBackground(RAYWHITE);
                EndDrawing();  // Fin du rendu graphique
            } else if (strcmp(action.actionName, "DEPLACEMENT") == 0) {
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            } else if (strcmp(action.actionName, "INITIALISATION") == 0) {
                initialiser_grille(&maGrille);
                VerificationInitit(&q, &maGrille, &isVerificationInit);
                

            }
        }


        niveau++; // Passer au niveau suivant
    }

    CloseWindow(); // 🛑 Fermer proprement Raylib
    printf("FIN DU JEU\n");
    return 0;
}
