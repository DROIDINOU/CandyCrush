#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "erreur.h"
#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// A FAIRE DEPLACER NIVEAU ET AMELIORER ATTRIBUTION ALEATOIRE
// REMETTRE LES CASCADES DANS EXPLICATION ET ORDONNE MAIN DANS ORDRE LOGIQUE

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
    EtatJeu etatJeu;
    etatJeu.niveausuivant = 0;
    etatJeu.findepartie = 0;
    etatJeu.coupsepuises = 0;

    /*____________________________________________________________________________________________________________________________

                                                 BOUCLE DE JEU
    -> Tant que niveaux est inferieur au niveau final
        -> Enfiler l'ACTION d'initialisation
        -> Tant que la queue n'est pas vide
                       -> Recuperer l'action en debut de la queue
                              -> Si action est ERREURQUEUEPLEINE
                                 - Afficher la queue est pleine
                              -> Si action est ERREURQUEUEVIDE
                                 - Afficher la queue est vide
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

        -> Passer au niveau suivant et increment le compteur de niveau de la structure Niveaux
    -> Afficher FIN DU JEU
    ___________________________________________________________________________________________________________________________   */

    while (NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        Actions actionInit = {INITIALISATION, {0, 0}, {0, 0}};
        Enfiler(&q, &actionInit);

        while (q.taille > 0)
        {
            Actions action = Defiler(&q);

            switch (action.actionName)
            {
            case INITIALISATION:
                initialiserGrille(&maGrille, &q, &etatJeu);
                break;

            case VERIFICATION:
                Verification(&maGrille, &q, &etatJeu);
                break;

            case CALCUL:
                Calcul(&q, &maGrille,
                       &action.pion1.x, &action.pion1.y,
                       &action.pion2.x, &action.pion2.y, false);
                break;

            case SUPPRESSIONV:
                SuppressionV(&maGrille,
                             &action.pion1.x, &action.pion1.y,
                             &action.pion2.x, &action.pion2.y,
                             &q);
                break;

            case SUPPRESSIONH:
                SuppressionH(&maGrille,
                             &action.pion1.x, &action.pion1.y,
                             &action.pion2.x, &action.pion2.y,
                             &q);
                break;

            case CHUTELIGNEENTIERE:
                AppliquerChuteLigne(&maGrille, action.pion1.y, &q);
                PAUSE(100);
                break;

            case CHUTECOLONNEENTIERE:
                AppliquerChuteColonne(&maGrille, action.pion1.x, action.pion1.y, &q);
                PAUSE(100);
                break;
            case CHUTEPARTIELLE:
                AppliquerChutePartielle(&maGrille, action.pion1.x, action.pion1.y, &q);
                PAUSE(100);
                break;

            case AFFICHAGE:
                afficherGrille(&maGrille, &q, &etatJeu);
                PAUSE(200);
                break;
            case RELANCERCALCUL:

                Enfiler(&q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
                break;

            case DEPLACEMENT:
                Deplacement(&q,
                            &maGrille,
                            action.pion1.x, action.pion1.y,
                            action.pion2.x, action.pion2.y);
                break;

            case LECTURE:
                LirePionsAChanger(&maGrille,
                                  &ligne, &colonne,
                                  &ligne1, &colonne1,
                                  &q);
                break;

            case ERREURACTION:
                GererErreurFatale(action.erreur);
                break;
            }
        }

        // Passe au niveau suivant

        NIVEAUX[0].compteurNiveau += 1;
    }
    return 0;
}