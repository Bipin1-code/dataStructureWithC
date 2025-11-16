
#include "bptree.h"

#define ISC 5

typedef struct{
  struct Node *node; //parent node of a child node
  int keyIndex; //index of child's Pointer on Parent array
  int childIndex;
} PathEntry;

typedef struct{
  PathEntry *entries; //the array of entries where actual data stored
  int capacity;
  int size; //number of entries
} PathStack;

PathStack* createStack(){
  PathStack *path = (PathStack*)malloc(sizeof(PathStack));
  path->capacity = ISC;
  path->size = 0;
  path->entries = (PathEntry*)malloc(path->capacity *sizeof(PathEntry));
  if(!path->entries){
    perror("Stack creation failed.\n");
    exit(1);
  }
  return path;
}

void push(Node *node, int idx, int pidx, PathStack *path){
  if(path->size >= path->capacity){
    path->capacity *= 2;
    PathEntry *temp = (PathEntry*)realloc(path->entries, path->capacity * sizeof(PathEntry));
    if(temp == NULL){
      perror("Stack memory reallocation failed.\n");
      exit(1);
    }
    path->entries = temp;
  }
  path->entries[path->size].node = node;
  path->entries[path->size].keyIndex = idx;
  path->entries[path->size].childIndex = pidx;
  path->size++;
}

PathEntry pop(PathStack *path){
  if(path->size <= 0){
    PathEntry empty = {NULL, -1, -1};
    return empty;
  }
  return path->entries[--path->size];
}

PathEntry* peek(PathStack *path){
  if(path->size <= 0){
    return NULL;
  }
  return &path->entries[path->size - 1];
}

void freeStack(PathStack *stack){
  free(stack->entries);
  free(stack);
}

int findKeyIndex(Node *node, int key){
  for(int i = 0; i < node->n; i++){
    if(key == node->keys[i])
      return i;
  }
  return -1;
}

void borrowFromLeftSibling(Node *leftSib, Node *child, Node *parent, int cPsIdx){

  //first shift child to make space for new key
  for(int i = child->n - 1; i >= 0; i--){
      child->keys[i+1] = child->keys[i];
      child->records[i+1] = child->records[i];
  }
  //due to duplicate copy n increase (although unique keys are still same as before shift)
  child->n++;
  
  //now dulpicate been replace by this donation & keys n remain same as after shift
  child->keys[0] = leftSib->keys[leftSib->n - 1];
  child->records[0] = leftSib->records[leftSib->n - 1];

  leftSib->n--;
  
  //Now we have update parent also
  parent->keys[cPsIdx-1] = child->keys[0]; //child's first key is same as parent key or vice-versa
}

void borrowFromRightSibling(Node *rightSib, Node *child, Node *parent, int cPsIdx){

  //increase the child's keys number for to make it greater than minkey

  int borrowedKey = rightSib->keys[0]; //save it before shifting (debugging)
  
  child->keys[child->n] = borrowedKey;
  child->records[child->n] = rightSib->records[0];


  //Right sibling donate it's first key to left now we need to rearrange it
  //shift all keys and records to 0 (leftside);
  for(int i = 1; i < rightSib->n; i++){
    rightSib->keys[i-1] = rightSib->keys[i];
    rightSib->records[i-1] = rightSib->records[i];
  }

  //now we need to update parent to maintain B+tree definition:
  //leaf's parent first key is always equal to right child 1st key
  parent->keys[cPsIdx] = rightSib->keys[0];
  
  rightSib->n--;
  child->n++;
 
}

void borrowFromLeftInternal(Node *parent, int parentIdx,  Node *underflowNode, Node *leftSib){
  //parentIdx is the index of underflowNode in parent's child pointer
  //leftSib is the sibling of underflowNode

  //make space in underflowNode for new key and child
  for(int i = underflowNode->n; i > 0; i--){
    underflowNode->keys[i] = underflowNode->keys[i-1];
  }
  for(int i = underflowNode->n + 1; i > 0; i--){
    underflowNode->cPs[i] = underflowNode->cPs[i - 1];
  }
 
  //Bring down the seprator key from parent
  underflowNode->keys[0] = parent->keys[parentIdx - 1];
  underflowNode->n++;

  //take the last child from left sibling
  underflowNode->cPs[0] = leftSib->cPs[leftSib->n];

  //Move the last key from left sibling to parent
  parent->keys[parentIdx - 1] = leftSib->keys[leftSib->n - 1];

  //update left Sibling
  leftSib->n--;
}

