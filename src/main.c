#define SDL_MAIN_HANDLED
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // pour PlaySound

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

int main(int argc, char *argv[])
{
    // Initialisation
    srand(time(NULL));
    // Démarre la musique de fond en boucle

    GrilleBonbons maGrille;
    Queue q;
    EtatJeu etatJeu;

    InitialiserQueue(&q);
    int ligne, colonne, ligne1, colonne1;
    etatJeu.niveausuivant = 1;
    etatJeu.findepartie = 0;
    etatJeu.coupsepuises = 0;
    etatJeu.grillePrete = 0;

    // Moteur de jeu
    while (NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        VerifierEtatJeu(&etatJeu, &q); // initialise la grille et vérifie l'état du jeu
        PlaySound(CHEMINSMUSIQUES[NIVEAUX[0].compteurNiveau], NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
                PAUSE(100);
                break;
            case CHUTECOLONNEENTIERE:
                AppliquerChuteColonneEntiere(&maGrille, action.pion1.x, action.pion1.y, &q);
                PAUSE(100);
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

    printf("🎮 FIN DU JEU\n");
    PlaySound(NULL, NULL, 0); // stop son

    return 0;
}
