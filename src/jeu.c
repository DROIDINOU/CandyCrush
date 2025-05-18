/*
 * Module de gestion de l'état du jeu Candy Crush Clone
 * Contient l'initialisation, le menu d'accueil,
 * le contrôle des clics, la fin de niveau/jeu,
 * et l'affichage des différents états.
 */

#include "raylib.h"    // Bibliothèque graphique et audio
#include "queue.h"     // Gestion de la file d'actions
#include "constante.h" // Constantes globales (NIVEAUX, NB_COULEURS, etc.)
#include "affichage.h" // Fonctions d'affichage de la grille
#include "matrice.h"   // Gestion de la matrice de bonbons
#include "jeu.h"       // Définitions des types Jeu, EtatJeu, etc.
#include <stdbool.h>   // Type bool
#include <string.h>    // Pour memset ou strcmp si nécessaire

/**
 * Initialise tous les champs de l'état du jeu.
 * - attenteClics: mode saisie de deux clics pour un swap
 * - clicCompteur: nombre de clics enregistrés
 * - coordonneesClic[4]: positions ligne/colonne des deux clics
 * - timers et flags pour fin de niveau, fin de jeu, explosions
 */
void initialiserEtatJeu(EtatJeu *etat)
{
    etat->attenteClics = false;
    etat->clicCompteur = 0;
    for (int i = 0; i < 4; i++)
        etat->coordonneesClic[i] = -1; // valeurs invalides par défaut

    etat->etatAttente = false;
    etat->tempsDebutAttente = 0.0;
    etat->dureeAttente = 0.3; // délai avant nouvelle action

    etat->etatFinNiveau = false;
    etat->tempsDebutFinNiveau = 0.0;
    etat->dureeFinNiveau = 2.5; // durée d'affichage du message "Fin de niveau"

    etat->etatFinJeu = false;
    etat->tempsDebutFinJeu = 0.0;
    etat->dureeFinJeu = 2.5; // durée d'affichage du message "Fin du jeu"

    etat->niveauPrecedent = -1; // pour forcer le premier chargement de musique

    etat->explosionEnCours = false;
    etat->tempsExplosion = 0.0;
    etat->dureeExplosion = 1.0; // explosion visible pendant 1 seconde
    etat->typeExplosion = AUCUNE_EXPLOSION;
    etat->pretPourNiveauSuivant = false;
}

/**
 * Affiche le menu d'accueil tant que le jeu n'est pas démarré
 * - Afficheun texte statique
 * - Attends que l'utilisateur appuie sur ESPACE pour démarrer
 */
void afficherMenuAccueil(bool *jeuDemarre)
{
    while (!WindowShouldClose() && !(*jeuDemarre))
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("CANDY CRUSH CLONE", 350, 400, 40, DARKBLUE);
        DrawText("Appuyez sur ESPACE pour commencer", 300, 500, 30, DARKGRAY);
        if (IsKeyPressed(KEY_SPACE))
            *jeuDemarre = true; // passe l'état jeuDemarre à true
        EndDrawing();
    }
}

/**
 * Traite la fin de niveau en deux étapes :
 * 1) Affichage du message fin-niveau pendant dureeFinNiveau
 * 2) Réinitialisation de la queue pour lancer le niveau suivant
 */
void verifierFinNiveau(EtatJeu *etat, Queue *q)
{
    printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    // Étape 1 : temporisation du message "Fin du niveau"
    if (etat->etatFinNiveau &&
        !etat->pretPourNiveauSuivant &&
        (GetTime() - etat->tempsDebutFinNiveau >= etat->dureeFinNiveau))
    {
        printf("⏳ Fin niveau affiché, prêt pour niveau suivant\n");
        etat->pretPourNiveauSuivant = true;
        return; // on quitte pour laisser le message encore une frame
    }

    // Étape 2 : passage effectif au niveau suivant
    if (etat->pretPourNiveauSuivant)
    {
        printf("➡️ Passage au niveau suivant (compteur = %d)\n", NIVEAUX[0].compteurNiveau);
        InitialiserQueue(q);

        Actions nouvelleAction = {INITIALISATION, {0, 0}, {0, 0}, true};
        Enfiler(q, &nouvelleAction); // enfile l'action d'initialisation

        // Reset des flags pour la prochaine boucle
        etat->etatFinNiveau = false;
        etat->pretPourNiveauSuivant = false;
    }
}

