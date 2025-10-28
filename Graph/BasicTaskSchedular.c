
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int top;
  int size;
  char *arrVal;
} Stack; //16 bytes

int isEmpty(Stack *s){
  if(s->top == -1){
    return 1;
  }
  return 0;
}

int isFull(Stack *s){
  if(s->top == s->size - 1){
    return 1;
  }
  return 0;
}

void push(Stack *s, char data){
  if(isFull(s)){
    perror("Stack overflow\n");
    return;
  }
  s->top++;
  s->arrVal[s->top] = data;
}

char pop(Stack *s){
  if(isEmpty(s)){
    perror("Stack underflow\n");
    return -1;
  }
  char data = s->arrVal[s->top];
  s->top--;
  return data;
}

char peek(Stack *s){
  if(isEmpty(s)){
    puts("Stack is empty\n");
    return -1;
  }
  char data = s->arrVal[s->top];
  return data;
}

Stack* initStack(int sz){
  Stack *s = (Stack*)malloc(sizeof(Stack));
  if(!s){
    perror("ERROR: Memory allocation for STACK is failed!\n");
    return NULL;
  }
  s->size = sz;
  s->top = -1;
  s->arrVal = (char*)malloc(sz * sizeof(char));
  if(!(s->arrVal)){
    perror("ERROR: Memory allocation for STACK's array is failed!\n");
    return NULL;
  }
  return s;
}

typedef struct vertex{
  char value;
  struct vertex* next;
} Vertex;

typedef struct{
  int vertices;
  Vertex **adj;
  int *visited;
  int *used;
} Graph;


Vertex* createVertex(char ch){
  Vertex *newV = (Vertex*)malloc(sizeof(Vertex));
  if(!(newV)){
    perror("Memory allocation failed for vertex creation block.\n");
    return NULL;
  }
  newV->value = ch;
  newV->next = NULL;
  return newV;
}

Graph* initGraph(int vertices){
  Graph *g =(Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("Memory allocation for Graph if failed.\n");
    return NULL;
  }
  g->vertices = vertices;
  g->visited = (int*)malloc(vertices * sizeof(int));
  if(!(g->visited)){
    perror("Memory allocation for Graph's visited array is failed.\n");
    return NULL;
  }
  g->adj = (Vertex**)malloc(vertices * sizeof(Vertex*));
  if(!(g->adj)){
    perror("Memory allocation for Graph's adjacency array is failed.\n");
    return NULL;
  }
  g->used = (int*)malloc(vertices * sizeof(int));
  if(!(g->used)){
    perror("Memory allocation for Graph's used array is failed.\n");
    return NULL;
  }
  for(int i = 0; i < vertices; i++){
    g->visited[i] = 0;
    g->used[i] = 0;
    g->adj[i] = NULL;
  }
  return g;
}

void addEdge(Graph *g, char src, char dest){
  Vertex *v = createVertex(dest);
  v->next = g->adj[src - 'A'];
  g->adj[src - 'A'] = v;

  g->used[src - 'A'] = 1;
  g->used[dest - 'A'] = 1;
}

void dfsOn(Graph *g, int v, Stack *st){
  g->visited[v] = 1;
  Vertex *temp = g->adj[v];
  while(temp != NULL){
    int connectedV = temp->value - 'A';
    if(g->visited[connectedV] == 0){
      dfsOn(g, connectedV, st);
    }
    temp =  temp->next;
  }
  push(st, 'A' + v);
}

void scheduleTasks(Graph *g, Stack *st){
  for(int v = 0; v < g->vertices; v++){
    if(g->used[v] && g->visited[v] == 0){
      dfsOn(g, v, st);
    }
  }
}

void displayStack(Stack *s){
  if(isEmpty(s)){
    printf("Stack is empty.\n");
    return;
  }
  printf("NULL");
  for(int i = s->top; i >= 0; i--){
    if(s->arrVal[i] == '\0'){
      continue;
    }
    printf(" <- %c", s->arrVal[i]);
  }
  printf("\n");
}


void displayGraph(Graph *g){
  for(int i = 0; i < g->vertices; i++){
    Vertex *temp = g->adj[i];
    printf("Vertex [%c]: ", i + 'A');
    while(temp){
      printf(" %c -> ", temp->value);
      temp = temp->next;
    }
    printf(" NULL\n");
  }
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->vertices; i++){
    Vertex *temp = g->adj[i];
    while(temp){
      Vertex *next = temp->next;
      free(temp);
      temp = next;
    }
  }
  free(g->adj);
  free(g->visited);
  free(g);
}

int main(){
  int size = 26;
  Stack *s = initStack(size);
  Graph *graph = initGraph(26);

  addEdge(graph, 'A', 'B');
  addEdge(graph, 'D', 'E');

  displayGraph(graph);

  scheduleTasks(graph, s);
 
  displayStack(s);

  free(s);   
  freeGraph(graph);
  
  return 0;
}
