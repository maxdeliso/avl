/*
 * Max DeLiso <me@maxdeliso.name>
 * AVL balanced BST implementation.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "bst_tree.h"

struct tree_node * tree_node_init(int val) {
  struct tree_node * new_tree_ptr = malloc(sizeof(struct tree_node));
  new_tree_ptr -> lft = NULL;
  new_tree_ptr -> rgt = NULL;
  new_tree_ptr -> val = val;
  new_tree_ptr -> bal = 0;
  return new_tree_ptr;
}

void tree_node_dump(const struct tree_node * tn) {
  printf("tree_node<%p> { lft: <%p>, rgt: <%p>, val: <%i> }\n",
         (void *) tn,
         (void *) tn -> lft,
         (void *) tn -> rgt,
         tn -> val);
}

void tree_node_destroy(struct tree_node * tn, size_t depth) {
  (void) depth;
  free(tn);
}

int tree_balance_factor(const struct tree_node * tn) {
  return tn -> bal;
}

static int tree_is_bst_walker(const struct tree_node * tn,
                              const int min_val,
                              const int max_val) {
  if(tn == NULL) return 1;
  else if(tn -> val < min_val || tn -> val > max_val) return 0;
  else {
    return tree_is_bst_walker(tn -> lft,
                              tn -> val < min_val ? tn -> val : min_val,
                              tn -> val)
      &&   tree_is_bst_walker(tn -> rgt,
                              tn -> val,
                              tn -> val > max_val ? tn -> val : max_val);
  }
}

int tree_is_bst(const struct tree_node * tn) {
  return tree_is_bst_walker(tn, INT_MIN, INT_MAX);
}

int tree_height(const struct tree_node * tn) {
  if(tn) {
    int lft_height = tree_height(tn -> lft);
    int rgt_height = tree_height(tn -> rgt);

    if(lft_height > rgt_height) {
      return lft_height + 1;
    } else {
      return rgt_height + 1;
    }
  } else {
    return -1;
  }
}

void tree_insert(const int val, struct tree_node * tn) {
  if(val <= tn -> val) {
    if(tn -> lft) {
      tree_insert(val, tn -> lft);
    } else {
      tn -> lft = tree_node_init(val);
    }
  } else {
    if(tn -> rgt) {
      tree_insert(val, tn -> rgt);
    } else {
      tn -> rgt = tree_node_init(val);
    }
  }
}

struct tree_node * tree_insert_avl(const int val, struct tree_node * tn) {
  assert(tn != NULL);

  // A1
  struct tree_node * t = NULL; // Knuth specifies that this should contain a special sentinel
  struct tree_node * p = tn;
  struct tree_node * s = tn;
  struct tree_node * q = NULL;

  // A2, A3, A4
  while(true) {
    if(val < p -> val) {
      q = p -> lft;

      if(q == NULL) {
        q = tree_node_init(val); // contains A5
        p -> lft = q;
        break;
      } else if(q -> bal != 0) {
        t = p;
        s = q;
      }
    } else if(val > p -> val) {
      q = p -> rgt;

      if(q == NULL) {
        q = tree_node_init(val); // contains A5
        p -> rgt = q;
        break;
      } else if(q -> bal != 0){
        t = p;
        s = q;
      }
    } else {
      return tn; // element is already present
    }

    p = q;
  }

  // A6
  balance_t a;
  struct tree_node * r = NULL;

  if(val < s -> val) {
    a = -1;
    r = p = s -> lft;
  } else {
    a = 1;
    r = p = s -> rgt;
  }

  while(p != q) { // NOTE: uses q from A2 - A5
    if(val < p -> val) {
      p -> bal = -1;
      p = p -> lft;
    } else if(val > p -> val) {
      p -> bal = 1;
      p = p -> rgt;
    } // if p == q then loop exits
  }

  // A7
  if(s -> bal == 0) {
    s -> bal = a; // uses a after being set in A6
    return tn;
  } else if(s -> bal == -a) {
    s -> bal = 0;
    return tn;
  }

  assert(s -> bal == a);
  assert(r -> bal == a || r -> bal == -a || r -> bal == 0);

  if(r -> bal == a) {
    // A8 - single rotation
    p = r;
    (* tree_link(s, a)) = * tree_link(r, -a);
    (* tree_link(r, -a)) = s;
    s -> bal = 0;
    r -> bal = 0;
  } else if(r -> bal == -a) {
    // A9 - double rotation
    p = * tree_link(r, -a);
    (* tree_link(r, -a)) = * tree_link(p, a);
    (* tree_link(p, a)) = r;
    (* tree_link(s, a)) = * tree_link(p, -a);
    (* tree_link(p, -a)) = s;

    if(p -> bal == a) {
      s -> bal = -1;
      r -> bal = 0;
    } else if(p -> bal == 0) {
      s -> bal = 0;
      r -> bal = 0;
    } else if(p -> bal == -a) {
      s -> bal = 0;
      r -> bal = 1;
    }
  }

  // A10
  // need to check if t is non-null prior to dereferencing
  if(t != NULL) {
    if(s == t -> rgt) {
      t -> rgt = p;
    } else {
      t -> lft = p;
    }

    assert(tree_is_bst(tn));
    return tn;
  } else {
    assert(tree_is_bst(p));
    return p;
  }
}

struct tree_node * * tree_link(struct tree_node * tn, const balance_t a) {
  assert(tn != NULL);
  assert(a == -1 || a == 1);

  if(a == -1) {
    return & (tn -> lft);
  } else /* a == 1 */ {
    return & (tn -> rgt);
  }
}

int tree_contains(const int val, struct tree_node * tn) {
  if(tn) {
    if(val == tn -> val) return 1;
    else if(val > tn -> val) return tree_contains(val, tn -> rgt);
    else return tree_contains(val, tn -> lft);
  } else {
    return 0;
  }
}

static void tree_visit_inorder_helper(visitor_t visitor, struct tree_node * tn, size_t depth) {
  if(tn) {
    if(tn -> lft) tree_visit_inorder_helper(visitor, tn -> lft, depth + 1);
    visitor(tn, depth);
    if(tn -> rgt) tree_visit_inorder_helper(visitor, tn -> rgt, depth + 1);
  }
}

void tree_visit_inorder(visitor_t visitor, struct tree_node * tn) {
  tree_visit_inorder_helper(visitor, tn, 0);
}

static void tree_visit_postorder_helper(visitor_t visitor, struct tree_node * tn, size_t depth) {
  if(tn) {
    tree_visit_postorder_helper(visitor, tn -> lft, depth + 1);
    tree_visit_postorder_helper(visitor, tn -> rgt, depth + 1);
    visitor(tn, depth);
  }
}

void tree_visit_postorder(visitor_t visitor, struct tree_node * tn) {
  tree_visit_postorder_helper(visitor, tn, 0);
}

static void tree_visit_preorder_helper(visitor_t visitor, struct tree_node * tn, size_t depth) {
  if(tn) {
    visitor(tn, depth);
    tree_visit_preorder_helper(visitor, tn -> lft, depth + 1);
    tree_visit_preorder_helper(visitor, tn -> rgt, depth + 1);
  }
}

void tree_visit_preorder(visitor_t visitor, struct tree_node * tn) {
  tree_visit_preorder_helper(visitor, tn, 0);
}

void tree_destroy(struct tree_node * tn) {
  tree_visit_postorder(tree_node_destroy, tn);
}
