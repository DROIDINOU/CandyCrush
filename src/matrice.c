#include <stdio.h>
#include "constante.h"
#include <stdlib.h>
#include <stdbool.h>
#include "matrice.h"
#include "affichage.h"
#include <string.h>

// UNE ACTION A LA FOIS
// A FAIRE INITIALISER DOIT VERIFIER TOUT DANS CALCUL 
// FAIRE FONCTION VERIFICATION
// BONUS FAIRE FONCTION CASCADE
// supprimer struct en double
// FAUDRA VERIFIER QUE COORDONNEES PAS DEJA DANS QUEUE






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

int nombreGelatine = rand() % 5 + 1;  // Nombre de gelatines aléatoire entre 1 et 5
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j].gelatine = false;  // Initialisation sans gelatine
        }
    }

    while (nombreGelatine > 0) {
        int x = rand() % TAILLE;
        int y = rand() % TAILLE;
        if (!grille->tableau[x][y].gelatine) {

            grille->tableau[x][y].gelatine = true;  // Placement de la gelatine
            nombreGelatine--;
        }
    }

}

bool VerificationInitit(Queue *q, GrilleBonbons *grille, int *currentX, int *currentY, int *currentX2, int *currentY2) {
    // Ajouter une action à la queue pour l'indice courant
    
    // Vérifier que la queue n'est pas pleine avant d'enfiler
    printf("Entrée VerificationInitit: x1=%d, y1=%d, x2=%d, y2=%d\n", *currentX, *currentY, *currentX2, *currentY2);
    
    // Incrémenter les indices de manière coordonnée
    *currentY += 1;  // Incrémenter Y pour la première position (currentY)
    *currentY2 += 1;  // Incrémenter Y pour la deuxième position (currentY2)
    
    // Vérifier si on a atteint la fin de la colonne (max colonne)
    if (*currentY >= grille->colonnes) {  
        *currentY = 0;  // Réinitialiser Y et avancer sur la ligne suivante
        *currentX += 1;  // Avancer sur la ligne suivante (incrémenter X)
    }
    
    // Vérifier si on a atteint la fin de la colonne pour le deuxième indice
    if (*currentY2 >= grille->colonnes) {  
        *currentY2 = 0;  // Réinitialiser Y2 et avancer sur la ligne suivante
        *currentX2 += 1;  // Avancer sur la ligne suivante (incrémenter X2)
    }

    // Vérifier si on a terminé l'initialisation de la grille
    if (*currentX2 >= grille->lignes) {  // Vérifier si on a fini d'initialiser toute la grille
        return true;  // L'initialisation est terminée
    }
    
    printf("Après Incrémentation: x1=%d, y1=%d, x2=%d, y2=%d\n", *currentX, *currentY, *currentX2, *currentY2);
    return false;  // Si la grille n'est pas encore complètement initialisée
}


int ObtenirReponseAuMessage(const char message[][3][MAXLONGUEUR], int index) {
    int reponse;
    do {
        printf("%s", message[index][0]);  // Affiche le message
        int result = scanf(" %d", &reponse);  // Lit un seul caractère
        while (getchar() != '\n');

        // Vérifie que la réponse est valide
        if (result == -1) {
            printf("Reponse invalide. Essayez encore.\n");
        }
    } while (reponse < 1 || reponse > 21);  // Si la réponse n'est pas valide, on redemande

    return reponse -1;
}



