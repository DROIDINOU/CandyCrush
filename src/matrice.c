#include "queue.h"
#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#define NOMBREMESSAGES 4
#define MAXLONGUEUR 100

// UNE ACTION A LA FOIS
// A FAIRE INITIALISER DOIT VERIFIER TOUT DANS CALCUL 
// FAIRE FONCTION VERIFICATION
// BONUS FAIRE FONCTION CASCADE
// supprimer struct en double
// FAUDRA VERIFIER QUE COORDONNEES PAS DEJA DANS QUEUE


const char MESSAGEETREPONSESATTENDUES[NOMBREMESSAGES][3][MAXLONGUEUR] = {
     {"\nVoulez-vous continuer?","O","N"},
    {"\nMode affichage simple ou complet?","S","C"},
    {"Veuillez entrer la colonne (a, b, c, d, e, f, g, h) : ", "abcdefgh"},
    {"Veuillez entrer la ligne (1,2,3,4,5,6,7,8) : ", "12345678"} 
    };


void ConfigureConsoleForC() {
    // Configurer la locale pour UTF-8
    setlocale(LC_ALL, ".UTF-8");
    // Configurer l'encodage de sortie de la console sur UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // Configurer l'encodage d'entrée de la console sur UTF-8
    SetConsoleCP(CP_UTF8);

    // Utiliser une page de code UTF-8
    system("chcp 65001");}
/***************************************************************************************************************************
                                                  INIT GRILLE

 ****************************************************************************************************************************/

void initialiser_grille(GrilleBonbons *grille) {
    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j].pion = couleurs[rand() % 5];  // Remplissage avec des couleurs
            grille->tableau[i][j].gelatine = false;  // Par défaut, pas de gelatine
        }
    }
}

void VerificationInitit(Queue *q, GrilleBonbons *grille){
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            // VERIFICATION HORIZONTALE
            // VERIFICATION VERTICLAE
            Actions action = {"ACTION", {i,j}, {i+2, j+2}};
            Calcul(q,grille, i, j, i+2, j+2);
            //printf("bonbon gelatine toujours present \n");
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

int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index) {
    int reponse;

    do {
        printf("%s", message[index][0]);  // Affiche le message
        int result = scanf(" %d", &reponse);  // Lit un seul caractère
        while (getchar() != '\n');

        // Vérifie que la réponse est valide
        if (result == -1) {
            printf("Réponse invalide. Essayez encore.\n");
        }
    } while (reponse < 0 || reponse > 20);  // Si la réponse n'est pas valide, on redemande

    return reponse -1;
}



