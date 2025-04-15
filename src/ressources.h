#ifndef RESSOURCES_H
#define RESSOURCES_H

#include "raylib.h"

void chargerTextures(Texture2D textures[]);
void libererTextures(Texture2D textures[]);
Texture2D chargerTextureExplosion();

void initialiserFenetre();
void initialiserAudio();
Music chargerMusiqueAttente();
void gererMusiqueParNiveau(int *niveauPrecedent, Music *currentMusic, bool *musicChargee);
void gererEtatMusical(bool *etatAttente, double dureeAttente, double *tempsDebutAttente,
                      Music *musiqueAttente, Music *musiqueActuelle, bool musicChargee);
#endif