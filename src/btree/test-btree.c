#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "btree.h"

// Fonction de comparaison pour des entiers
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Fonction de comparaison pour des chaînes de caractères
int compare_str(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s1, s2);
}
// Fonction pour imprimer un nœud
void print_noeud(void *data, void *extra_data) {
    int *val = (int *)data;
    printf("%d ", *val);
}
// Fonction pour imprimer un nœud de chaîne de caractères
void print_noeud_str(void *data, void *extra_data) {
    const char *str = *(const char **)data;
    printf("%s ", str);
}
// Fonction pour imprimer l'arbre
void print_tree(BTree btree, int level) {
    if (btree) {
        print_tree(btree->right, level + 1);
        for (int i = 0; i < level; i++) {
            printf("   ");
        }
        printf("%s (%s)\n", (char *)btree->data, btree->couleur == ROUGE ? "ROUGE" : "NOIR");
        print_tree(btree->left, level + 1);
    }
}

void test_arbre_bicolor(void) {
    int valeurs[] = {2, 4, 6, 8, 10, 12};
    BTree racine = NULL;
    for (int i = 0; i < sizeof(valeurs) / sizeof(valeurs[0]); i++) {
        racine = btree_insert_sorted(&racine, &valeurs[i], sizeof(int), compare_int);
    }   
    printf("Arbre bicolore avec des entiers :\n");
    btree_post_order(racine, print_noeud, NULL);
    print_tree(racine, 0);
    btree_delete(racine, NULL);
}

int main(void) {
    test_arbre_bicolor();
    return EXIT_SUCCESS;
}
