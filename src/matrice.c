#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include <time.h>

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

void initialiser_grille(GrilleBonbons *grille) {
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j].pion = couleurs[rand() % 5];  // Remplissage avec des couleurs
            grille->tableau[i][j].gelatine = false;  // Par défaut, pas de gelatine
        }
    }
}

   

void initialiserGrilleGelatine(GrilleBonbons *grille) {
    int nombreGelatine = rand() % 5 + 1;  // Nombre de gelatines aléatoire entre 1 et 5
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j].pion = '.';  // Initialisation de la grille
            grille->tableau[i][j].gelatine = false;  // Initialisation sans gelatine
        }
    }

    while (nombreGelatine > 0) {
        int x = rand() % TAILLE;
        int y = rand() % TAILLE;
        if (!grille->tableau[x][y].gelatine) {
            grille->tableau[x][y].pion = 'G';  // Initialisation de la grille

            grille->tableau[x][y].gelatine = true;  // Placement de la gelatine
            nombreGelatine--;
        }
    }
}

void afficher_grille(GrilleBonbons *grille) {
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            printf(" %c ", grille->tableau[i][j].pion);  // Affichage de la grille
        }
        printf("\n");
    }
}


int caractereDansChaine(const char chaine[], char caractere) {
    for (int i = 0; chaine[i] != '\0'; i++) {
        if (chaine[i] == caractere) {
            return i;  // Caractère trouvé
        }
    }
    return -1;  // Caractère non trouvé
}



int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index) {
    char reponse;

    do {
        printf("%s", message[index][0]);  // Affiche le message
        scanf(" %1c", &reponse);  // Lit un seul caractère
        while (getchar() != '\n');

        // Vérifie que la réponse est valide
        if (caractereDansChaine(message[index][1], reponse) == -1) {
            printf("Réponse invalide. Essayez encore.\n");
        }
    } while (caractereDansChaine(message[index][1], reponse) == -1);  // Si la réponse n'est pas valide, on redemande

    int indexReponse = caractereDansChaine(message[index][1], reponse);
    return indexReponse;
}



void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion, Queue *q ) {
    printf("Coordonnees du premier pion : (%d, %d)\n", coordonneeXPremierPion, coordonneeYPremierPion);
    printf("Coordonnees du deuxième pion : (%d, %d)\n", coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
    // ajouter action ici dans la queue;
    Actions action = {"DEPLACEMENT", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    printf("Action : %s\n", action.actionName);
}

void Deplacement (GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion){
    char temp = grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion;
    grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion = grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion;
    grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion = temp;
        Actions action = {"CALCUL", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};

}

/*Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes


/*_____________________________________________________________________________________________________________________________*/

void Calcul(GrilleBonbons *grille, int x1, int y1, int x2, int y2) {
    int compteur = 1;
    char pion = grille->tableau[x2][y2].pion;
    int i, j;

    // Vérification pour le pion en (x2, y2)
    // Vérification verticale (colonne)
    int xDebut = x2;
    int xFin = x2;

    i = x2 + 1;
    while (i < TAILLE && grille->tableau[i][y2].pion == pion) {
        compteur++;
        xFin = i;
        i++;
    }

    i = x2 - 1;
    while (i >= 0 && grille->tableau[i][y2].pion == pion) {
        compteur++;
        xDebut = i;
        i--;
    }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONV", {xDebut, y2}, {xFin, y2}};
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
    }

    // Vérification horizontale (ligne)
    compteur = 1;
    pion = grille->tableau[x2][y2].pion;
    int yDebut = y2;
    int yFin = y2;

    j = y2 + 1;
    while (j < TAILLE && grille->tableau[x2][j].pion == pion) {
        compteur++;
        yFin = j;
        j++;
    }

    j = y2 - 1;
    while (j >= 0 && grille->tableau[x2][j].pion == pion) {
        compteur++;
        yDebut = j;
        j--;
    }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONH", {x2, yDebut}, {x2, yFin}};
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
    }

    // Vérification pour le pion en (x1, y1)
    compteur = 1;
    pion = grille->tableau[x1][y1].pion;

    // Vérification verticale (colonne)
    xDebut = x1;
    xFin = x1;

    i = x1 + 1;
    while (i < TAILLE && grille->tableau[i][y1].pion == pion) {
        compteur++;
        xFin = i;
        i++;
    }

    i = x1 - 1;
    while (i >= 0 && grille->tableau[i][y1].pion == pion) {
        compteur++;
        xDebut = i;
        i--;
    }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONV", {xDebut, y1}, {xFin, y1}};
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
    }

    // Vérification horizontale (ligne)
    compteur = 1;
    pion = grille->tableau[x1][y1].pion;
    yDebut = y1;
    yFin = y1;

    j = y1 + 1;
    while (j < TAILLE && grille->tableau[x1][j].pion == pion) {
        compteur++;
        yFin = j;
        j++;
    }

    j = y1 - 1;
    while (j >= 0 && grille->tableau[x1][j].pion == pion) {
        compteur++;
        yDebut = j;
        j--;
    }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONH", {x1, yDebut}, {x1, yFin}};
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
    }
}

void SuppressionH(){}


void SuppressionV(){}



/***************************************************************************************************************************
                                                  MAIN

 ****************************************************************************************************************************/




typedef struct {
    int coupAJouer;
} Niveau;






int main() {
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires une fois
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    // Initialiser et afficher la grille
    initialiser_grille(&maGrille);
    printf("\nGrille generee :\n");
    afficher_grille(&maGrille);


    GrilleBonbons maGrilleGelatine;
    maGrilleGelatine.lignes = TAILLE;
    maGrilleGelatine.colonnes = TAILLE;

    initialiserGrilleGelatine(&maGrilleGelatine);
    afficher_grille(&maGrilleGelatine);
    Queue q;
    initialiser_queue(&q);
    int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1, &q);
    Deplacement(&maGrille, ligne, colonne, ligne1, colonne1);
    afficher_grille(&maGrille);
    Calcul(&maGrille,ligne, colonne, ligne1, colonne1);

    return 0;
}
