#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "btree-h"

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

// Affichage d’un nœud entier
void print_noeud(void *a, void *b) {
    if (!a) return;
    Tree node = (Tree)a;
    int val = *(int *)node->data;
    printf("Valeur du nœud : %-3d | Balance : %d\n", val, node->balance);
}

// Affichage d’un nœud chaîne de caractères
void print_noeud_str(void *a, void *b) {
    if (!a) return;
    Tree node = (Tree)a;
    char *val = *(char **)node->data;
    printf("Valeur du nœud : %-10s | Balance : %d\n", val, node->balance);
}

// Affichage visuel de l’arbre (indentation selon la profondeur)
void print_tree(Tree node, int level) {
    if (!node) return;
    print_tree(node->right, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d (bal=%d)\n", *(int *)node->data, node->balance);
    print_tree(node->left, level + 1);
}

void test_arbre_bicolor(void) {
    int valeurs[] = {2, 4, 6, 8, 10, 12};
    BTree racine = NULL;
    for (int i = 0; i < sizeof(valeurs) / sizeof(valeurs[0]); i++) {
        racine = btree_insert(racine, &valeurs[i], sizeof(int), compare_int);
    }
    printf("Arbre bicolore avec des entiers :\n");
    btree_inorder(racine, print_noeud, NULL);
    print_tree(racine, 0);
    btree_free(racine);
    printf("\n");
    char *str_valeurs[] = {"cinq", "trois", "sept", "un", "neuf", "deux"};
    BTree racine_str = NULL;
    for (int i = 0; i < sizeof(str_valeurs) / sizeof(str_valeurs[0]); i++) {
        racine_str = btree_insert(racine_str, &str_valeurs[i], sizeof(char *), compare_str);
    }
    printf("Arbre bicolore avec des chaînes de caractères :\n");
    btree_inorder(racine_str, print_noeud_str, NULL);
    print_tree(racine_str, 0);
    btree_free(racine_str);

}

int main(void) {
    test_arbre_bicolor();
    return EXIT_SUCCESS;
}
