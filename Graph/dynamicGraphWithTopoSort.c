
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[20];
  int top;
  int capacity;
  char **vD; //vD stands for vertex data 
} Stack;

typedef struct Vertex{
  char str[20];
  struct Vertex *next;
} Vertex;

typedef struct{
  int vertices;
  int capacity; //Now converting to dynamic size Graph
  Vertex **adj;
  int *visited;
  char **labels;
} Graph;

//-------Stack------------//
int isEmpty(Stack *stk){
  if(stk->top == -1){
    return 1;
  }
  return 0;
}

int isFull(Stack *stk){
  if(stk->top == stk->capacity - 1){
    return 1;
  }
  return 0;
}

void push(Stack *stk, const char *str){
  if(isFull(stk)){
    puts("Stack is fulled.");
    return;
  }
  stk->top++;
  strncpy(stk->vD[stk->top], str, 20);
}

char* peek(Stack *stk){
  if(isEmpty(stk)){
    puts("Stack is empty");
    return NULL;
  }
  char* str = stk->vD[stk->top];
  return str;
}

char* pop(Stack *stk){
  if(isEmpty(stk)){
    puts("Stack is empty.");
    return NULL;
  }
  char* str = stk->vD[stk->top];
  stk->top--;
  return str;
}

Stack* createStack(int capacity){
  Stack *st = (Stack*)malloc(sizeof(Stack));
  if(!st){
    perror("ERROR: Memory allocation failed for stack.\n");
    return NULL;
  }
  
  st->top = -1;
  st->capacity = capacity;
  memset(st->name, ' ', 20);

  st->vD = (char**)calloc(capacity, sizeof(char*));
  if(!(st->vD)){
    perror("ERROR: Memory allocation failed for stack's inner array.\n");
    return NULL;
  }
  for(int i = 0; i < capacity; i++){
    st->vD[i] = (char*)calloc(20, sizeof(char));
  }
  
  return st;
}

//------------Vertex------------//
Vertex* createVertex(const char *s){
  Vertex *newV = (Vertex*)malloc(sizeof(Vertex));
  if(!newV){
    perror("Memory allocation failed for newVertex.\n");
    return NULL;
  }
  strncpy(newV->str, s, 20);
  newV->next = NULL;
  
  return newV;
}

//------------AddEdge------------//
int getVIndex(Graph *g, const char *label){
  for(int i = 0; i < g->vertices; i++){
    if(strncmp(g->labels[i], label, 20) == 0){
      return i;
    }
    //return free index of the label array
    if(g->labels[i][0] == '\0'){
      strncpy(g->labels[i], label, 20);
      return i;
    }
  }
  //for dynamic addition of Vertex or increase size of graph
  if(g->vertices == g->capacity){
    g->capacity *=  2;
    g->labels = (char**)realloc(g->labels, g->capacity * sizeof(char*));
    g->adj = (Vertex**)realloc(g->adj, g->capacity * sizeof(Vertex*));
    g->visited = (int*)realloc(g->visited, g->capacity * sizeof(int));
    
    for(int i = g->vertices; i < g->capacity; i++){
      g->labels[i] = (char*)calloc(20, sizeof(char));
      g->adj[i] = NULL;
      g->visited[i] = 0;
    }
  }
  int index = g->vertices++;
  strncpy(g->labels[index], label, 20);

  return index;
}

void addEdge(Graph *g, const char *src, const char *dest){
  int srcIdx = getVIndex(g, src);
  getVIndex(g, dest);

  Vertex *v = createVertex(dest);
  v->next = g->adj[srcIdx];
  g->adj[srcIdx] = v;
}

//------------Graph-------------//
Graph* createGraph(int num){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("Graph memo failed.\n");
    return NULL;
  }
  
  g->vertices = num;
  g->capacity = num;
  
  g->adj = (Vertex**)malloc(num * sizeof(Vertex*));
  if(!(g->adj)){
    perror("Memory allocation failed.\n");
    return NULL;
  }

  g->visited = (int*)calloc(num, sizeof(int));
  g->labels = (char**)calloc(num, sizeof(char*));
  
  if(!(g->visited) && !(g->labels)){
    perror("Memory allocation failed inside createGraph.\n");
    free(g->adj);
    free(g);
    return NULL;
  }

  for(int i = 0; i < num; i++){
    g->labels[i] = (char*)calloc(20, sizeof(char));
    g->adj[i] = NULL;
  }
  return g;
}

void displayGraph(Graph *g){
  for(int i = 0; i < g->vertices; i++){
    printf("%d: Vertex[%10s]: ", i, g->labels[i][0] ? g->labels[i]: "Empty");
    Vertex *t = g->adj[i];
    while(t){
      printf("%s -> ", t->str);
      t = t->next;
    }
    printf("NULL\n");
  }
}

//----------TopoSort-----------//
void dfsOn(Graph *g, int idx, Stack *st){
  g->visited[idx] = 1;
  Vertex *t = g->adj[idx];
  while(t != NULL){
    int nextIdx = getVIndex(g, t->str);
    if(g->visited[nextIdx] == 0){
      dfsOn(g, nextIdx, st);
    }
    t = t->next;
  }
  push(st, g->labels[idx]);
}


void topoSort(Graph *g, Stack *s){
  for(int i = 0; i < g->vertices; i++){
    if(g->labels[i][0] != '\0' && g->visited[i] == 0){
      dfsOn(g, i, s);
    }
  }
}

void displayStack(Stack *st){
  for(int i = 0; i < st->capacity; i++){
    if(st->vD[i][0] != '\0')
      printf("%s, ", st->vD[i] );
  }
  printf("\n");
}
//-----------Free Memory------------//
void freeGraph(Graph *g){
  for(int i = 0; i < g->vertices; i++){
    free(g->labels[i]);
    Vertex *t = g->adj[i];
    while(t){
      Vertex *tn = t->next;
      free(t);
      t = tn;
    }
  }
  free(g->labels);
  free(g->adj);
  free(g->visited);
  free(g);
}

//-----------Main-----------------//
int main(){
  int capacity = 5;
  /* Stack *s = createStack(capacity); */
  Graph *grp = createGraph(capacity);

  addEdge(grp, "A", "Steam");
  addEdge(grp, "Steam", "Run");
  addEdge(grp, "Run", "Jump");
  addEdge(grp, "Fly", "Sky");
  addEdge(grp, "Glide", "Sky");
  addEdge(grp, "Happy", "Glide");
  addEdge(grp, "Jump", "Fly");

  /*
    Always create stack after creating all vertices because size of stack
    depend on graph dynamic size although it's not necessary but if want all graph's vertex's string as data in a output then it is.
  */    
  Stack *s = createStack(grp->capacity);
  displayGraph(grp);

  topoSort(grp, s);

  displayStack(s);

  freeGraph(grp);
  
  return 0;
}