void borrowFromRightInternal(Node *parent, int parentIdx, Node *underflowNode, Node *rightSib){
  //parentIdx is the Index of underflowNode in parent's child pointer
  //righSib is the right sibling of underflowNode

  //bring down the separator key from parent
  underflowNode->keys[underflowNode->n]= parent->keys[parentIdx];
  underflowNode->n++;

  //Take first child from right sibling
  underflowNode->cPs[underflowNode->n] = rightSib->cPs[0];

  //move the first key from right sibling to parent
  parent->keys[parentIdx] = rightSib->keys[0];

  //shift keys and children in right sibling;
  for(int i = 0; i < rightSib->n; i++){
    rightSib->keys[i] = rightSib->keys[i+1];
  }
  for(int i =0; i <= rightSib->n; i++){
    rightSib->cPs[i] = rightSib->cPs[i+1];
  }

  rightSib->n--;
  
}

void mergeInternalNodes(Node *parent, int parentIdx, Node *leftNode, Node * rightNode){

  //Bring  down the separator key from parent
  leftNode->keys[leftNode->n] = parent->keys[parentIdx];
  leftNode->n++;

  //copy all keys from rightNode
  for(int i = 0; i < rightNode->n; i++){
    leftNode->keys[leftNode->n + i] = rightNode->keys[i];
  }
  //copy all children fromrightNode
  for(int i = 0; i <= rightNode->n; i++){
    leftNode->cPs[leftNode->n+i] = rightNode->cPs[i];
  }

  leftNode->n += rightNode->n;

  //Remove the separator key and child pointer from parent
  for(int i = parentIdx; i < parent->n - 1; i++){
    parent->keys[i] = parent->keys[i+1];
    parent->cPs[i+1] = parent->cPs[i+2];
  }
  parent->n--;
  
  free(rightNode);
}

void handleInternalUnderflow(Node *parent, int underflowChildIdx, PathStack *p, Node **root){
  
  Node *underflowNode = parent->cPs[underflowChildIdx];

  if(underflowNode->n >= MinKey){
    return;
  }

    //try borrowing from right sibling
  if(underflowChildIdx < parent->n){
    Node *rightSib = parent->cPs[underflowChildIdx + 1];
    if(rightSib->n > MinKey){
      borrowFromRightInternal(parent, underflowChildIdx, underflowNode, rightSib);
      return;
    }
  }
  
  //try to borrowing from left sibling first
  if(underflowChildIdx > 0){
    Node *leftSib = parent->cPs[underflowChildIdx - 1];
    if(leftSib->n > MinKey){
      borrowFromLeftInternal(parent, underflowChildIdx, underflowNode, leftSib);
      return;
    }
  }

  //if borrowing is not possible, merge with sibling
  if(underflowChildIdx > 0){
    Node *leftSib = parent->cPs[underflowChildIdx - 1];
    mergeInternalNodes(parent, underflowChildIdx - 1, leftSib, underflowNode);

    //update parent's child pointer
    parent->cPs[underflowChildIdx - 1] = leftSib;
    parent->cPs[underflowChildIdx] = NULL; //this slot will be shifted later
  }
  else if(underflowChildIdx < parent->n){
    //merge with right sibling
    Node *rightSib = parent->cPs[underflowChildIdx + 1];
    mergeInternalNodes(parent, underflowChildIdx, underflowNode, rightSib);
  }

  //Check if parent became underflowed
  if(parent->n < MinKey){
    if(parent == *root){
      if(parent->n == 0 && parent->type == INTERNAL){
	Node *newRoot = parent->cPs[0];
	free(parent);
	*root = newRoot;
      }
    }else{
      if(p->size > 0){
	PathEntry grandParentInfo = pop(p);
	handleInternalUnderflow(grandParentInfo.node, grandParentInfo.keyIndex, p, root);
      }
    }
  }
}

//Merge leafNode with left
void mergeLeafNodes(Node *leftSib, Node *child, Node *parent, int cPsIdx, PathStack *p, Node **root){
    // Added p and root parameters for recursive handling
    
    int leftSize = leftSib->n;

    // Copy all child keys and records to leftSib
    for(int i = 0; i < child->n; i++) {
        leftSib->keys[leftSize + i] = child->keys[i];
        leftSib->records[leftSize + i] = child->records[i];
    }
    leftSib->n = leftSize + child->n;

    // Update leaf chain
    leftSib->next = child->next;

    // Free child resources
    for(int i = 0; i < child->n; i++) {
        free(child->records[i]);
    }
    free(child);

    // Remove the separator key and child pointer from parent
    for(int i = cPsIdx; i < parent->n - 1; i++) {
      parent->keys[i] = parent->keys[i + 1];
      parent->cPs[i + 1] = parent->cPs[i + 2];
    }
    parent->n--;

    // Check if parent became underflowed
    if(parent->n < MinKey) {
        if(parent == *root) {
            // Parent is root - handle root underflow
            if(parent->n == 0 && parent->type == INTERNAL) {
                Node *newRoot = parent->cPs[0];
                free(parent);
                *root = newRoot;
            }
        } else {
            // Parent is not root - need to handle recursively
            // Get grandparent from stack
            if(p->size > 0) {
                PathEntry grandParentInfo = pop(p);
                handleInternalUnderflow(grandParentInfo.node, grandParentInfo.childIndex, p, root);
            }
        }
    }
}

