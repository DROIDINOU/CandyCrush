#ifndef ERREUR_H
#define ERREUR_H

typedef enum
{
    PASDERREUR = 0,
    ERREURQUEUEPLEINE, // 0 : Aucun problème
    ERREURQUEUEVIDE,   // 1 : Erreur liée à la mémoire
    ERREURDEPLACEMENT, // 2 : Erreur liée au déplacement des pions par l'utilisateur
    TYPEINCONNU,
    ECHECGENERATIONCOULEURALEATOIRE,  // 3 : Type inconnu
    ECHECGENERATIONTENTATIVES,        // 4 : Erreur de génération aléatoire
    ECHECGENERATIONOBSTACLEALEATOIRE, // 5 : Erreur de génération aléatoire
    ERREURENTREEINVALIDE,             // 6 : Erreur entrée invalide (doit être un chiffre)
    ERREURNOMBREENTREEINCORRECTE,     // 7: Chiffre hors plage grille
} CodeErreur;

void GererErreurFatale(CodeErreur codeErreur);
void GererErreurNonFatale(CodeErreur codeErreur);

#endif