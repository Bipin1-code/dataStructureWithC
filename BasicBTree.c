

#include <stdio.h>
#include <stdlib.h>

typedef struct BTNode{
  int *keys; //Array of keys
  struct BTNode **child; //Array of child pointers
  int n; //current number of keys
  int leaf; // 1 if leaf, 0 if internal
} BTNode;

BTNode* createNode(int order, int leaf){
  BTNode *node = (BTNode*)malloc(sizeof(BTNode));
  node->keys = (int*)calloc((order-1), sizeof(int));
  node->child = (BTNode**)calloc(order, sizeof(BTNode*));
  node->n = 0;
  node->leaf = leaf;
  return node;
}

void splitChild(BTNode *parent, int idx, int order){

  // Step 1:
  //This is the child we are splitting (it is full) 
  BTNode *fullChild = parent->child[idx];

  //Create a sibling node for the right half of keys
  //leaf flag is the same as fullChild.
  BTNode *newChild = createNode(order, fullChild->leaf);

  //t middle index of keys
  //for order = 3, t = 1-> the 1st index(middle key) is promoted.
  int t = order/2;

  //Step 2: Copy second half keys to new child
  //Number of keys moved to new child =
  //(total keys in fullchild) - (left half) - (middle one)
  newChild->n = fullChild->n - t - 1;
  
  //copy to newChild right half of fullchild
  for(int j = 0; j < newChild->n; j++){
    newChild->keys[j] = fullChild->keys[j + t + 1];
  }

  //Step 3: Copy child pointer if not leaf
  if(!fullChild->leaf){
    for(int j = 0; j <= newChild->n; j++){
      newChild->child[j] = fullChild->child[j + t + 1];
    }
  }

  //update or adjust the fullChild's size which become left child in our case
  fullChild->n = t;

  //Step 4: Move parent's keys and children to make space 
  for(int j = parent->n; j > idx; j--){
    parent->keys[j] = parent->keys[j - 1];
    parent->child[j+1] = parent->child[j];
  }

  //Step 5: Insert the middle key into parent
  //Middle key from FullChild is promoted up into the parent
  parent->keys[idx] = fullChild->keys[t];

  //right half(newChild) becomes the child after that promoted key
  parent->child[idx+1] = newChild;
  //Parent gained one new key
  parent->n++;
}

void insertNonFull(BTNode *node, int key, int order){
  int i = node->n - 1;

  if(node->leaf){
    //Shift keys to insert
    while(i >= 0 && node->keys[i] > key){
      node->keys[i+1] = node->keys[i];
      i--;
    }
    node->keys[i+1] = key;
    node->n++;
  }else{
    //Find child to descend
    while(i >= 0 && node->keys[i] > key)
      i--;
    i++;

    //Split child is full
    if(node->child[i]->n == (order-1)){
      splitChild(node, i, order);

      if(key > node->keys[i])
	i++;
    }
    insertNonFull(node->child[i], key, order);
  }
}

void insert(BTNode **root, int key, int order){
  if(*root == NULL){
    *root = createNode(order, 1);
    (*root)->keys[0] = key;
    (*root)->n = 1;
    return;
  }

  //full case
  if((*root)->n == (order - 1)){
    //Allocate a new node to become the new root
    //leaf = 0 because root will have children now
    BTNode *newRoot = createNode(order, 0);

    //Make the old root the 0th child of this new root.
    //Temporarily, newRoot has no keys yet, only a child.
    newRoot->child[0] = *root;

    //Split the full child (old root) into two children and
    //move its middle key up into newRoot.
    splitChild(newRoot, 0, order);

    //Update the caller's root pointer(imp: this is why we used **root)
    *root = newRoot;
  }
  //non full case
  insertNonFull(*root, key, order);
  
}

void shiftNode(BTNode *root, int index){
  for(int i = index; i < root->n - 1; i++){
    root->keys[i] = root->keys[i+1];
  }
  root->n--;
}

int getPredecessor(BTNode *root){
  BTNode *cur = root;
  while(!cur->leaf){
    cur = cur->child[cur->n];
  }
  return cur->keys[cur->n - 1];
}

int getSuccessor(BTNode *root){
  BTNode *cur = root;
  while(!cur->leaf){
    cur = cur->child[0];
  }
  return cur->keys[0];
}

