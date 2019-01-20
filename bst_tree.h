/*
 * Max DeLiso <me@maxdeliso.name>
 * C header for BST implementation.
 */

#ifndef _TREE_H
#define _TREE_H

typedef char balance_t;

struct tree_node {
  struct tree_node *lft, *rgt;
  int val;
  balance_t bal;
};

typedef void (* visitor_t)(struct tree_node * vt, size_t depth);

struct tree_node * tree_node_init(int val);
void tree_node_dump(const struct tree_node * tn);
void tree_node_destroy(struct tree_node * tn, size_t depth);
int  tree_balance_factor(const struct tree_node * tn);
int  tree_is_bst(const struct tree_node * tn);
int  tree_height(const struct tree_node * tn);
void tree_insert(const int val, struct tree_node * tn);
struct tree_node * tree_insert_avl(const int val, struct tree_node * tn);
struct tree_node * * tree_link(struct tree_node * tn, const balance_t a);
int  tree_contains(const int val, struct tree_node * tn);
void tree_visit_inorder(visitor_t visitor, struct tree_node * tn);
void tree_visit_postorder(visitor_t visitor, struct tree_node * tn);
void tree_visit_preorder(visitor_t visitor, struct tree_node * tn);
void tree_destroy(struct tree_node * tn);

#endif
