#include "constante.h"
#include <stdio.h>
#include <stdlib.h>
// ****************************             CONSTANTES GLOBALES                     **************************** //

// Definition des couleurs des bonbons (on va en rajouter)
// Type : tableau de char representant les differentes couleurs (dans bibliotheque jeu on utilisera des images)
const CouleurBonbons COULEURS[MAXCOULEUR] = {
    JAUNE, VERT, BLEU, ROUGE, MAUVE, BLANC, GRIS, JAUNE_CLAIR, ROSE};

// Définition des messages d'état du jeu.
// Type : tableau de characteres
const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR] = {
    "Vous avez epuise tous vos coups.\n",
    "Felicitations! Vous accedez au niveau suivant!\n",
    "Votre score est de :\n",
    "Vous avez perdu. Essayez encore.\n",
    "Fin du jeu\n",
    " Chargement de la grille... \n",
    "les pions doivent etre adjacents\n",
};

// explication des jokers disponibles par niveau
const char EXPLICATIONJOCKER[NOMBREMAXJOCKER][MAXLONGUEUREXPLICATIONJOCKER] = {
    "Pas de joker disponible",
    "Pas de jocker disponible",
    "Bombe explose la grille si\n   adjacente a victoire horizontale",
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
//             - compteurNiveau (int) : numéro du niveau actuel. (utilise dans niveau 1 -> -1 ailleurs)
//             - randomColorModulo (int) : nombre utilisé pour l attribution aléatoire de couleurs
//             - joker : sous-structure contenant :
//                  - aSuperBonbon (bool) : indique si un super bonbon est disponible.
//                  - nombreSuperBonbons (int) : nombre de super bonbons disponibles.
//                  - superBonbon (CouleurBonbons) : couleur du super bonbon.
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

// tableau des chemins fichiers audio musique niveau
const char CHEMINSMUSIQUES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE] = {
    "assets/play.wav",
    "assets/stranger-things-124008.wav",
    "assets/kim-lightyear-legends-109307.wav",

};

// tableau des chemins audio musiques chute du niveau
const char CHEMINSMUSIQUESCHUTES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE] = {
    "assets/item-pick-up-38258.wav",
    "assets/jellysplash_56f256e05113918.wav",
    "assets/male_death.wav",
};
