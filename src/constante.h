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
// MESSAGE D'ETAT DU JEU
extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR];

// MESSAGE DE SAISIE UTILISATEUR POUR ECHANGE BONBONS
typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int nombreLigneOuColonne;
} MessagesEchangeBonbons;

extern MessagesEchangeBonbons MESSAGESECHANGEBONBONS[NOMBREMESSAGES];

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

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau; // Niveau actuel
} Niveaux;

// YABLEAU DE STUCTURES NIVEAU AVEC COMME LONGUEUR LE NOMBRE DE NIVEAUX
extern Niveaux NIVEAUX[FINALNIVEAU];

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