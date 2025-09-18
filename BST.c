
/// Let's do Binary Search Tree

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
  int data;
  struct Node *left;
  struct Node *right;
} Node;


Node* createNode(int data){
  Node *newChild = (Node*)malloc(sizeof(Node));
  newChild->data = data;
  newChild->left = NULL;
  newChild->right = NULL;
  return newChild;
}

void insertNode(Node **root, int data){
  if(*root == NULL) {
    *root = createNode(data);
    printf("%d is inserted.\n", (*root)->data);
    return;
  }
  if((*root)->data > data){
    insertNode(&(*root)->left, data);
  }else if (data > (*root)->data){
    insertNode(&(*root)->right, data);
  }else{
    printf("Duplicate value %d ignored.\n", data);
  }
}

Node* search(Node *root, int key){
  if(root == NULL || root->data == key)
    return root;
  if(key < root->data)
    return search(root->left, key);
  else
    return search(root->right, key);  
}

Node* findMin(Node *root){
  while(root->left != NULL) root = root->left;
  return root;
}

Node* findMax(Node *root){
  while(root->right != NULL) root = root->right;
  return root;
}

void deleteNode(Node **root, int data){
  if(*root == NULL) return;

  if(data < (*root)->data){
    deleteNode(&(*root)->left, data);
  }else if(data > (*root)->data){
    deleteNode(&(*root)->right, data);
  }else{
    //Node Found
    if((*root)->left == NULL && (*root)->right == NULL){
      //Case 1: leaf
      free(*root);
      *root = NULL;
    }else if((*root)->left == NULL){
      //Case 2: only right child
      Node *temp = *root;
      *root = (*root)->right;
      free(temp);
    }else if((*root)->right == NULL){
      //Case 2: only left child
      Node *temp = *root;
      *root = (*root)->left;
      free(temp);
    }else{
      //Case 3: Two children
      Node *temp = findMin((*root)->right);
      (*root)->data = temp->data;
      deleteNode(&(*root)->right, temp->data);
    }
  }
}

int height(Node *root){
  if(root == NULL) return -1; //Empyt tree
  int leftHeight = height(root->left);
  int rightHeight = height(root->right);
  return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int isBalancedOptimized(Node *root, int *height){
  if(root == NULL){
    *height = -1;
    return 1;
  }

  int leftHeight = 0 , rightHeight = 0;

  int leftBalanced = isBalancedOptimized(root->left, &leftHeight);
  int rightBalanced = isBalancedOptimized(root->right, &rightHeight);

  *height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

  if(!leftBalanced || !rightBalanced) return 0;
  if(abs(leftHeight - rightHeight) > 1) return 0;

  return 1;
}

int isBalanced(Node *root){
  int height = 0;
  return isBalancedOptimized(root, &height);
}

void inOrderTraversal(Node *root){
  if(root == NULL) return;
  inOrderTraversal(root->left);
  printf("%d ", root->data);
  inOrderTraversal(root->right);
}

void freeTree(Node *root){
  if(root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

int main(){

  Node *root = NULL;
  while(1){
    int choice, data;
    
    printf("Hello Welcome to Tree creation simulator!\n");
    printf("Choose the following menu to do action:\n");
    printf("====== 1. Insert Node on Tree =============\n");
    printf("====== 2. Delete Node from Tree ===========\n");
    printf("====== 3. For to Find Max data ============\n");
    printf("====== 4. For to Find Min data ============\n");
    printf("====== 5. For to search data ==============\n");
    printf("====== 6. For to know the height of tree ==\n");
    printf("= 7. For to check tree is balanced or not.=\n");
    printf("====== 8. See tree and  Exit ==============\n");
    fflush(stdout);
    scanf("%d", &choice);
    getchar();
    
    switch(choice){
    case 1:
      printf("Enter the data you want to insert inside tree's node.\n");
      fflush(stdout);
      scanf("%d", &data);
      getchar();
      insertNode(&root, data);
      break;
    case 2:
      printf("Enter the data you want to delete from tree.\n");
      fflush(stdout);
      scanf("%d", &data);
      getchar();
      deleteNode(&root, data);
      break;
    case 3:{
      Node *temp = findMax(root);
      if(temp) printf("Max data is %d on tree.\n", temp->data);
      break;
    }  
    case 4:{
      Node *temp = findMin(root);
      if(temp) printf("Min data is %d on tree.\n", temp->data);
      break;
    }     
    case 5:{
      printf("Enter the data you want to search on tree.\n");
      fflush(stdout);
      scanf("%d", &data);
      getchar();
      Node *temp = search(root, data);
      if(temp) printf("Your %d is found in tree.\n", data);
      else printf("%d not found.\n", data);
      break;
    }
    case 6:
      data = height(root);
      printf("Height of tree: %d\n", data);
      break;
    case 7:
      if(isBalanced(root))
        printf("Tree is balanced.\n");
      else
        printf("Tree is NOT balanced.\n");
      break;
    case 8:
      printf("Your tree have these data:\n");
      inOrderTraversal(root);
      freeTree(root);
      return 0;
    default:
      printf("Invalid choice!\n");
    }
  }
}



