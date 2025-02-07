#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAILLE 20  // Taille de la grille
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR] = {
     {"\nVoulez-vous continuer?","O","N"},
    {"\nMode affichage simple ou complet?","S","C"},
    {"Veuillez entrer la colonne (a, b, c, d, e, f, g, h) : ", "abcdefgh"},
    {"Veuillez entrer la ligne (1,2,3,4,5,6,7,8) : ", "12345678"} };

typedef struct {
    int x;
    int y;
} Coordonnees;

typedef struct {
    int coupAJouer
} Niveau;

typedef struct {
    char pion;
    bool gelatine;
} Case;

typedef struct {
    int lignes;
    int colonnes;
    Case tableau[TAILLE][TAILLE];
} GrilleBonbons;

void InitialiserCoordonnees(Coordonnees *c, int x, int y) {
    c->x = x;
    c->y = y;
}

void initialiser_grille(GrilleBonbons *grille) {
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j].pion = couleurs[(i + j) % 5];  // Remplissage avec des couleurs
            grille->tableau[i][j].gelatine = false;  // Par défaut, pas de gelatine
        }
    }
}

Coordonnees CoordonneeGelatine(Coordonnees *c) {
    c->x = rand() % TAILLE;
    c->y = rand() % TAILLE;

    return *c;
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
        Coordonnees c;
        c = CoordonneeGelatine(&c);
        if (grille->tableau[c.x][c.y].pion == '.') {
            grille->tableau[c.x][c.y].gelatine = true;  // Placement de la gelatine
            grille->tableau[c.x][c.y].pion = 'G';  // Afficher 'G' pour les cases avec gelatine
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

int main() {
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;

    // Initialiser et afficher la grille
    initialiser_grille(&maGrille);
    printf("\nGrille generee :\n");
    afficher_grille(&maGrille);

    int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    printf("Case selectionnee : %c\n", maGrille.tableau[ligne][colonne].pion);

    GrilleBonbons maGrilleGelatine;
    maGrilleGelatine.lignes = TAILLE;
    maGrilleGelatine.colonnes = TAILLE;

    initialiserGrilleGelatine(&maGrilleGelatine);
    afficher_grille(&maGrilleGelatine);

    return 0;
}
