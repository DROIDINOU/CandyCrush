#include "constante.h"

const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR] = {
    {"\nVoulez-vous continuer?", "O", "N"},
    {"\nMode affichage simple ou complet?", "S", "C"},
    {"Veuillez entrer la colonne (a, b, c, d, e, f, g, h) : ", "abcdefgh"},
    {"Veuillez entrer la ligne (1,2,3,4,5,6,7,8) : ", "12345678"}};

// Definition des couleurs des bonbons (on va en rajouter)
// Type : tableau de char representant les differentes couleurs (plus tard ce sera des images de bonbons)
const char COULEURS[MAXCOULEUR] = {'J', 'V', 'B', 'R', 'M', 'W', 'G', 'Y', 'P'};

// Définition des niveaux du jeu sous forme d'un tableau de structures.
// Type : tableau de Niveaux (structure définie dans constante.h).
//        -élément de Niveau :
//             - coups : sous-structure contenant :
//                  - coupsRestants (int) : nombre de coups disponibles.
//                  - coupsJoues (int) : nombre de coups déjà utilisés.
//             - obstacles : sous-structure contenant :
//                  - typeObstacle (char[MAX_LONGUEUR]) : type d'obstacle.
//                  - randomObstacles (int) : quantité d'obstacles à éliminer.
//             - compteurNiveau (int) : numéro du niveau actuel.
//             - nombre utilisé pour l attribution aléatoire de gelatines et éventuellement de bonbons
Niveaux NIVEAUX[FINALNIVEAU] = {
    {{5, 0}, {"Gelatine", 1}, 0, 5},  // Niveau 1
    {{3, 0}, {"Gelatine", 2}, -1, 6}, // Niveau 2
    {{2, 0}, {"Gelatine", 4}, -1, 7}  // Niveau 3
};
