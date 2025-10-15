#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tree-avl.h"

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

void test_arbre_avl_int(void) {
    int valeurs[] = {2, 4, 6, 8, 10, 12};
    Tree racine = NULL;

    // Insertion des valeurs
    for (int i = 0; i < 6; i++) {
        tree_insert_avl(&racine, &valeurs[i], sizeof(int), compare_int);
    }

    printf("Arbre après insertion (parcours PostFixe)\n");
    tree_post_order(racine, print_noeud, NULL);

    printf("\nStructure de l'arbre\n");
    print_tree(racine, 0);

    // Suppression d’une valeur
    int val_to_delete = 6;
    racine = tree_delete_avl(racine, &val_to_delete, compare_int, NULL);

    printf("\nArbre après suppression de %d (parcours PostFixe)\n", val_to_delete);
    tree_post_order(racine, print_noeud, NULL);

    printf("\nStructure de l'arbre après suppression\n");
    print_tree(racine, 0);

    // Réinsertion
    tree_insert_avl(&racine, &valeurs[2], sizeof(int), compare_int);
    printf("\nArbre après réinsertion de %d (parcours PostFixe)\n", valeurs[2]);
    tree_post_order(racine, print_noeud, NULL);

    // Recherche de valeurs
    int val_to_search = 10;
    Tree found = tree_search_avl(racine, &val_to_search, compare_int);
    if (found) {
        printf("\nValeur %d trouvée dans l'arbre\n", val_to_search);
    } else {
        printf("\nValeur %d non trouvée dans l'arbre\n", val_to_search);
    }

    int val_not_found = 7;
    found = tree_search_avl(racine, &val_not_found, compare_int);
    if (found) {
        printf("Valeur %d trouvée dans l'arbre\n", val_not_found);
    } else {
        printf("Valeur %d non trouvée dans l'arbre\n", val_not_found);
    }

    tree_delete(racine, NULL);
}

void test_arbre_avl_str(void) {
    const char *mots[] = {"pomme", "banane", "ananas", "orange", "kiwi", "mangue"};
    Tree racine = NULL;

    // Insertion des chaînes
    for (int i = 0; i < 6; i++) {
        tree_insert_avl(&racine, &mots[i], sizeof(char *), compare_str);
    }

    printf("\nArbre AVL (chaînes) après insertion\n");
    tree_post_order(racine, print_noeud_str, NULL);

    // Recherche
    const char *mot_chercher = "kiwi";
    Tree found = tree_search_avl(racine, &mot_chercher, compare_str);
    if (found) {
        printf("\nMot \"%s\" trouvé dans l'arbre\n", mot_chercher);
    } else {
        printf("\nMot \"%s\" non trouvé dans l'arbre\n", mot_chercher);
    }

    // Suppression
    const char *mot_supprimer = "cerise";
    racine = tree_delete_avl(racine, &mot_supprimer, compare_str, NULL);

    printf("\nArbre AVL (chaînes) après suppression de \"%s\"\n", mot_supprimer);
    tree_post_order(racine, print_noeud_str, NULL);

    tree_delete(racine, NULL);
}

int main(void) {
    test_arbre_avl_int();
    test_arbre_avl_str();
    return EXIT_SUCCESS;
}
