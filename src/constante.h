#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>
#define TAILLE 12 // MISE A 10 POUR TESTING MAIS AU FINAL METTRE A 20 (devrait rien changer)
#define NOMBREMESSAGES 2
#define MAXLONGUEUR 100
#define LONGUEURMESSAGEETREPONSE 40
#define FINALNIVEAU 3
#define NOMBREMESSAGESJEU 7 // Nombre de messages concernant l'état du jeu
#define MAXCOULEUR 9        // Maximum de couleurs de bonbons (jokers exclus)
#define VIDE -1
#define NOMBREMUSIQUES 3     // Nombre de musiques disponibles
#define MAXCHEMINMUSIQUE 100 // Longueur maximale du chemin de la musique
#define PLAGEJOCKERINF 1000
#define PLAGEJOCKERSUP 1100          // Plage supérieure pour les jokers fixéé arbitrairement a 1100
#define PLAGEINFBONBONSDEJOCKER 2000 // Plage inférieure pour les bonbons de joker
#define PLAGESUPBONBONSDEJOCKER 2100 // Plage supérieure pour les bonbons de joker
#define NOMBREMAXJOCKER 100
#define MAXLONGUEUREXPLICATIONJOCKER 100

/*________________________________________________________________________________________________________________

                                                **** ENUMERATIONS

                    -> TypeMessage : type de messages
                    -> CouleurBonbons : couleurs des bonbons
                    -> TypeElementRandom : type d'élément aléatoire (couleur ou obstacle)
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
    PASDESUPERBONBON = -1000,
    SUPPRIMERGRILLE = 1000,
    SUPPRIMERMOITIEGRILLE = 1001,
    SUPPRIMERTROIsLIGNES = 1002,
    REDUIREGRILLEDEMOITIE = 1003,

    // bonbons effets des joker sans effet de chute (2000-2999)
    BONBONVICTOIRESUPPRIMERGRILLE = 2000,
} CouleurBonbons; // renommer

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

typedef struct
{
    int niveausuivant;
    int findepartie;
    int coupsepuises;
    int findesniveaux; // verifier si utile
    int grillePrete;
    int introduction;
} EtatJeu;

typedef struct
{
    CouleurBonbons pion;
    bool gelatine;
} Case;

// *** Structures de la grille de bonbons
// supprimer ce qui n est pas utilise
typedef struct
{
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
    int calcX; // Coordonnée X utilisee parcourir la grille
    int calcY; // Coordonnée Y utilisee pour parcourir la grille
    int estVerifiee;
    // int estInitialisee; // Indique si la grille a déjà été vérifiée
    int affiche; // element utilise pour afficher la grille
    int deplacement;
    bool relancerDepuisDebut; // 🆕
    // bool casesAGenerer[TAILLE];
    int nombreGelatineDuNiveau;
    int nombreGelatinesRestantes; // Nombre de gelatines à générer

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

typedef struct
{
    bool aSuperBonbon;
    int nombreSuperBonbons;
    CouleurBonbons superBonbon; // EXEMPLE GELATINES MAIS ON VA EN METTRE D 'AUTRES PAR LA SUITE
} JokerBonbons;

// STRUCTURE IMBRIQUEE NIVEAU
typedef struct
{
    Coups coupsNiveau;
    Obstacles obstacleNiveau;
    int compteurNiveau; // Niveau actuel
    int randomColorModulo;
    JokerBonbons joker; // A voir si on en a besoin
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
                     -> CHEMINSMUSIQUES : tableau de chemins des musiques
_________________________________________________________________________________________
 */

extern Niveaux NIVEAUX[FINALNIVEAU];                              // Tableau des niveaux du jeux
extern const CouleurBonbons COULEURS[MAXCOULEUR];                 // Tableau des couleurs des bonbons
extern const char MESSAGEETATJEU[NOMBREMESSAGESJEU][MAXLONGUEUR]; // Tableau des messages d'état du jeu
extern const char EXPLICATIONJOCKER[NOMBREMAXJOCKER][MAXLONGUEUREXPLICATIONJOCKER];
extern MessagesEchangeBonbons MESSAGESECHANGEBONBONS[NOMBREMESSAGES]; // Tableau des messages d'échange de bonbons
extern const char CHEMINSMUSIQUES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE];  // Tableau des chemins des musiques
extern const char CHEMINSMUSIQUESCHUTES[NOMBREMUSIQUES][MAXCHEMINMUSIQUE];

#endif