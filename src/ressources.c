#include "raylib.h"
#include "constante.h"
#include <stdio.h>
#include <string.h>

/**
 * Charge toutes les textures de bonbons depuis le dossier candyimages
 * et les stocke dans le tableau textures[] indexé par les constantes de couleur.
 */
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

/**
 * Parcourt le tableau de textures et libère chacune d’elles.
 * À appeler avant de fermer le programme pour éviter les fuites GPU.
 */
void libererTextures(Texture2D textures[])
{
    for (int i = 0; i < NB_COULEURS; i++)
    {
        UnloadTexture(textures[i]);
    }
}

/**
 * Charge une texture d’explosion (exemple : cercle blanc) et la retourne.
 * Affiche un debug message à chaque appel.
 */
Texture2D chargerTextureExplosion()
{
    printf("Chargement de la texture d'explosion\n");
    return LoadTexture("../candyimages/circle_01.png");
}

/**
 * Ouvre une fenêtre Raylib en 1000×1000 pixels et fixe le FPS à 60.
 */
void initialiserFenetre()
{
    InitWindow(1000, 1000, "Candy Crush Clone");
    SetTargetFPS(60);
}

/**
 * Initialise le périphérique audio de Raylib.
 * À appeler une seule fois avant tout usage de l’audio.
 */
void initialiserAudio()
{
    InitAudioDevice();
}

/**
 * Charge et retourne la musique d’attente (écran de chargement).
 */
Music chargerMusiqueAttente()
{
    return LoadMusicStream("../assets/jellysplash_56f256e05113918.mp3");
}

/**
 * Change la musique en fonction du niveau actuel.
 * - niveauPrecedent : pointeur vers le dernier niveau chargé
 * - currentMusic    : pointeur vers la musique courante
 * - musicChargee    : pointeur vers le flag de chargement
 *
 * Si le niveau a changé :
 *   • Arrête et décharge l’ancienne musique si chargée
 *   • Charge la nouvelle musique (fichier music_niveau_X.mp3)
 *   • Lance la lecture et ajuste le volume
 *   • Met à jour niveauPrecedent et musicChargee
 */
void gererMusiqueParNiveau(int *niveauPrecedent, Music *currentMusic, bool *musicChargee)
{
    // On ne refait rien si on est sur le même niveau
    if (*niveauPrecedent != NIVEAUX[0].compteurNiveau)
    {
        char chemin[100];
        // Construit le chemin du fichier de musique selon le niveau
        sprintf(chemin, "../assets/music_niveau_%d.mp3", NIVEAUX[0].compteurNiveau);

        // Si une musique est déjà chargée, on l’arrête et on la décharge
        if (*musicChargee)
        {
            StopMusicStream(*currentMusic);
            UnloadMusicStream(*currentMusic);
            *musicChargee = false;
        }

        // Charge la nouvelle musique depuis le chemin généré
        *currentMusic = LoadMusicStream(chemin);

        // Vérifie que le chargement a réussi via ctxData non NULL
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

        // Met à jour le niveau précédemment chargé
        *niveauPrecedent = NIVEAUX[0].compteurNiveau;
    }
}

/**
 * Met à jour les flux musicaux en cours :
 * - Si musique principale chargée : update du stream
 * - Si on est en « état attente » : joue la musique d’attente
 *   jusqu’à ce que la durée soit écoulée, puis retourne à la musique principale.
 *
 * Paramètres :
 *  - etatAttente       : flag indiquant si on doit jouer la musique d’attente
 *  - dureeAttente      : durée totale de l’état d’attente (en secondes)
 *  - tempsDebutAttente : pointeur au timestamp de début d’attente (GetTime())
 *  - musiqueAttente    : pointeur vers le stream de la musique d’attente
 *  - musiqueActuelle   : pointeur vers le stream de la musique de jeu
 *  - musicChargee      : bool indiquant si la musique de jeu est chargée
 */
void gererEtatMusical(bool *etatAttente, double dureeAttente, double *tempsDebutAttente,
                      Music *musiqueAttente, Music *musiqueActuelle, bool musicChargee)
{
    // Toujours mettre à jour la musique principale si elle tourne
    if (musicChargee)
        UpdateMusicStream(*musiqueActuelle);

    // En mode attente, on met à jour et on relance la musique d’attente si nécessaire
    if (*etatAttente)
    {
        UpdateMusicStream(*musiqueAttente);

        // Si la musique d’attente est terminée, la relancer
        if (!IsMusicStreamPlaying(*musiqueAttente))
        {
            SetMusicVolume(*musiqueAttente, 1.0f);
            if (musicChargee)
                SetMusicVolume(*musiqueActuelle, 0.0f);
            PlayMusicStream(*musiqueAttente);
        }

        // Dès que le temps d’attente est écoulé, on sort de l’état attente
        if ((GetTime() - *tempsDebutAttente) >= dureeAttente)
        {
            *etatAttente = false;
            StopMusicStream(*musiqueAttente);
            if (musicChargee)
                SetMusicVolume(*musiqueActuelle, 0.2f);
        }
    }
}
