#include "queue.h"
#define TAILLE 20  // Taille de la grille
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR] = {
     {"\nVoulez-vous continuer?","O","N"},
    {"\nMode affichage simple ou complet?","S","C"},
    {"Veuillez entrer la colonne (a, b, c, d, e, f, g, h) : ", "abcdefgh"},
    {"Veuillez entrer la ligne (1,2,3,4,5,6,7,8) : ", "12345678"} 
    };


/***************************************************************************************************************************
                                                  INIT GRILLE

 ****************************************************************************************************************************/
typedef struct {
    char pion;
    bool gelatine;
} Case;

typedef struct {
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
} GrilleBonbons;

void initialiser_grille(GrilleBonbons *grille);
void initialiserGrilleGelatine(GrilleBonbons *grille);

void afficher_grille(GrilleBonbons *grille);
int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index);


void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion, Queue *q );

typedef struct {
    int coupAJouer;
} Niveau;


/*Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes


/*_____________________________________________________________________________________________________________________________*/

void Calcul(GrilleBonbons *grille, int x1, int y1, int x2, int y2);

void SuppressionH(){}


void SuppressionV(){}

void Verification(){}





/***************************************************************************************************************************
                                                  MAIN

 ****************************************************************************************************************************/
