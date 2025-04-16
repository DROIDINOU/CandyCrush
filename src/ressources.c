#include "raylib.h"
#include "constante.h"
#include <stdio.h>
#include <string.h>

void chargerTextures(Texture2D textures[])
{
    textures[MAUVE] = LoadTexture("../candyimages/5cd560a569ed85884c879cb1da8e7d68.png");
    textures[ROUGE] = LoadTexture("../candyimages/poignee_de_bonbons.png");
    textures[BLEU] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-candy-crush-candy-crush-5ad0dcad6773e1.4200818515236374214238.png");
    textures[VERT] = LoadTexture("../candyimages/580b57fcd9996e24bc43c517.png");
    textures[JAUNE] = LoadTexture("../candyimages/kisspng-candy-crush-saga-candy-crush-soda-saga-lollipop-ga-sweet-cheats-for-candy-crush-saga-1-2-ipa-war4-5b67aab6d41443.6117650115335205668687.png");
    textures[BLANC] = LoadTexture("../candyimages/293530-P7Q9H0-62.jpg");
    textures[GRIS] = LoadTexture("../candyimages/pile-sweet-donuts.jpg");
    textures[JAUNE_CLAIR] = LoadTexture("../candyimages/5939.jpg");
    textures[ROSE] = LoadTexture("../candyimages/kisspng-spiral-circle-magenta-lollipop-5b2c7a0466c112.1881360715296414764209.png");
}

void libererTextures(Texture2D textures[])
{
    for (int i = 0; i < NB_COULEURS; i++)
    {
        UnloadTexture(textures[i]);
    }
}

Texture2D chargerTextureExplosion()
{
    printf("????????????????????????????????????????????????????????????????????????\n");
    return LoadTexture("../candyimages/circle_01.png");
}

void initialiserFenetre()
{
    InitWindow(1000, 1000, "Candy Crush Clone");
    SetTargetFPS(60);
}

void initialiserAudio()
{
    InitAudioDevice();
}

Music chargerMusiqueAttente()
{
    return LoadMusicStream("../assets/jellysplash_56f256e05113918.mp3");
}

void gererMusiqueParNiveau(int *niveauPrecedent, Music *currentMusic, bool *musicChargee)
{
    if (*niveauPrecedent != NIVEAUX[0].compteurNiveau)
    {
        char chemin[100];
        sprintf(chemin, "../assets/music_niveau_%d.mp3", NIVEAUX[0].compteurNiveau);

        if (*musicChargee)
        {
            StopMusicStream(*currentMusic);
            UnloadMusicStream(*currentMusic);
            *musicChargee = false;
        }

        *currentMusic = LoadMusicStream(chemin);
        if (currentMusic->ctxData == NULL)
        {
            printf("⚠ Erreur chargement musique : %s\n", chemin);
        }
        else
        {
            PlayMusicStream(*currentMusic);
            SetMusicVolume(*currentMusic, 0.2f);
            *musicChargee = true;
        }

        *niveauPrecedent = NIVEAUX[0].compteurNiveau;
    }
}

void gererEtatMusical(bool *etatAttente, double dureeAttente, double *tempsDebutAttente,
                      Music *musiqueAttente, Music *musiqueActuelle, bool musicChargee)
{
    if (musicChargee)
        UpdateMusicStream(*musiqueActuelle);

    if (*etatAttente)
    {
        UpdateMusicStream(*musiqueAttente);

        if (!IsMusicStreamPlaying(*musiqueAttente))
        {
            SetMusicVolume(*musiqueAttente, 1.0f);
            if (musicChargee)
                SetMusicVolume(*musiqueActuelle, 0.0f);
            PlayMusicStream(*musiqueAttente);
        }

        if ((GetTime() - *tempsDebutAttente) >= dureeAttente)
        {
            *etatAttente = false;
            StopMusicStream(*musiqueAttente);
            if (musicChargee)
                SetMusicVolume(*musiqueActuelle, 0.2f);
        }
    }
}
