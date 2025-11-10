
/*
  What is Kahn's Algorithm ?
  -> Kahn's Algorithm is a famous algorithm used to perform topological sorting on a Directed Acyclic Graph (DAG).
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int rear, front, capacity, size;
  char **qArr;
} Queue;

Queue* createQueue(int capc){
  Queue *q = (Queue*)malloc(sizeof(Queue));
  if(!q){
    perror("ERROR: Memory allocation failed for Queue.\n");
    return NULL;
  }
  q->rear = capc - 1;
  q->front = 0;
  q->size = 0;
  q->capacity = capc;
  q->qArr = (char**)malloc(capc * sizeof(char*));
  for(int i = 0; i < capc; i++){
    q->qArr[i] = (char*)malloc(20 * sizeof(char));
    q->qArr[i][0] = '\0';
  }
  return q;
}

int isEmpty(Queue *q){
  return q->size == 0;
}

int isFull(Queue *q){
  return q->size == q->capacity;
}

void enqueue(Queue *q, const char *str){
  if(isFull(q)){
    puts("Queue is overflow.");
    return;
  }
  q->rear = (q->rear + 1) % q->capacity;
  strncpy(q->qArr[q->rear], str, 20);
  q->size++;
}

char* dequeue(Queue *q){
  if(isEmpty(q)){
    puts("Queue is underflow.");
    return NULL;
  }
  char *str = q->qArr[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size--;

  return str; 
}

void freeQueue(Queue *q){
  for(int i = 0; i <q->capacity; i++)
    free(q->qArr[i]);
  free(q->qArr);
  free(q);
}

typedef struct Vertex{
  char value[20];
  struct Vertex *next;
} Vertex;

Vertex* createVertex(const char *str){
  Vertex *v = (Vertex*)malloc(sizeof(Vertex));
  strncpy(v->value, str, 20);
  v->next = NULL;
  return v;
}

typedef struct {
  int numV;
  int *visited;
  Vertex **adj;
  char **label;
} Graph;

Graph* createGraph(int nV){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("ERROR: Graph memory allocation failed.\n");
    return NULL;
  }
  g->numV = nV;
  g->visited = (int*)calloc(nV, sizeof(int));
  g->label = (char**)malloc(nV * sizeof(char*));
  g->adj = (Vertex**)malloc(nV * sizeof(Vertex*));

  for(int i = 0; i < nV; i++){
    g->label[i] = (char*)malloc(20 * sizeof(char));
    g->label[i][0] = '\0';
    g->adj[i] = NULL;
  }

  return g;
}

int getIndex(Graph *g, const char *str){
  int index = -1;
  for(int i = 0; i < g->numV; i++){
    if(g->label[i][0] == '\0'){
      strncpy(g->label[i], str, 20);
      index = i;
      break;
    }else if(strncmp(str, g->label[i], 20) == 0){
      index = i;
      break; 
    }
  }
  return index;
}

void addEdge(Graph *g, const char *src, const char *dest){
  int srcIdx = getIndex(g, src);
  if(srcIdx == -1){
    puts("OoPs!! string is not present in label array and \
label is already filled.\n");
    return;
  }
  getIndex(g, dest);

  Vertex *newV = createVertex(dest);
  newV->next = g->adj[srcIdx];
  g->adj[srcIdx] = newV;
}

void KahnsTopoSort(Graph *g){
  int inDegree[g->numV];
  memset(inDegree, 0, sizeof(inDegree));

  for(int i = 0; i < g->numV; i++){
    Vertex *v = g->adj[i];
    while(v){
      int index = getIndex(g, v->value);
      inDegree[index]++;
      v = v->next;
    }
  }
  
  Queue *q = createQueue(g->numV);
  for(int i = 0; i <  g->numV; i++){
    if(inDegree[i] == 0){
      enqueue(q, g->label[i]);
    }
  }
  int count = 0;
  while(!isEmpty(q)){
    char *str = dequeue(q);
    printf(" %s ", str);
    count++;
    
    int idx = getIndex(g, str);
  
    for(Vertex *vn = g->adj[idx]; vn != NULL; vn = vn->next){
      int index = getIndex(g, vn->value);
      inDegree[index]--;
      if(inDegree[index] == 0)
	enqueue(q, g->label[index]);
    }
  }
  
  if(count != g->numV)
    puts("\nGraph has a cycle!");
  
  freeQueue(q);
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->numV; i++){
    Vertex *v = g->adj[i];
    while(v){
      Vertex *tmp = v;
      v = v->next;
      free(tmp);
    }
    free(g->label[i]);
  }
  free(g->visited);
  free(g->label);
  free(g->adj);
  free(g);
}

int main(){

  Graph *g = createGraph(5);

  addEdge(g, "Alice", "Bob");
  addEdge(g, "Bob", "James");
  addEdge(g, "James", "Gora");
  addEdge(g, "Alice", "Jeff");

  KahnsTopoSort(g);
   
  freeGraph(g);
  return 0;
}