// Similarly for with right merge:
void mergeLeafNodesRight(Node *child, Node *rightSib, Node *parent, int cPsIdx, PathStack *p, Node **root){
  int childSize = child->n;
    
  // Copy all keys from right sibling to child
  for(int i = 0; i < rightSib->n; i++){
    child->keys[childSize + i] = rightSib->keys[i];
    child->records[childSize + i] = rightSib->records[i];
  }
  child->n = childSize + rightSib->n;
    
  // Update leaf chain
  child->next = rightSib->next;
    
  // Free right sibling resources
  for(int i = 0; i < rightSib->n; i++){
    free(rightSib->records[i]);
  }
  free(rightSib);

  // Remove the separator key from parent
  for(int i = cPsIdx; i < parent->n - 1; i++){
    parent->keys[i] = parent->keys[i + 1];
    parent->cPs[i + 1] = parent->cPs[i + 2];
  }
  parent->n--;
  
  // Check if parent became underflowed (same logic as above)
  if(parent->n < MinKey){
    if(parent == *root){
      if(parent->n == 0 && parent->type == INTERNAL){
	Node *newRoot = parent->cPs[0];
	free(parent);
	*root = newRoot;
      }
    }else{
      if(p->size > 0){
	PathEntry grandParentInfo = pop(p);
	handleInternalUnderflow(grandParentInfo.node, grandParentInfo.childIndex, p, root);
      }
    }
  }
}


int deleteFromLeaf(Node *parent, int cPsIdx, int key, PathStack *p, Node **root){

  printf("We are inside  deleteFrom leaf and cPsIndex: %d\n", cPsIdx);
   
  //cPsIdx is child Pointer Index
  Node* child = parent->cPs[cPsIdx]; //child may contains key
  
  int keyIndex = findKeyIndex(child, key);
  printf("KeyIndex : %d\n",keyIndex);
  if(keyIndex == -1) return -1; //this is only condition of fail below all true
  
  if(child->n > MinKey){
    //cPs Index = 0, in this case we don't have to update parent, if keys > minKey
    for(int i= keyIndex; i < child->n - 1; i++){
      child->keys[i] = child->keys[i+1];
      child->records[i] = child->records[i+1];
    }
    child->n--;
    return 1;
  }

  if(cPsIdx > 0  && parent->cPs[cPsIdx-1]->n > MinKey){
    Node *leftSib = parent->cPs[cPsIdx-1];
    borrowFromLeftSibling(leftSib, child, parent, cPsIdx);

    keyIndex = findKeyIndex(child, key);

    for(int i = keyIndex; i < child->n - 1; i++){
      child->keys[i] = child->keys[i+1];
      child->records[i] = child->records[i+1];
    }
    child->n--;
    return 1;
  }
  else if(cPsIdx < parent->n && parent->cPs[cPsIdx+1]->n > MinKey){
    Node *rightSib = parent->cPs[cPsIdx+1];
    borrowFromRightSibling(rightSib, child, parent, cPsIdx);
    
    keyIndex = findKeyIndex(child, key);
    //Now we can delete key
    for(int i = keyIndex; i < child->n - 1; i++){
      child->keys[i] = child->keys[i+1];
      child->records[i] = child->records[i+1];
    }
    child->n--;
    return 1;
  }
  else{
    //Merge with Sibling
    if(cPsIdx > 0){
      Node *leftSib = parent->cPs[cPsIdx-1];
      printf("%d", leftSib->keys[0]);
      mergeLeafNodes(leftSib, child, parent, key, cPsIdx - 1, p, root);
      printf("child after merge : %d and leftSib : %d\n", child->keys[0], leftSib->keys[0]);
      deleteFromLeaf(parent, cPsIdx, key, p, root);
    }
    else{
      Node *rightSib = parent->cPs[cPsIdx+1];
      printf("RightSib: %d\n", rightSib->keys[0]);
      mergeLeafNodesRight(child, rightSib, parent, key, cPsIdx, p, root);
      printf("child after merge : %d and rightSib : %d\n", child->keys[0], rightSib->keys[0]);
      
      deleteFromLeaf(parent, cPsIdx, key, p, root);
    }
    return 1;
  }
}


