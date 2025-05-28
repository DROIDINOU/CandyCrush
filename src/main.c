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
    srand(time(NULL));
    GrilleBonbons maGrille;
    Queue q;
    InitialiserQueue(&q);
    int ligne, colonne, ligne1, colonne1;

    EtatJeu etatJeu;
    etatJeu.niveausuivant = 1;
    etatJeu.findepartie = 0;
    etatJeu.coupsepuises = 0;
    etatJeu.grillePrete = 0; // ✅ La grille n’est pas encore prête

    while (NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        printf("[DEBUG] ⏳ Pré-initialisation — compteurNiveau=%d, coupsJoués=%d, niveausuivant=%d\n",
               NIVEAUX[0].compteurNiveau,
               NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues,
               etatJeu.niveausuivant);

        // ✅ Initialiser le niveau si demandé
        if (etatJeu.niveausuivant == 1)
        {
            printf("[DEBUG] 🚀 Initialisation du niveau %d\n", NIVEAUX[0].compteurNiveau);
            Actions actionInit = {INITIALISATION, {0, 0}, {0, 0}};
            Enfiler(&q, &actionInit);
            etatJeu.niveausuivant = 0;
            etatJeu.grillePrete = 0; // ✅ La grille n’est pas encore prête
        }

        // 🛡 Si la queue est vide et qu'on n’a pas fini → relancer une vérification
        if (q.taille == 0 && !etatJeu.findepartie && !etatJeu.niveausuivant && etatJeu.grillePrete)
        {
            printf("[DEBUG] 🔁 Queue vide mais jeu en cours → relance Verification\n");
            Enfiler(&q, &(Actions){VERIFICATION, {0, 0}, {0, 0}, false});
        }

        // 🛑 Si plus d'action ET pas de passage de niveau → blocage, on casse
        if (q.taille == 0 && !etatJeu.niveausuivant && !etatJeu.findepartie)
        {
            printf("[ERREUR] ❌ Bloqué sans action et sans passage de niveau. Fin forcée.\n");
            break;
        }

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
                if (!maGrille.estInitialisee)
                {
                    printf("[DEBUG] AppliquerSuppressions() déclenché après Calcul\n");
                    AppliquerSuppressions(&maGrille, &q);
                }
                break;

            case SUPPRESSIONV:
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y,
                             &action.pion2.x, &action.pion2.y, &q);
                break;

            case SUPPRESSIONH:
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y,
                             &action.pion2.y, &action.pion2.x, &q);
                break;

            case CHUTELIGNEENTIERE:
                AppliquerChuteLigne(&maGrille, action.pion1.y, &q);
                PAUSE(100);
                break;

            case CHUTECOLONNEENTIERE:
                AppliquerChuteColonne(&maGrille, action.pion1.x, action.pion1.y, &q);
                PAUSE(100);
                break;

            case PREPAREGENERATIONHAUT:
                afficherGrille(&maGrille, &q, &etatJeu);
                PAUSE(150);
                Enfiler(&q, &(Actions){GENERATIONHAUT, {action.pion1.x, action.pion1.y}, {0, 0}, false});
                break;

            case GENERATIONHAUT:
                AppliquerGenerationHaut(&maGrille, action.pion1.x, action.pion1.y, &q);
                break;

            case LANCERCASCADEV:
                LancerCascadeVerticale(&maGrille, action.pion1.x, action.pion2.x, action.pion1.y, &q);
                PAUSE(200);
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

            case RELANCERCALCUL:
                maGrille.calcX = 0;
                maGrille.calcY = 0;
                Enfiler(&q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
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

    printf("🎮 FIN DU JEU\n");
    return 0;
}
