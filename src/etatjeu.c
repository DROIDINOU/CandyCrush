
// #include <stdio.h> // Fonctions d’entrée/sortie (printf, scanf, etc.)
#include "etatjeu.h"
#include "constante.h"
#include "queue.h"
#include "erreur.h"

/***************************************************************************************************************************
                                                   ETAT DU JEU
        -> Verifie l'état du jeu sur base des élments de la structure d'état du jeu
           - int niveausuivant;
           - int findepartie;
           - int coupsepuises;
           - int grillePrete;  // flag utilise pour vérifier que la grille est stable

 ****************************************************************************************************************************/

void InitGrilleEtVerifierEtatJeu(EtatJeu *etatJeu, Queue *q)
{
    // Si niveaU SUIVANT est actif on initialise la grille
    if (etatJeu->niveausuivant == 1)
    {
        Actions actionInit = {INITIALISATION, {0, 0}, {0, 0}};
        Enfiler(q, &actionInit);
        etatJeu->niveausuivant = 0;
        etatJeu->grillePrete = 0; // La grille sera prête après l'init complète
    }

    // essentiellement pour debug et pour eviter blocage jeu si probleme de desynchronisation
    // Si la queue est vide alors que la grille est prête,
    // qu'on n'est pas en fin de partie, ni en changement de niveau :
    // Cela signifie que plus aucune action n'est prévue, ce qui ne devrait pas arriver
    // Pour éviter un blocage du moteur on force un recalcul de la grille.
    // Cela permet de relancer une détection de victoire potentielle, ou un mouvement restant.

    if (q->taille == 0 && !etatJeu->findepartie && !etatJeu->niveausuivant && etatJeu->grillePrete)
    {
        // printf(" Queue vide mais jeu en cours -> relance Verification\n");
        Actions actionCalcul = {CALCUL, {0, 0}, {0, 0}, false, PASDESUPERBONBON};
        Enfiler(q, &actionCalcul);
    }
    // Si la queue est vide et qu'on n'est pas en fin de partie ou niveau suivant, il y a un probleme
    // de conception du jeu
    // idem on devrait jamais arriver ici
    if (q->taille == 0 && !etatJeu->niveausuivant && !etatJeu->findepartie)
    {
        GererErreurFatale(TYPEINCONNU);
    }
}