/**
 * Vérifie si le jeu doit passer de l'état "fin de jeu"
 * à la fermeture après l'affichage du message.
 * Retourne false si le délai est écoulé.
 */
bool verifierFinJeu(bool etatFinJeu, double tempsDebutFinJeu, double dureeFinJeu)
{
    if (etatFinJeu && (GetTime() - tempsDebutFinJeu >= dureeFinJeu))
    {
        return false; // fin du jeu terminée
    }
    return etatFinJeu;
}

/**
 * Gère les clics souris pour sélectionner deux bonbons à échanger :
 * - Convertit la position écran en indices ligne/colonne
 * - Stocke dans coordonneesClic
 * - Enfile l'action de swap après deux clics
 * - Retourne false pour sortir du mode attente clics
 */
bool gererClics(GrilleBonbons *grille, int *clicCompteur, int coordonneesClic[], Queue *q)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 posSouris = GetMousePosition();
        int tailleCase = 50;
        int grilleLargeur = grille->colonnes * tailleCase;
        int grilleHauteur = grille->lignes * tailleCase;
        int offsetX = (1000 - grilleLargeur) / 2;
        int offsetY = (1000 - grilleHauteur) / 2;

        int xRel = posSouris.x - offsetX;
        int yRel = posSouris.y - offsetY;

        // Vérifie si le clic est bien dans la zone de la grille
        if (xRel >= 0 && xRel < grilleLargeur && yRel >= 0 && yRel < grilleHauteur)
        {
            int colonne = xRel / tailleCase;
            int ligne = yRel / tailleCase;
            coordonneesClic[*clicCompteur * 2] = ligne;
            coordonneesClic[*clicCompteur * 2 + 1] = colonne;
            (*clicCompteur)++;

            if (*clicCompteur == 2)
            {
                LirePionsAChanger(grille,
                                  coordonneesClic[0], coordonneesClic[1],
                                  coordonneesClic[2], coordonneesClic[3], q);
                return false; // désactive mode attenteClics
            }
        }
    }
    return true; // reste en attente de clics
}

/**
 * Met à jour l'état temporel :
 * - Si en attente de clics, appelle gererClics
 * - Désactive les explosions après dureeExplosion
 */
void gererEtatTemporel(EtatJeu *etat, GrilleBonbons *grille, Queue *q)
{
    if (etat->attenteClics)
    {
        etat->attenteClics = gererClics(grille,
                                        &etat->clicCompteur,
                                        etat->coordonneesClic,
                                        q);
    }

    if (etat->explosionEnCours && GetTime() - etat->tempsExplosion >= etat->dureeExplosion)
    {
        etat->explosionEnCours = false; // fin de l'affichage d'explosion
    }
}

/**
 * Affiche la grille, les messages de fin de niveau/jeu :
 * - Utilise afficher_grille pour le rendu principal
 * - Affiche un cadre noir et du texte pour fin de niveau
 * - Boucle jusqu'à ESCAPE pour la fin de jeu
 */
void afficherEtatsEtFin(EtatJeu *etat, char *buffer,
                        Texture2D *textures,
                        GrilleBonbons *grille,
                        Queue *q,
                        Texture2D explosionTexture)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    afficher_grille(grille, textures, q, explosionTexture, etat);

    // Message "FIN DU NIVEAU"
    if (etat->etatFinNiveau)
    {
        sprintf(buffer, "FIN DU NIVEAU %d !", NIVEAUX[0].compteurNiveau);
        DrawRectangle(200, 350, 600, 150, BLACK);
        DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
        DrawText(buffer, 320, 390, 40, RAYWHITE);
        DrawText("Préparation du niveau suivant...", 260, 440, 25, GRAY);
    }

    // Message "FIN DU JEU"
    if (etat->etatFinJeu)
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawRectangle(200, 350, 600, 150, BLACK);
            DrawRectangleLinesEx((Rectangle){200, 350, 600, 150}, 4, RAYWHITE);
            sprintf(buffer, "FIN DU JEU  FELICITATIONS!");
            DrawText(buffer, 220, 390, 30, RAYWHITE);
            DrawText("Appuyez sur [ECHAP] pour quitter", 270, 440, 20, GRAY);
            EndDrawing();

            if (IsKeyPressed(KEY_ESCAPE))
            {
                NIVEAUX[0].compteurNiveau += 1; // incrémente pour rejouer
                break;
            }
        }
    }

    EndDrawing();
}