void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion, Queue *q ) {
    printf("Coordonnees du premier pion : (%d, %d)\n", coordonneeXPremierPion, coordonneeYPremierPion);
    printf("Coordonnees du deuxieme pion : (%d, %d)\n", coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
    // ajouter action ici dans la queue;
    Actions action = {"DEPLACEMENT", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    enfiler(q,action);
    imprimer_queue(q);
    //printf("Action : %s\n", action.actionName);
}

void Deplacement (Queue *q,GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion){
    char temp = grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion;
    grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion = grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion;
    grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion = temp;
        Actions action = {"CALCUL", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
        enfiler(q,action);
        imprimer_queue(q);
}

/*Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes


/*_____________________________________________________________________________________________________________________________*/

void Calcul(Queue *q, GrilleBonbons *grille, int x1, int y1, int x2, int y2) {
    bool suiteDetectee = false;
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

    if (compteur == 3) {
        Actions action = {"SUPPRESSIONV", {xDebut, y2}, {xFin, y2}};
        enfiler(q,action);
        imprimer_queue(q);
        suiteDetectee = true;
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
        SuppressionV(grille, xDebut, y2, xFin, y2);
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

    if (compteur == 3) {
        Actions action = {"SUPPRESSIONH", {x2, yDebut}, {x2, yFin}};
        suiteDetectee = true;
        enfiler(q,action);
        imprimer_queue(q);
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
        SuppressionH(grille, x2, yDebut, x2, yFin);

    }

    // Vérification pour le pion en (x1, y1)
    compteur = 1;
    pion = grille->tableau[x1][y1].pion;

    // Vérification verticale (colonne)
    xDebut = x1;
    xFin = x1;

    i = x1 + 1;
    while ( i < TAILLE && grille->tableau[i][y1].pion == pion) {
        compteur++;
        xFin = i;
        i++;
    }

    i = x1 - 1;
    while ( i >= 0 && grille->tableau[i][y1].pion == pion) {
        compteur++;
        xDebut = i;
        i--;
    }

    if (compteur == 3) {
        Actions action = {"SUPPRESSIONV", {xDebut, y1}, {xFin, y1}};
        enfiler(q,action);
        imprimer_queue(q);
        suiteDetectee = true;
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
        SuppressionV(grille, xDebut, y1, xFin, y1);
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

    if (compteur == 3) {
        Actions action = {"SUPPRESSIONH", {x1, yDebut}, {x1, yFin}};
        enfiler(q,action);
        imprimer_queue(q);
        suiteDetectee = true;
        printf("Action: %s\n", action.actionName);
        printf("Pion 1: (%d, %d)\n", action.pion1.x, action.pion1.y);
        printf("Pion 2: (%d, %d)\n", action.pion2.x, action.pion2.y);
        SuppressionH(grille, x1, yDebut, x1, yFin);

    }

    if (!suiteDetectee){
        Actions action = {"VERIFICATION", {x1, yDebut}, {x1, yFin}};
        enfiler(q,action);
        imprimer_queue(q);
    }
}


bool Verification(GrilleBonbons *grille){
    
     for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            if(grille->tableau[i][j].pion == 'G'){
           // printf("bonbon gelatine toujours present \n");
            return false;}
        }
    }

    return true;
}



void SuppressionV(GrilleBonbons *grille, int x1, int y1, int x2, int y2){

     if (x1 < x2){

        for (int i = x1; i <= x2; i++){
        grille->tableau[i][y1].pion = 'Y';
    }
     }

     else if (x1 > x2) {
          for (int i = x1; i >= x2; i--){
              grille->tableau[i][y1].pion = 'Z';
                       }
                       }
    
}

void SuppressionH(GrilleBonbons *grille, int x1, int y1, int x2, int y2){

     if (y1 < y2){

        for (int i = y1; i <= y2; i++){
        grille->tableau[x1][i].pion = 'S';
    }
     }

     else if (y1 > y2) {
          for (int i = y1; i >= y2; i--){
              grille->tableau[x1][i].pion = 'N';
                       }
                       }
    
}






/***************************************************************************************************************************
                                                  MAIN

 ****************************************************************************************************************************/



int main() {
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires une fois
    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE;
    maGrille.colonnes = TAILLE;
    Queue q;

    // Initialiser et afficher la grille
    initialiser_grille(&maGrille);
    printf("\nGrille generee :\n");
    afficher_grille(&maGrille);
    VerificationInitit(&q, &maGrille);
    GrilleBonbons maGrilleGelatine;
    maGrilleGelatine.lignes = TAILLE;
    maGrilleGelatine.colonnes = TAILLE;

    initialiserGrilleGelatine(&maGrilleGelatine);
    afficher_grille(&maGrilleGelatine);
    initialiser_queue(&q);
    int ligne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int colonne = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int ligne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    int colonne1 = ObtenirReponseAuMessage(MESSAGEETREPONSESATTENDUES, 3);
    LirePionsAChanger(&maGrille, ligne, colonne, ligne1, colonne1,&q);
    Deplacement(&q,&maGrille, ligne, colonne, ligne1, colonne1);
    afficher_grille(&maGrille);
    Calcul(&q,&maGrille,ligne, colonne, ligne1, colonne1);
    Verification(&maGrille);
    afficher_grille(&maGrille);


    return 0;
}
