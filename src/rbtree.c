#include "rbtree.h"

#include <stdlib.h>
#include <stdbool.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p -> nil = (node_t *)calloc(1, sizeof(node_t));
  p -> nil -> color = RBTREE_BLACK;
  p -> nil -> parent = p -> nil -> left = p -> nil -> right = p -> nil;

  p -> root = p -> nil;
  return p;
}

void delete_rbtree_helper(rbtree *t, node_t *node) {
  if (node != t -> nil) {
    delete_rbtree_helper(t, node -> left);
    delete_rbtree_helper(t, node -> right);
    free(node);
  }
}

void delete_rbtree(rbtree *t) {
  delete_rbtree_helper(t, t -> root);
  free(t -> nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x -> right;
  x -> right = y -> left;
  if (y -> left != t -> nil) {
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;

  if (x -> parent == t -> nil) {
    t -> root = y;
  }
  else if (x == x -> parent -> left) {
    x -> parent -> left = y;
  }
  else {
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x -> left;
  x -> left = y -> right;
  if (y -> right != t -> nil) {
    y -> right -> parent = x;
  }
  y -> parent = x -> parent;

  if (x -> parent == t -> nil) {
    t -> root = y;
  }
  else if (x == x -> parent -> left) {
    x -> parent -> left = y;
  }
  else {
    x -> parent -> right = y;
  }
  y -> right = x;
  x -> parent = y;
}

void rb_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED) {
    if ((z -> parent) == (z -> parent -> parent -> left)) {
      node_t *y = z -> parent -> parent -> right;
      if (y -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }
      else {
        if (z == z -> parent -> right) {
          z = z -> parent;
          left_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent -> parent);
      }
    }
    else {
    node_t *y = z -> parent -> parent -> left;
      if (y -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }
      else {
        if (z == z -> parent -> left) {
          z = z -> parent;
          right_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z -> key = key;

  node_t *x = t -> root;
  node_t *y = t -> nil;

  while (x != t -> nil) {
    y = x;
    if ((z -> key) <= (x -> key)) {
      x = x -> left;
    } else {
      x = x -> right;
    }
  }

  z -> parent = y;
  if (y == t -> nil) {
    t -> root = z;
  }
  else if ((z -> key) < (y -> key)) {
    y -> left = z;
  } 
  else {
    y -> right = z;
  }
  
  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED;
  rb_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur = t -> root;
  while (cur != t -> nil) {
    if (cur -> key == key) {
      return cur;
    }
    if (cur -> key < key) {
      cur = cur -> right;
    } else {
      cur = cur -> left;
    }
  }
  
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *cur = t -> root;

  if (cur == t -> nil) {
    return cur;
  }

  while (cur -> left != t -> nil) {
    cur = cur -> left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t -> root;

  if(cur == t -> nil) {
    return cur;
  }
  
  while (cur -> right != t-> nil) {
    cur = cur -> right;
  }
  return cur;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u -> parent == t -> nil){
    t -> root = v;
  }
  else if (u == u -> parent -> left) {
    u -> parent -> left = v;
  }
  else {
    u -> parent -> right = v;
  }
  v -> parent = u -> parent;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  while (x != t -> root && x -> color == RBTREE_BLACK) {
    if (x == x -> parent -> left) {
      node_t *w = x -> parent -> right;
      if (w -> color == RBTREE_RED) {
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);
        w = x -> parent -> right;
      }
      if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) {
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else {
        if (w -> right -> color == RBTREE_BLACK) {
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w = x -> parent -> right;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    } else {
      node_t *w = x -> parent -> left;
      if (w -> color == RBTREE_RED) {
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        w = x -> parent -> left;
      }
      if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK) {
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else {
        if (w -> left -> color == RBTREE_BLACK) {
          w -> right -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          left_rotate(t, w);
          w = x -> parent -> left;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t -> root;
      }
    }
  }
  x -> color = RBTREE_BLACK;
}

node_t *subtree_min(node_t *node, node_t *nil) {
  while (node -> left != nil) {
    node = node -> left;
  }
  return node;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  node_t *x;
  color_t y_original_color = y -> color;

  if (z -> left == t -> nil) {
    x = z -> right;
    rb_transplant(t, z, z->right);
  }
  else if (z -> right == t -> nil) {
    x = z -> left;
    rb_transplant(t, z, z -> left);
  }
  else {
    y = subtree_min(z -> right, t -> nil);
    y_original_color = y -> color;
    x = y -> right;

    if (y != z -> right) {
      rb_transplant(t, y, y -> right);
      y -> right = z -> right;
      y -> right -> parent = y;
    }
    else {
      x -> parent = y;
    }
    rb_transplant(t, z, y);
    y -> left = z -> left;
    y -> left -> parent = y;
    y -> color = z -> color;
  }
  if (y_original_color == RBTREE_BLACK) {
    rb_delete_fixup(t, x);
  }
  free(z);
  return 0;
}

void rbtree_to_array_helper(const rbtree *t, node_t *node, key_t *arr, size_t *index) {

  if (node == t -> nil) {
    return;
  }
  
  rbtree_to_array_helper(t, node -> left, arr, index);
  arr[(*index)++] = node -> key;
  rbtree_to_array_helper(t, node -> right, arr, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t -> root == t -> nil || n == 0){
    return 0;
  }
  
  size_t index = 0;
  rbtree_to_array_helper(t, t -> root, arr, &index);
  return index;
}
