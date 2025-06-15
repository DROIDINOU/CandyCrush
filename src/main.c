#define SDL_MAIN_HANDLED
#include <windows.h>
#include <mmsystem.h>
// #pragma comment(lib, "winmm.lib") // pour PlaySound
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrice.h"
#include "main.h"
#include "affichage.h"
#include "queue.h"
#include "erreur.h"
#include "constante.h"
#include "etatjeu.h"

int main(int argc, char *argv[]) // obligatoire pour playsound
{

    // DECLARATIONS ET INITIALISATIONS
    srand(time(NULL));

    GrilleBonbons maGrille; // declaration de la structure maGrille
    Queue q;                // declarationde la structure Queue
    EtatJeu etatJeu;        // declarationde la structure etatJeu

    int ligne, colonne, ligne1, colonne1; // declaration des parametres a encoder par utilisateur

    InitialiserQueue(&q); // initialisation des elements de la structure Queue
    // initialisation des elements de la structure etatJeu
    etatJeu.niveausuivant = 0;
    etatJeu.findepartie = 0;
    etatJeu.coupsepuises = 0;
    etatJeu.grillePrete = 0;
    etatJeu.introduction = 1;                // on affiche la grille d'intodution
    afficherGrille(&maGrille, &q, &etatJeu); // lance affichage introduction
    etatJeu.niveausuivant = 1;               // on passe au niveau suivant apres l intro

    // Moteur de jeu
    // tant que le niveau est un niveau du jeu
    while (NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {

        VerifierEtatJeu(&etatJeu, &q);                                                                    // initialise la grille et vérifie l'état du jeu(calcul est relance si la grille n est pas prete)
        PlaySound(CHEMINSMUSIQUES[NIVEAUX[0].compteurNiveau], NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // jouer musique de fond en arriere plan

        while (q.taille > 0) // Tant que la queue n'est pas vide on defile les actions
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
            case CHUTELIGNEENTIERE:
                AppliquerChuteLigneEntiere(&maGrille, action.pion1.y, &q);
                // PAUSE(100);
                break;
            case CHUTECOLONNEENTIERE:
                AppliquerChuteColonneEntiere(&maGrille, action.pion1.x, action.pion1.y, &q);
                // PAUSE(100);
                break;
            case GENERATIONHAUT:
                AppliquerGenerationHaut(&maGrille, action.pion1.x, action.pion1.y, &q);
                break;
            case CHUTEHORIZONTALEPARTIELLE:
                AppliquerChuteHorizontalePartielle(&maGrille, action.pion1.x, action.pion1.y, &q);
                break;
            case CHUTECOLONNEPARTIELLE:
                AppliquerChuteColonnePartielle(&maGrille, action.pion1.x, action.pion2.x,
                                               action.pion1.y, &q);
            case HORIZONTALDJOKER:
                ActiverJokerHorizontalVictoire(&maGrille, action.joker, &q);

                PAUSE(200);
                break;
            case AFFICHAGE:
                afficherGrille(&maGrille, &q, &etatJeu);
                PAUSE(200);
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

    PlaySound(NULL, NULL, 0); // stop musique de fond

    return 0;
}
