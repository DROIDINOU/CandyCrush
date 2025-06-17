#include <stdio.h>     // Fonctions d’entrée/sortie (printf, scanf, etc.)
#include <stdlib.h>    // Fonctions utilitaires (rand, etc.)
#include <stdbool.h>   // Support du type bool
#include <time.h>      // Pour time(), srand(), etc.
#include <locale.h>    // Pour setlocale (affichage des caractères spéciaux)
#include <windows.h>   // Fonctions Windows (Sleep, PlaySound, etc.)
#include "constante.h" // Constantes globales
#include "erreur.h"    // Gestion des erreurs
#include "matrice.h"   // Structures et logique de la grille
#include "affichage.h" // Affichage de la grille et messages

// Efface tout l'écran (CMD sous Windows ou "clear" sur Unix)
static void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 ____________________________________________________________________________________________________________________________

                        **** FONCTIONS DE LECTURE DES ENTREES UTILISATEUR

    -> Fonctions : ObtenirReponseAuMessage - LireQuatreCoordonnees - LirePionsAChanger
    -> Parametres ObtenirReponseAuMessage  : int index
    -> Parametres LireQuatreCoordonnees : int *x1 - int *y1 - int *x2 - int *y2
    -> Parametres LirePionsAChanger : GrilleBonbons *grille - int *coordonneeXPremierPion -
                                     int *coordonneeYPremierPion - int *coordonneeXDeuxiemePion -
                                     int *coordonneeYDeuxiemePion - Queue *q
    -> ObtenirReponseAuMessage : Affiche le message et lit la reponse de l'utilisateur
    -> LireQuatreCoordonnees : Lit les coordonnees des pions a changer
    -> LirePionsAChanger : Lit les coordonnees des pions a changer et ajoute l'action DEPLACEMENT dans la queue
                           avec les coordonnees des bonbons
___________________________________________________________________________________________________________________________
*/

// Affiche un message et lit la réponse de l'utilisateur
// index est l'index du message dans le tableau MessagesReponses
int ObtenirReponseAuMessage(int index)
{
    int choixUtilisateur;
    int result;
    int caractereLu; // utilisé pour vider le tampon d'entrée après scanf;

    do
    {
        // Affiche le message et la plage
        printf("%s %d : ",
               MESSAGESECHANGEBONBONS[index].message,
               MESSAGESECHANGEBONBONS[index].nombreLigneOuColonne);

        // Lecture de l'entier
        result = scanf("%d", &choixUtilisateur);

        //  Vider le reste de la ligne (jusqu'au '\n' ou EOF)
        while ((caractereLu = getchar()) != '\n' && caractereLu != EOF)
            ;

        // si pas un chiffre erreur et retourne -1
        if (result != 1)
        {
            GererErreurNonFatale(ERREURENTREEINVALIDE);
            choixUtilisateur = -1;
        }
        // controle si la plage est dan limite grille
        else if (choixUtilisateur < 1 || choixUtilisateur > TAILLE)
        {
            GererErreurNonFatale(ERREURNOMBREENTREEINCORRECTE);
            choixUtilisateur = -1;
        }

    } while (choixUtilisateur == -1);

    // retourne le choix de l utilisateur utilisable pour le programmeur (-1)
    return choixUtilisateur - 1;
}

// Vérifie si les pions sont adjacents (horizontalement ou verticalement)
bool EstPionAdjacent(int x1, int y1, int x2, int y2)
{
    return (x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1);
}

// lit les coordonnees entrees par l utilisateur pour le swap de pions
bool LireQuatreCoordonnees(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ObtenirReponseAuMessage(1);

    *y1 = ObtenirReponseAuMessage(0);

    *x2 = ObtenirReponseAuMessage(1);

    *y2 = ObtenirReponseAuMessage(0);

    return EstPionAdjacent(*x1, *y1, *x2, *y2); // Vérifie si les pions sont adjacents
}

// lit les coordonnees des pions a changer et ajoute l'action DEPLACEMENT dans la queue
// avec les coordonnees des bonbons
void LirePionsAChanger(GrilleBonbons *grille,
                       int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion,
                       int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion,
                       Queue *q)
{
    bool choixValide = false;

    while (!choixValide)
    {
        if (LireQuatreCoordonnees(coordonneeXPremierPion,
                                  coordonneeYPremierPion,
                                  coordonneeXDeuxiemePion,
                                  coordonneeYDeuxiemePion))
        {
            Actions action = {
                DEPLACEMENT,
                {*coordonneeXPremierPion, *coordonneeYPremierPion},
                {*coordonneeXDeuxiemePion, *coordonneeYDeuxiemePion}};
            Enfiler(q, &action);
            choixValide = true;
        }
        else
        { // message d erreur utilisateur
            GererErreurNonFatale(ERREURDEPLACEMENT);
        }
    }
}

/*____________________________________________________________________________________________________________________________
                                       **** FONCTION D'AFFICHAGE

-> afficherMessagePleinEcran - afficherGrille
-> Params : grille - queue
-> Afficher la grille des bonbons
-> Afficher la grille des gelatines
______________________________________________________________________________________________________________________________
*/

void afficherMessagePleinEcran(const char *texte, int largeur, int hauteur)
{
    int len = strlen(texte); // longueur du texte a afficher
    int padding = (largeur - len) / 2;
    if (padding < 0)
        padding = 0;

    int ligneTexte = hauteur / 2;

    // Lignes vides avant le message
    for (int i = 0; i < ligneTexte; i++)
    {
        for (int j = 0; j < largeur; j++)
            printf(" ");
        printf("\n");
    }

    // Ligne contenant le message centré en orange
    for (int i = 0; i < padding; i++)
        printf(" ");
    printf("\033[38;5;208m%s\033[0m", texte);
    for (int i = 0; i < largeur - padding - len; i++)
        printf(" ");
    printf("\n");

    // Lignes vides après
    for (int i = ligneTexte + 1; i < hauteur; i++)
    {
        for (int j = 0; j < largeur; j++)
            printf(" ");
        printf("\n");
    }
}

void afficherGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu)
{
    if (etatJeu->introduction == 1)
    {
        const char *texte = "Bienvenue dans CRUSHLOSSON !\n"
                            "Deplacez les pions adjacents.\n"
                            "Essayez de former des lignes de 3 ou plus pour gagner !\n"
                            "But du jeu : supprimer les gelatines\n"
                            "Des djokers sont disponibles pour vous aider !\n"
                            "Pressez ENTER pour commencer !\n";

        afficherMessagePleinEcran(texte, 20, 20);
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            // on vide les caractères tapés avant le '\n'
        }
        // vide les caractères jusqu’à '\n'
        etatJeu->introduction = 0; // pour ne plus l’afficher ensuite
        return;
    }
    clearScreen(); // permet d'avoir une grille fixe tout au long du jeu
    PAUSE(50);     // donne au terminal le temps d'afficher

    // afficher message fin de partie (coups epuises ou fin de jeu)
    if (etatJeu->findepartie == 1)
    {
        const char *texte = etatJeu->coupsepuises
                                ? MESSAGEETATJEU[MESSAGECOUPSEPUISES]
                                : MESSAGEETATJEU[MESSAGEFINJEU];

        afficherMessagePleinEcran(texte, grille->colonnes * 4, grille->lignes);
        PAUSE(2000);
        return;
    }
    // afficher message niveau suivant
    if (etatJeu->niveausuivant == 1)
    {
        PAUSE(400);
        afficherMessagePleinEcran(MESSAGEETATJEU[MESSAGEFELICITATIONS],
                                  grille->colonnes * 4,
                                  grille->lignes);
        PAUSE(2000);
        return;
    }
    // les colonnes sont fixees a max 20 (la grille fixe et le  hud ne peuvent pas en contenir plus)
    // dans ce cas il faudra prevoir un hud reduit mais vu que des jokers jouant sur la taille de grille
    // demande d'utiliser des tableaux dynamiques cela n a pas ete mis en oeuvre
    const char *emojiColonnes[20] = {
        "①", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⑩",
        "⑪", "⑫", "⑬", "⑭", "⑮", "⑯", "⑰", "⑱", "⑲", "⑳"};
    // En-tête colonnes
    printf("  ");
    for (int col = 0; col < grille->colonnes; col++)
        printf("  %s ", emojiColonnes[col]);
    printf("\n");

    const char *emojiLignes[20] = {
        "①", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⑩",
        "⑪", "⑫", "⑬", "⑭", "⑮", "⑯", "⑰", "⑱", "⑲", "⑳"};
    // Affichage des lignes
    for (int row = 0; row < grille->lignes; row++)
    {
        // en-têtes lignes
        printf("%s  ", emojiLignes[row]);
        // affichage des cases
        for (int col = 0; col < grille->colonnes; col++)
        {
            int pion = grille->tableau[row][col].pion;
            bool gelatine = grille->tableau[row][col].gelatine;

            const char *emoji;
            // cases de chute
            if (pion == VIDE)
            {
                emoji = "⬇️";
            }
            // pions
            else
            {
                switch (pion)
                {
                case JAUNE:
                    emoji = "🍋";
                    break; // citron
                case VERT:
                    emoji = "🍏";
                    break; // pomme verte
                case BLEU:
                    emoji = "💎";
                    break; // diamant
                case ROUGE:
                    emoji = "🍒";
                    break; // cerise
                case MAUVE:
                    emoji = "🍇";
                    break; // raisin
                case BLANC:
                    emoji = "🧊";
                    break; // glace
                case GRIS:
                    emoji = "🪨";
                    break; // roche
                case JAUNE_CLAIR:
                    emoji = "🧁";
                    break; // cupcake
                case ROSE:
                    emoji = "🍓";
                    break; // fraise
                // jOKERS
                case SUPPRIMERGRILLE:
                    emoji = "💣";
                    break;
                case BONBONVICTOIRESUPPRIMERGRILLE:
                    emoji = "🔱";
                    break;

                default:
                    emoji = "❓";
                    break;
                }
            }
            // Affichage gelatine
            if (gelatine)
                printf("\033[48;5;225m %s \033[0m", emoji); // fond rose clair
            else
                printf(" %s ", emoji);
        }

        printf("\n");
    }
    // === HUD + Barre de progression ===
    // Affichage de la barre de progression et des informations du niveau (niveau,coups joués, jockers et explications)
    int coupsJoues = NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues;
    int coupsMax = NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupAJouer;
    int pourcentage = (coupsJoues * 100) / coupsMax;
    int barres = pourcentage / 5; // 20 segments (chaque = 5%)

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    // Affichage des informations du niveau
    printf("🎯 Niveau : %d     💥 Coups joués : %d/%d\n",
           NIVEAUX[0].compteurNiveau + 1, coupsJoues, coupsMax);
    printf("🧼  Gelatines a supprimer : %d/%d\n",
           grille->nombreGelatinesRestantes, grille->nombreGelatineDuNiveau);
    // adjustement de la barre de progression (coups joues/coups max)
    printf("📊 Progression : [");
    for (int i = 0; i < 20; i++)
        printf("%s", i < barres ? "\033[38;5;82m█\033[0m" : "▁"); // Vert pour la progression
    printf("] %d%%\n", pourcentage);
    // Affichage des jokers et explications si djoker disponible
    printf("🃏 Djokers : %s\n", EXPLICATIONJOCKER[NIVEAUX[0].compteurNiveau]);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}
