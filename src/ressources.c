#include "raylib.h"
#include "constante.h"

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
    return LoadTexture("../candyimages/circle_01.png");
}
