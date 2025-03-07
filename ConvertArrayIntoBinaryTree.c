
///In this program we are converting a array into binary tree structure

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
  int data;
  struct Node *nextL;
  struct Node *nextR;
} Node;

Node* createNode(int value){
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->data = value;
  newNode->nextL = NULL;
  newNode->nextR = NULL;
  return newNode;
}

Node* insert(Node* root, int value){
  if(root == NULL){
    return createNode(value);
  }
  if(value < root->data){
    root->nextL = insert(root->nextL, value);
  }else{
    root->nextR = insert(root->nextR, value);
  }
  return root;
}

void printArray(int n[], size_t s){
  for(size_t i = 0; i < s; i++){
    printf("%d ", n[i]);
  }
  printf("\n");
}

void inorderPrint(Node* root){
  if(root != NULL){
    inorderPrint(root->nextL);
    printf("%d ", root->data);
    inorderPrint(root->nextR);
  }
}

void freeTree(Node* root){
  if(root != NULL){
    freeTree(root->nextL);
    freeTree(root->nextR);
    free(root);
  }
}

void fillArray(int arr[10], size_t s){
  printf("Enter 10 numbers for to fill the array:\n");
  fflush(stdout);
  for(size_t i = 0; i < s; i++){
    scanf("%d", &arr[i]);
  }
}

int main(){
  int numbers[10]; //we declare the array and its size;
  fillArray(numbers, 10);
  printArray(numbers, 10);
  Node* root = NULL;
  for(size_t i = 0; i < 10; i++){
    root = insert(root, numbers[i]);
  }
  printf("Inorder Traversal: \n");
  inorderPrint(root);
  printf("\n");

  freeTree(root);
  return 0;
}
