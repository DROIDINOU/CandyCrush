#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
 ____________________________________________________________________________________________________________________________

                        **** FONCTIONS DE LECTURE DES ENTREES UTILISATEUR

    -> Fonctions : ObtenirReponseAuMessage - LireQuatreCoordonnees
    -> Parametres ObtenirReponseAuMessage  : message - index
    -> Parametres LireQuatreCoordonnees : x1 - y1 - x2 - y2
    -> ObtenirReponseAuMessage : Affiche le message et lit la reponse de l'utilisateur
    -> LireQuatreCoordonnees : Lit les coordonnees des pions a changer
___________________________________________________________________________________________________________________________
*/

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

                                        **** INITIALISATION

         -> Structures de données : Niveaux - Grille - Queue
           - Niveaux : structure globale definie dans constante.h contenant les informations sur les niveaux
           - Grille : structure definie dans constante.h contenant les informations sur la grille
           - Queue : structure definie dans queue.h contenant les informations sur la queue

           // Attention travailler sur les structures globales et non locales

        -> Initialisation nombres aleatoires
        -> Declaration de la structure grille de bonbons
        -> Declaration et Initialisation de la queue q
    ___________________________________________________________________________________________________________________________
    */

    srand(time(NULL)); // Generateur de nombres aleatoires

    GrilleBonbons maGrille; // Declaration de la structure grille de bonbons
    Queue q;                // Declaration de la queue q
    initialiser_queue(&q);  // Initialisation de la queue q

    /*____________________________________________________________________________________________________________________________

                                                 BOUCLE DE JEU
    -> Tant que le nombre de niveaux est inferieur au niveau final
    -> Enfiler l'ACTION d'initialisation
    -> Tant que la queue n'est pas vide
                       -> Recuperer l'action en haut de la queue
                              -> Si l'action est AFFICHAGE
                                 - Afficher la grille
                              -> Si l'action est CALCUL
                                 - Calculer si trois pions se suivent en Vertical ou en Horizontal
                              -> Si l action est SUPPESSIONV
                                      - Supprimer les pions verticaux
                              -> Si l action est SUPPESSIONH
                                      - Supprimer les pions horizontaux
                              -> Si l'action est VERIFICATION
                                    -> verifier si il reste des gelatines
                              -> Si l'action est DEPLACEMENT
                                      -> Deplacer les pions
                              -> Si l'action est LECTURE
                                      - Lire les coordonnees des pions a changer (fonctions lecture entrees utilisateur)
                              -> Si l'action est INITIALISATION
                                        -> Initialiser la grille
                                        -> Ajouter une action CALCUL avec les coordonnees de la premiere case
    -> Passer au niveau suivant et increment le compteur de niveau de la structure Niveaux
    -> Afficher FIN DU JEU
    ___________________________________________________________________________________________________________________________   */

    while (niveaux[0].compteurNiveau < FINALNIVEAU)
    {
        printf("Vous venez d'entrer dans le Niveau %d\n", niveaux[0].compteurNiveau + 1);
        Actions action = {"INITIALISATION", {0, 0}, {0, 0}, true};
        enfiler(&q, action);

        while (q.taille > 0)
        {

            // Récupérer l'action en haut de la queue
            action = defiler(&q);
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
                niveaux[niveaux[0].compteurNiveau].coupsNiveau.coupsJoues += 1;
                printf("coups joues = %d\n", niveaux[niveaux[0].compteurNiveau].coupsNiveau.coupsJoues);
                printf("coups a jouer = %d\n", niveaux[niveaux[0].compteurNiveau].coupsNiveau.coupAJouer);
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

        niveaux[0].compteurNiveau += 1;
    }

    printf("FIN DU JEU\n");

    return 0;
}