void LirePionsAChanger(GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion, Queue *q ) {
    // ajouter action ici dans la queue;
    Actions action = {"DEPLACEMENT", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
    //printf("Action : %s\n", action.actionName);
    enfiler(q, action);

}

void Deplacement (Queue *q,GrilleBonbons *grille, int coordonneeXPremierPion, 
                       int coordonneeYPremierPion, int coordonneeXDeuxiemePion, 
                       int coordonneeYDeuxiemePion){
    printf ("Deplacement de %d %d vers %d %d\n", coordonneeXPremierPion, coordonneeYPremierPion, coordonneeXDeuxiemePion, coordonneeYDeuxiemePion);
    char temp = grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion;
    grille->tableau[coordonneeXPremierPion][coordonneeYPremierPion].pion = grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion;
    grille->tableau[coordonneeXDeuxiemePion][coordonneeYDeuxiemePion].pion = temp;
        Actions action = {"CALCUL", {coordonneeXPremierPion, coordonneeYPremierPion}, {coordonneeXDeuxiemePion, coordonneeYDeuxiemePion}};
        enfiler(q, action);
}

/*Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de calculer si trois pions se
suivent en Vertical ou en Horizontal. Si trois pions se suivent en vertical, la fonction devra ajouter une
action « Suppression V » sur la Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une
action « Suppression H » sur la Queue. Si la Queue est pleine, il faut afficher un message d’erreur et
arrêter le programme*/

// faut retourner pion sup et inf !!
// prevoir une verification des victoires deja presentes


/*_____________________________________________________________________________________________________________________________*/

void Calcul(Queue *q, GrilleBonbons *grille, int *x1, int *y1, int *x2, int *y2) {
    int compteur;
    char pion;
    int i, j;
    bool isVerificationInit = true;
       imprimer_queue(q);
    if (!est_vide(q)) {
        Actions currentAction = q->elements[q->debut];  // Accéder à l'action en cours
        printf("Action en cours : %s\n", currentAction.actionName);

        if (strcmp(currentAction.actionName, "INITIALISATION") == 0) {
            // Si l'action est INITIALISATION, lancer la vérification
            printf("Lancement de la vérification initiale...\n");
            bool isVerificationInit = VerificationInitit(q, grille, x1, y1, x2, y2);
            printf("Après VerificationInitit: x1=%d, x2=%d\n", *x1, *x2);
            defiler(q);
        }
    }


    // Vérification pour le pion en (*x2, *y2)
    pion = grille->tableau[*x2][*y2].pion;

    // Vérification verticale (colonne)
    compteur = 1;
    int xDebut = *x2, xFin = *x2;
    i = *x2 + 1;
    while (i < TAILLE && grille->tableau[i][*y2].pion == pion) { compteur++; xFin = i; i++; }
    i = *x2 - 1;
    while (i >= 0 && grille->tableau[i][*y2].pion == pion) { compteur++; xDebut = i; i--; }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONV", {xDebut, *y2}, {xFin, *y2},false};
        printf("suppression !!!!!!!!!!!!!!!!!");
        enfiler(q, action);
        return;
    }

    // Vérification horizontale (ligne)
    compteur = 1;
    int yDebut = *y2, yFin = *y2;
    j = *y2 + 1;
    while (j < TAILLE && grille->tableau[*x2][j].pion == pion) { compteur++; yFin = j; j++; }
    j = *y2 - 1;
    while (j >= 0 && grille->tableau[*x2][j].pion == pion) { compteur++; yDebut = j; j--; }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONH", {*x2, yDebut}, {*x2, yFin},false};
        printf("suppression !!!!!!!!!!!!!!!!!");
        enfiler(q, action);
        return;
    }

    // Vérification pour le pion en (*x1, *y1)
    pion = grille->tableau[*x1][*y1].pion;

    // Vérification verticale (colonne)
    compteur = 1;
    xDebut = *x1, xFin = *x1;
    i = *x1 + 1;
    while (i < TAILLE && grille->tableau[i][*y1].pion == pion) { compteur++; xFin = i; i++; }
    i = *x1 - 1;
    while (i >= 0 && grille->tableau[i][*y1].pion == pion) { compteur++; xDebut = i; i--; }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONV", {xDebut, *y1}, {xFin, *y1},false};
        printf("suppression !!!!!!!!!!!!!!!!!");
        enfiler(q, action);
        return;
    }

    // Vérification horizontale (ligne)
    compteur = 1;
    yDebut = *y1, yFin = *y1;
    j = *y1 + 1;
    while (j < TAILLE && grille->tableau[*x1][j].pion == pion) { compteur++; yFin = j; j++; }
    j = *y1 - 1;
    while (j >= 0 && grille->tableau[*x1][j].pion == pion) { compteur++; yDebut = j; j--; }

    if (compteur >= 3) {
        Actions action = {"SUPPRESSIONH", {*x1, yDebut}, {*x1, yFin},false};
        printf("suppression !!!!!!!!!!!!!!!!!");

        enfiler(q, action);
        return;
    }

    // Si aucune suppression n'a été effectuée, vérifier si l'initialisation est terminée
    if (isVerificationInit) {
        Actions action = {"VERIFICATION", {0, 0}, {0, 0},false};
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        enfiler(q, action);
        return;
    }

    if (!isVerificationInit) {
        printf("?????????????????????????????????????????????");
        printf("voila x1=%d et y1=%d\n", *x1, *y1);
        Actions action = {"CALCUL", {*x1, *x2}, {*y1, *y2}, false};
        enfiler(q, action);
        return;
    }
    // Si l'initialisation est terminée, ajouter l'action "VERIFICATION" dans la queue
    
}


