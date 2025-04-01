#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "erreur.h"

void GererErreurFatale(CodeErreur codeErreur)
{
    switch (codeErreur)
    {
    case ERREURQUEUEPLEINE:
        printf("la queue est pleine ... \n");
        break;

    case ERREURQUEUEVIDE:
        printf("la queue est vide ... \n");
        break;
    case ERREURDEPLACEMENT:
        /* code */
        break;

    default:
        break;
    }
}

void GererErreurEntree() {}