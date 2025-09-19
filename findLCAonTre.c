
#include <stdio.h>
#include <stdlib.h>

#define MIN_INT -2147483648
#define MAX_INT 2147483647

typedef struct Node{
  int data;
  struct Node *left;
  struct Node *right;
} Node;

Node* createNode(int data){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

void insertNode(Node **root, int data){
    if(*root == NULL){
      *root = createNode(data);
      return;
    }
    if((*root)->data > data){
      insertNode(&(*root)->left, data);
    }else if((*root)->data < data){
      insertNode(&(*root)->right, data);
    }else{
      printf("Duplicate value %d is ignore!\n", data);
    }
}

int searchNode(Node *root, int data){
  if(root == NULL) return 0;
  if(root->data == data) return 1;
  else{
    if(root->data > data){
      return searchNode(root->left, data);
    }else{
      return searchNode(root->right, data);
    }
  }
}

int isBSTUtil(Node *root, int min, int max){
  /*
    What's the idea:
    let root data = 10
    start at root: allowed range = (MIN_INT, MAX_INT)
    Go left (5): allowed range = (MIN_INT, root->data);
    Go right(15): allowd range = (root->data, MAX_INT);
    Go left of 15 (6): allowed range = (10, root->data(15))
    BST property violet on node (6) because range(10, 15) "6 < 10"
   */
  if(root == NULL) return 1;
  if(root->data <= min || root->data >= min)
    return 0;
  return isBSTUtil(root->left, min, root->data) &&
    isBSTUtil(root->right, root->data, max);
}

int checkIsBST(Node *root){
  return isBSTUtil(root, MIN_INT, MAX_INT);
}

void inOrdertraversal(Node *root){
  if(root == NULL) return;
  inOrdertraversal(root->left);
  printf("%d ", root->data);
  inOrdertraversal(root->right);
}

Node* findLCA(Node *root, int target1, int target2){
  if(root == NULL) return NULL;
  if(root->data > target1 && root->data > target2){
   return findLCA(root->left, target1, target2);
  }else if(root->data < target1 && root->data < target2){
   return findLCA(root->right, target1, target2);
  }else{
    return root;
  }
}

void freeTree(Node *root){
  if(root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}


int main(){
  size_t size;
  printf("Enter the number of data or node you wanna create on tree:\n");
  fflush(stdout);
  scanf("%zu", &size);
  getchar();
  int *arrOfNodedata = calloc(size, sizeof(int));
  if(!arrOfNodedata){
    printf("Memory allocation failed!\n");
    return 1;
  }
  printf("Enter the data you want in Node of tree:(each data must be unique)\n");
  for(size_t i = 0; i < size; i++){
    scanf("%d", &arrOfNodedata[i]);
  }

  Node *root = NULL;

  for(size_t i = 0; i < size; i++){
    insertNode(&root, arrOfNodedata[i]);
  }

  inOrdertraversal(root);
  printf("\n");

  if(checkIsBST(root)){
    printf("Tree is valid Binary search tree (BST).");
  }else{
    printf("Tree you just create is not a valid BST (Binary search tree.\n)");
  }

  int target1 = 0, target2 = 0;
  printf("Enter the two nodes data for to find out their LCA:\n");
  fflush(stdout);
  scanf("%d%d", &target1, &target2);
  getchar();
  if(searchNode(root, target1) && searchNode(root, target2)){
    Node *LCA = findLCA(root, target1, target2);
    printf("LCA of %d and %d is %d\n", target1, target2, LCA->data);
  }else{
    printf("%d and %d are not present in tree\n", target1, target2);
  }
  free(arrOfNodedata);
  freeTree(root);
  return 0;
}
