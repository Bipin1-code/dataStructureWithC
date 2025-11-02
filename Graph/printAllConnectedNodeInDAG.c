/*
  Print all nodes reachable from a node which in degree is Zero. (Done)
  Now I need solve the cycle path also. //This works only for directed graph
  We can also print path from a given node:
  1. just skip indegree loop and use pass if condition to find correct node and
     call pathFinder (keep in mind output print from give node to last connected node)
     due to directed graph.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int top;
  int capacity;
  char **item;
} Stack;

Stack* createStack(int cp){
  Stack *stk = (Stack*)malloc(sizeof(Stack));
  if(!stk){
    perror("Memory allocation failed for stack.\n");
    return NULL;
  }
  stk->top = -1;
  stk->capacity = cp;
  stk->item = (char**)malloc(cp * sizeof(char*));
  if(!(stk->item)){
    perror("Memory allocation failed for stack.\n");
    return NULL;
  }
  for(int i = 0; i < cp; i++){
    stk->item[i] = (char*)calloc(20, sizeof(char));
  }
  return stk;
}

int isEmpty(Stack *s){
  return s->top == -1;
}

int isFull(Stack *s){
  return s->top == (s->capacity - 1);
}

void push(Stack *s, const char *str){
  if(isFull(s)){
    puts("Stack is OverFlow.");
    return;
  }
  s->top++;
  strncpy(s->item[s->top], str, 20);
}

char* peek(Stack *s){
  if(isEmpty(s)){
    puts("Stack is Underflow.");
    return NULL;
  }
  char *str = s->item[s->top];
  return str;
}

char* pop(Stack *s){
  if(isEmpty(s)){
    puts("Stack is Underflow.");
    return NULL;
  }
  char *str = s->item[s->top];
  s->top--;
  return str;
}

void freeStack(Stack *s){
  for(int i = 0; i < s->capacity; i++){
    free(s->item[i]);
  }
  free(s->item);
  free(s);
}

void displayStack(Stack *s){
  for(int i = s->top; i >= 0; i--){
    printf("%s-> ", s->item[i]);
  }
  printf("NULL\n");
}

typedef struct Node{
  char nodeName[20];
  struct Node *next;
} Node;

typedef struct{
  int numOfNodes;
  int *visitedNode;
  Node **neighborNodes;
  char **label;
} Graph;

Node* createNode(const char *name){
  Node *newNode = (Node*)malloc(sizeof(Node));
  if(!newNode){
    perror("Memory allocation failed for newNode i.\n");
    return NULL;
  }
  strncpy(newNode->nodeName, name, 20);
  newNode->next = NULL;

  return newNode;
}

Graph* createGraph(int numNodes){
  Graph *grf = (Graph*)malloc(sizeof(Graph));
  if(!grf){
    perror("Memory allocation failed for graph.\n");
    return NULL;
  } 
  grf->numOfNodes = numNodes;
  grf->visitedNode = (int*)calloc(numNodes, sizeof(int));
  grf->label = (char**)malloc(numNodes * sizeof(char*));
  grf->neighborNodes = (Node**)malloc(numNodes * sizeof(Node*));
  
  if(!(grf->visitedNode) || !(grf->label) || !(grf->neighborNodes)){
    perror("Memory allocation failed inside create Graph.\n");
    free(grf->visitedNode);
    free(grf->label);
    free(grf);
    return NULL;
  }

  for(int i = 0; i < numNodes; i++){
    grf->label[i] = (char*)calloc(20, sizeof(char));
    grf->neighborNodes[i] = NULL;
  }
  
  return grf;
}

int getIndexOfNode(Graph *g, const char *src){
  int index = -1;
  for(int i = 0; i < g->numOfNodes; i++){
    if(g->label[i][0] == '\0'){
      strncpy(g->label[i], src, 20);
      index = i;
      break;
    }else if(strncmp(g->label[i], src, 20) == 0){
      index = i;
      break;
    }
  }
  return index;
}

void drawEdge(Graph *g, const char *src, const char *dest){
  int srcIdx = getIndexOfNode(g, src);
  if(srcIdx == -1){
    perror("Failed to get index of src!\n");
    return;
  }
  getIndexOfNode(g, dest);
  Node *node = createNode(dest);
  node->next = g->neighborNodes[srcIdx];
  g->neighborNodes[srcIdx] = node;
}

void dfsOn(Graph *g, Stack *s, int idx){
  g->visitedNode[idx] = 1;
  Node *tN = g->neighborNodes[idx];
  while(tN != NULL){
    int index = getIndexOfNode(g, tN->nodeName);
    if(!g->visitedNode[index]){
      dfsOn(g, s, index);
    }
    tN = tN->next;
  }
  push(s, g->label[idx]);
}

void topologicalSort(Graph *g, Stack *s){
  for(int i = 0; i < g->numOfNodes; i++){
    if(g->label[i][0] != '\0' && g->visitedNode[i] == 0)
      dfsOn(g, s, i);
  }
}

void printPath(char *path[], int depth){
  puts("We are inside printPath");
  for(int i = 0; i < depth; i++){
    printf("%s", path[i]);
    if(i < depth - 1) printf("->");
  }
  printf("\n");
}

void pathFinder(Graph *g, int idx, int *visited, char *path[], int depth){
  visited[idx] = 1;
  path[depth++] = g->label[idx];
  
  Node *tN = g->neighborNodes[idx];

  if(!tN){
    printPath(path, depth);
  }else{
    while(tN){
      int index = getIndexOfNode(g, tN->nodeName);
      if(!visited[index]){
	pathFinder(g, index, visited, path, depth);
      }else{
	Node *ctN = g->neighborNodes[index];
	while(ctN){
	  //printf("%s\n ", ctN->nodeName); //debugger line
	  int index = getIndexOfNode(g, ctN->nodeName);
	  if(!visited[index]){
	    pathFinder(g, index, visited, path, depth);
	  }
	  ctN = ctN->next;
	}
      }
      tN = tN->next;
    }   
  }

  visited[idx] = 0; 
}

void displayConnectedNodes(Graph *g){
  int inDegree[g->numOfNodes];
  memset(inDegree, 0, sizeof(inDegree));
  
  for(int i = 0; i < g->numOfNodes; i++){
    Node *tN = g->neighborNodes[i];
    while(tN){
      int index = getIndexOfNode(g, tN->nodeName);
      inDegree[index]++;
      tN = tN->next;
    }
  }
  
  for(int i = 0; i < g->numOfNodes; i++){
    if(inDegree[i] == 0){
      int visited[g->numOfNodes];
      memset(visited, 0, sizeof(visited));
      char *path[g->numOfNodes];
      pathFinder(g, i, visited, path, 0);
    }
  }
}

void displayGraph(Graph *g){
  for(int i = 0; i < g->numOfNodes; i++){
    printf("%s ", g->label[i]);
    Node *tN = g->neighborNodes[i];
    while(tN){
      printf("%s ", tN->nodeName);
      tN = tN->next;
    }
    printf("NULL\n");
  }
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->numOfNodes; i++){
    Node *tN = g->neighborNodes[i];
    while(tN != NULL){
      Node *ttN = tN->next;
      free(tN);
      tN = ttN;
    }
    free(g->label[i]);
    free(g->neighborNodes[i]);
  }
  free(g->visitedNode);
  free(g->label);
  free(g->neighborNodes);
  free(g);
}

int main(){

  Graph *g = createGraph(9);

  drawEdge(g, "A", "B");
  drawEdge(g, "B", "C");
  drawEdge(g, "D", "E");
  drawEdge(g, "E", "F");
  drawEdge(g, "C", "D");
  drawEdge(g, "F", "C");
  drawEdge(g, "C", "G");
  drawEdge(g, "G", "H");
  drawEdge(g, "H", "I");
 
  Stack *s = createStack(g->numOfNodes);

  displayGraph(g);
    
  topologicalSort(g, s);
  
  puts("Display Stack:");
  displayStack(s);
  
  puts("Display Connected Graph:");
  displayConnectedNodes(g);

  freeStack(s);
  freeGraph(g);
  
  return 0;
}

/*
  OUTPUT:
  A B NULL
  B C NULL
  C G D NULL
  D E NULL
  E F NULL
  F C NULL
  G H NULL
  H I NULL
  I NULL
  Display Stack:
  A-> B-> C-> D-> E-> F-> G-> H-> I-> NULL
  Display Connected nodes:
  We are inside printPath
  A->B->C->G->H->I
  We are inside printPath
  A->B->C->D->E->F->G->H->I

 */
