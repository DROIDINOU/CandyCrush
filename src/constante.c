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
// changer ca en tableau de structure

const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR] = {
    "Vous avez epuise tous vos coups.\n",
    "Felicitations! Vous accedez au niveau suivant!\n",
    " Votre score est de :\n",
    "Vous avez perdu. Essayez encore.\n",
    "Fin du jeu\n"};

MessagesEtReponses MessagesReponses[NOMBREMESSAGES] = {
    {"Veuillez entrer la colonne 1 a", TAILLE},
    {"Veuillez entrer la ligne 1 a", TAILLE}};

Niveaux niveaux[FINALNIVEAU] = {
    {{5, 0}, {"Gelatine", 1}, 0},
    {{3, 0}, {"Gelatine", 2}, 0},
    {{2, 0}, {"Gelatine", 4}, 0}};