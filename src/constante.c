#include "constante.h"

/*________________________________________________________________________________________________________________
                                 **** CONSTANTES

     -> MESSAGEETREPONSESATTENDUES : Tableau de messages et de réponses attendues

     -> MESSAGEETATJEU : Tableau des messages de l'etat du jeu

     -> Structure des Niveaux
        * Elements : coupsNiveau - obstacleNiveau - compteurNiveau
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

const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR] = {
    "Vous avez epuise tous vos coups.\n",
    "Felicitations! Vous accedez u niveau suivant!\n",
    " Votre score est de :\n",
    "Vous avez perdu. Essayez encore.\n",
    "Fin du jeu\n"};

Niveaux niveaux[FINALNIVEAU] = {
    {{5, 0}, {"Gelatine", 1}, 0},
    {{3, 0}, {"Gelatine", 2}, 0},
    {{2, 0}, {"Gelatine", 4}, 0}};