/*
 * Max DeLiso <me@maxdeliso.name>
 * AVL balanced BST implementation test driver.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "bst_tree.h"

void pretty_print_tree_node(struct tree_node * tn, size_t depth) {
  for(size_t i = 0; i < depth; i++) {
    printf("*");
  }
  printf("%d\n", tn -> val);
}

void pretty_print_tree(struct tree_node * tn) {
  int balance_factor;
  printf("inorder:\n");
  tree_visit_inorder(pretty_print_tree_node, tn);

  printf("\npostorder:\n");
  tree_visit_postorder(pretty_print_tree_node, tn);

  printf("\npreorder:\n");
  tree_visit_preorder(pretty_print_tree_node, tn);

  balance_factor = tree_balance_factor(tn);

  printf("\nbalance factor: %i\n\n", balance_factor);
}

int rand_in_range(int max) {
  return (rand() % (max + 1)) - (max / 2);
}

int main(int argc, char **argv) {
  int i, j;
  struct tree_node * tn = NULL;
  int number_of_trials = 10;
  int nodes_per_trial = 32;
  int range = 100;

  if(argc == 2) {
    nodes_per_trial = atoi(argv[1]);
  } else if(argc == 3) {
    number_of_trials = atoi(argv[1]);
    nodes_per_trial = atoi(argv[2]);
  } else if(argc == 4) {
    number_of_trials = atoi(argv[1]);
    nodes_per_trial = atoi(argv[2]);
    range = atoi(argv[3]);
  }

  unsigned int seed = time(0);
  srand(seed);
  printf("seed: %i\n", seed);

  for(i = 0; i < number_of_trials; ++i) {

    tn = tree_node_init(rand_in_range(range));

    for(j = 1; j < nodes_per_trial; ++j) {
      pretty_print_tree(tn);
      tn = tree_insert_avl(rand_in_range(range), tn);
    }

    pretty_print_tree(tn);
    assert(tree_is_bst(tn));

    tree_contains(rand_in_range(range), tn);
    tree_destroy(tn);
  }

  return 0;
}
