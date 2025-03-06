#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// a ameliorer et a deplacer dans main ou creer fichier
int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index)
{
    int reponse;
    do
    {
        printf("%s", message[index][0]);     // Affiche le message
        int result = scanf(" %d", &reponse); // Lit un seul caractère
        while (getchar() != '\n')
            ;

        // Vérifie que la réponse est valide
        if (result == -1)
        {
            // printf("Reponse invalide. Essayez encore.\n");
        }
    } while (reponse < 1 || reponse > 21); // Si la réponse n'est pas valide, on redemande

    return reponse - 1;
}

void LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    printf("x1 = %d\n", *x1);

    *y1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    printf("y1 = %d\n", *y1);

    *x2 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    printf("x2 = %d\n", *x2);

    *y2 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    printf("y2 = %d\n", *y2);
}

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
    Queue q;
    initialiser_queue(&q);

    /*____________________________________________________________________________________________________________________________

                                                            BOUCLE DE JEU
    ___________________________________________________________________________________________________________________________   */
    // Initialiser et afficher la grille
    while (niveau < 3)
    {
        printf("Vous venez d'entrer dans le Niveau %d\n", niveau + 1);
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
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }

            else if (strcmp(action.actionName, "LECTURE") == 0)
            {

                int ligne, colonne, ligne1, colonne1;
                LireQuatreCoordonnees(&ligne, &colonne, &ligne1, &colonne1);
                LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiserGrille(&maGrille);
                Actions action = {"CALCUL", {0, 0}, {0, 0}}; // On ajoute une action CALCUL avec les nouvelles coordonnées
                enfiler(&q, action);
            }
        }

        // Passer au niveau suivant
        niveau++;
    }

    printf("FIN DU JEU\n");

    return 0;
}