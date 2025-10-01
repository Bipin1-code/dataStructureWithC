#include <stdio.h>
#include <stdlib.h>

#ifndef T
#define T 2
#endif

#define MaxKey (2 * T - 1)
#define MinKey (T - 1)
#define MaxCP (2*T)

typedef enum{LEAF, INTERNAL} NodeType;

typedef struct Node{
  NodeType type;
  int keys[MaxKey];
  int n;
  union{
    struct{
      struct Node *cPs[MaxCP];
    };
    struct{
      void* records[MaxKey];
      struct Node *next;
    };
  }; 
} Node;

Node* createNode(NodeType type){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->type = type;
  newNode->n = 0;
  if(type == LEAF){
    for(int i = 0; i < MaxKey; i++){
      newNode->records[i] = NULL;
    }
    newNode->next = NULL;
  }else{
    for(int i = 0; i < MaxCP; i++){
      newNode->cPs[i] = NULL;
    }
  }
  return newNode;
}

void displayBPlusTree(Node *root, int level){
  if(root == NULL) return;

  for(int i = 0; i < level; i++)
    printf("    ");
  
  if(root->type == LEAF){
    printf("[Leaf | keys: ");
    for(int i = 0; i < root->n; i++){
      printf("%d ", root->keys[i]);
    }
    printf("]\n");
  }else{
    printf("[Internal | keys: ");
    for(int i = 0; i < root->n; i++){
      printf("%d ", root->keys[i]);
    }
    printf("]\n");
    
    for(int i = 0; i <= root->n; i++){
      displayBPlusTree(root->cPs[i], level+1);
    }
  }
}

Node* findFirstLeaf(Node *root){
  Node *cur = root;
  while(cur->type == INTERNAL){
    if(cur->cPs[0] == NULL) return NULL;
    cur = cur->cPs[0];
  }
  return cur;
}

void displayLeafChain(Node *root){
  if(root == NULL){
    return;
  }
  printf("Leaves in order:\n");
  Node *leaf = (root->type == INTERNAL) ? findFirstLeaf(root) : root;
  
  while(leaf != NULL){
    printf("[ ");
    for(int i = 0; i < leaf->n; i++){
      printf("%d ", leaf->keys[i]);
    }
    printf("] -> ");
    leaf = leaf->next;
  }
  printf("NULL\n");
}

void freeTree(Node *root){
  if(root == NULL) return;
  if(root->type == INTERNAL){
    for(int i = 0; i <= root->n; i++){
      freeTree(root->cPs[i]);
    }
  }
  if(root->type == LEAF){
    for(int i = 0; i < root->n; i++){
      free(root->records[i]);
    }
  }
  free(root);
}

void splitNode(Node *parent, int idx){
  Node *fullChild = parent->cPs[idx];
  Node *newChild = createNode(fullChild->type);

  if(newChild->type == LEAF){
    int mid = (fullChild->n) / 2; //we can assign minKey
    newChild->n = fullChild->n - mid;
    
    for(int i = 0; i < newChild->n; i++){
      newChild->keys[i] = fullChild->keys[mid+i];
      newChild->records[i] = fullChild->records[mid+i];
    }

    fullChild->n = mid;
    
    newChild->next = fullChild->next;
    fullChild->next = newChild;

    for(int i = parent->n; i > idx; i--){
      parent->keys[i] = parent->keys[i-1];
      parent->cPs[i+1] = parent->cPs[i];
    }
  
    parent->keys[idx] = newChild->keys[0];
    parent->cPs[idx+1] = newChild;
    parent->n++;
    
  }else{
    int mid = (fullChild->n) / 2;
    
    newChild->n = fullChild->n - mid - 1;

    for(int i = 0; i < newChild->n; i++){
      newChild->keys[i] = fullChild->keys[mid+1+i];
    }
    
    for(int i = 0; i <= newChild->n; i++){
      newChild->cPs[i] = fullChild->cPs[mid+i+1];
    }

    int promotedKey = fullChild->keys[mid];
    fullChild->n = mid;

    for(int i = parent->n; i > idx; i--){
      parent->keys[i] = parent->keys[i-1];
      parent->cPs[i+1] = parent->cPs[i];
    }

    parent->keys[idx] = promotedKey;
    parent->cPs[idx+1] = newChild;
    parent->n++;
  }
}

void insertInNonFull(Node *node, int key){

  if(node->type == LEAF){
    if(node->n >= MaxKey){
      printf("SANITY CHECK FAILED: Node has %d keys (max: %d)\n", node->n, MaxKey);
      return;
    }
    int i = node->n - 1;
    while(i >= 0 && key < node->keys[i]){
      node->keys[i+1] = node->keys[i];
      node->records[i+1] = node->records[i];
      i--;
    }
    node->keys[i+1] = key;
    node->records[i+1] = malloc(sizeof(int));
    *((int*)node->records[i+1]) = key;
    node->n++;
    return;
  }else{
    //internal condition
    int i = 0;
    while(i < node->n && key >= node->keys[i]){
      i++;
    }
    if((node->cPs[i] != NULL) && (node->cPs[i]->n == MaxKey)){
      splitNode(node, i);
      if(key > node->keys[i]){
	i++;
      }
    }
    if(node->cPs[i] != NULL){
      insertInNonFull(node->cPs[i], key);
    }
  }
}

void insertKey(Node **node, int key){
  if(*node == NULL){
    *node = createNode(LEAF);
    (*node)->keys[0] = key;
    (*node)->records[0] = malloc(sizeof(int));
    *((int*)(*node)->records[0]) = key;         
    (*node)->n = 1;
    (*node)->next = NULL;
    return;
  }
  if((*node)->n == MaxKey){
    //we need to splite node
    Node *newRoot = createNode(INTERNAL);
    newRoot->cPs[0] = *node;
    splitNode(newRoot, 0); //0 index
    *node = newRoot;
  }
  insertInNonFull(*node, key);
}

