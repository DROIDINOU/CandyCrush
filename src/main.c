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

// Regroupe tout ce qui concerne Raylib
void initialiserRaylib(Texture2D textures[], Texture2D *explosionTexture, Music *attenteMusic)
{
    initialiserFenetre();
    initialiserAudio();

    *attenteMusic = chargerMusiqueAttente();
    chargerTextures(textures);
    *explosionTexture = chargerTextureExplosion();

    printf("Explosion texture: id=%d, w=%d, h=%d\n", explosionTexture->id, explosionTexture->width, explosionTexture->height);
}

void libererRessources(Texture2D textures[], Texture2D *explosionTexture, Music *attenteMusic, Music *currentMusic, bool musicChargee)
{
    libererTextures(textures);
    UnloadTexture(*explosionTexture);
    StopMusicStream(*attenteMusic);
    UnloadMusicStream(*attenteMusic);

    if (musicChargee)
    {
        StopMusicStream(*currentMusic);
        UnloadMusicStream(*currentMusic);
    }

    CloseAudioDevice();
    CloseWindow();
}

int main()
{
    srand(time(NULL));
    char buffer[50];

    Texture2D textures[NB_COULEURS];
    Texture2D explosionTexture;
    Music attenteMusic, currentMusic;
    bool musicChargee = false;

    initialiserRaylib(textures, &explosionTexture, &attenteMusic);

    bool jeuDemarre = false;
    afficherMenuAccueil(&jeuDemarre);

    GrilleBonbons maGrille;
    Queue q;
    InitialiserQueue(&q);
    Actions initAction = {INITIALISATION, {0, 0}, {0, 0}, true};
    Enfiler(&q, &initAction);

    EtatJeu etat;
    initialiserEtatJeu(&etat);

    while (!WindowShouldClose() && NIVEAUX[0].compteurNiveau < FINALNIVEAU)
    {
        gererMusiqueParNiveau(&etat.niveauPrecedent, &currentMusic, &musicChargee);
        gererEtatMusical(&etat.etatAttente, etat.dureeAttente, &etat.tempsDebutAttente, &attenteMusic, &currentMusic, musicChargee);

        if (q.taille > 0)
        {
            Actions action = Defiler(&q);
            maGrille.lastAction = action.actionName;
            maGrille.pion1Affiche = action.pion1;
            maGrille.pion2Affiche = action.pion2;

            switch (action.actionName)
            {
            case CALCUL:
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
                break;
            case SUPPRESSIONH:
                etat.explosionP1 = action.pion1;
                etat.explosionP2 = action.pion2;
                etat.typeExplosion = SUPPRESSIONH;
                etat.explosionEnCours = true;
                etat.tempsExplosion = GetTime();
                SuppressionH(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                break;
            case SUPPRESSIONV:
                etat.explosionP1 = action.pion1;
                etat.explosionP2 = action.pion2;
                etat.typeExplosion = SUPPRESSIONV;
                etat.explosionEnCours = true;
                etat.tempsExplosion = GetTime();
                SuppressionV(&maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y, &q);
                break;
            case VERIFICATION:
                Verification(&maGrille, &q);
                maGrille.estVerifiee = 1;
                break;
            case DEPLACEMENT:
                maGrille.estVerifiee = 0;
                NIVEAUX[NIVEAUX[0].compteurNiveau].coupsNiveau.coupsJoues++;
                Deplacement(&q, &maGrille, action.pion1.x, action.pion1.y, action.pion2.x, action.pion2.y);
                break;
            case LECTURE:
                etat.attenteClics = true;
                etat.clicCompteur = 0;
                break;
            case INITIALISATION:
                initialiser_grille(&maGrille);
                Calcul(&q, &maGrille, &action.pion1.x, &action.pion1.y, &action.pion2.x, &action.pion2.y);
                break;
            case FINNIVEAU:
                etat.etatFinNiveau = true;
                etat.tempsDebutFinNiveau = GetTime();
                maGrille.estVerifiee = 0;
                break;
            case FIN:
                etat.etatFinJeu = true;
                etat.tempsDebutFinJeu = GetTime();
                break;
            default:
                break;
            }
        }

        gererEtatTemporel(&etat, &maGrille, &q);
        afficherEtatsEtFin(&etat, buffer, textures, &maGrille, &q, explosionTexture);
        verifierFinNiveau(&etat, &q);
    }

    libererRessources(textures, &explosionTexture, &attenteMusic, &currentMusic, musicChargee);

    printf("FIN DU JEU\n");
    return 0;
}