// transformer en bool et laisser calcul gerer l action supprmier parametre queue
Actions Verification(GrilleBonbons *grille, Queue *q) {
    bool gelatinePresente = false;  // On suppose qu'il n'y a pas de gélatine au début

    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            if (grille->tableau[i][j].gelatine) {
                printf("Bonbon gélatine toujours présent\n");
                Actions action = {"LECTURE", {0, 0}, {0, 0},false};
                printf("oooooooooooooooooooo\n");
                enfiler(q, action);
                gelatinePresente = true;  // On détecte de la gélatine
                return action;  // On quitte immédiatement la fonction
            }
        }
    }

    // Si on arrive ici, c'est qu'il n'y avait pas de gélatine
    if (!gelatinePresente) {
        printf("FIN NIVEAU\n");
        Actions action = {"FINNIVEAU", {0, 0}, {0, 0},false};
        enfiler(q, action);
        return action;
    }

    Actions action = {"FIN", {0, 0}, {0, 0},false};
    return action;
}



// va falloir faire un bool pour que init supprime pas gelatines
void SuppressionV(GrilleBonbons *grille, int x1, int y1, int x2, int y2,Queue *q) {

    char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    if (x1 < x2) {
        for (int i = x1; i <= x2; i++) {
            int index = rand() % 5;
            printf("yessssssssssssssssssssssss");
            char couleur = couleurs[index];
            grille->tableau[i][y1].pion = couleur;
            if (grille->tableau[i][y1].gelatine) {
                  printf("t as eu une gelatine!!");
                  grille->tableau[i][y1].gelatine = false;

            }

        }
    } else if (x1 > x2) {
        for (int i = x1; i >= x2; i--) {
            int index = rand() % 5;

            printf("yessssssssssssssssssssssss");
            char couleur = couleurs[index];
            grille->tableau[i][y1].pion = couleur;
            if (grille->tableau[i][y1].gelatine) {
                printf("t as eu une gelatine!!");
            }
        
        }
    }
    Actions action = {"CALCUL", {x1,y1}, {x2, y2},false};
        if (q->taille < LONGUEUR) {
            enfiler(q, action);
        } else {printf("memeproblemeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");}
        
}

 void SuppressionH(GrilleBonbons *grille, int x1, int y1, int x2, int y2,Queue *q){ 
        char couleurs[] = {'J', 'V', 'B', 'R', 'M'};
    if (y1 < y2){ 
        for (int i = y1; i <= y2; i++){
            int index = rand() % 5;
            char couleur = couleurs[index];
             grille->tableau[x1][i].pion = couleur; 
             if(grille->tableau[x1][i].gelatine){ 
                                printf("t as eu une gelatine!!");
} } } 
    else if (y1 > y2) { 
        for (int i = y1; i >= y2; i--){ 
            int index = rand() % 5;
            printf("yessssssssssssssssssssssss");

            char couleur = couleurs[index];
            grille->tableau[x1][i].pion = couleur; 
            if(grille->tableau[x1][i].gelatine){ 
                    printf("t as eu une gelatine!!");
} 
                } 
                } 
                Actions action = {"CALCUL", {x1,y1}, {x2, y2},false};
                if (q->taille < LONGUEUR) {
                    enfiler(q, action);
                } else {printf("memeproblemeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");}
            } 





/***************************************************************************************************************************
                                                  MAIN

 ****************************************************************************************************************************/

