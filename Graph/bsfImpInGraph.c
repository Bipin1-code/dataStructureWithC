

#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int front, rear, size, capacity;
  int *qArr;
} Queue;

Queue* createQueue(int capV){
  Queue *q = (Queue*)malloc(capV * sizeof(Queue));
  if(!q){
    perror("ERROR: Memory allocation failed in Queue Builder.\n");
    return NULL;
  }
  q->front = 0;
  q->rear = capV - 1;
  q->size = 0;
  q->capacity = capV;
  q->qArr = (int*)calloc(capV, sizeof(int));
  if(!(q->qArr)){
    perror("ERROR: Memory allocation failed in Queue Builder.\n");
    free(q);
    return NULL;
  }
  return q;
}

int isEmpty(Queue *q){
  return q->size == 0;
}

int isFull(Queue *q){
  return q->size == q->capacity;
}

void enqueue(Queue *q, int inValue){
  if(isFull(q)){
    puts("Queue is overflow.");
    return;
  }
  q->rear = (q->rear + 1) % q->capacity;
  q->qArr[q->rear] = inValue;
  q->size++;
}

int dequeue(Queue *q){
  if(isEmpty(q)){
    puts("Queue is underflow.");
    return -1;
  }
  int rValue = q->qArr[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size--;

  return rValue; 
}

void freeQueue(Queue *q){
  free(q->qArr);
  free(q);
}

typedef struct Vertex{
  int value;
  struct Vertex *next;
} Vertex;

Vertex* createVertex(int value){
  Vertex *v = (Vertex*)malloc(sizeof(Vertex));
  if(!v){
    perror("ERROR: Memory allocation failed in vertex builder.\n");
    return NULL;
  }
  v->value = value;
  v->next = NULL;
  return v;
}

typedef struct{
  int numVertex;
  Vertex **adj;
} Graph;

Graph* createGraph(int numofVrtx){
  Graph* g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("ERROR: Memory allocation failed in Graph builder.\n");
    return NULL;
  }
  g->numVertex = numofVrtx;
  g->adj = (Vertex**)malloc(numofVrtx * sizeof(Vertex*));
  if(!(g->adj)){
    perror("ERROR: Memory allocation failed in Graph builder.\n");
    free(g);
    return NULL;
  }
  for(int i = 0; i < numofVrtx; i++){
    g->adj[i] = NULL;
  }
  return g;
}

void addEdge(Graph *g, int src, int dest){
  Vertex *newV = createVertex(dest);
  newV->next = g->adj[src];
  g->adj[src] = newV;

  //below lines makes it undirected graph
  /* newV = createVertex(src); */
  /* newV->next = g->adj[dest]; */
  /* g->adj[dest] = newV; */
}

void bsfTraversal(Graph *g, int startV, int *visited){
  static int countCall = 0;
  printf("Call number : [%d]\n", countCall++);
  visited[startV] = 1;
  Queue *q = createQueue(g->numVertex);
  enqueue(q, startV);
  
  while(!isEmpty(q)){
    int val = dequeue(q);
    printf(" %d ", val);
    for(Vertex *v = g->adj[val]; v != NULL; v = v->next){
      int vValue = v->value;
      if(visited[vValue] == 0){
	enqueue(q, vValue);
	visited[vValue] = 1;
      }else{
	printf(" [Cycle detect at vertex %d] ", v->value);
      }
    }
  }
  freeQueue(q);
}


void findAllConnectedVertex(Graph *g){
  int *visited = (int*)calloc(g->numVertex, sizeof(int));
  if(!visited){
    perror("ERROR: Memory allocation failed for vertex tracer array.\n");
    return;
  }
  for(int i = 0; i < g->numVertex; i++){
    if(visited[i] == 0){
      bsfTraversal(g, i, visited);
      printf("\n");
    }  
  }
  free(visited);
}


void freeGraph(Graph *g){
  for(int i = 0; i < g->numVertex; i++){
    free(g->adj[i]);
  }
  free(g->adj);
  free(g);
}

int main(){

  printf("It's BSF session!!\n");
  Graph *g = createGraph(6);
  if(g){
    puts("Graph canvas is ready to fill.");
  }

  addEdge(g, 0, 1);
  addEdge(g, 1, 2);
  addEdge(g, 2, 3);
  addEdge(g, 3, 4);
  addEdge(g, 4, 5);
  addEdge(g, 4, 1);
  
  
  findAllConnectedVertex(g);
  
  freeGraph(g);
  
  return 0;
}

