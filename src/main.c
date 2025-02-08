#include <SDL.h>
#include <stdio.h>

#define TAILLE_PETITE 4


/*
typedef struct {
    int lignes;
    int colonnes;
    char tableau[TAILLE_PETITE][TAILLE_PETITE];
} GrilleBonbons;



void initialiser_grille(GrilleBonbons *grille) {
    char couleurs[] = {'R', 'V', 'B', 'J'};  // Rouge, Vert, Bleu, Jaune
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            grille->tableau[i][j] = couleurs[(i + j) % 4];  // Remplissage avec des couleurs
        }
    }
}

void afficher_grille(SDL_Renderer *renderer, GrilleBonbons *grille) {
    for (int i = 0; i < grille->lignes; i++) {
        for (int j = 0; j < grille->colonnes; j++) {
            SDL_Rect rect;
            rect.x = j * 100;  // Position x du rectangle
            rect.y = i * 100;  // Position y du rectangle
            rect.w = 100;      // Largeur du rectangle
            rect.h = 100;      // Hauteur du rectangle

            // Définir la couleur du rectangle
            switch (grille->tableau[i][j]) {
                case 'R': SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;  // Rouge
                case 'V': SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;  // Vert
                case 'B': SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;  // Bleu
                case 'J': SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break; // Jaune
            }

            SDL_RenderFillRect(renderer, &rect);  // Dessiner le rectangle
        }
    }
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        return -1;
    }

    SDL_Window* pWindow = SDL_CreateWindow("Ma première application SDL2",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           640,
                                           480,
                                           SDL_WINDOW_SHOWN);

    if (!pWindow) {
        fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return -1;
    }

    GrilleBonbons maGrille;
    maGrille.lignes = TAILLE_PETITE;
    maGrille.colonnes = TAILLE_PETITE;
    initialiser_grille(&maGrille);

    // Boucle principale
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        afficher_grille(renderer, &maGrille);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}

// gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2

*/





