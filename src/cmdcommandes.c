// del /p *.exe
//  affichage.c main.c matrice.c constante.c queue.c
// cls

// a faire ne pas supprimer gelatines a initialisation et
// les faire descendre ???? ou les gerer au niveau de la structure de niveau
// calcul doit etre lance a partir de initialiser grille

#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct
{
    char nom[50];
    int age;
} Personne;

/*
int main()
{
    Personne personnes[MAX];
    int n, i, j;

    // Lecture du nombre de personnes
    printf("Entrez le nombre de personnes : ");
    scanf("%d", &n);

    // Lecture des données
    for (i = 0; i < n; i++)
    {
        printf("Entrez le nom de la personne %d : ", i + 1);
        scanf("%s", personnes[i].nom);
        printf("Entrez l'âge de %s : ", personnes[i].nom);
        scanf("%d", &personnes[i].age);
    }

    // Suppression des personnes âgées de 20 ans ou plus
    for (i = 0; i < n; i++)
    {
        if (personnes[i].age >= 20)
        {
            // Décalage des éléments
            for (j = i; j < n - 1; j++)
            {
                personnes[j] = personnes[j + 1];
            }
            n--; // Réduction de la taille du tableau
            i--; // Vérification du nouvel élément à la même position
        }
    }

    // Affichage du tableau nettoyé
    printf("\nListe des personnes après suppression :\n");
    for (i = 0; i < n; i++)
    {
        printf("Nom: %s, Age: %d\n", personnes[i].nom, personnes[i].age);
    }

    return 0;
}
*/

void nettoyerPersonnes(Personne personnes[], int *n)
{
    int i, j;
    for (i = 0; i < *n; i++)
    {
        if (personnes[i].age >= 20)
        {
            // Décalage des éléments
            for (j = i; j < *n - 1; j++)
            {
                personnes[j] = personnes[j + 1];
            }
            (*n)--; // attention sans parentheses, -- a priorité sur *
            i--;    // Vérification du nouvel élément à la même position
        }
    }
}

int main()
{
    Personne personnes[MAX];
    int n, i;

    // Lecture du nombre de personnes
    printf("Entrez le nombre de personnes : ");
    scanf("%d", &n);

    // Lecture des données
    for (i = 0; i < n; i++)
    {
        printf("Entrez le nom de la personne %d : ", i + 1);
        scanf("%s", personnes[i].nom);
        printf("Entrez l'age de %s : ", personnes[i].nom);
        scanf("%d", &personnes[i].age);
    }

    // Nettoyage des personnes de 20 ans et plus
    nettoyerPersonnes(personnes, &n);

    // Affichage du tableau nettoyé
    printf("\nListe des personnes :\n");
    for (i = 0; i < n; i++)
    {
        printf("Nom: %s, Age: %d\n", personnes[i].nom, personnes[i].age);
    }

    return 0;
}
