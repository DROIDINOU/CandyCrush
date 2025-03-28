#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// A FAIRE DEPLACER NIVEAU ET AMELIORER ATTRIBUTION ALEATOIRE
// PREVOIR OBSTACLES ET AUTRES

int main()
{

    /*
    ____________________________________________________________________________________________________________________________

                                        **** INITIALISATION

        -> Structures de données : Niveaux - Grille - Queue
           - Niveaux : structure globale definie dans constante.h contenant les informations sur les niveaux
           - Grille : structure definie dans constante.h contenant les informations sur la grille
           - Queue : structure definie dans queue.h contenant les informations sur la queue

        -> Initialisation nombres aleatoires
        -> Declaration de la structure grille de bonbons
        -> Declaration et Initialisation de la queue q
        -> Declaration des variables ligne, colonne, ligne1, colonne1 (variables servant lors de l echange des
           coordonnees des bonbons)
    ___________________________________________________________________________________________________________________________
    */

    srand(time(NULL));                    // Generateur de nombres aleatoires
    GrilleBonbons maGrille;               // Declaration de la structure grille de bonbons
    Queue q;                              // Declaration de la queue q
    InitialiserQueue(&q);                 // Initialisation de la queue q
    int ligne, colonne, ligne1, colonne1; // Declaration des variables ligne, colonne, ligne1, colonne1

    /*____________________________________________________________________________________________________________________________

                                                 BOUCLE DE JEU
    -> Tant que niveaux est inferieur au niveau final
        -> Enfiler l'ACTION d'initialisation
        -> Tant que la queue n'est pas vide
                       -> Recuperer l'action en debut de la queue
                              -> Si l'action est AFFICHAGE
                                 - Afficher la grille
                              -> Si l'action est CALCUL
                                 - Calculer si trois pions ou plus se suivent en Vertical ou en Horizontal
                              -> Si l action est SUPPESSIONV
                                - Supprimer les suites de pions verticaux
                              -> Si l action est SUPPESSIONH
                                - Supprimer les suites de pions horizontaux
                              -> Si l'action est VERIFICATION
                                - verifier si il reste des gelatines
                              -> Si l'action est DEPLACEMENT
                                - Deplacer les pions
                              -> Si l'action est LECTURE
                                - Lire les coordonnees des pions a changer
                              -> Si l'action est INITIALISATION
                                    - Initialiser la grille
                                    - Ajouter une action CALCUL avec les coordonnees de la premiere case
        -> Passer au niveau suivant et increment le compteur de niveau de la structure Niveaux
    -> Afficher FIN DU JEU
    ___________________________________________________________________________________________________________________________   */

    while (NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        printf("Vous venez d'entrer dans le Niveau %d\n", NIVEAUX[0].compteurNiveau + 1);
        Actions action = {"INITIALISATION", {0, 0}, {0, 0}};
        Enfiler(&q, &action);

        while (q.taille > 0)
        {

            // Récupérer l'action en haut de la queue
            action = Defiler(&q);
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
                // faux pas mettre ca dans main va falloir passer niveau ds main
                NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues += 1;
            }

            else if (strcmp(action.actionName, "LECTURE") == 0)
            {
                LirePionsAChanger(&maGrille, &ligne, &colonne, &ligne1, &colonne1, &q);
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiserGrille(&maGrille, &q);
            }
        }

        // Passer au niveau suivant
        // Pour le moment je met niveau 3 quand coups epuises changer ca pour plus de clareté
        NIVEAUX[0].compteurNiveau += 1;
    }

    printf(MESSAGEETATJEU[4]);

    return 0;
}