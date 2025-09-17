
#include <stdio.h>
#include <stdlib.h>

//Node of Tree
typedef struct Node{
  int data;
  struct Node *left;
  struct Node *right;
} Node;

typedef struct LNode{
  Node* item;
  struct LNode *next;
} LNode;;

typedef struct{
  LNode *front;
  LNode *rear;
} Queue;

Node* createNode(int data){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->left = newNode->right = NULL;

  return newNode;
}

Queue* createQueue(){
  Queue *q = malloc(sizeof(Queue));
  q->front = NULL;
  q->rear = NULL;

  return q;
}

void enqueue(Queue *q, Node *value){
  LNode *newLNode = (LNode*)malloc(sizeof(LNode));
  newLNode->item = value;
  newLNode->next = NULL;

  if(q->rear == NULL){
    q->front = newLNode;
    q->rear = newLNode;
  }else{
    q->rear->next = newLNode;
    q->rear = newLNode;
  }
}

Node* dequeue(Queue *q){
  if(q->front == NULL){
    printf("Queue is underflow!\n");
    return NULL;
  }

  LNode *temp = q->front;
  Node *tNode = temp->item;

  q->front = q->front->next;

  if(q->front == NULL){
    q->rear = NULL;
  }
  
  free(temp);
  return tNode;
}

Node* peek(Queue *q){
  if(q->front == NULL){
    printf("Queue is empty.\n");
    return NULL;
  }
  return q->front->item;
}

void freeQueue(Queue *q){
  LNode *current = q->front;
  LNode *next;
  while(current != NULL){
    next = current->next;
    free(current);
    current = next;
  }
  free(q);
}

void levelOrderTraversal(Node *root){
  if(root == NULL) return;

  Queue *q = createQueue();
  enqueue(q, root);
  
  while(q->front != NULL){
    Node* current = dequeue(q);
    printf("%d ", current->data);

    if(current->left != NULL) enqueue(q, current->left);
    if(current->right != NULL) enqueue(q, current->right);
  }
  freeQueue(q);
}

void freeTree(Node *root){
  if(root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

int main(){
  Node *root = createNode(10);
  root->left = createNode(20);
  root->right = createNode(30);
  root->left->left = createNode(40);
  root->left->right = createNode(50);
  root->right->left = createNode(60);

  levelOrderTraversal(root);

  freeTree(root);
  
  return 0;
}
