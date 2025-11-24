

#include <stdio.h>
#include <stdlib.h>

//------------------Stack-------------------------//

typedef struct{
  int top;
  int capacity;
  int *stackArr;
} Stack;

Stack* createStack(int capa){
  Stack *s = (Stack*)malloc(sizeof(Stack));
  if(!s){
    perror("ERROR: Memory allocation failed for stack.\n");
    return NULL;
  }
  s->top = -1;
  s->capacity = capa;
  s->stackArr = (int*)calloc(capa, sizeof(int));

  return s;
}

int isStackFull(Stack *s){
  return s->top == s->capacity - 1;
}

int isStackEmpty(Stack *s){
  return s->top == -1;
}

void push(Stack *s, int val){
  if(isStackFull(s)){
    puts("Stack is overflow");
    return;
  }
  s->top++;
  s->stackArr[s->top] = val;
}

int pop(Stack *s){
  if(isStackEmpty(s)){
    puts("Stack is underflow");
    return -1;
  }
  int rv = s->stackArr[s->top];
  s->top--;
  return rv;
}

void freeStack(Stack *s){
  free(s->stackArr);
  free(s);
}

//----------------------Graph--------------------//

typedef struct Node{
  int nName;
  struct Node *next;
} Node;

Node* createNode(int value){
  Node *newNode = (Node*)malloc(sizeof(Node));
  if(!newNode){
    perror("ERROR: Memory allocation failed.\n");
    return NULL;
  }
  newNode->nName = value;
  newNode->next  = NULL; 

  return newNode;
}

typedef struct{
  int nNode;
  Node **adjNode;
} Graph;

Graph* createGraph(int numOfNode){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("ERROR: Memory allocation failed for Graph.\n");
    return NULL;
  }
  g->nNode = numOfNode;
  g->adjNode = (Node**)malloc(numOfNode * sizeof(Node*));
  if(!(g->adjNode)){
    perror("ERROR: Memory allocation failed for Graph.\n");
    free(g);
    return NULL;
  }
  for(int i = 0; i < numOfNode; i++){
    g->adjNode[i] = NULL;
  }

  return g;
}

void addEdge(Graph *g, int src, int dest){
  Node *n = createNode(dest);
  n->next = g->adjNode[src];
  g->adjNode[src] = n;
}

void displayGraph(Graph *g){
  for(int u = 0; u < g->nNode; u++){
    Node *v = g->adjNode[u];
    printf("[%d]: {", u);
    while(v){
      printf("%d->", v->nName);
      v = v->next;
    }
    printf("NULL}\n");
  }
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->nNode; i++){
    Node *v = g->adjNode[i];
    Node *tv = NULL;
    while(v){
      tv = v;
      v = v->next;
      free(tv);
    }
  }
  free(g->adjNode);
  free(g);
}

//-----------------Tarjan's Algorithm-------------//


int globalTime = 0;

int min(int u, int v){
  return (u < v) ? u : v;
}

void dfsOnNode(Graph *g, Stack *s, int *disc, int *low, int *stackMmb, int u){
  disc[u] = low[u] = ++globalTime;
  push(s, u);
  stackMmb[u] = 1;
  
  Node *v = g->adjNode[u];
  while(v){
    if(disc[v->nName] == -1){
      dfsOnNode(g, s, disc, low, stackMmb, v->nName);
      low[u] = min(low[u], low[v->nName]);
    }else if(stackMmb[v->nName] == 1){
      low[u] = min(low[u], disc[v->nName]);
    }
    v = v->next;
  }

  if(low[u] == disc[u]){
    printf("SCC: ");
    while(1){
      if(isStackEmpty(s)) break;
      int w = pop(s);
      stackMmb[w] = 0;
      printf("%d ", w);
      if(w == u) break;
    }
    printf("\n");
  } 
}

void TarjanAlgo(Graph *g){
  //Discovery time
  int disc[g->nNode];
  
  //Lowest discovery time reachable from src Node (u)
  int low[g->nNode];
  
  Stack *s = createStack(g->nNode);
  //Track a src node (u) present in stack or not
  int stackMember[g->nNode];
  
  globalTime = 0;
 
  for(int i = 0; i < g->nNode; i++){
    disc[i] = -1;
    low[i] = -1;
    stackMember[i] = 0;
  }

  for(int u = 0; u < g->nNode; u++){
    if(disc[u] == -1)
      dfsOnNode(g, s, disc, low, stackMember, u);
  }
  freeStack(s);
}

int main(){
  printf("Tarjan's alogorithm session:\n");

  Graph *g = createGraph(5);
  addEdge(g, 0, 1);
  addEdge(g, 1, 2);
  addEdge(g, 2, 0);
  addEdge(g, 3, 4); 

  displayGraph(g);

  puts("Tarjan's Algorithm session:");
  TarjanAlgo(g);
  
  freeGraph(g);
  return 0;
}
