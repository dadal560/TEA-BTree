#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../tree-avl/tree-avl.h"
#include "../btree/btree.h"

// Test de performance pour les arbres AVL et Bicolores
int test_sizes[] = {10, 50, 100, 150, 200, 300, 500, 750, 1000};
int num_sizes = 9;

// Structure pour dictionnaire (mot:définition)
typedef struct {
    char mot[50];
    char definition[200];
} Dictionnaire;


/ Fonction de comparaison pour des entiers
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Fonction de comparaison pour dictionnaire (par mot)
int compare_dict(const void *a, const void *b) {
    const Dictionnaire *d1 = (const Dictionnaire *)a;
    const Dictionnaire *d2 = (const Dictionnaire *)b;
    return strcmp(d1->mot, d2->mot);
}
