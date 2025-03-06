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

    /*
    ____________________________________________________________________________________________________________________________

                                                         INITIALISATION
    ____________________________________________________________________________________________________________________________
    */

    srand(time(NULL)); // Generateur de nombres aleatoires
    Niveaux NiveauJeu; // A améliorer
    int niveau = 0;    // A améliorer

    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    Queue q;
    initialiser_queue(&q);

    /*____________________________________________________________________________________________________________________________

                                                            BOUCLE DE JEU
    ___________________________________________________________________________________________________________________________   */
    // Initialiser et afficher la grille
    while (niveau < 3)
    {
        printf("Niveau %d\n", niveau + 1);
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
                afficher_grille(&maGrille, &q);
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
            }
            else if (strcmp(action.actionName, "DEPLACEMENT") == 0)
            {
                printf("Deplacement de (%d,%d) vers (%d,%d)\n", action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }

            else if (strcmp(action.actionName, "LECTURE") == 0)
            {

                int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                printf("ligne %d\n", ligne);
                int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiserGrille(&maGrille);
                // deplacer calcul dans initialisation
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
                printf("initialisation");
            }
        }

        // Passer au niveau suivant
        niveau++;
    }

    printf("FIN DU JEU\n");

    return 0;
}