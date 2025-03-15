#include <stdbool.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define TAILLE 10
#define NOMBREMESSAGES 2
#define MAXLONGUEUR 100
#define LONGUEURMESSAGEETREPONSE 40

#define FINALNIVEAU 3
#define NOMBREMESSAGESJEU 5 // Nombre de messages concernant l'état du jeu

/*________________________________________________________________________________________________________________
                    **** STRUCTURES GLOBALES:  MESSAGES D'ETAT DU JEU - MESSAGE DE SAISIE UTILISATEUR - NIVEAUX
___________________________________________________________________________________________________________________
 */

typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int reponseChiffre;
} MessagesReponses;

typedef struct
{
    int coupAJouer;
    int coupsJoues; // Le nombre de coups joués par le joueur
} Coups;

typedef struct
{
    char typeObstacle[20];
    int randomObstacle;
} Obstacles;

typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau;
} Niveaux;

extern MessagesReponses MESSAGESREPONSES[NOMBREMESSAGES];
extern Niveaux NIVEAUX[FINALNIVEAU];
extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR];

/*________________________________________________________________________________________________________________
                    **** STRUCTURES DE LA GRILLE DE BONBONS ET DES CASES DE LA GRILLE DE BONBONS
___________________________________________________________________________________________________________________
 */

typedef struct
{
    char pion;
    bool gelatine;
} Case;

typedef struct
{
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
    int calcX; // Coordonnée X qu’on est en train de vérifier
    int calcY; // Coordonnée Y qu’on est en train de vérifier
    int estVerifiee;
    int estInitialisee;  // Indique si la grille a déjà été vérifiée
    int calculsRestants; // Nombre d'actions CALCUL en attente
    int affiche;
    int deplacement;
    bool gelatinePresente;

} GrilleBonbons;

#endif