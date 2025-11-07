
//A ring buffer = circular queue using an array;


#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int rear, front, curSize, capacity;
  int *storage;
} Queue;

Queue* createQueue(int size){
  Queue *q = (Queue*)malloc(sizeof(Queue));
  if(!q){
    perror("Memory allocation for Queue is failed.\n");
    return NULL;
  }
  q->curSize = 0;
  q->capacity = size;
  q->rear = size - 1;
  q->front = 0;
  q->storage =(int*)calloc(size, sizeof(int));
  return q;
}

int isEmpty(Queue *q){
  return q->curSize == 0;
}

int isFull(Queue *q){
  return q->curSize == q->capacity;
}

void enqueue(Queue *q, int data){
  if(isFull(q)){
    puts("Queue is Overflow.");
    return;
  }
  q->rear = (q->rear + 1) % q->capacity;
  q->storage[q->rear] = data;
  q->curSize++;
}

int dequeue(Queue *q){
  if(isEmpty(q)){
    puts("Queue is underflow.");
    return -1;
  }
  int giveV = q->storage[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->curSize--;
  return giveV;
}

void printQueue(Queue *q){
  if(isEmpty(q)){
    puts("Queue is empty\n");
    return;
  }
  int count = q->curSize;
  int i = (q->front + 1) % q->capacity;
  while(count--){
    printf("%d ->", q->storage[i]);
    i = (i + 1) % q->capacity;
  }
  printf("\n");
}

void freeQueue(Queue *q){
  free(q->storage);
  free(q);
}


int main(){

  printf("Hello Queue.\n");

  Queue *q = NULL;
  q = createQueue(5);

  int res = isEmpty(q);
  printf("Empty res = %d\n", res);

  enqueue(q, 12);
  enqueue(q, 13);
  
  res = isEmpty(q);
  printf("Empty res = %d\n", res);

  int value = dequeue(q);
  printf("Value is %d\n", value);

  enqueue(q, 1);
  enqueue(q, 21);
  enqueue(q, 10);
  
  res = isFull(q);
  printf("Full res = %d\n", res);

  enqueue(q, 23);
  res = isFull(q);
  printf("Full res = %d\n", res);

  printQueue(q);
  
  freeQueue(q);
  q = NULL;
  if(!q){
    puts("Successfully free queue.");
  }
  
  return 0;
}
