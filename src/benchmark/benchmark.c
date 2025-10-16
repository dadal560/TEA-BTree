#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../tree-avl/tree-avl.h"
#include "../btree/btree.h"

// Tailles de test
int test_sizes[] = {10, 50, 100, 150, 200, 300, 500, 750, 1000};
int num_sizes = 9;

// Structure pour dictionnaire (mot:définition)
typedef struct {
    char mot[50];
    char definition[200];
} Dictionnaire;

// Fonction de comparaison pour des entiers
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Fonction de comparaison pour dictionnaire (par mot)
int compare_dict(const void *a, const void *b) {
    const Dictionnaire *d1 = (const Dictionnaire *)a;
    const Dictionnaire *d2 = (const Dictionnaire *)b;
    return strcmp(d1->mot, d2->mot);
}

void benchmark_trees() {
    printf("Benchmark des arbres AVL et Bicolores\n");
    printf("Taille\tAVL Insert (ms)\tBicolore Insert (ms)\tAVL Search (ms)\tBicolore Search (ms)\n");

    for (int i = 0; i < num_sizes; i++) {
        int size = test_sizes[i];

        // Génération des données aléatoires
        int *data = (int *)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            data[j] = rand() % (size * 10);
        }

        // === Benchmark AVL ===
        TreeAVL avl_tree = NULL;
        clock_t start_avl_insert = clock();
        for (int j = 0; j < size; j++) {
            tree_avl_insert(&avl_tree, &data[j], sizeof(int), compare_int);
        }
        clock_t end_avl_insert = clock();

        clock_t start_avl_search = clock();
        for (int j = 0; j < size; j++) {
            tree_avl_search(avl_tree, &data[j], compare_int);
        }
        clock_t end_avl_search = clock();

        double avl_insert_time = ((double)(end_avl_insert - start_avl_insert)) / CLOCKS_PER_SEC * 1000;
        double avl_search_time = ((double)(end_avl_search - start_avl_search)) / CLOCKS_PER_SEC * 1000;

        tree_avl_delete(avl_tree, NULL);

        // === Benchmark Bicolore ===
        BTree btree = NULL;
        clock_t start_btree_insert = clock();
        for (int j = 0; j < size; j++) {
            btree_insert(&btree, &data[j], sizeof(int), compare_int);
        }
        clock_t end_btree_insert = clock();

        clock_t start_btree_search = clock();
        for (int j = 0; j < size; j++) {
            btree_search(btree, &data[j], compare_int);
        }
        clock_t end_btree_search = clock();

        double btree_insert_time = ((double)(end_btree_insert - start_btree_insert)) / CLOCKS_PER_SEC * 1000;
        double btree_search_time = ((double)(end_btree_search - start_btree_search)) / CLOCKS_PER_SEC * 1000;

        btree_delete(btree, NULL);

        // === Affichage des résultats ===
        printf("%d\t%.3f\t\t%.3f\t\t\t%.3f\t\t\t%.3f\n",
               size,
               avl_insert_time,
               btree_insert_time,
               avl_search_time,
               btree_search_time);

        free(data);
    }
}
int main() {
    srand((unsigned int)time(NULL));

    benchmark_trees();

    return 0;
}