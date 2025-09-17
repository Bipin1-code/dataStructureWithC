
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

typedef struct Node{
  int data;
  struct Node *left, *right;
} Node;

typedef struct {
  Node* item[SIZE];
  int front, rear;
} Queue;

Node* createNode(int data){
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  newNode->left = newNode->right = NULL;
  return newNode;
}

Queue* createQueue(){
  Queue* q = malloc(sizeof(Queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

int isEmpty(Queue *q){
  return (q->front == -1);
}

void enqueue(Queue *q, Node* value){
  if(q->rear == SIZE - 1){
    printf("Queue is overflow!\n");
    return;
  }
  if(isEmpty(q)){
    q->front = 0;
  }
  q->item[++q->rear] = value;
}

Node* dequeue(Queue *q){
  if(isEmpty(q)){
    printf("Queue is underflow!\n");
    return NULL;
  }
  Node *item = q->item[q->front];
  if(q->front == q->rear){
    q->front = q->rear = -1;
  }else{
    q->front++;
  }
  return item;
}

void levelOrderTraversal(Node *root){
  if(root == NULL) return;

  Queue *q = createQueue();
  enqueue(q, root);

  while(!isEmpty(q)){
    Node* current = dequeue(q);
    printf("%d ", current->data);

    if(current->left != NULL){
      enqueue(q, current->left);
    }
    if(current->right != NULL){
      enqueue(q, current->right);
    }
  }
  free(q);
}

void freeTree(Node *root){
  if(root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

int main(){
  Node *root = createNode(1);
  root->left = createNode(2);
  root->right = createNode(3);
  root->left->left = createNode(4);
  root->left->right = createNode(5);
  root->right->left = createNode(6);
  root->right->right = createNode(7);

  printf("Level order Traversal: \n");
  levelOrderTraversal(root);

  freeTree(root);

  return 0;
}
