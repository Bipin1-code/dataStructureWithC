
//Queue implemention with array

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

typedef struct{
  int item[SIZE];
  int front;
  int rear;
} Queue;

//Initialize queue
void initQueue(Queue *q){
  q->front = -1;
  q->rear = -1;
}

int isEmpty(Queue *q){
  return q->front == -1;
}

int isFull(Queue *q){
  return q->rear == SIZE - 1;
}

//Add element to queue
void enqueue(Queue *q, int value){
  if(isFull(q)){
    printf("Queue is full!\n");
    return;
  }
  if(isEmpty(q)){
    q->front = 0;
  }
  q->item[++q->rear] = value;
  printf("%d enqueued\n", value);
}

//Remove element from queue
int dequeue(Queue *q){
  if(isEmpty(q)){
    printf("Queue is empty!\n");
    return -1;
  }
  int value = q->item[q->front];
  if(q->front == q->rear){
    //queue has only one element
    q->front = q->rear  = -1;
  }else{
    q->front++;
  }
  return value;
}

//Peek front element
int peek(Queue *q){
  if(isEmpty(q)){
    printf("Queue is empty!\n");
    return -1;
  }
  return q->item[q->front];
}

int main(){
  Queue q;
  initQueue(&q);

  enqueue(&q, 10);
  enqueue(&q, 20);
  enqueue(&q, 30);

  printf("Front element: %d\n", peek(&q));

  printf("%d dequeued\n", dequeue(&q));
  printf("%d dequeued\n", dequeue(&q));

  enqueue(&q, 40);
  printf("Front element: %d\n", peek(&q));
  
  return 0;
}
