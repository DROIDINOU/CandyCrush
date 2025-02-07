#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAILLE_PETITE 4
#define TAILLE_MOYENNE 6
#define TAILLE_GRANDE 8
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR] = {
     {"\nVoulez-vous continuer?","O","N"},
    {"\nMode affichage simple ou complet?","S","C"},
    {"Veuillez entrer la colonne (a, b, c, d, e, f, g, h) : ", "abcdefgh"},
    {"Veuillez entrer la ligne (1,2,3,4,5,6,7,8) : ", "12345678"} };



typedef struct {
    char question[MAXLONGUEUR];
    char reponses[MAXLONGUEUR][MAXLONGUEUR];
} Message;

typedef struct {
    int w;
    int x;
    int y;
    int z;

} Directions;




typedef struct {
    char couleur;  // Rouge, Vert, Bleu, Jaune
    char type;     // Type de bonbon, par exemple 'N' pour normal, 'S' pour spécial
    int taille;    // Taille du bonbon
} Bonbon;

typedef struct {
    int taille;
} TailleGrille;

// Définition de la structure GrilleBonbons
typedef struct {
    int lignes;
    int colonnes;
    char tableau[TAILLE_GRANDE][TAILLE_GRANDE];  // Remplace "grille" par "tableau"
} GrilleBonbons;

void initialiser_grille(GrilleBonbons *grille) {
    char couleurs[] = {'R', 'V', 'B', 'J'};  // Rouge, Vert, Bleu, Jaune
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j] = couleurs[(i + j) % 4];  // Remplissage avec des couleurs
        }
    }
}        
    



void afficher_grille(GrilleBonbons *grille) {
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            printf(" %c ", grille->tableau[i][j]);  // Affichage de la grille
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
// Fonction pour obtenir une réponse utilisateur au messages predefinis dans tableau MESSAGE
int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index) {
    char reponse;
    int resultat;

    do {
        printf("%s", message[index][0]);  // Affiche le message
        resultat = scanf(" %1c", &reponse);  // Lit un seul caractère
        while (getchar() != '\n');

        // Vérifie que la réponse est valide
        if (caractereDansChaine(message[index][1], reponse) == -1) {
            printf("Réponse invalide. Essayez encore.\n");
        }
    } while (caractereDansChaine(message[index][1], reponse) == -1);  // Si la réponse n'est pas valide, on redemande
    int indexReponse = caractereDansChaine(message[index][1], reponse);
    return indexReponse;
}

void ChangeCouleur (GrilleBonbons *grille, int ligne, int colonne){
      char couleurs[] = {'R', 'V', 'B', 'J'};  // Rouge, Vert, Bleu, Jaune
      char couleurActuelle = grille -> tableau[ligne][colonne];
      while(couleurActuelle == grille -> tableau[ligne][colonne] ){
             grille -> tableau[ligne][colonne] = couleurs[rand()%4];
      }
      printf("nouvelle couleur: %c\n", grille -> tableau[ligne][colonne] );
}

bool CompteurClickDisponibles (int *pnombreClickDisponible){
      if (*pnombreClickDisponible < 3){
         *pnombreClickDisponible -= 1;
          return true;
      }
      else {
          return false;
      } 

}
// FAIRE FONCTION DETECTION PREMIER NIVEAU ET FONCTION CASCADE
bool DetecterAlignementHorizontal3(GrilleBonbons *grille){
    Directions horizontale;
    horizontale.w = 1;
    horizontale.x = 0;
    horizontale.y = 0;
    horizontale.z = -1;
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            if (grille->tableau[i][j] == grille->tableau[i][j+1] && grille->tableau[i][j] == grille->tableau[i][j+2]){
                printf("alignement horizontal detecte\n");
                //FonctionCascade
                return true;
            }
        }
    }
    printf("pas alignement horizontal detecte\n");
    return false;
}

void ProcederNiveau1(GrilleBonbons *grille){
       DetecterAlignementHorizontal3(grille);
       int nombreClickDisponible = 3;
       while(!DetecterAlignementHorizontal3(grille) || CompteurClickDisponibles(&nombreClickDisponible))
       {int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
        int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
           printf ("case selectionnee\n : %c",grille->tableau[ligne][colonne]);
    ChangeCouleur(grille,ligne,colonne);
    DetecterAlignementHorizontal3(grille);}

}

int main() {
    GrilleBonbons maGrille;  // Nom de la variable : maGrille
    TailleGrille taille;
    taille.taille = TAILLE_GRANDE;
    // Initialisation de la grille

    maGrille.lignes = taille.taille;
    maGrille.colonnes = taille.taille;

    // Initialiser et afficher la grille
    initialiser_grille(&maGrille);
    printf("\nGrille generee :\n");
    afficher_grille(&maGrille);
    int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    printf ("case selectionnee\n : %c",maGrille.tableau[ligne][colonne]);
    ChangeCouleur(&maGrille,ligne,colonne);
   ProcederNiveau1(&maGrille);
    return 0;
}
