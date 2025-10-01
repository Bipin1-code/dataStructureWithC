
//t = minimum degree of  tree;
//n = 2t - 1; max number of data in a node
//minkey = t-1

#include <stdio.h>
#include <stdlib.h>

#define T 2 //For to find optimal t you should know Block_Size
/*
  Block_Size = overhead + [(2T-1) * sizeof(dataType of key(data)) + [(2T) * sizeof(dataType Of child pointer)]
  What is overhead?
  Overhead refers to the extra storage space used for metadata and management information rather than for the actual data itself.

  overhead of Node in our case is:
  8+8+4+4 = 24 bytes

  In case of T = 2,
  Blocksize = 68 bytes
*/

typedef struct Node{
  int *data;    //8 bytes
  struct Node **ptC; //8 bytes
  int n;  //4bytes
  int leaf; //4bytes
} Node;

Node* createNode(int leaf){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = (int*)calloc((2 * T - 1), sizeof(int));
  newNode->ptC = (Node**)calloc(2 * T, sizeof(Node*));
  newNode->n = 0;
  newNode->leaf = leaf;
  return newNode;
}

void splitNode(Node *parent, int idx){
  //Initial root present inside newRoot(parent)'s index 0
  Node *fullChild = parent->ptC[0];
  Node *newChild = createNode(fullChild->leaf); //for to send half data of fullChild here
  int midData = T-1;
  
  newChild->n = fullChild->n - midData - 1; //1 is data which Goes up to parent node

  //copy fullChild mid+ data on newChild
  for(size_t i = 0; i < newChild->n; i++){
    newChild->data[i] = fullChild->data[i + midData + 1];
  }

  //if fullChild is not leaf we have to copy child pointers also
  if(!fullChild->leaf){
    for(size_t i = 0; i <= newChild->n; i++){
      newChild->ptC[i] = fullChild->ptC[i + midData + 1];
    }
  }

  //update size of data in fullChild
  fullChild->n = midData;

  //Shift parent data and pointer to create space for new data
  for(int i = parent->n; i > idx; i--){
    parent->data[i] = parent->data[i-1];
    parent->ptC[i+1] = parent->ptC[i];
  }

  parent->data[idx] = fullChild->data[midData];
  parent->ptC[idx+1] = newChild;
  parent->n++;
}

void insertInNonFull(Node *node, int data){
  int i = node->n - 1;
  if(node->leaf){
    //shift keys until u find a hole to insert
    while(i >= 0 && data < node->data[i]){
      node->data[i+1] = node->data[i];
      i--;
    }
    node->data[i+1] = data;
    node->n++;
  }else{
    //internal node case;
    int j = 0;
    while(j < node->n && data > node->data[j]){
      j++;
    }
    if(node->ptC[j]->n == (2 * T - 1)){

      splitNode(node, j);

      if(data > node->data[j]){
	j++;
      } 
    }
    insertInNonFull(node->ptC[j], data);
  }
}

void insert(Node **node, int data){
  if(*node == NULL){
    *node = createNode(1);
    (*node)->data[0] = data;
    (*node)->n++;
    return;
  }
  //isNodeFull
  if((*node)->n == (2*T - 1)){
    //Yes, it is now we have no space to fill new data
    Node *newRoot = createNode(0);//We need to split so tree gets' new root
    newRoot->ptC[0] = *node;
    splitNode(newRoot, 0);
    *node = newRoot; //update root
  }
  //if node is not full,
  insertInNonFull(*node, data);
}

int getPredecessor(Node *node){
  Node *current = node;
  while(!current->leaf){
    current = current->ptC[current->n];
  }
  return current->data[current->n - 1];
}

int getSuccessor(Node *node){
  Node *current = node;
  while(!current->leaf){
    current = current->ptC[0];
  }
  return current->data[0];
}

void mergeKeyRightLeftChild(int idx, Node *parent){

  Node *RChild = parent->ptC[idx+1];
  Node *LChild = parent->ptC[idx];
  
  LChild->data[T-1] = parent->data[idx];

  for(int i = 0; i < RChild->n; i++){
    LChild->data[T + i] = RChild->data[i];
  }

  if(!LChild->leaf){
    for(int i = 0; i <= RChild->n; i++){
      LChild->ptC[T + i] = RChild->ptC[i];
    }
  }

  for(int i = idx+1; i < parent->n; i++){
    parent->data[i-1] = parent->data[i];
  }
  for(int i = idx+2; i <= parent->n; i++){
    parent->ptC[i-1] = parent->ptC[i];
  }

  LChild->n += RChild->n + 1;
  parent->n--;

  free(RChild->data);
  free(RChild->ptC);
  free(RChild);
}

