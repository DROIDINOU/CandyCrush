#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

// GRILLE
#define TAILLE 10    // Taille de la grille (10x10) (maximum 20 vu que la grille est fixe ecran peut pas contenir plus)
#define MAXCOULEUR 9 // Maximum de couleurs de bonbons (jokers exclus)
#define VIDE -1      // Valeur pour une case vide dans la grille

// MESSAGES UTILISATEUR (messages état jeu et échange de bonbons)
#define NOMBREMESSAGES 2            // Nombre de messages pir l echange de bonbons
#define MAXLONGUEUR 100             // Longueur maximale des messages d'état du jeu
#define LONGUEURMESSAGEETREPONSE 40 // Longueur maximale des messages d'échange de bonbons
#define NOMBREMESSAGESJEU 7         // Nombre de messages concernant l'état du jeu

// NIVEAUX
#define FINALNIVEAU 3

// MUSIQUE
#define NOMBREMUSIQUES 3     // Nombre de musiques disponibles
#define MAXCHEMINMUSIQUE 100 // Longueur maximale du chemin de la musique

// JOKERS
#define PLAGEJOCKERINF 1000              // Plage inférieure pour les jokers fixéé arbitrairement a 1000 (type de joker)
#define PLAGEJOCKERSUP 2000              // Plage supérieure pour les jokers fixéé arbitrairement a 1100 (type de joker)
#define DECALAGEAFFICHAGEJOKER 1000      // Décalage pour l'affichage des jokers
#define NOMBREMAXJOCKER 1000             // Nombre maximum de jokers (on voit large!!)
#define MAXLONGUEUREXPLICATIONJOCKER 100 // Longueur maximale de l'explication des jokers

/*________________________________________________________________________________________________________________

                                                **** ENUMERATIONS

                    -> TypeMessage : type de messages
                    -> CouleurBonbons : couleurs des bonbons et jokers
                    -> TypeElementRandom : type d'élément aléatoire (couleur ou obstacle) (on pourrait l'utiliser pour les jokers mais
                    pour le nombre de jocker est fixe)
___________________________________________________________________________________________________________________
 */
typedef enum
{
    MESSAGECOUPSEPUISES,
    MESSAGEFELICITATIONS,
    MESSAGESCORE,
    MESSAGEPERDU,
    MESSAGEFINJEU,
    MESSAGECHARGEMENT,
    MESSAGEPIONSNONADJACENTS,
    MESSAGEINITIALISATION,
} TypeMessage;

typedef enum
{
    // bonbons classiques
    // limite egale MAXCOULEUR
    JAUNE = 1,
    VERT = 2,
    BLEU = 3,
    ROUGE = 4,
    MAUVE = 5,
    BLANC = 6,
    GRIS = 7,
    JAUNE_CLAIR = 8,
    ROSE = 9,
    // bonbons spéciaux (jokers) - sans effet de chute (1000-1999)
    // si chute definir une plage pour les chutes
    PASDESUPERBONBON = -1000,
    SUPPRIMERGRILLE = 1000,
    SUPPRIMERMOITIEGRILLE = 1001,
    SUPPRIMERTROIsLIGNES = 1002,
    REDUIREGRILLEDEMOITIE = 1003,

    // bonbons effets des joker sans effet de chute (2000-2999)
    // si chute definir une plage pour les chutes
    BONBONVICTOIRESUPPRIMERGRILLE = 2000,
} CouleurBonbons; // renommer

// enum pour les types d'éléments aléatoires
//  (couleurs ou obstacles) pour la génération aléatoire
//  on pourrait l'utiliser pour les jokers mais pour le nombre de jocker est fixe
typedef enum
{
    COULEURALEATOIRE,
    OBSTACLEALEATOIRE
} TypeElementRandom;

/*________________________________________________________________________________________________________________

                                                          **** STRUCTURES
                                            -> EtatJeu : structure de l'état du jeu
                                            -> GrilleBonbons : structure de la grille de bonbons
                                            -> Coups : structure des coups
                                            -> Obstacles : structure des obstacles
                                            -> Niveaux : structure des niveaux
                                            -> MessagesEchangeBonbons : structure des messages d'échange de bonbons
___________________________________________________________________________________________________________________
*/

