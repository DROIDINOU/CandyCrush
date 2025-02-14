#include <stdio.h>
#include "constante.h"
#include <stdlib.h>
#include <stdbool.h>
#include "matrice.h"
#include "affichage.h"

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
// verifier si 0 pas possible
int nombreGelatine = rand() % 20 + 1;  // Nombre de gelatines aléatoire entre 1 et 5
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

void VerificationInitit(Queue *q, GrilleBonbons *grille, bool *isVerificationInit) {
    for (int i = 0; i < grille->lignes - 2; i++) {
        for (int j = 0; j < grille->colonnes - 2; j++) {
            // Vérification horizontale et verticale
            Actions action = {"CALCUL", {i, j}, {i + 2, j + 2}};
            
            // Vérifier que la queue n'est pas pleine avant d'enfiler
            while (q->taille >= LONGUEUR) {
                Actions dequeuedAction = defiler(q);
                // Vous pouvez ajouter ici le traitement des actions désenfilées si nécessaire
            }
            enfiler(q, action);
        }
    }
    *isVerificationInit = true;  // Indiquer que l'initialisation a eu lieu
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

void Calcul(Queue *q, GrilleBonbons *grille, int x1, int y1, int x2, int y2,bool *isVerificationInit) {
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

    if (compteur >= 3) {
        xFin = xDebut + 2;  // Limite la suppression à 3 pions alignés
        Actions action = {"SUPPRESSIONV", {xDebut, y2}, {xFin, y2}};
        if (q->taille < LONGUEUR) {
            enfiler(q, action);
        } else {
        }
        suiteDetectee = true;
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
        yFin = yDebut + 2;  // Limite la suppression à 3 pions alignés
        Actions action = {"SUPPRESSIONH", {x2, yDebut}, {x2, yFin}};
        if (q->taille < LONGUEUR) {
            enfiler(q, action);
        } else {
        }

        suiteDetectee = true;
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
        xFin = xDebut + 2;  // Limite la suppression à 3 pions alignés
        Actions action = {"SUPPRESSIONV", {xDebut, y1}, {xFin, y1}};
        if (q->taille < LONGUEUR) {
            enfiler(q, action);
        } else {
        }

        suiteDetectee = true;
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
        yFin = yDebut + 2;  // Limite la suppression à 3 pions alignés
        Actions action = {"SUPPRESSIONH", {x1, yDebut}, {x1, yFin}};       
        if (q->taille < LONGUEUR) {
            enfiler(q, action);
        } else {
        }

        suiteDetectee = true;
    }

    if (!suiteDetectee && est_vide(q)){
        Actions action = {"VERIFICATION", {0, 0}, {0, 0}};
        if (q->taille < LONGUEUR) {
            enfiler(q, action);
        } else {
        }
    }
}
Actions Verification(GrilleBonbons *grille, Queue *q) {
    bool gelatinePresente = false;  // On suppose qu'il n'y a pas de gélatine au début

    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            if (grille->tableau[i][j].gelatine) {
                printf("Bonbon gélatine toujours présent\n");
                Actions action = {"LECTURE", {0, 0}, {0, 0}};
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
        Actions action = {"FINNIVEAU", {0, 0}, {0, 0}};
        enfiler(q, action);
        return action;
    }

    Actions action = {"FIN", {0, 0}, {0, 0}};
    return action;
}



// va falloir faire un bool pour que init supprime pas gelatines
void SuppressionV(GrilleBonbons *grille, int x1, int y1, int x2, int y2,Queue *q) {
    printf("Entrée dans SuppressionV avec x1=%d, y1=%d, x2=%d, y2=%d\n", x1, y1, x2, y2);
    fflush(stdout);

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
    Actions action = {"CALCUL", {x1,y1}, {x2, y2}};
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
                Actions action = {"CALCUL", {x1,y1}, {x2, y2}};
                if (q->taille < LONGUEUR) {
                    enfiler(q, action);
                } else {printf("memeproblemeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");}
            } 





/***************************************************************************************************************************
                                                  MAIN

 ****************************************************************************************************************************/

