#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "erreur.h"
#include "constante.h"
#include "etatjeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// ATTENTION VA CERTAINEMENT FALLOIR AJOUTER QQCHOSE POUR ETATJEUFIN DE PARTIE
// A FAIRE DEPLACER NIVEAU ET AMELIORER ATTRIBUTION ALEATOIRE
// REMETTRE LES CASCADES DANS EXPLICATION ET ORDONNE MAIN DANS ORDRE LOGIQUE
/*------------------------------------------------------------------------------------------------------------------------
                                        MOTEUR DU JEU BASE SUR QUEUE CIRCULAIRE DE TAILLE FIXE
--------------------------------------------------------------------------------------------------------------------------/*/

int main()
{
    srand(time(NULL));
    GrilleBonbons maGrille; // Déclaration Grille principale du jeu (tableau de bonbons)
    Queue q;                // Déclaration file circulaire
    EtatJeu etatJeu;        // Déclaration état du jeu (initialisation, niveaux, ...)

    InitialiserQueue(&q);                 // Initialisation des elements de la queue (debut, fin et taille)
    int ligne, colonne, ligne1, colonne1; // Variables de lecture utilisateur
    etatJeu.niveausuivant = 1;            // Déclenche l'initialisation du premier niveau
    etatJeu.findepartie = 0;              // fin de partie à 0
    etatJeu.coupsepuises = 0;             // aucun coup joue
    etatJeu.grillePrete = 0;              // La grille n’est pas encore prête

    while (NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        printf("[DEBUG] ⏳ Pré-initialisation — compteurNiveau=%d, coupsJoués=%d, niveausuivant=%d\n",
               NIVEAUX[0].compteurNiveau,
               NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues,
               etatJeu.niveausuivant);

        VerifierEtatJeu(&etatJeu, &q); // lance action initialisation
        while (q.taille > 0)
        {
            Actions action = Defiler(&q);

            switch (action.actionName)
            {
            case INITIALISATION:
                printf("[TRACE] 🚨 Appel à initialiserGrille() depuis [MAIN] ligne %d\n", __LINE__);
                initialiserGrille(&maGrille, &q, &etatJeu);
                break;

            case VERIFICATION:
                Verification(&maGrille, &q, &etatJeu);
                break;

            case CALCUL:
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y,
                       &action.pion2.x, &action.pion2.y, false);
                break;

            case SUPPRESSIONV:
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y,
                             &action.pion2.x, &action.pion2.y, &q);
                break;

            case SUPPRESSIONH:
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y,
                             &action.pion2.x, &action.pion2.y, &q);
                break;
            // ACTIONS RAJOUTEES POUR AFFICHER LES CASCADES EN DIRECT
            case CHUTELIGNEENTIERE:
                AppliquerChuteLigneEntiere(&maGrille, action.pion1.y, &q);
                PAUSE(100);
                break;

            case CHUTECOLONNEENTIERE:
                AppliquerChuteColonneEntiere(&maGrille, action.pion1.x, action.pion1.y, &q);
                PAUSE(100);
                break;

            case GENERATIONHAUT:
                AppliquerGenerationHaut(&maGrille, action.pion1.x, action.pion1.y, &q);
                break;

            case CHUTEPARTIELLE:
                AppliquerChuteVerticalePartielle(&maGrille, action.pion1.x, action.pion1.y,
                                                 action.pion2.x, &q);
                PAUSE(300);
                break;

            case CHUTEVERTICALEHORIZONTALE:
                AppliquerChuteVerticaleDepuisH(&maGrille, action.pion1.x, action.pion1.y, &q);
                break;

            case CHUTEVERTICALE:
                printf("[DEBUG] CHUTEVERTICALE déclenchée : ligne %d à %d, colonne %d\n",
                       action.pion1.x, action.pion2.x, action.pion1.y);
                LancerCascadeVerticale(&maGrille, action.pion1.x, action.pion2.x,
                                       action.pion1.y, &q);
                PAUSE(300);
                break;

            case AFFICHAGE:
                afficherGrille(&maGrille, &q, &etatJeu);
                PAUSE(300);
                break;

            case DEPLACEMENT:
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y,
                            action.pion2.x, action.pion2.y);
                break;

            case LECTURE:
                LirePionsAChanger(&maGrille, &ligne, &colonne, &ligne1, &colonne1, &q);
                break;

            case ERREURACTION:
                GererErreurFatale(action.erreur);
                break;
            }
        }
    }

    printf("🎮 FIN DU JEU\n"); // faudra afficher cela aussi
    return 0;
}