// *** Structure de l'état du jeu
typedef struct
{
    int introduction;  // Indique si l'introduction a été affichée
    int coupsepuises;  // Indique si les coups sont épuisés
    int niveausuivant; // Indique si le niveau suivant est prêt à être joué
    int findepartie;   // Indique si la partie est terminée
    int grillePrete;   // Indique si la grille est prête

} EtatJeu;

/*________________________________________________________________________________________________________________
                                                           GRILLE
_________________________________________________________________________________________________________________
*/

// *** Structure de la case de la grille de bonbons
typedef struct
{
    CouleurBonbons pion; // Couleur du pion
    bool gelatine;       // Indique si la case contient de la gélatine
} Case;

// *** Structure de la grille de bonbons
typedef struct
{
    int lignes;                   // Nombre de lignes de la grille
    int colonnes;                 // Nombre de colonnes de la grille
    Case tableau[TAILLE][TAILLE]; // Tableau de cases représentant la grille de bonbons
    int calcX;                    // Coordonnée X utilisee parcourir la grille
    int calcY;                    // Coordonnée Y utilisee pour parcourir la grille
    int deplacement;              // Indique si un déplacement a été effectué
    bool relancerDepuisDebut;     // Indique si la grille doit être relancée depuis le début
    int nombreGelatineDuNiveau;   // Nombre de gelatines à générer pour le niveau
    int nombreGelatinesRestantes; // Nombre de gelatines à générer

} GrilleBonbons;

/*________________________________________________________________________________________________________________
                                                           NIVEAUX
_________________________________________________________________________________________________________________
*/
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

typedef struct
{
    bool aSuperBonbon;          // Indique si un super bonbon est disponible
    int nombreSuperBonbons;     // Nombre de super bonbons disponibles
    CouleurBonbons superBonbon; // Couleur du super bonbon
} JokerBonbons;

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    Coups coupsNiveau;        // Structure des coups pour le niveau
    Obstacles obstacleNiveau; // Structure des obstacles pour le niveau
    int compteurNiveau;       // Niveau actuel
    int randomColorModulo;    // Modulo pour la génération de couleurs aléatoires
    JokerBonbons joker;       // Structure des jokers pour le niveau
} Niveaux;

/*________________________________________________________________________________________________________________
                                                           MESSAGE DEPLACEMENT
_________________________________________________________________________________________________________________
*/
typedef struct
{
    char message[LONGUEURMESSAGEETREPONSE];
    int nombreLigneOuColonne; // Valeur attendue pour l'entrée utilisateur (colonne ou ligne)
} MessagesEchangeBonbons;

/*________________________________________________________________________________________________________________
                    **** CONSTANTES GLOBALES
                     -> NIVEAUX : tableau de niveaux
                     -> COULEURS : tableau de couleurs
                     -> MESSAGEETATJEU : tableau de messages d'état du jeu
                     -> MESSAGESECHANGEBONBONS : tableau de messages d'échange de bonbons
                     -> CHEMINSMUSIQUES : tableau de chemins des musiques
_________________________________________________________________________________________
 */

extern Niveaux NIVEAUX[FINALNIVEAU];                              // Tableau des niveaux du jeux
extern const CouleurBonbons COULEURS[MAXCOULEUR];                 // Tableau des couleurs des bonbons
extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR]; // Tableau des messages d'état du jeu
extern const char EXPLICATIONJOCKER[NOMBREMAXJOCKER][MAXLONGUEUREXPLICATIONJOCKER];
extern MessagesEchangeBonbons MESSAGESECHANGEBONBONS[NOMBREMESSAGES];      // Tableau des messages d'échange de bonbons
extern const char CHEMINSMUSIQUES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE];       // Tableau des chemins des musiques
extern const char CHEMINSMUSIQUESCHUTES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE]; // Tableau des chemins des musiques de chute

#endif