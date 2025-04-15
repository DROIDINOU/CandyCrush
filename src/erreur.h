#ifndef ERREUR_H
#define ERREUR_H

typedef enum
{
    ERREURQUEUEPLEINE, // 0 : Aucun problème
    ERREURQUEUEVIDE,   // 1 : Erreur liée à la mémoire
    ERREURDEPLACEMENT, // 2 : Erreur liée au déplacement des pions par l'utilisateur
    TYPEINCONNU,
    ECHECGENERATIONCOULEURALEATOIRE,  // 3 : Type inconnu
    ECHECGENERATIONTENTATIVES,        // 4 : Erreur de génération aléatoire
    ECHECGENERATIONOBSTACLEALEATOIRE, // 5 : Erreur de génération aléatoire
} CodeErreur;

void GererErreurFatale(CodeErreur codeErreur);
void GererErreurNonFatale(CodeErreur codeErreur);

#endif
