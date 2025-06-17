#ifndef ERREUR_H
#define ERREUR_H

typedef enum
{
    PASDERREUR = 0,                       // 0 : pas d erreur
    ERREURQUEUEPLEINE = 1,                // 1 : Erreur queue pleine
    ERREURQUEUEVIDE = 2,                  // 2 : Erreur queue vide
    ERREURDEPLACEMENT = 3,                // 3 : Erreur liée au déplacement des pions par l'utilisateur
    TYPEINCONNU = 4,                      // 4 : Type inconnu
    ECHECGENERATIONCOULEURALEATOIRE = 5,  // 5 : Type inconnu
    ECHECGENERATIONTENTATIVES = 6,        // 6 : Erreur de génération aléatoire
    ECHECGENERATIONOBSTACLEALEATOIRE = 7, // 7 : Erreur de génération aléatoire
    ERREURENTREEINVALIDE = 8,             // 8 : Erreur entrée invalide (doit être un chiffre)
    ERREURNOMBREENTREEINCORRECTE = 9,     // 9: Chiffre hors plage grille
} CodeErreur;

void GererErreurFatale(CodeErreur codeErreur);
void GererErreurNonFatale(CodeErreur codeErreur);

#endif