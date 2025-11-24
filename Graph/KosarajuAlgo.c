
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-------------Stack-------------/
typedef struct{
  int top;
  int size;
  int capacity;
  int *array;
} Stack;

Stack* createStack(int capacity){
  Stack *s = (Stack*)malloc(sizeof(Stack));
  if(!s){
    perror("ERROR: Memory allocation failed for stack.\n");
    return NULL;
  }
  s->top = -1;
  s->size = 0;
  s->capacity = capacity;
  s->array = (int*)calloc(capacity, sizeof(int));
  
  return s;
}

int isStackEmpty(Stack *s){
  return s->size == 0;
}

int isStackFull(Stack *s){
  return s->size == s->capacity;
} 

void push(Stack *s, int value){
  if(isStackFull(s)){
    puts("Stack is overflow");
    return;
  }
  s->top++;
  s->array[s->top] = value;
  s->size++;
}

int pop(Stack *s){
  if(isStackEmpty(s)){
    puts("Stack is underflow");
    return 0;
  }
  int rV = s->array[s->top];
  s->top--;
  s->size--;
  return rV;
}

void freeStack(Stack *s){
  free(s->array);
  free(s);
}
//-----------------Graph-----------------/

typedef struct Node{
  int dest;
  struct Node *next;
} Node;

Node* createNode(int value){
  Node *newN = (Node*)malloc(sizeof(Node));
  if(!newN){
    perror("ERROR: Memory allocation failed for new Node.\n");
    return NULL;
  }
  newN->dest = value;
  newN->next = NULL;

  return newN;
}

typedef struct{
  int nNode;
  int size;
  int *visited;
  Node **adjNode;
} Graph;

Graph* createGraph(int numOfNode){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("ERROR: Memory allocation failed for Graph.\n");
    return NULL;
  }
  g->nNode = numOfNode;
  g->visited = (int*)calloc(numOfNode, sizeof(int));
  g->adjNode = (Node**)malloc(numOfNode * sizeof(Node*));
  if(!g->visited || !g->adjNode){
    free(g->visited);
    free(g->adjNode);
    free(g);
    return NULL;
  }
  for(int i = 0; i < numOfNode; i++){
    g->adjNode[i] = NULL;
  }
  
  return g;
}

void addEdge(Graph *g, int src, int dest){
  Node *newN = createNode(dest);
  newN->next = g->adjNode[src];
  g->adjNode[src] = newN;
}

void displayGraph(Graph *g){
  for(int i = 0; i < g->nNode; i++){
    Node *n = g->adjNode[i];
    printf("[%d]->", i);
    while(n){
      printf("%d->", n->dest);
      n = n->next;
    }
    printf("NULL\n");
  }
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->nNode; i++){
    Node *node = g->adjNode[i];
    Node *tNode = NULL;
    while(node){
      tNode = node;
      node = node->next;
      free(tNode);   
    }
  }
  free(g->adjNode);
  free(g->visited);
  free(g);
}

//-------------------------Kosaraju's Algorithm--------------------//
Graph* reverseGraph(Graph *g){
  Graph *rv = createGraph(g->nNode);

  for(int u = 0; u < g->nNode; u++){
    Node *vr = g->adjNode[u];
    while(vr){
      addEdge(rv, vr->dest, u);
      vr = vr->next;
    }
  }
  return rv;
}


void dfsOnNodePush(Graph *g, Stack *s, int u){
  g->visited[u] = 1;
  Node *v = g->adjNode[u];
  while(v){
    if(g->visited[v->dest] == 0)
      dfsOnNodePush(g, s, v->dest);
    v = v->next;
  }
  push(s, u);
}

void dfsOnNodePop(Graph *revG, Stack *s, int u){
  revG->visited[u] = 1;
  printf("%d, ", u);
  Node *v = revG->adjNode[u];
  while(v){
    if(revG->visited[v->dest] == 0)
      dfsOnNodePop(revG, s, v->dest);
    v = v->next;
  }
}

void kosarajuAlgo(Graph *g, Stack *s){
  for(int i = 0; i < g->nNode; i++){
    if(g->visited[i] != 1)
      dfsOnNodePush(g, s, i);
  }
  //reverse graph
  Graph *revG = reverseGraph(g);
  int count = 1; 
  while(!isStackEmpty(s)){
    int u = pop(s);
    if(revG->visited[u] == 0){
      printf("SCC[%d] = {", count++);
      dfsOnNodePop(revG, s, u);
      printf("}\n");
    }
  }
  freeGraph(revG);
}

void displayStack(Stack *s){
  printf("capacity = %d and top = %d\n", s->capacity, s->top);
  for(int i = 0; i < s->size; i++){
    printf("%d, ", s->array[i]);
  }
  printf("\n");
}

int main(){

  printf("Kosaraju's Algorithm session:\n");
  //test 1 = Passed
  /* Graph *g = createGraph(5); */
  /* addEdge(g, 0, 1); */
  /* addEdge(g, 1, 2); */
  /* addEdge(g, 2, 0); */
  /* addEdge(g, 3, 4);  */

  //Test 2 = Passed
  /* Graph *g = createGraph(5); */
  /* addEdge(g, 0, 1); */
  /* addEdge(g, 1, 0); */
  /* addEdge(g, 1, 2); */
  /* addEdge(g, 2, 3); */
  /* addEdge(g, 3, 4); */
  /* addEdge(g, 4, 3); */

  //test 3 = passed
  /* Graph *g = createGraph(9); */
  /* addEdge(g, 0, 1); */
  /* addEdge(g, 1, 2); */
  /* addEdge(g, 2, 3); */
  /* addEdge(g, 3, 0); */
  /* addEdge(g, 3, 4); */
  /* addEdge(g, 4, 5); */
  /* addEdge(g, 5, 4); */
  /* addEdge(g, 5, 6); */
  /* addEdge(g, 6, 7); */
  /* addEdge(g, 7, 8); */
  /* addEdge(g, 8, 7); */

  //test 4 = Passed
  Graph *g = createGraph(9);
  addEdge(g, 0, 1);
  addEdge(g, 1, 0);
  addEdge(g, 1, 2);
  addEdge(g, 2, 3);
  addEdge(g, 2, 6);
  addEdge(g, 3, 4);
  addEdge(g, 4, 5);
  addEdge(g, 5, 3);
  addEdge(g, 5, 6);
  addEdge(g, 6, 7);
  addEdge(g, 7, 6);
  addEdge(g, 7, 8);
  
  displayGraph(g);

  puts("\nAfter implementing Kosaraju's Algorithm:");
  Stack *s = createStack(g->nNode);

  kosarajuAlgo(g, s);
  
  displayStack(s);

  freeStack(s);
  freeGraph(g);
 
  return 0;
}
