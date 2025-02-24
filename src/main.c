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
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires une fois
    Niveaux NiveauJeu; // A améliorer
    int niveau = 0;    // A améliorer

    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    Queue q;
    initialiser_queue(&q);

    // Initialiser et afficher la grille
    while (niveau < 1)
    {
        maGrille.estVerifiee = 0;
        Actions initialisationAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
        enfiler(&q, initialisationAction);
        Actions action;
        while (q.taille > 0)
        {

            // Récupérer l'action en haut de la queue
            action = defiler(&q);
            maGrille.suppressionsRestantes = 0;
            if (strcmp(action.actionName, "AFFICHAGE") == 0)
            {
                afficher_grille(&maGrille);
            }
            else if (strcmp(action.actionName, "CALCUL") == 0)
            {
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                printf("decompte V dans pre main : %d ", maGrille.suppressionsRestantes);
                // printf("decompte V dans main : %d ", maGrille.suppressionsRestantes );
            }
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                printf("decompte H dans pre main : %d ", maGrille.suppressionsRestantes);
                // printf("decompte H dans main : %d ", maGrille.suppressionsRestantes );
            }
            else if (strcmp(action.actionName, "VERIFICATION") == 0)
            {
                Verification(&maGrille, &q);
                // printf("Verification dans main");
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
                Verif(&q, &maGrille); // tester deplacement dans initialiser grille
            }
        }

        // Passer au niveau suivant
        niveau++;
    }
    // imprimer_queue(&q);

    printf("FIN DU JEU\n");

    return 0;
}