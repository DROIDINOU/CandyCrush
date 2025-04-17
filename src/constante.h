// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>
#include "erreur.h"
#define FINALNIVEAU 3
// Déclaration d'une constante avec extern
#define TAILLE 10
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100
#define MAXCOULEUR 9
#define NB_COULEURS 9

/* #define LONGUEURMESSAGEETREPONSE 40
#define NOMBREMESSAGESJEU 7 // Nombre de messages concernant l'état du jeu*/

// PEUT ETRE METTRE CELA DANS MATRICE VU QU ON L UTILISE QUE LA
extern const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR];

/*

typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int nombreLigneOuColonne;
} MessagesEchangeBonbons;

extern const CouleurBonbons COULEURS[MAXCOULEUR];                     // Tableau des couleurs des bonbons
extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR];     // Tableau des messages d'état du jeu
extern MessagesEchangeBonbons MESSAGESECHANGEBONBONS[NOMBREMESSAGES]; // Tableau des messages d'échange de bonbons








*/

/*________________________________________________________________________________________________________________

                                                **** ENUMERATIONS
                    -> TypeMessage : type de message (erreur, succès, etc.)
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

extern const CouleurBonbons COULEURS[MAXCOULEUR];

// supprimer coordonees
typedef struct
{
    int x;
    int y;
} Coordonnees;

typedef enum
{
    INITIALISATION,
    AFFICHAGE,
    VERIFICATION,
    SUPPRESSIONH,
    SUPPRESSIONV,
    CALCUL,
    DEPLACEMENT,
    LECTURE,
    FINNIVEAU,
    FIN,
    AUCUNE_EXPLOSION,
} ActionType;
// supprimer Actions

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
    int calcX;           // Coordonnée X qu’on est en train de vérifier
    int calcY;           // Coordonnée Y qu’on est en train de vérifier
    int estVerifiee;     // Indique si la grille a déjà été vérifiée
    int calculsRestants; // Nombre d'actions CALCUL en attente
    int suppressionsRestantes;
    int affiche;
    int finniveau;
    ActionType lastAction; // Ajout pour mémoriser la dernière action
    Coordonnees pion1Affiche;
    Coordonnees pion2Affiche;
    int deplacement;
    int estInitialisee;

} GrilleBonbons;

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    int coupAJouer;
    int coupsJoues; // Le nombre de coups joués par le joueur
} Coups;

typedef struct
{
    char typeObstacle[20]; // EXEMPLE GELATINES MAIS ON VA EN METTRE D 'AUTRES PAR LA SUITE
    int randomObstacle;    // NOMBRE ALEATOIRE D OBSTACLE (A AMELIORER)
} Obstacles;

typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau; // Niveau actuel
    int randomColorModulo;
} Niveaux;

/*
typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int nombreLigneOuColonne;
} MessagesEchangeBonbons;
 */

/*________________________________________________________________________________________________________________
                   **** CONSTANTES GLOBALES
                    -> NIVEAUX : tableau de niveaux
                    -> COULEURS : tableau de couleurs
                    -> MESSAGEETATJEU : tableau de messages d'état du jeu
                    -> MESSAGESECHANGEBONBONS : tableau de messages d'échange de bonbons
_________________________________________________________________________________________
*/

// YABLEAU DE STUCTURES NIVEAU AVEC COMME LONGUEUR LE NOMBRE DE NIVEAUX
extern Niveaux NIVEAUX[FINALNIVEAU];

#endif // CONSTANTS_H