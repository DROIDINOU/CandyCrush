#include <stdio.h>
#include <stdlib.h>

#include "constante.h"
#include "generationaleatoire.h"
#include "erreur.h"

/***************************************************************************************************************************
                                                   GENERATION ALEATOIRE
    Fonction : GenerationAleatoire
    Parametres : type (TypeElementRandom) - maxTentatives (int)


    Description : Fonction qui génère un nombre aléatoire en fonction du type demandé (couleur ou obstacle).
                  En fonction du type, elle génère un nombre aléatoire entre 0 et le nombre maximum de couleurs ou d'obstacles.
                  Si maxTentatives est atteint, la fonction retourne une erreur.
                  Pour le moment les types sont : ALEATOIRECOULEUR et ALEATOIREOBSTACLE et maxTentatives n'est pas utilisé
                  dans ces cas.
                  MaxTentatives sera utilise pour des cas ou il n'y a pas d'obligation de générer un nombre aléatoire
                  (exemple : un nombre de points bonus qui sera généré aléatoirement et pourrait ne pas être attribué, genre de
                   jeu bonus)
 ****************************************************************************************************************************/

int GenerationAleatoire(TypeElementRandom type, int maxTentatives)
{
    // Si maxTentatives est inférieur ou égal à zéro, il y a eu une erreur dans la génération

    if (maxTentatives <= 0)
    {
        GererErreurFatale(ECHECGENERATIONTENTATIVES);
    }

    switch (type)
    {
    case COULEURALEATOIRE:
    {
        int couleurAleatoire = rand() % NIVEAUX[NIVEAUX[0].compteurNiveau].randomColorModulo;

        if (couleurAleatoire < MAXCOULEUR) // Vérifie si la couleur est valide
        {
            // Si la couleur est valide, on retourne la valeur
            if (couleurAleatoire == 0)
            {
                couleurAleatoire = 1; // Jamais 0 couleur
            }
            return couleurAleatoire;
        }
        else
        {
            GererErreurFatale(ECHECGENERATIONCOULEURALEATOIRE); // Si la couleur est valide, on retourne la valeur
        }
    }
    break;

    case OBSTACLEALEATOIRE:
    {
        // Nombre minimum d'obstacles à générer
        // On utilise le randomObstacle de la structure NIVEAUX pour déterminer le nombre d'obstacles minimum à générer
        int nombreMinObstacle = NIVEAUX[NIVEAUX[0].compteurNiveau].obstacleNiveau.randomObstacle;

        // Génère une valeur entre minimum obstacle et 2 * minimum obstacle - 1
        int obstaclesAleatoire = rand() % nombreMinObstacle + nombreMinObstacle;

        // Vérifie si la valeur générée est dans les bornes pas plus de gélatines que 3/4 du nombre de cases totales

        if (obstaclesAleatoire < TAILLE - TAILLE / 4)
        {
            if (obstaclesAleatoire == 0)
            {
                obstaclesAleatoire = 1; // Jamais 0 obstacle
            }
            return obstaclesAleatoire;
        }
        else
        {
            GererErreurFatale(ECHECGENERATIONOBSTACLEALEATOIRE); // Si la couleur est valide, on retourne la valeur
        }
    }
    break;

    default:
        GererErreurFatale(TYPEINCONNU); // Type non reconnu
    }

    // Appel récursif si les conditions ne sont pas remplies sera éventuellement utilisé en cas
    // d'attributions de points bonus (jeux bonus) ou d'autres types de génération aléatoire
    return GenerationAleatoire(type, maxTentatives - 1);
}
