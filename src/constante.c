#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
// ****************************             CONSTANTES GLOBALES                     **************************** //

// Definition des couleurs des bonbons (on va en rajouter)
// Type : tableau de char representant les differentes couleurs (plus tard ce sera des images de bonbons)
const CouleurBonbons COULEURS[MAXCOULEUR] = {
    JAUNE, VERT, BLEU, ROUGE, MAUVE, BLANC, GRIS, JAUNE_CLAIR, ROSE};

// Définition des messages d'état du jeu.
// Type : tableau de characteres
// rmq peut etre mettre const char*
const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR] = {
    "Vous avez epuise tous vos coups.\n",
    "Felicitations! Vous accedez au niveau suivant!\n",
    "Votre score est de :\n",
    "Vous avez perdu. Essayez encore.\n",
    "Fin du jeu\n",
    " Chargement de la grille... \n",
    "les pions doivent etre adjacents\n",
}; // Message d'erreur si les pions ne sont pas adjacents

const char EXPLICATIONJOCKER[NOMBREMAXJOCKER][MAXLONGUEUREXPLICATIONJOCKER] = {
    "Pas de joker disponible",
    "Pas de jocker disponible",
    "Bombe explose la grille si\nadjacente a victoire horizontale",
    "Explication du joker 4",
    "Explication du joker 5"};

// Tableau contenant les messages addressés à l'utilisateur et le type de reponse attendue
// Type : tableau de MessagesReponses (structure définie dans constante.h).
//        -élément de MessagesReponses:
//            - message (tableau de characteres): message d'affichage pour l'utilisateur.
//            - nombreLigneOuColonne(int): valeur attendue pour l'entrée utilisateur.
MessagesEchangeBonbons MESSAGESECHANGEBONBONS[NOMBREMESSAGES] = {
    {"🎮 Veuillez entrer la colonne 1 a", TAILLE},
    {"🎮 Veuillez entrer la ligne 1 a", TAILLE}};

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
//             - nombre utilisé pour l attribution aléatoire de couleurs
Niveaux NIVEAUX[FINALNIVEAU] = {
    {
        {7, 0},
        {"Gelatine", 1},
        0,
        5,
        {false, 0, PASDESUPERBONBON},
    },                                                              // Niveau 1
    {{6, 0}, {"Gelatine", 2}, -1, 6, {false, 0, PASDESUPERBONBON}}, // Niveau 2
    {{5, 0}, {"Gelatine", 4}, -1, 7, {true, 1, SUPPRIMERGRILLE}}    // Niveau 3
};

const char CHEMINSMUSIQUES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE] = {
    "assets/play.wav",
    "assets/stranger-things-124008.wav",
    "assets/kim-lightyear-legends-109307.wav",

};

const char CHEMINSMUSIQUESCHUTES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE] = {
    "assets/item-pick-up-38258.wav",
    "assets/jellysplash_56f256e05113918.wav",
    "assets/male_death.wav",
};
