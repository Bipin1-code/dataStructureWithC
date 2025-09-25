//B+Tree finally

/* TO DO:
   Some DBs use slot directories: 
   Leaf has a byte array buffer (like 4 KB page).
   At the end, a slot table stores offsets for each key's record.
   This allows variable-length records without using extra pointers, but implementation is more complex.
*/

#include <stdio.h>
#include <stdlib.h>

#define T 2 //just experiment value for now
#define MaxKey (2*T - 1)
#define MinKey (T-1)
#define MaxCP (2*T)

typedef enum {INTERNAL, LEAF} NodeType;

typedef struct Node{
  NodeType type;//enum 
  int keys[MaxKey];
  int n;

  union{
    //internal node fields
    struct{
      struct Node *cPs[MaxCP]; //for child pointers
    };
    //leaf node fields
    struct{
      void *records[MaxKey]; //record pointers
      struct Node *next; //leaf chaining->acts as linkedList
    };
  };
} Node;

//Constructors for Node
Node* createNode(NodeType type){
  Node *node = (Node*)malloc(sizeof(Node)); //dynamic memory allocation in heap
  node->type = type;
  node->n = 0;

  if(type == LEAF){
    node->next = NULL;
    for(int i = 0; i < MaxKey; i++){
      node->records[i] = NULL;
    }
  }else{
    for(int i = 0; i < MaxCP; i++){
      node->cPs[i] = NULL;
    }
  }

  return node;
}

//Helper function
void splitChild(Node *parent, int idx){
  Node *fullChild = parent->cPs[0];
  Node *newChild = createNode(fullChild->type);

  int mid = T-1; //seperate index

  newChild->n = fullChild->n - mid;
  //Copy the keys to new Child from fullChild
  for(int i = 0; i < newChild->n; i++){
    newChild->keys[i] = fullChild->keys[i+mid];
  }
  
  if(fullChild->type == INTERNAL){
    for(int i = 0; i <= newChild->n; i++){
      newChild->cPs[i] = fullChild->cPs[i+mid+1];
    }
  }

  //in case of leaf we need to copy records to newChild
  if(fullChild->type == LEAF){
    for(int i = 0; i < newChild->n; i++){
      newChild->records[i] = fullChild->records[i+mid];
    }
  }
  
  fullChild->n = mid; //adjust the new n for fullChild

  //shift the keys and children for to make a room to new duplicate key
  for(int i = parent->n; i > idx; i--){
      parent->keys[i] = parent->keys[i-1];
      parent->cPs[i+1] = parent->cPs[i];  
  }
  parent->keys[idx] = fullChild->keys[mid];  //insert duplicate key
  parent->cPs[idx+1] = newChild; //insert the child also
  parent->n++; //update the n for parents

  newChild->next = fullChild->next;
  fullChild->next = newChild;

}

void insertNonFull(Node *node, int key){
  int i = node->n-1;
  if(node->type == LEAF){
    while(i >= 0 && node->keys[i] > key){
      node->keys[i+1] = node->keys[i];
      node->records[i+1] = node->records[i];
      //no need to concern about child because we are on leaf
      i--;
    }
    node->keys[i+1] = key;
    node->n++;
    return;
  }else{
    //internal case
    //traverse over node to find the appropriate
    int j = 0;
    while(j < node->n && key > node->keys[j]){
      j++;
    }
    if(node->cPs[j]->n == MaxKey){
      splitChild(node, j);
      if(key > node->keys[j]){
	j++;
      }
    }
    insertNonFull(node->cPs[j], key); 
  } 
}

//insert function comes below this line
/* My general idea is to insert the key into appropriate place
  later make a function for to insert record inside key*/
void insertKey(Node **node, int key){
  if(*node == NULL){
    *node = createNode(LEAF);
    (*node)->keys[0] = key;
    (*node)->n = 1;
    return;
  }
  //case 1: node is leaf 
  if((*node)->n == MaxKey){
    Node *newRoot = createNode(INTERNAL);
    newRoot->cPs[0] = *node;
    splitChild(newRoot, 0);
    *node = newRoot;//update root  	
  }
  insertNonFull(*node, key);
}

//delete function comes below this line

//search function comes below this line

//update function comes below this line



//free tree so no memory leak...
void freeTree(Node *node){
  if(node == NULL) return;
  
  if(node->type == LEAF){
    Node *current = node;
    Node *nextNode;
    while(current != NULL){
      nextNode = current->next;
      for(int i = 0; i < current->n; i++){
	free(current->records[i]);
      }
      free(current);
      current = nextNode;
    }
    return;
  }else if(node->type == INTERNAL){
    for(int i = 0; i <= node->n; i++){
      freeTree(node->cPs[i]);
    }
  }
  free(node);
}

int main(){
  Node *root = createNode(LEAF);

  /* //insert keys manually for testing  */
  /* root->keys[0] = 5; */
  /* root->records[0] = "Record A";  //do not free this because its a string literal and records[0] is point to that address which presents in read-only memo */
  /* root->n++; */

  /* root->keys[1] = 6; */
  /* root->records[1] = "Record B"; */
  /* root->n++; */
  
  /* printf("Leaf node has %d keys: %d, %d\n", root->n, root->keys[0], root->keys[1]); */
  /* printf("Records: %s, %s\n", (char*)root->records[0], (char*)root->records[1]); */

  insertKey(&root, 20);
  insertKey(&root, 30);
  insertKey(&root, 10);
   
  freeTree(root);
  
  return 0;
}
