/*
  Dijkstra algorithm for to find shortest path.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  char dest[10];
  int weight;
} Edge;

typedef struct Node{
  char name[10];
  int size; //this track size of adjE array 
  Edge *adjE;
  struct Node *next;
} Node;


Node* createNode(const char *str){
  Node* node = (Node*)malloc(sizeof(Node));
  if(!node){
    perror("ERROR: Memory allocation failed.\n");
    return NULL;
  }
  strncpy(node->name, str, 9);
  node->name[9] = '\0';
  node->size = 0;
  node->adjE = NULL;
  node->next = NULL;

  return node;
}

void updateEdgeListOfNode(Node *src, const char *dest, int weight){
  static int count = 1;
  
  printf("[%d] Debugger lines: I am inside updateEdgeListofNode function. for %s\n", count++, src->name);
 
  printf("%d\n", src->size);
  puts("We are inside if block");
  
  Edge *tE = (Edge*)realloc(src->adjE, (src->size+1) * sizeof(Edge));
  if(!tE){
    perror("Memory updatation for Node's ArrayOfEgdes Failed.\n");
    return;
  }
  src->adjE = tE;
 
  strncpy(src->adjE[src->size].dest, dest, 10);
  src->adjE[src->size].dest[9] = '\0';
  src->adjE[src->size].weight = weight;
  src->size++;
  printf("%d\n", src->size);
}

typedef struct{
  int numNode;
  int *isNodeCreated;
  char **label;
  int *visited;
  Node *nodes; 
} Graph;

Graph* createGraph(int numNode){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("ERROR: Memory allocation failed.\n");
    return NULL;
  }
  g->numNode = numNode;
  g->visited = (int*)calloc(numNode, sizeof(int));
  g->isNodeCreated = (int*)calloc(numNode, sizeof(int));
  g->label = (char**)malloc(numNode * sizeof(char*));
  for(int i = 0; i < numNode; i++){
    g->label[i] = (char*)malloc(10 * sizeof(char));
    g->label[i][0] = '\0';
  }
  g->nodes = (Node*)malloc(numNode * sizeof(Node));
  memset(g->nodes, 0, numNode * sizeof(Node));
  
  return g;
}

int getIndex(Graph *g, const char *str){
  int index = -1;
  for(int i = 0; i < g->numNode; i++){
    if(g->label[i][0] == '\0'){
      strncpy(g->label[i], str, 9);
      index = i;
      break;
    }else if(strncmp(str, g->label[i], 10) == 0){
      index = i;
      break;
    }
  }
  return index;
}

void addEdge(Graph *g, const char *src, const char *dest, int weight){
  int srcIdx = getIndex(g, src);
  int destIdx = getIndex(g, dest);
  if(srcIdx == -1 || destIdx == -1){
    perror("Index label is not avaiable for to allote!\n");
    return;
  }
  
  if(!(g->isNodeCreated[srcIdx])){
    Node *newSrc = createNode(src);
    g->isNodeCreated[srcIdx] = 1;
    g->nodes[srcIdx] = *newSrc;
    free(newSrc);
   
  }
  if(!(g->isNodeCreated[destIdx])){
    Node *newDest = createNode(dest);
    g->nodes[destIdx] = *newDest;
    g->isNodeCreated[destIdx] = 1;
    free(newDest);
  }

  updateEdgeListOfNode(&g->nodes[srcIdx], dest, weight);
}

void displayNodesEdgesList(Graph *g){
  for(int i = 0; i < g->numNode; i++){
    Node *n = &(g->nodes[i]);
    if(!g->isNodeCreated[i]) continue;
    printf("Node %s (edges: %d) \n", n->name, n->size);
    printf("[%d] : ", i);
    int j = 0;
    while(j < n->size ){
      printf("{ name = %s | weight = %d }, ", n->adjE[j].dest, n->adjE[j].weight);
      j++;
    }
    printf("\n");
  }
  printf("\n");
}

void display_rDist(Graph *g, int rDist[], int pathTracker[]){

  for(int i = 0; i < g->numNode; i++){ 
    if(rDist[i] == 0x3f3f3f3f)
      printf("%s = Unreachable\n", g->label[i]);
    else if(i == g->numNode -1)
      printf(" %s = %d ", g->label[i], rDist[i]);
    else
      printf(" %s = %d -> ", g->label[i], rDist[i]);
  }
  printf("\nPathTrack:\n");
  
  for(int i = 0; i < g->numNode; i++){
    if(pathTracker[i] == 1)
      printf("%s ->", g->label[i]);
  }
  printf("Home\n");
}

void unknownShortPathAlgo(Graph *g, const char *start){
  int rDist[g->numNode];
  memset(rDist, 0x3f, sizeof rDist);
  int pathTracker[g->numNode];
  memset(pathTracker, 0, sizeof pathTracker);
  
  int startIdx = getIndex(g, start);
  rDist[startIdx] = 0;
   pathTracker[startIdx] = 1; 
  for(int i = 0; i < g->numNode; i++){
    //these are missing variables and for loop on rDist
    int u = -1;
    int minDist = 0x3f3f3f3f;
    //loop over rDist to find smallest value in it
    for(int v = 0; v < g->numNode; v++){
      if(!(g->visited[v]) && rDist[v] < minDist){
	minDist = rDist[v];
	u = v;
      }
    }

    if(u == -1) break;
    g->visited[u] = 1;
    
    Node *n = &(g->nodes[u]);
    //Relax neighbors of u
    //u mean smallest rDist value containing idx for now to me
    int j = 0;
    while(j < n->size){
      int curNIdx = getIndex(g, n->adjE[j].dest);
      int distSum = rDist[u] + n->adjE[j].weight;
      if(!(g->visited[curNIdx]) && distSum < rDist[curNIdx]){
	rDist[curNIdx] = distSum;
	pathTracker[u] = 1;
      }
      j++;
    }
  }
  //Our main destionation is E so, we need maker it as 1
  pathTracker[g->numNode-1] = 1; 
  display_rDist(g, rDist, pathTracker);
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->numNode; i++){ 
    free(g->nodes[i].adjE);
    free(g->label[i]);
  }
  free(g->label);
  free(g->visited);
  free(g->isNodeCreated);
  free(g->nodes);
  free(g);
}

int main(){

  Graph *g = createGraph(5);
  addEdge(g, "A", "B", 10);
  addEdge(g, "B", "C", 20);
  addEdge(g, "C", "D", 6);
  addEdge(g, "A", "C", 15);
  addEdge(g, "C", "E", 12);
  addEdge(g, "D", "E", 10);

  displayNodesEdgesList(g);
 
  //Experiment
  unknownShortPathAlgo(g, "A");
  
  for(int i = 0; i < g->numNode; i++){
    printf("[%s] = %d ", g->label[i], g->visited[i]);
  }
  printf("\n");

  freeGraph(g);
  
  return 0;
}
