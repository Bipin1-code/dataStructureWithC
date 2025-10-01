
//Queue implementation with Linked list
/*
  We don't have fix the size (it grows as long as memory is available).
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
  int data;
  struct Node *next;
} Node;

typedef struct Queue{
  Node *front; //front referring to head 
  Node *rear; //rear referring to tail
} Queue;

Queue* createQueue(){
  Queue *q = (Queue*)malloc(sizeof(Queue)); //make sure generic pointer cast to Queue type
  q->front = NULL;
  q->rear = NULL;
  return q;
}

//Enqueue happens on rear end
void enqueue(Queue *q, int value){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = value;
  newNode->next = NULL;

  if(q->rear == NULL){
    q->front = newNode;
    q->rear = newNode;
  }else{
    //Link old rear to newNode
    q->rear->next = newNode;
    //Update rear
    q->rear = newNode;
  }
  printf("%d enqueued.\n", value);
}

//Dequeue happens on front end
int dequeue(Queue *q){
  if(q->front == NULL){
    printf("Queue underflow!\n");
    return -1;
  }

  Node *temp = q->front;
  int value = temp->data;
  
  q->front = q->front->next;
  if(q->front == NULL){
    q->rear = NULL;
  }
  free(temp);
  return value;
}

int peek(Queue *q){
  if(q->front == NULL){
    printf("Queue is empty.\n");
    return -1;
  }
  return q->front->data;
}

void freeQueue(Queue *q){
  Node *current =  q->front;
  Node *next;
  while(current != NULL){
    next = current->next;
    free(current); //free each node
    current = next;
  }
  free(q); //finally free the Queue structure
}

int main(){
  Queue *q = createQueue();
  enqueue(q, 10);
  enqueue(q, 20);
  enqueue(q, 40);

  printf("Dequeued: %d\n", dequeue(q));
  printf("Front element: %d\n", peek(q));

  enqueue(q, 60);
  printf("Dequeued: %d\n", dequeue(q));

  freeQueue(q);
  
  return 0;
}
