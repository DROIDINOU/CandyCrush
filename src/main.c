#include "raylib.h"
#include "matrice.h"
#include "main.h"
#include "jeu.h"
#include "affichage.h"
#include "queue.h"
#include "constante.h"
#include "ressources.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    Music currentMusic;
    bool musicChargee = false;
    srand(time(NULL));
    char buffer[50];
    initialiserFenetre();
    initialiserAudio();
    Music attenteMusic = chargerMusiqueAttente();

    bool jeuDemarre = false;
    afficherMenuAccueil(&jeuDemarre);

    Texture2D textures[NB_COULEURS]; // ← Assure-toi que NB_COULEURS est bien défini dans constante.h
    chargerTextures(textures);
    Texture2D explosionTexture = chargerTextureExplosion();
    printf("Explosion texture: id=%d, w=%d, h=%d\n",
           explosionTexture.id, explosionTexture.width, explosionTexture.height);
    GrilleBonbons maGrille;

    Queue q;
    InitialiserQueue(&q);
    Actions initAction = {"INITIALISATION", {0, 0}, {0, 0}, true};
    Enfiler(&q, &initAction);

    EtatJeu etat;
    initialiserEtatJeu(&etat);

    while (!WindowShouldClose() && NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        gererMusiqueParNiveau(&etat.niveauPrecedent, &currentMusic, &musicChargee);

        gererEtatMusical(&etat.etatAttente, etat.dureeAttente, &etat.tempsDebutAttente,
                         &attenteMusic, &currentMusic, musicChargee);
        if (q.taille > 0)
        {
            Actions action = Defiler(&q);
            strcpy(maGrille.lastAction, action.actionName);
            maGrille.pion1Affiche = action.pion1;
            maGrille.pion2Affiche = action.pion2;

            if (strcmp(action.actionName, "CALCUL") == 0)
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            else if (strcmp(action.actionName, "SUPPRESSIONH") == 0)
            {
                Coordonnees tmp1 = action.pion1;
                Coordonnees tmp2 = action.pion2;

                maGrille.pion1Affiche = tmp1;
                maGrille.pion2Affiche = tmp2;

                // 🔥 Sauvegarde des coordonnées pour explosion
                etat.explosionP1 = tmp1;
                etat.explosionP2 = tmp2;

                strcpy(etat.typeExplosion, "SUPPRESSIONH");
                etat.explosionEnCours = true;
                etat.tempsExplosion = GetTime();

                SuppressionH(&maGrille, &tmp1.x, &tmp1.y, &tmp2.x, &tmp2.y, &q);
            }
            else if (strcmp(action.actionName, "SUPPRESSIONV") == 0)
            {
                Coordonnees tmp1 = action.pion1;
                Coordonnees tmp2 = action.pion2;

                maGrille.pion1Affiche = tmp1;
                maGrille.pion2Affiche = tmp2;

                etat.explosionP1 = tmp1;
                etat.explosionP2 = tmp2;

                strcpy(etat.typeExplosion, "SUPPRESSIONV");
                etat.explosionEnCours = true;
                etat.tempsExplosion = GetTime();

                SuppressionV(&maGrille, &tmp1.x, &tmp1.y, &tmp2.x, &tmp2.y, &q);
            }

            else if (strcmp(action.actionName, "VERIFICATION") == 0)
            {
                Verification(&maGrille, &q);
                maGrille.estVerifiee = 1;
            }
            else if (strcmp(action.actionName, "DEPLACEMENT") == 0)
            {
                maGrille.estVerifiee = 0;
                NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues++;
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
            }
            else if (strcmp(action.actionName, "LECTURE") == 0)
            {
                etat.attenteClics = true;
                etat.clicCompteur = 0;
            }
            else if (strcmp(action.actionName, "INITIALISATION") == 0)
            {
                initialiser_grille(&maGrille);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
            }
            else if (strcmp(action.actionName, "FINNIVEAU") == 0)
            {
                etat.etatFinNiveau = true;
                etat.tempsDebutFinNiveau = GetTime();
                maGrille.estVerifiee = 0;
            }
            else if (strcmp(action.actionName, "FIN") == 0)
            {
                etat.etatFinJeu = true;
                etat.tempsDebutFinJeu = GetTime();
            }
        }
        gererEtatTemporel(&etat, &maGrille, &q);
        afficherEtatsEtFin(&etat, buffer, textures, &maGrille, &q, explosionTexture);
        verifierFinNiveau(&etat, &q); // ← à la fin
    }

    libererTextures(textures);
    UnloadTexture(explosionTexture);

    StopMusicStream(attenteMusic);
    UnloadMusicStream(attenteMusic);

    if (musicChargee)
    {
        StopMusicStream(currentMusic);
        UnloadMusicStream(currentMusic);
    }

    CloseAudioDevice();
    CloseWindow();

    printf("FIN DU JEU\n");
    return 0;
}