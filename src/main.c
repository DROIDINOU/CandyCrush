#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires une fois
    Niveaux NiveauJeu;
    int niveau = 0;
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;
    Queue q;
    initialiser_queue(&q);

    // Initialiser et afficher la grille
    while (niveau < 1) {
        Actions initialisationAction = {"INITIALISATION", {0, 0}, {0, 0}};
        enfiler(&q, initialisationAction);
        bool isVerificationInit = false; // Déclaration du booléen ici
        // Ajouter une action "FINNIVEAU" pour permettre à la boucle interne de se terminer
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
                Calcul(&q, &maGrille,action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &isVerificationInit);
            } else if (strcmp(action.actionName, "SUPPRESSIONV") == 0) {
                printf("entre dans le SUPPRESSIONV");
                SuppressionV(&maGrille,action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &q);
                afficher_grille(&maGrille);
            } else if (strcmp(action.actionName, "SUPPRESSIONH") == 0) {
               
                SuppressionH(&maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y, &q);
                //afficher_grille(&maGrille);
            } else if (strcmp(action.actionName, "VERIFICATION") == 0) {
                printf("entre dans la verification");
                Verification(&maGrille, &q);
            } else if (strcmp(action.actionName, "DEPLACEMENT") == 0) {
                
                Deplacement(&q, &maGrille,action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
                //afficher_grille(&maGrille);
            } else if (strcmp(action.actionName, "INITIALISATION") == 0) {
                initialiser_grille(&maGrille);
                //afficher_grille(&maGrille);
                VerificationInitit(&q, &maGrille, &isVerificationInit);
            }
        }

        // Passer au niveau suivant
        niveau++;
    }
    printf("FIN DU JEU\n");

    return 0;
}


   