#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

#define TAILLE 10 // MISE A 10 POUR TESTING MAIS AU FINAL METTRE A 20 (devrait rien changer)
#define NOMBREMESSAGES 2
#define MAXLONGUEUR 100
#define LONGUEURMESSAGEETREPONSE 40

#define FINALNIVEAU 3
#define NOMBREMESSAGESJEU 6 // Nombre de messages concernant l'état du jeu
#define MAXCOULEUR 9

/*________________________________________________________________________________________________________________
                    **** STRUCTURES GLOBALES:  MESSAGES D'ETAT DU JEU - MESSAGE DE SAISIE UTILISATEUR - NIVEAUX
___________________________________________________________________________________________________________________
 */
// PEUT ETRE METTRE CELA DANS MATRICE VU QU ON L UTILISE QUE LA

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

extern const CouleurBonbons COULEURS[MAXCOULEUR];
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

typedef enum
{
    ALEATOIRECOULEUR,
    ALEATOIREOBSTACLE
} TypeElementRandom;

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau; // Niveau actuel
    int randomColorModulo;
} Niveaux;

// YABLEAU DE STUCTURES NIVEAU AVEC COMME LONGUEUR LE NOMBRE DE NIVEAUX
extern Niveaux NIVEAUX[FINALNIVEAU];

/*________________________________________________________________________________________________________________
                    **** STRUCTURES DE LA GRILLE DE BONBONS ET DES CASES DE LA GRILLE DE BONBONS
___________________________________________________________________________________________________________________
 */

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

#endif