int deleteFromInternal(Node *node, int key, PathStack *p, Node **root){
  if(node == NULL) return -1; //for safe concern although we have checked it before

  int i = 0;
  while(i < node->n && key >= node->keys[i]){
    i++;
  }
  int keyIndex = i;
  Node *child = NULL;
  
  if( i > 0 && i <= node->n && key == node->keys[i-1]){
    keyIndex = i-1;
    push(node, keyIndex, i, p);
    child = node->cPs[i];
  }else{
    push(node, keyIndex, keyIndex, p);
    child = node->cPs[keyIndex];
  }

  if (child == NULL) {
    return -1;
  }
 
  int result = 0;

  if(child->type == LEAF){
    PathEntry parentInfo = pop(p);
    int originalFirstKey = child->keys[0];
    printf("originalFirstKey: %d", originalFirstKey);
    result = deleteFromLeaf(parentInfo.node, parentInfo.childIndex, key, p, root);
    printf("childkey: %d\n", child->keys[0]);

  }else{
    //continue searching in internal node
    result = deleteFromInternal(child, key, p, root);

    if(result == 1 && keyIndex < node->n && node->keys[keyIndex] == key){

      Node* rightSubtree = node->cPs[keyIndex+1];
      Node* leftmostLeaf = rightSubtree;
      while(leftmostLeaf->type == INTERNAL){
	leftmostLeaf = leftmostLeaf->cPs[0];
      }
      if(leftmostLeaf->n > 0){
	node->keys[keyIndex] = leftmostLeaf->keys[0];
      }
    }
    
    // After deletion from child, check if child became underflowed
    if(result == 1 && child->type == INTERNAL && child->n < MinKey) {
      PathEntry *parentInfo = peek(p);
      if(parentInfo != NULL && parentInfo->node != NULL){
	handleInternalUnderflow(parentInfo->node, parentInfo->childIndex, p, root);
      }
    }
  }
  return result;
}

void deleteKey(Node **root, int key){
  if(root == NULL){
    printf("There is no B+tree exist.\n");
    return;
  }
  //initial root is leaf then we can delete key from node
  if((*root)->type == LEAF){
    int i = 0;
    while((*root)->n > i && key > (*root)->keys[i]){
      i++;
    }
    if((*root)->n > i && key == (*root)->keys[i]){
      while(i < (*root)->n-1 ){
	(*root)->keys[i] = (*root)->keys[i+1];
	(*root)->records[i] = (*root)->records[i+1];
	i++;
      }
      (*root)->n--;
      printf("%d is deleted successfully.\n", key);
      
      if((*root)->n == 0){
	free(*root);
	*root = NULL;
      }
    }else{
      printf("%d is not present in tree.\n", key);
    }
  }else{
    //for Internal case
    //stack to keep track the parent node and index of key
    PathStack *p = createStack();
    //we need to move down to leaf node which contains key
    if(deleteFromInternal(*root, key, p, root) == 1){
      printf("%d is deleted successfully.\n", key);
      
      //maybe root may change for to be safe update root
      if((*root)->n == 0 && (*root)->type == INTERNAL){
	Node *newRoot = (*root)->cPs[0];
	free(*root);
	*root = newRoot;
      }
    }else{
      printf("%d is not present in tree.\n", key);
    }
    freeStack(p);
  }
}

int main(){

  Node *bpt = NULL;
  //all passed
  insertKey(&bpt, 40);
  insertKey(&bpt, 20);
  insertKey(&bpt, 60);
  insertKey(&bpt, 10);
  insertKey(&bpt, 30);
  insertKey(&bpt, 50);
  insertKey(&bpt, 70);

  // Insert to force multiple splits  //all passed
  /* insertKey(&bpt, 5); */
  /* insertKey(&bpt, 15); */
  /* insertKey(&bpt, 25); */
  /* insertKey(&bpt, 35); */
  /* insertKey(&bpt, 45); */
  /* insertKey(&bpt, 55); */
  /* insertKey(&bpt, 65); */
  /* insertKey(&bpt, 75); */
  
  displayBPlusTree(bpt, 0);
  displayLeafChain(bpt);

  deleteKey(&bpt, 40);  // Delete root
  deleteKey(&bpt, 20); //Cause merge
  insertKey(&bpt, 45);  //Insert after delete
  deleteKey(&bpt, 60);  // Complex redistribution

  /* // Delete to test underflow handling  //all passed */
  /* deleteKey(&bpt, 5);  */
  /* deleteKey(&bpt, 15);  */
  /* deleteKey(&bpt, 25);  */
  /* deleteKey(&bpt, 35); */
  
  
  displayBPlusTree(bpt, 0);
  displayLeafChain(bpt);
  
  freeTree(bpt);
  
  return 0;
}


