#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>
#define TAILLE 20 // MISE A 10 POUR TESTING MAIS AU FINAL METTRE A 20 (devrait rien changer)
#define NOMBREMESSAGES 2
#define MAXLONGUEUR 100
#define LONGUEURMESSAGEETREPONSE 40
#define FINALNIVEAU 3
#define NOMBREMESSAGESJEU 7 // Nombre de messages concernant l'état du jeu
#define MAXCOULEUR 9

/*________________________________________________________________________________________________________________

                                                **** ENUMERATIONS

                    -> CouleurBonbons : couleurs des bonbons
                    -> TypeElementRandom : type d'élément aléatoire (couleur ou obstacle)
___________________________________________________________________________________________________________________
 */
typedef enum
{
    MESSAGE_COUPS_EPUISES,
    MESSAGE_FELICITATIONS,
    MESSAGE_SCORE,
    MESSAGE_PERDU,
    MESSAGE_FIN_JEU,
    MESSAGE_CHARGEMENT,
    MESSAGE_PIONS_NON_ADJACENTS
} TypeMessage;

typedef enum
{
    JAUNE,
    VERT,
    BLEU,
    ROUGE,
    MAUVE,
    BLANC,
    GRIS,
    JAUNE_CLAIR,
    ROSE
} CouleurBonbons;

typedef enum
{
    COULEURALEATOIRE,
    OBSTACLEALEATOIRE
} TypeElementRandom;

/*________________________________________________________________________________________________________________

                                                          **** STRUCTURES
                                            -> GrilleBonbons : structure de la grille de bonbons
                                            -> Coups : structure des coups
                                            -> Obstacles : structure des obstacles
                                            -> Niveaux : structure des niveaux
                                            -> MessagesEchangeBonbons : structure des messages d'échange de bonbons
___________________________________________________________________________________________________________________
*/

// *** Structures de la grille de bonbons

typedef struct
{
    CouleurBonbons pion;
    bool gelatine;
} Case;

typedef struct
{
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
    int calcX; // Coordonnée X utilisee parcourir la grille
    int calcY; // Coordonnée Y utilisee pour parcourir la grille
    int estVerifiee;
    int estInitialisee; // Indique si la grille a déjà été vérifiée
    // int calculsRestants; // Nombre d'actions CALCUL en attente
    int affiche; // element utilise pour afficher la grille
    int deplacement;

} GrilleBonbons;

// *** Structures pour les niveaux
typedef struct
{
    int coupAJouer; // Le nombre de coups à jouer
    int coupsJoues; // Le nombre de coups joués par le joueur
} Coups;

typedef struct
{
    char typeObstacle[20]; // EXEMPLE GELATINES MAIS ON VA EN METTRE D 'AUTRES PAR LA SUITE
    int randomObstacle;    // NOMBRE ALEATOIRE D OBSTACLE (A AMELIORER)
} Obstacles;

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau; // Niveau actuel
    int randomColorModulo;
} Niveaux;

typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int nombreLigneOuColonne;
} MessagesEchangeBonbons;

/*________________________________________________________________________________________________________________
                    **** CONSTANTES GLOBALES
                     -> NIVEAUX : tableau de niveaux
                     -> COULEURS : tableau de couleurs
                     -> MESSAGEETATJEU : tableau de messages d'état du jeu
                     -> MESSAGESECHANGEBONBONS : tableau de messages d'échange de bonbons
_________________________________________________________________________________________
 */

extern Niveaux NIVEAUX[FINALNIVEAU];                                  // Tableau des niveaux du jeux
extern const CouleurBonbons COULEURS[MAXCOULEUR];                     // Tableau des couleurs des bonbons
extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR];     // Tableau des messages d'état du jeu
extern MessagesEchangeBonbons MESSAGESECHANGEBONBONS[NOMBREMESSAGES]; // Tableau des messages d'échange de bonbons

#endif