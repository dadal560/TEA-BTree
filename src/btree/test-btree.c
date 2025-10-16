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
        int *val = (int *)btree->data;
        printf("%d (%s)\n", *val, btree->couleur == ROUGE ? "ROUGE" : "NOIR");
        print_tree(btree->left, level + 1);
    }
}

//Fonction pour imprimer l'arbre de chaînes de caractères
void print_tree_str(BTree btree, int level) {
    if (btree) {
        print_tree_str(btree->right, level + 1);
        for (int i = 0; i < level; i++) {
            printf("   ");
        }
        const char *str = *(const char **)btree->data;
        printf("%s (%s)\n", str, btree->couleur == ROUGE ? "ROUGE" : "NOIR");
        print_tree_str(btree->left, level + 1);
    }
}


void test_arbre_bicolor(void) {
    int valeurs[] = {2, 4, 6, 8, 10, 12};
    BTree racine = NULL;
    for (int i = 0; i < sizeof(valeurs) / sizeof(valeurs[0]); i++) {
        btree_insert(&racine, &valeurs[i], sizeof(int), compare_int);
    }   
    printf("Arbre bicolore avec des entiers :\n");
    btree_post_order(racine, print_noeud, NULL);
    printf("\nStructure de bicolore\n");
    print_tree(racine, 0);

    // Suppression d'un nœud 2
    int val_to_delete = 2;
    btree_delete_node(&racine, &val_to_delete, compare_int);
    printf("\nArbre après suppression de %d (parcours PostFixe)\n", val_to_delete);
    btree_post_order(racine, print_noeud, NULL);
    printf("\nStructure de l'arbre après suppression\n");
    print_tree(racine, 0);

    // Insertion d'un nœud 14
    int val_to_insert = 14;
    if (btree_insert(&racine, &val_to_insert, sizeof(int), compare_int)) {
        printf("\nArbre après insertion de %d (parcours PostFixe)\n", val_to_insert);
        btree_post_order(racine, print_noeud, NULL);
        printf("\nStructure de l'arbre après insertion\n");
        print_tree(racine, 0);
    } else {
        printf("\nÉchec de l'insertion de %d\n", val_to_insert);
    }
    btree_delete(racine, NULL);

    // Test avec des chaînes de caractères
    const char *mots[] = {"pomme", "banane", "ananas", "orange", "kiwi", "mangue"};
    BTree racine_str = NULL;
    for (int i = 0; i < sizeof(mots) / sizeof(mots[0]); i++) {
        btree_insert(&racine_str, &mots[i], sizeof(char *), compare_str);
    }
    printf("\nArbre bicolore avec des chaînes de caractères :\n");
    btree_post_order(racine_str, print_noeud_str, NULL);
    printf("\nStructure de bicolore\n");
    print_tree_str(racine_str, 0);
    
    // Suppression d'un nœud "banane"
    const char *str_to_delete = "banane";
    btree_delete_node(&racine_str, &str_to_delete, compare_str);
    printf("\nArbre après suppression de %s (parcours PostFixe)\n", str_to_delete);
    btree_post_order(racine_str, print_noeud_str, NULL);
    printf("\nStructure de l'arbre après suppression\n");
    print_tree_str(racine_str, 0);

    // Insertion d'un nœud "fraise"
    const char *str_to_insert = "fraise";
    if (btree_insert(&racine_str, &str_to_insert, sizeof(char *), compare_str)) {
        printf("\nArbre après insertion de %s (parcours PostFixe)\n", str_to_insert);
        btree_post_order(racine_str, print_noeud_str, NULL);
        printf("\nStructure de l'arbre après insertion\n");
        print_tree_str(racine_str, 0);
    } else {
        printf("\nÉchec de l'insertion de %s\n", str_to_insert);
    }
    btree_delete(racine_str, NULL);
}

int main(void) {
    test_arbre_bicolor();
    return EXIT_SUCCESS;
}
