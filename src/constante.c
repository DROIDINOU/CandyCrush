#include "constante.h"

// ****************************             CONSTANTES GLOBALES                     **************************** //

// Définition des messages d'état du jeu.
// Type : tableau de characteres
const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR] = {
    "Vous avez épuisé tous vos coups.\n",
    "Félicitations! Vous accédez au niveau suivant!\n",
    "Votre score est de :\n",
    "Vous avez perdu. Essayez encore.\n",
    "Fin du jeu\n"};

// Tableau contenant les messages addressés à l'utilisateur et le type de reponse attendue
// Type : tableau de MessagesReponses (structure définie dans constante.h).
//        -élément de MessagesReponses:
//            - message (tableau de characteres): message d'affichage pour l'utilisateur.
//            - reponseChiffre(int): valeur attendue pour l'entrée utilisateur.
MessagesReponses MESSAGESREPONSES[NOMBREMESSAGES] = {
    {"Veuillez entrer la colonne 1 a ", TAILLE},
    {"Veuillez entrer la ligne 1 a ", TAILLE}};

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
Niveaux NIVEAUX[FINALNIVEAU] = {
    {{5, 0}, {"Gelatine", 1}, 0}, // Niveau 1
    {{3, 0}, {"Gelatine", 2}, 1}, // Niveau 2
    {{2, 0}, {"Gelatine", 4}, 2}  // Niveau 3
};
