#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

void initialiser_grille(GrilleBonbons *grille) {
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j].pion = couleurs[(i + j) % 5];  // Remplissage avec des couleurs
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
    printf("Coordonnées du premier pion : (%d, %d)\n", coordonneeXPremierPion, coordonneeYPremierPion);
    printf("Coordonnées du deuxième pion : (%d, %d)\n", coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
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



















/*_____________________________________________________________________________________________________________________________*/

typedef struct {
    int coupAJouer;
} Niveau;






int main() {
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
    LirePionsAChanger(&maGrille, 1, 2, 3, 4, &q);
    Deplacement(&maGrille, 1, 2, 3, 4);
    afficher_grille(&maGrille);

    return 0;
}
