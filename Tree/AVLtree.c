
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
  int data;
  struct Node *left;
  struct Node *right;
  int height;
} Node;

Node* createNode(int data){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->height = 0;
  return newNode;
}

void preOrderTraversal(Node *root){
  if(root == NULL) return;
  printf("%d ", root->data);
  preOrderTraversal(root->left);
  preOrderTraversal(root->right);
}

int isBalancedOptm(Node *root, int *height){
  if(root == NULL){
    *height = -1;
    return 1; //tree is balance
  }
  int lfHeight = 0, rtHeight = 0;

  int leftBalanced = isBalancedOptm(root->left, &lfHeight);
  int rightBalanced = isBalancedOptm(root->right, &rtHeight);

  *height = 1 + (lfHeight > rtHeight ? lfHeight : rtHeight);

  if(!leftBalanced || !rightBalanced) return 0;
  if(abs(lfHeight - rtHeight) > 1) return 0;

  return 1;
}

int isTreeBalanced(Node *root){
  int height = 0;
  return isBalancedOptm(root, &height);
}

//Now we have height field in Node
int nodeHeight(Node *n) {
    return (n == NULL) ? -1 : n->height; 
}

void updateHeight(Node *n){
  if(n != NULL){
    n->height = 1 + (nodeHeight(n->left) > nodeHeight(n->right) ? nodeHeight(n->left) : nodeHeight(n->right));
  }
}

Node* rightRotation(Node *y){
  Node *x = y->left; 
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  updateHeight(y);
  updateHeight(x);
  return x; //new root
}

Node* leftRotation(Node *x){
  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  updateHeight(x);
  updateHeight(y);
  return y; //new root
}

int getBalanceFactor(Node *root){
  if(root == NULL) return 0;
  return nodeHeight(root->left) - nodeHeight(root->right);
}

void insertAVL(Node **root, int data){
  //1. Same as BST insert
  if(*root == NULL){
    *root = createNode(data);
    return;
  }
  if(data < (*root)->data){
    insertAVL(&(*root)->left, data);
  }else if(data > (*root)->data){
    insertAVL(&(*root)->right, data);
  }else{
    printf("Duplicate %d value is ignored.\n", data);
    return;
  }

  updateHeight(*root);
  
  int balanceFactor = getBalanceFactor(*root);

  //Now we have to deal with 4 unbalanced cases:
  //Left-left
  if((balanceFactor > 1) && (data < (*root)->left->data)){
    *root = rightRotation(*root);
  }
  
  //Right-right
  else if((balanceFactor < -1) && (data > (*root)->right->data)){
    *root = leftRotation(*root);
  }
  
  //left-Right
  else if((balanceFactor > 1) && (data > (*root)->left->data)){
    (*root)->left = leftRotation((*root)->left);
    *root = rightRotation(*root);
  }

  //right-Left
  else if((balanceFactor < -1) && (data < (*root)->right->data)){
    (*root)->right = rightRotation((*root)->right);
    *root = leftRotation(*root);
  }
}

Node* findMin(Node *root){
  while(root->left != NULL) root = root->left;
  return root;
}

void deleteAVL(Node **root, int data){
  if(*root == NULL) return;
  if(data < (*root)->data){
    deleteAVL(&(*root)->left, data);
  }else if(data > (*root)->data){
    deleteAVL(&(*root)->right, data);
  }else{
    if((*root)->left == NULL && (*root)->right == NULL){
      free(*root);
      *root = NULL;//this is the reason to check null before UpdateHeight
    }else if((*root)->left == NULL || (*root)->right == NULL){
      Node *temp = *root;
      *root = (*root)->left ? (*root)->left : (*root)->right;
      free(temp);
    }else{
      Node *temp = findMin((*root)->right);
      (*root)->data = temp->data;
      deleteAVL(&(*root)->right, temp->data);
    }
  }

  if(*root == NULL) return; //if we don't check this 
  updateHeight(*root); //*root == NULL ->crash code
  int balanceFactor = getBalanceFactor(*root);

  /*
    In deletion: we don't know which way imbalance came from,
    so we rely on child's balace factor instead of delete node
    (data). In case of insertion, we know which way the imbalance
    came from (due to inserted node(data)), so we use that info.
  */
  //LL
  if(balanceFactor > 1 && getBalanceFactor((*root)->left) >= 0){
    *root = rightRotation(*root);
  }
  //RR
  if(balanceFactor < -1 && getBalanceFactor((*root)->right) <= 0){
    *root = leftRotation(*root);
  }
  //LR
  if(balanceFactor > 1 && getBalanceFactor((*root)->left) < 0){
    (*root)->left = leftRotation((*root)->left);
    *root = rightRotation(*root);
  }
  //RL
  if(balanceFactor < -1 && getBalanceFactor((*root)->right) > 0){
    (*root)->right = rightRotation((*root)->right);
    (*root) = leftRotation(*root);
  }
}

void freeTree(Node *root){
  if(root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

int main(){

  printf("AVL:\n");

  Node *root1 = NULL;

  insertAVL(&root1, 20);
  insertAVL(&root1, 30);
  insertAVL(&root1, 45);
  insertAVL(&root1, 51);
  insertAVL(&root1, 66);
  insertAVL(&root1, 81);

  preOrderTraversal(root1);
  printf("\n");

  if(isTreeBalanced(root1)){
    printf("Tree is balanced.\n");
  }else{
    printf("Tree is not balanced.\n");  
  }

  deleteAVL(&root1, 66);
  preOrderTraversal(root1);
  printf("\n");

  if(isTreeBalanced(root1)){
    printf("Tree is balanced.\n");
  }else{
    printf("Tree is not balanced.\n");  
  }
  
  freeTree(root1);
  
  return 0;
}