void mergeChildrenShift(BTNode *parent, int idx, int order){
  BTNode *left = parent->child[idx];
  BTNode *right = parent->child[idx+1];

  left->keys[left->n] = parent->keys[idx];
  left->n++;

  //Copy all keys from right to left child
  for(int i = 0; i < right->n; i++){
    left->keys[left->n + i] = right->keys[i];
  }
  
  //Copy child pointers if left child is not leaf from right
  if(!left->leaf){
    for(int i = 0; i <= right->n; i++){
      left->child[left->n + i] = right->child[i];
    }
  }
  left->n += right->n;

  //Remove the key and child from parent
  for(int i = idx; i < parent->n - 1; i++)
    parent->keys[i] = parent->keys[i+1];

  for(int i = idx + 1; i < parent->n; i++)
    parent->child[i] = parent->child[i+1];
  
  parent->n--;
   
  free(right->keys);
  free(right->child);
  free(right);
}

void deleteFromNode(BTNode *root, int key, int order, int minKeys);

void borrowFromLeft(BTNode *parent, int idx, int order, int minKeys){
  BTNode *child = parent->child[idx];
  BTNode *leftSibling = parent->child[idx-1];
    
  // Make space in child for the new key (shift right)
  for (int i = child->n; i > 0; i--) {
    child->keys[i] = child->keys[i-1];
  }
    
  // Bring down the key from parent to child
  child->keys[0] = parent->keys[idx-1];
    
  // If not leaf, also move the last child pointer from left sibling
  if (!child->leaf) {
    for (int i = child->n + 1; i > 0; i--) {
      child->child[i] = child->child[i-1];
    }
    child->child[0] = leftSibling->child[leftSibling->n];
  }
    
  // Move the largest key from left sibling up to parent
  parent->keys[idx-1] = leftSibling->keys[leftSibling->n-1];
    
  // Update counts
  child->n++;
  leftSibling->n--;

}

void borrowFromRight(BTNode *parent, int idx){
  BTNode *child = parent->child[idx];
  BTNode *rightSibling = parent->child[idx+1];
    
  // Bring down the key from parent to child (at the end)
  child->keys[child->n] = parent->keys[idx];
    
  // If not leaf, move the first child pointer from right sibling
  if (!child->leaf) {
    child->child[child->n + 1] = rightSibling->child[0];
  }
    
  // Move the smallest key from right sibling up to parent
  parent->keys[idx] = rightSibling->keys[0];
    
  // Shift keys in right sibling left
  for (int i = 1; i < rightSibling->n; i++) {
    rightSibling->keys[i-1] = rightSibling->keys[i];
  }
    
  // If not leaf, shift child pointers in right sibling
  if (!rightSibling->leaf) {
    for (int i = 1; i <= rightSibling->n; i++) {
      rightSibling->child[i-1] = rightSibling->child[i];
    }
  }
    
  // Update counts
  child->n++;
  rightSibling->n--;
}

void fillChild(BTNode *parent, int idx, int order, int minKeys){
  //Try to borrow from left sibling else right
  if(idx > 0 && parent->child[idx-1]->n > minKeys){
    borrowFromLeft(parent, idx);
  }else if(idx < parent->n && parent->child[idx+1]->n > minKeys){
    borrowFromRight(parent, idx);
  }else{
    if(idx < parent->n){
      mergeChildrenShift(parent, idx, order);
    }else{
      mergeChildrenShift(parent, idx - 1, order);
    }
  }
}

int shouldMerge(BTNode *parent, int idx, int minKeys){
  BTNode *targetChild = parent->child[idx];
  //Rule 1: Target child is at minimum capacity
  if(targetChild->n > minKeys) return 0;
  //Rule 2: Check left sibling (if exist)
  if(idx > 0){
    BTNode *leftSibling = parent->child[idx - 1];
    if(leftSibling->n > minKeys) return 0;
  }
  //Rule 3: Check right sibling (if exist)
  if(idx < parent->n){
    BTNode *rightSibling = parent->child[idx+1];
    if(rightSibling->n > minKeys) return 0;
  }
  return 1;
}

void removeFromLeaf(BTNode *root, int idx){
  if(root == NULL) return;
  shiftNode(root, idx);
}

void handleInternalNodeDeletion(BTNode *node, int idx, int order, int minKeys){
  //case 1: Predecessor exists
  if(node->child[idx]->n >= minKeys){
    int pred = getPredecessor(node->child[idx]);
    node->keys[idx] = pred;
    deleteFromNode(node->child[idx], pred, order, minKeys);
  }else if(node->child[idx+1]->n >= minKeys){
    //case 2: Successor exists
    int succ = getSuccessor(node->child[idx+1]);
    node->keys[idx] = succ;
    deleteFromNode(node->child[idx+1], succ, order, minKeys);
  }else{
    //Case 3: Merge needed
    mergeChildrenShift(node, idx, order);
    deleteFromNode(node->child[idx], node->keys[idx], order, minKeys);
  }
}

