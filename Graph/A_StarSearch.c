/*
  What is A* search?
  A* (A-Star) is a pathfinding and graph search algorithm used to
  find the shortest path between a start node & a goal node.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node{
  int nName;
  int weight;
  struct Node *next;
} Node;

Node* createNode(int value, int cost){
  Node* newN = (Node*)malloc(sizeof(Node));
  if(!newN){
    perror("Node memory allocation failed.\n");
    return NULL;
  }
  newN->nName = value;
  newN->weight = cost;
  newN->next = NULL;

  return newN;
}

typedef struct{
  int nNode;
  Node **adjNode;
} Graph;

Graph* createGraph(int numOfNode){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("Graph memory allocation failed.\n");
    return NULL;
  }
  g->nNode = numOfNode;
  g->adjNode = (Node**)malloc(numOfNode * sizeof(Node*));
  if(!(g->adjNode)){
    perror("Graph memory allocation failed.\n");
    free(g);
    return NULL;
  }
  for(int n = 0; n < numOfNode; n++){
    g->adjNode[n] = NULL;
  }

  return g;
}

void addEdge(Graph *g, int src, int dest, int cost){
  Node *n1 = createNode(dest, cost);
  n1->next = g->adjNode[src];
  g->adjNode[src] = n1;

  Node *n2 = createNode(src, cost);
  n2->next = g->adjNode[dest];
  g->adjNode[dest] = n2;
}

void displayGraph(Graph *g){
  for(int n = 0; n < g->nNode; n++){
    Node *ngbN = g->adjNode[n];
    printf("[%d]: { ", n);
    while(ngbN){
      printf("node %d on %d cost ", ngbN->nName, ngbN->weight);
      ngbN = ngbN->next;
    }
    printf("NULL }\n");
  }
}

void freeGraph(Graph *g){
  for(int n = 0; n < g->nNode; n++){
    Node *ngbN = g->adjNode[n];
    Node *tN = NULL;
    while(ngbN){
      tN = ngbN;
      ngbN = ngbN->next;
      free(tN);
    }
  }
  free(g->adjNode);
  free(g);
}

//----------Min-Priority-Queue-------------------//

typedef struct{
  int node;
  int f;
} HeapNode;

typedef struct{
  int size;
  int capacity;
  HeapNode *fscore;
} MinHeap;

MinHeap* createMinHeap(int capacity){
  MinHeap *mh = (MinHeap*)malloc(sizeof(MinHeap));
  if(!mh){
    perror("Memory allocation for Min Heap failed.\n");
    return NULL;
  }
  mh->size = 0;
  mh->capacity = capacity;
  mh->fscore = (HeapNode*)malloc(capacity * sizeof(HeapNode));
  if(!(mh->fscore)){
    perror("Memory allocation for Min Heap failed.\n");
    free(mh);
    return NULL;
  }
  
  return mh;
}

void swap(HeapNode *a, HeapNode *b){
  HeapNode t = *a;
  *a = *b;
  *b = t;
}

void minHeapInsert(MinHeap *mh, int node, int f){
  HeapNode hn = {node, f};
  int i = mh->size;
  if(mh->size == mh->capacity){
    puts("MinHeap is overflow");
    return;
  }
  mh->fscore[i] = hn;
  mh->size++;

  //Heapify up or (bubble upward)
  //Fix heap after insertion
  while(i != 0 && mh->fscore[(i - 1) / 2].f > mh->fscore[i].f){
    swap(&(mh->fscore[(i - 1) / 2]), &(mh->fscore[i]));
    i = (i - 1) / 2;
  }
}

HeapNode extractMin(MinHeap *mh){
  HeapNode root = {-1, -1};
  if(mh->size <= 0) return root;
  root = mh->fscore[0];
  mh->fscore[0] = mh->fscore[mh->size - 1];
  mh->size--;

  //Heaoify down or (bubble downward)
  //Fix heap after extraction or removal
  int i = 0;
  while(1){
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int smallest = i;

    if(left < mh->size && mh->fscore[left].f < mh->fscore[smallest].f)
      smallest = left;
    
    if(right < mh->size && mh->fscore[right].f < mh->fscore[smallest].f)
      smallest = right;

    if(smallest == i) break;

    swap(&(mh->fscore[i]), &(mh->fscore[smallest]));
    i = smallest;
  }

  return root;
}

int isMinHeapEmpty(MinHeap *mh){
  return mh->size == 0;
}

//-----------------A* search-------------//

typedef struct{
  int x, y;
} Point;

int heuristicFunc(int a, int b, Point *coords){
  int dx = coords[a].x - coords[b].x;
  int dy = coords[a].y - coords[b].y;
  
  return abs(dx) + abs(dy); 
}

void A_StarSearch(Graph *g, int start, int goal, Point *coords){
  //cost tracker form start to current node
  int gScore[g->nNode];
  int closeSet[g->nNode];
  int parent[g->nNode];

  MinHeap *openSet = createMinHeap(g->nNode * 5);
  
  for(int i = 0; i < g->nNode; i++){
    gScore[i] = 0x3f3f3f3f;
    closeSet[i] = 0;
    parent[i] = -1;
  }

  gScore[start] = 0;
  minHeapInsert(openSet, start, heuristicFunc(start, goal, coords));

  while(!isMinHeapEmpty(openSet)){
    HeapNode cur = extractMin(openSet);
    int u = cur.node;

    if(closeSet[u]) continue;
    closeSet[u] = 1;

    if(u == goal) break;

    for(Node *n = g->adjNode[u]; n != NULL; n = n->next){
      int v = n->nName;
      int w = n->weight;

      if(closeSet[v]) continue;

      if(gScore[u] + w < gScore[v]){
	gScore[v] = gScore[u] + w;
        parent[v] = u;
	int f = gScore[v] + heuristicFunc(v, goal, coords);
	minHeapInsert(openSet, v, f);
      }
    }
  }

  //Print path
  if(parent[goal] == -1){
    printf("No Path Found.\n");
    return;
  }

  printf("Path: ");
  int path[1000], k = 0, cur = goal;
  while(cur != -1){
    path[k++] = cur;
    cur = parent[cur];
  }
  for(int i = k-1; i >= 0; i--)
    printf("%d ", path[i]);
  printf("\n");

  //--- freeMinHeap
  free(openSet->fscore);
  free(openSet);
}


int main(){
  printf("A* search session.\n");

  Graph *g = createGraph(5);

  /* addEdge(g, 0, 1, 4); */
  /* addEdge(g, 1, 2, 5); */
  /* addEdge(g, 1, 3, 1); */
  /* addEdge(g, 2, 4, 3); */

  addEdge(g, 0, 1, 4);
  addEdge(g, 1, 3, 2);
  addEdge(g, 0, 2, 5);
  addEdge(g, 2, 3, 1);
  addEdge(g, 3, 4, 2);
  addEdge(g, 1, 4, 7);


  displayGraph(g);

  //For to use A* search we need knownlegde that help to apply heuristic based
  Point coords[5] = {
    {0, 0},   // Node 0
    {2, 1},   // Node 1
    {1, 3},   // Node 2
    {3, 4},   // Node 3
    {5, 5}    // Node 4
  };

    
  A_StarSearch(g, 0, 4, coords);
  
  freeGraph(g);
  
  return 0;
}
