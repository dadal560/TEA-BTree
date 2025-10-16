#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../tree-avl/tree-avl.h"
#include "../btree/btree.h"

int test_sizes[] = {10, 50, 100, 150, 200, 300, 500, 750, 1000};
int num_sizes = 9;

typedef struct {
    char mot[50];
    char definition[200];
} Dictionnaire;

int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compare_dict(const void *a, const void *b) {
    const Dictionnaire *d1 = (const Dictionnaire*)a;
    const Dictionnaire *d2 = (const Dictionnaire*)b;
    return strcmp(d1->mot, d2->mot);
}

void benchmark_int() {
    FILE *file = fopen("benchmark_int.dat", "w");
    printf("\n=== Benchmark Entiers ===\n");
    printf("Taille\tAVL Insert (ms)\tBicolore Insert (ms)\n");

    for (int i = 0; i < num_sizes; i++) {
        int size = test_sizes[i];
        int *data = malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) data[j] = rand() % (size * 10);

        Tree avl = NULL;
        clock_t t1 = clock();
        for (int j = 0; j < size; j++)
            tree_insert_avl(&avl, &data[j], sizeof(int), compare_int);
        double avl_insert = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000;

        BTree bt = NULL;
        t1 = clock();
        for (int j = 0; j < size; j++)
            btree_insert(&bt, &data[j], sizeof(int), compare_int);
        double btree_insert = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000;

        printf("%d\t%.3f\t\t%.3f\n", size, avl_insert, btree_insert);
        fprintf(file, "%d %.3f %.3f\n", size, avl_insert, btree_insert);

        tree_delete(avl, NULL);
        btree_delete(bt, NULL);
        free(data);
    }

    fclose(file);
}

void benchmark_dict() {
    FILE *file = fopen("benchmark_dict.dat", "w");
    printf("\n=== Benchmark Dictionnaire ===\n");
    printf("Taille\tAVL Insert (ms)\tBicolore Insert (ms)\n");

    for (int i = 0; i < num_sizes; i++) {
        int size = test_sizes[i];
        Dictionnaire *data = malloc(size * sizeof(Dictionnaire));

        for (int j = 0; j < size; j++) {
            sprintf(data[j].mot, "mot%d", rand());
            sprintf(data[j].definition, "definition%d", j);
        }

        Tree avl = NULL;
        clock_t t1 = clock();
        for (int j = 0; j < size; j++)
            tree_insert_avl(&avl, &data[j], sizeof(Dictionnaire), compare_dict);
        double avl_insert = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000;

        BTree bt = NULL;
        t1 = clock();
        for (int j = 0; j < size; j++)
            btree_insert(&bt, &data[j], sizeof(Dictionnaire), compare_dict);
        double btree_insert = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000;

        printf("%d\t%.3f\t\t%.3f\n", size, avl_insert, btree_insert);
        fprintf(file, "%d %.3f %.3f\n", size, avl_insert, btree_insert);

        tree_delete(avl, NULL);
        btree_delete(bt, NULL);
        free(data);
    }

    fclose(file);
}

void generer_graphique() {
    FILE *gnuplot = popen("gnuplot", "w");
    if (gnuplot == NULL) {
        perror("Erreur lors du lancement de gnuplot");
        return;
    }

    fprintf(gnuplot,
        "set terminal png size 800,600\n"
        "set output 'benchmark.png'\n"
        "set title 'Comparaison AVL vs Bicolore'\n"
        "set xlabel 'Nombre de données'\n"
        "set ylabel 'Temps (ms)'\n"
        "set grid\n"
        "plot 'benchmark_int.dat' using 1:2 with linespoints title 'AVL - Int',\\\n"
        "     'benchmark_int.dat' using 1:3 with linespoints title 'Bicolore - Int',\\\n"
        "     'benchmark_dict.dat' using 1:2 with linespoints title 'AVL - Dict',\\\n"
        "     'benchmark_dict.dat' using 1:3 with linespoints title 'Bicolore - Dict',\\\n"
        "     x with lines title 'f(x)=x'\n"
    );
    fflush(gnuplot);
    pclose(gnuplot);

    printf("\nGraphique généré : benchmark.png \n");
}

int main() {
    srand((unsigned int)time(NULL));
    benchmark_int();
    benchmark_dict();
    generer_graphique();
    return 0;
}
