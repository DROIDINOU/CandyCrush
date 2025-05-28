#include "etatjeu.h"
#include "constante.h"
#include "queue.h"
#include <stdio.h>

void VerifierEtatJeu(EtatJeu *etatJeu, Queue *q)
{
    // ✅ Initialiser le niveau si demandé
    if (etatJeu->niveausuivant == 1)
    {
        printf("[DEBUG] 🚀 Initialisation du niveau %d\n", NIVEAUX[0].compteurNiveau);
        Actions actionInit = {INITIALISATION, {0, 0}, {0, 0}};
        Enfiler(q, &actionInit);
        etatJeu->niveausuivant = 0;
        etatJeu->grillePrete = 0; // La grille sera prête après l'init complète
    }

    // 🔁 Si la queue est vide mais le jeu continue, on relance la vérification
    if (q->taille == 0 && !etatJeu->findepartie && !etatJeu->niveausuivant && etatJeu->grillePrete)
    {
        printf("[DEBUG] 🔁 Queue vide mais jeu en cours → relance Verification\n");
        Enfiler(q, &(Actions){VERIFICATION, {0, 0}, {0, 0}, false});
    }

    // ❌ Cas bloquant : plus d'action, pas de niveau, pas de fin → on sort
    if (q->taille == 0 && !etatJeu->niveausuivant && !etatJeu->findepartie)
    {
        printf("[ERREUR] ❌ Bloqué sans action et sans passage de niveau. Fin forcée.\n");
        exit(EXIT_FAILURE);
    }
}