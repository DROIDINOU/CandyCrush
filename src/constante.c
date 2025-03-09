#include "constante.h"

/*________________________________________________________________________________________________________________
                                 **** CONSTANTES
     -> Tableau de messages et de réponses attendues
     -> Parametres : NOMBREMESSAGES - MAXLONGUEUR
     -> Structure des Niveaux
     -> Elements : coupsNiveau - obstacleNiveau - compteurNiveau
               -> coupsNiveau : nombre de coups à jouer (int) - coups joués (int)
               -> obstacleNiveau : type d'obstacle (char) - nombre de gelatines (int)
               -> compteurNiveau : niveau actuel (int)
_____________________________________________________________________________________________
 */

const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR] = {
    {"\nVoulez-vous continuer?", "O", "N"},
    {"\nMode affichage simple ou complet?", "S", "C"},
    {"Veuillez entrer la colonne (a, b, c, d, e, f, g, h) : ", "abcdefgh"},
    {"Veuillez entrer la ligne (1,2,3,4,5,6,7,8) : ", "12345678"}};

Niveaux niveaux[FINALNIVEAU] = {
    {{3, 0}, {"Gelatine", 2}, 0},
    {{1, 0}, {"Blocage", 4}, 1},
    {{2, 0}, {"Explosion", 8}, 2}};