void deleteFromNode(BTNode *root, int key, int order, int minKeys){
  int idx = 0;
  while(idx < root->n && key > root->keys[idx])
    idx++;

  if(idx < root->n && root->keys[idx] == key){
    //key found in this node
    if(root->leaf){
      removeFromLeaf(root, idx);
    }else{
      handleInternalNodeDeletion(root, idx, order, minKeys);
    }
  }else{
    if (root->leaf) {
      printf("Key %d not found.\n", key);
      return;
    }

    // Flag to track if we're going to the last child
    int lastChild = (idx == root->n);
        
    // Ensure the child has enough keys before descending
    if (root->child[idx]->n == minKeys) {
      fillChild(root, idx, order, minKeys);
            
      // After fillChild, the tree structure may have changed
      // The key we're looking for might now be in a different child
      // OR might have moved up to the current node
            
      // Re-search for the key in the current node
      idx = 0;
      while (idx < root->n && key > root->keys[idx]) {
	idx++;
      }
            
      // Check if the key moved up to the current node during fillChild
      if (idx < root->n && root->keys[idx] == key) {
	// Key is now in this node - delete it here
	if (root->leaf) {
	  removeFromLeaf(root, idx);
	} else {
	  handleInternalNodeDeletion(root, idx, order, minKeys);
	}
	return;
      }
    }
    // If we're here, the key is still in the subtree of child[idx]
    // But after fillChild, idx might point to a different child
    deleteFromNode(root->child[idx], key, order, minKeys);
  }
}

void delete(BTNode **root, int key, int order, int minKeys){
  if(*root == NULL) return;

  deleteFromNode(*root, key, order, minKeys);

  if((*root)->n == 0){
    BTNode *oldRoot = *root;
    *root = (*root)->leaf ? NULL : (*root)->child[0];
    free(oldRoot->keys);
    free(oldRoot->child);
    free(oldRoot);
  }
}

// In-order traversal
void traverse(BTNode *node) {
  if (!node) return;

  for (int i = 0; i < node->n; i++) {
    // Traverse the left child first
    if (!node->leaf)
      traverse(node->child[i]);

    // Print current key
    printf("%d ", node->keys[i]);
  }

  // Traverse the last child
  if (!node->leaf)
    traverse(node->child[node->n]);
}

void freeTree(BTNode *node) {
    if (!node) return;

    // First free all children recursively
    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++) {
            freeTree(node->child[i]);
        }
    }

    // Free arrays
    free(node->keys);
    free(node->child);

    // Free the node itself
    free(node);
}

int findMinKey(int order){
  //order in minimum degree of tree
  return order - 1;
}

/* Print indentation helper */
static void print_indent(int level) {
    for (int i = 0; i < level; ++i) putchar(' ');
}

void btree_print_and_count(BTNode *node, int level, int *leafCount, int *internalCount) {
  if (!node) return;

  print_indent(level);
  printf("=== [level=%d] %s | n=%d | keys: ", level/2, node->leaf ? "LEAF" : "INTERNAL", node->n);

  for (int i = 0; i < node->n; ++i) {
    printf("%d", node->keys[i]);
    if (i + 1 < node->n) printf(", ");
  }
  printf("\n");

  if (node->leaf) {
    if (leafCount) (*leafCount)++;
  } else {
    if (internalCount) (*internalCount)++;
    /* print children with increased indent */
    for (int i = 0; i <= node->n; ++i) {
      if (node->child[i]) {
	btree_print_and_count(node->child[i], level + 2, leafCount, internalCount);
      } else {
	/* Optionally show empty child slots */
	print_indent(level + 2);
	printf("`== [empty child slot %d]\n", i);
      }
    }
  }
}

int main(){

  BTNode *bnode = NULL;
  int order = 3;
  insert(&bnode, 10, order);
  insert(&bnode, 20, order);
  insert(&bnode, 5, order);
  insert(&bnode, 6, order);
  insert(&bnode, 15, order);

  int minKey = findMinKey(order);
  int leafCount = 0, internalCount = 0;

  traverse(bnode);
  printf("\n");

  printf("B-Tree structure (indent shows depth):\n");
  btree_print_and_count(bnode, 0, &leafCount, &internalCount);

  printf("\nTotals: leaves = %d, internal nodes = %d\n", leafCount, internalCount);

  delete(&bnode, 10, order, minKey);
  traverse(bnode);
  printf("\n");

  leafCount = 0, internalCount = 0;
  printf("B-Tree structure (indent shows depth):\n");
  btree_print_and_count(bnode, 0, &leafCount, &internalCount);

  printf("\nTotals: leaves = %d, internal nodes = %d\n", leafCount, internalCount);
  
  freeTree(bnode);
  
  return 0;
}
