#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#ifdef _WIN32
#include <conio.h>   // pour _getch()
#include <windows.h> // pour Sleep() et SetConsoleCursorPosition
#define PAUSE(ms) Sleep(ms)
#else
#include <unistd.h> // pour usleep()
#define PAUSE(ms) usleep((ms) * 1000)
#endif
#include <stdbool.h>
#include "constante.h"
#include "erreur.h"
#include "queue.h"

int ObtenirReponseAuMessage(int index);
void afficherGrille(GrilleBonbons *grille, Queue *q, EtatJeu *etatJeu);
bool EstPionAdjacent(int x1, int y1, int x2, int y2);
void LirePionsAChanger(GrilleBonbons *grille, int *coordonneeXPremierPion,
                       int *coordonneeYPremierPion, int *coordonneeXDeuxiemePion,
                       int *coordonneeYDeuxiemePion, Queue *q);

#endif // AFFICHAGE_H