void borrowFromLeft(Node *parent, int idx){
  Node *child = parent->ptC[idx];
  Node *leftSibling = parent->ptC[idx-1];

  //shift child's data to make space
  for(int i = child->n -1; i >= 0; i--){
    child->data[i+1] = child->data[i];
  }

  //shift child pointers if not leaf
  if(!child->leaf){
    for(int i = child->n; i >= 0; i--){
      child->ptC[i+1] = child->ptC[i];
    }
  }
  child->data[0] = parent->data[idx-1];

  //Move last child from left sibling to child
  if(!child->leaf){
    child->ptC[0] = leftSibling->ptC[leftSibling->n];
  }

  //Move last key from left sibling to parent
  parent->data[idx-1] = leftSibling->data[leftSibling->n-1];

  child->n++;
  leftSibling->n--;
}

void borrowFromRight(Node *parent, int idx){
  Node *child = parent->ptC[idx];
  Node *rightSibling = parent->ptC[idx+1];

  //Move key from parent to child
  child->data[child->n] = parent->data[idx];

  //Move first child from right sibling to child
  if(!child->leaf){
    child->ptC[child->n+1] = rightSibling->ptC[0];
  }

  //Move first key from right sibling to parent
  parent->data[idx] = rightSibling->data[0];

  //Shift right sibling's data left
  for(int i = 1; i < rightSibling->n; i++){
    rightSibling->data[i-1] = rightSibling->data[i];
  }

  //Shift right sibling's children if not leaf
  if(!rightSibling->leaf){
    for(int i = 1; i <= rightSibling->n; i++){
      rightSibling->ptC[i-1] = rightSibling->ptC[i];
    }
  }
  child->n++;
  rightSibling->n--;
}

void fillChild(Node *parent, int idx){
  //Case 1: Borrow from left sibling if possible
  if(idx != 0 && parent->ptC[idx-1]->n >= T){
    borrowFromLeft(parent, idx);
  }else if(idx != parent->n && parent->ptC[idx+1]->n >= T){
    //case 2: Borrow from right sibling
    borrowFromRight(parent, idx);
  }else{
    //Case 3: Merge with sibling
    if(idx != parent->n){
      mergeKeyRightLeftChild(idx, parent);
    }else{
      mergeKeyRightLeftChild(idx-1, parent);
    }
  }
}

void deleteKey(Node **node, int data){
  if(*node == NULL) return;
  int i = 0;
  while( i < (*node)->n && data > (*node)->data[i])
    i++;

  if(i < (*node)->n && data == (*node)->data[i]){
    if((*node)->leaf){
      //leaf node case
      for(int j = i; j < (*node)->n - 1; j++){
        (*node)->data[j] = (*node)->data[j+1];
      }
      (*node)->n--;
      return;
    }else{
      //internal node case
      if((*node)->ptC[i]->n >= T){
	int pred = getPredecessor((*node)->ptC[i]);
	(*node)->data[i] = pred;
	deleteKey(&(*node)->ptC[i], pred);
      }else if((*node)->ptC[i+1]->n >= T){
	int succ = getSuccessor((*node)->ptC[i+1]);
	(*node)->data[i] = succ;
	deleteKey(&(*node)->ptC[i+1], succ);
      }else{
	//both have exact minKey (T-1) keys
	mergeKeyRightLeftChild(i, *node);
	deleteKey(&(*node)->ptC[i], data);
      }
    }
  }else{
    if((*node)->leaf){
      printf("data %d is not found in tree.\n", data);
      return;
    }
    
    int isLastChild = (i == (*node)->n);

    if((*node)->ptC[i]->n < T){
      fillChild(*node, i);
    }

    if(isLastChild)
      deleteKey(&(*node)->ptC[i - 1], data);
    else
      deleteKey(&(*node)->ptC[i], data);
    
  }

  if((*node)->n == 0){
    Node *temp = *node;
    if((*node)->leaf){
      *node = NULL;
    }else{
      *node = (*node)->ptC[0];
    }
    free(temp->data);
    free(temp->ptC);
    free(temp);
  } 
}

void traverseFormatted(Node *node, int level){
  if(!node) return;
  printf("Level %d (%s): ", level, node->leaf ? "leaf" : "internal");
  for(int i = 0; i < node->n; i++){
     printf("%d ", node->data[i]);
  }
  printf("\n");
  if(!node->leaf){
    for(int i = 0; i <= node->n; i++){
      traverseFormatted(node->ptC[i], level + 1);
    }
  }
}

void printTree(Node *root){
  traverseFormatted(root, 0);
}

void freeTree(Node *node){
  if(!node) return;
  if(!node->leaf){
    for(int i = 0; i <= node->n; i++){
      freeTree(node->ptC[i]);
    }
  }

  free(node->data);
  free(node->ptC);

  free(node);
}

int main(){
  Node *Btree = NULL;
  insert(&Btree, 5);
  insert(&Btree, 20);
  insert(&Btree, 30);
  insert(&Btree, 9);

  printf("Display tree:\n");
  printTree(Btree);
  printf("\n");
  
  insert(&Btree, 28);
  insert(&Btree, 41);

  printf("Display tree:\n");
  printTree(Btree);
  printf("\n");

  printf("Deletion\n");
  deleteKey(&Btree, 20);
  printf("Display tree:\n");
  printTree(Btree);
  printf("\n");
  
  freeTree(Btree);
  return 0;
}

