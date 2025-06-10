#include "etatjeu.h"
#include "constante.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************************************************************
                                                   ETAT DU JEU
        -> Verifie l'état du jeu sur base des élments de la structure d'état du jeu
           - int niveausuivant;
           - int findepartie;
           - int coupsepuises;
           - int findesniveaux; // verifier si utile
           - int grillePrete;  // flag utilise pour vérifier que la grille est stable

 ****************************************************************************************************************************/

void VerifierEtatJeu(EtatJeu *etatJeu, Queue *q)
{
    // Si niveaU SUIVANT est actif on initialise la grille
    if (etatJeu->niveausuivant == 1)
    {
        printf("[DEBUG] 🚀 Initialisation du niveau %d\n", NIVEAUX[0].compteurNiveau);
        Actions actionInit = {INITIALISATION, {0, 0}, {0, 0}};
        Enfiler(q, &actionInit);
        etatJeu->niveausuivant = 0;
        etatJeu->grillePrete = 0; // La grille sera prête après l'init complète
    }

    // garde fous liés aux risques de la queue circulaire, des sleep ou du programmeur... (essentiellement pour eviter de bloquer de
    // le jeu et pour debbugger)

    if (q->taille == 0 && !etatJeu->findepartie && !etatJeu->niveausuivant && etatJeu->grillePrete)
    {
        printf("[DEBUG] 🔁 Queue vide mais jeu en cours → relance Verification\n");
        Enfiler(q, &(Actions){CALCUL, {0, 0}, {0, 0}, false});
    }
    // Si la queue est vide et qu'on n'est pas en fin de partie ou niveau suivant, il y a un probleme
    // de conception du jeu
    if (q->taille == 0 && !etatJeu->niveausuivant && !etatJeu->findepartie)
    {
        GererErreurFatale(TYPEINCONNU);
    }
}