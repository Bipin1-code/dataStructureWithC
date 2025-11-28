/*
  What is Johnson's algorithm?
  Johnson's Algorithm is a graph algorithm used to
  find the shortest paths between all pairs of vertices
  in a weighted directed graph, even if it contains negative
  edge weights - as long as there are no negative cycles.  
 */

//Note: Some ot the codeline like ajdVlist is not needed, it's experiment code
//Code run perfectly, just adapt as your problem needs

#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex{
  int vName;
  int weight;
  struct Vertex *next;
} Vertex;

Vertex* createVertex(int n, int weight){
  Vertex *v = (Vertex*)malloc(sizeof(Vertex));
  if(!v){
    perror("Memory allocation failed for Vertex.\n");
    return NULL;
  }
  v->vName = n;
  v->weight = weight;
  v->next = NULL;

  return v;
}

typedef struct{
  int src;
  int dest;
  int weight;
} Edge;

typedef struct{
  int nVertex;
  Vertex **adjVList;
  int edgeCount;
  int edgeLCapacity;
  Edge *edgeList;
} Graph;

Graph* createGraph(int numOfVtx){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("Memory allocation failed for Graph.\n");
    return NULL;
  }
  g->nVertex = numOfVtx;
  g->edgeCount = 0;
  g->edgeLCapacity = (numOfVtx > 4) ? numOfVtx : 4;
  g->edgeList = (Edge*)malloc(g->edgeLCapacity * sizeof(Edge));
  g->adjVList = (Vertex**)malloc(numOfVtx * sizeof(Vertex*));
  if(!(g->edgeList) || !(g->adjVList)){
    free(g->edgeList);
    free(g->adjVList);
    free(g);
    return NULL;
  }
  for(int i = 0; i < numOfVtx; i++){
    g->adjVList[i] = NULL; 
  }
  
  return g;
}

//Here we can update EdgeList array because Graph having Vertex (V) can have more edge (E) than V
void recordNewEdgeToEList(Graph *g, Edge *newE){
  if(g->edgeCount == g->edgeLCapacity){
    g->edgeLCapacity *= 2;
    Edge *t = (Edge*)realloc(g->edgeList, g->edgeLCapacity * sizeof(Edge));
    if(!t){
      perror("Memory reallocation for Edgelist failed.\n");
      return;
    }
    g->edgeList = t;
  }
  g->edgeList[g->edgeCount++] = *newE;
}

void addEdgeInGraph(Graph *g, int src, int dest, int cost){
  if (src < 0 || src >= g->nVertex || dest < 0 || dest >= g->nVertex) {
    fprintf(stderr, "addEdgeInGraph: vertex out of range (%d -> %d)\n", src, dest);
    return;
  }
  Vertex *newV = createVertex(dest, cost);
  if(!newV) return;
  newV->next = g->adjVList[src];
  g->adjVList[src] = newV;

  Edge newE = {src, dest, cost};
  recordNewEdgeToEList(g, &newE);
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->nVertex; i++){
    Vertex *cur = g->adjVList[i];
    while(cur){
      Vertex *next = cur->next;
      free(cur);
      cur = next;
    }
  }
  free(g->adjVList);
  free(g->edgeList);
  free(g);
}

void printGraph(const Graph *g){
  printf("Graph adjacency list:\n");
  for(int i = 0; i < g->nVertex; i++){
    printf("%d -> ", i);
    Vertex *cur = g->adjVList[i];
    while(cur){
      printf("(%d, w=%d) ", cur->vName, cur->weight);
      cur = cur->next;
    }
    printf("\n");
  }

  printf("\nEdges:\n");
  for(int i = 0; i < g->edgeCount; i++){
    printf("%d -> %d (w=%d)\n",
	   g->edgeList[i].src,
	   g->edgeList[i].dest,
	   g->edgeList[i].weight);
  }
}

int addDummyVertex(Graph *g){
  int oldV = g->nVertex;
  int newV = oldV + 1;

  Vertex **tmpAdj = realloc(g->adjVList, newV * sizeof(Vertex*));
  if(!tmpAdj){
    perror("realloc adjVList");
    return -1;
  }
  g->adjVList = tmpAdj;
  g->adjVList[oldV] = NULL;

  int needE = g->edgeCount + oldV;
  if(needE > g->edgeLCapacity){
    int nc = g->edgeLCapacity;
    if(nc < needE) nc *= 2;
    Edge *tmpE = realloc(g->edgeList, nc * sizeof(Edge));
    if(!tmpE){
      perror("realloc edgeList for dummy");
      return -1;
    }
    g->edgeList = tmpE;
    g->edgeLCapacity = nc;
  }
 
  for(int v = 0; v < oldV; v++){
    g->edgeList[g->edgeCount].src = oldV;
    g->edgeList[g->edgeCount].dest = v;
    g->edgeList[g->edgeCount].weight = 0;
    g->edgeCount++;
    
    Vertex *node = createVertex(v, 0);
    node->next = g->adjVList[oldV];
    g->adjVList[oldV] = node;
  }
  
  g->nVertex = newV;
  return oldV;
}

int BellmanFord(Graph *g, int start, int *h){
  
  for(int i = 0; i < g->nVertex; i++){
    h[i] = 0x3f3f3f3f;
  }
  h[start] = 0;

  //first loop relaxation
  for(int it = 0; it < g->nVertex - 1; it++){
    int changed = 0;
    //inner loop relaxed edge
    for(int e = 0; e < g->edgeCount; e++){
      int u = g->edgeList[e].src;
      int v = g->edgeList[e].dest;
      int w = g->edgeList[e].weight;

      if(h[u] != 0x3f3f3f3f && h[u] + w < h[v]){
	h[v] = h[u] + w;
	changed = 1;
      }
    }
    if(!changed) break;
  }
  //detect negative cycle
  for(int i = 0; i < g->edgeCount; i++){
    int u = g->edgeList[i].src;
    int v = g->edgeList[i].dest;
    int w = g->edgeList[i].weight;

    if(h[u] != 0x3f3f3f3f && h[u] + w < h[v])
      return 0;
  }
  return 1;
}

typedef struct{
  int node;
  int dist;
} HeapItem;

typedef struct{
  HeapItem *a;
  int size;
  int cap;
} MinHeap;

MinHeap* createMinHeap(int cap){
  MinHeap *mh = (MinHeap*)malloc(sizeof(MinHeap));
  if(!mh){
    perror("Memory allocation failed for minheap used in Dijkstra.\n");
    return NULL;
  }
  mh->a = (HeapItem*)malloc((cap + 5) * sizeof(HeapItem));
  mh->size = 0;
  mh->cap = cap + 5;

  return mh;
}

void heapFree(MinHeap *h){
  free(h->a);
  free(h);
}

void heapItemSwap(HeapItem *x, HeapItem *y){
  HeapItem t = *x;
  *x = *y;
  *y = t;
}

void heapPush(MinHeap *mh, int node, int dist){
  if(mh->size + 1 >= mh->cap){
    mh->cap *= 2;
    mh->a = (HeapItem*)realloc(mh->a, mh->cap * sizeof(HeapItem));
  }
  int i = ++mh->size;
  mh->a[i].node = node;
  mh->a[i].dist = dist;
  while(i > 1){
    int p = i/2;
    if(mh->a[p].dist <= mh->a[i].dist) break;
    heapItemSwap(&(mh->a[p]), &(mh->a[i]));
    i = p;
  }
}

int heapEmpty(MinHeap *mh){
  return mh->size == 0;
}

HeapItem heapPop(MinHeap *mh){
  HeapItem ret = mh->a[1];
  mh->a[1] = mh->a[mh->size--];
  int i = 1;
  while(1){
    int l = 2 * i;
    int r = 2 * i + 1;
    int smallest = i;
    if(l <= mh->size && mh->a[l].dist < mh->a[smallest].dist)
      smallest = l;
    if(r <= mh->size && mh->a[r].dist < mh->a[smallest].dist)
      smallest = r;
    if(smallest == i) break;
    heapItemSwap(&(mh->a[i]), &(mh->a[smallest]));
    i = smallest;
  }
  return ret;
}

void Dijkstra(Graph *g, int start, int *distance){
 
  for(int i = 0; i < g->nVertex; i++){
    distance[i] = 0x3f3f3f3f;
  }
  distance[start] = 0;

  MinHeap *mh = createMinHeap(g->nVertex);
  heapPush(mh, start, 0);

  while(!heapEmpty(mh)){
    HeapItem it = heapPop(mh);
    int u = it.node;
    int d = it.dist;
    if(d != distance[u]) continue;
    //loop of adjVList force us to sync the edgelist and adjVlist
    //this cost as extra time (E*E)
    /* for(Vertex *cur = g->adjVList[u]; cur != NULL; cur = cur->next){ */
    /*   int v = cur->vName; */
    /*   int w = cur->weight; */
    /*   if(distance[u] + w < distance[v]){ */
    /* 	distance[v] = distance[u] + w; */
    /* 	heapPush(mh, v, distance[v]); */
    /*   } */
    /* } */
    //We do this instead
    for(int e = 0; e < g->edgeCount; e++){
      if(g->edgeList[e].src == u){
	int v = g->edgeList[e].dest;
	int w = g->edgeList[e].weight;
	if(distance[u] + w < distance[v]){
	  distance[v] = distance[u] + w;
	  heapPush(mh, v, distance[v]);
	}
      }
    }
  }
  heapFree(mh);
}

void reweightEdges(Graph *g, int *h){
  for(int e = 0; e < g->edgeCount; e++){
    int u = g->edgeList[e].src;
    int v = g->edgeList[e].dest;
    int w = g->edgeList[e].weight;

    long long nw = (long long)w + (long long)h[u] - (long long)h[v];
    if (nw < 0) nw = 0;
    g->edgeList[e].weight = (int)nw;
  }

  // Sync adjacency lists to match edgeList
  //Uncomment this only when you using adjVList for update or relaxing distance
  /* for(int u = 0; u < g->nVertex; u++){ */
  /*   Vertex *cur = g->adjVList[u]; */
  /*   while(cur){ */
  /*     int v = cur->vName; */
  /*     //Find matching edge in edgelist */
  /*     for (int e = 0; e < g->edgeCount; e++) { */
  /* 	if (g->edgeList[e].src == u && g->edgeList[e].dest == v) { */
  /* 	  cur->weight = g->edgeList[e].weight; */
  /* 	  break; */
  /* 	} */
  /*     } */
  /*     cur = cur->next; */
  /*   } */
  /* } */
}

int JohnsonAlgo(Graph *g, int *result, int V){
  //Step 1: Add dummy vertex q=5 with weight 0
  int originalV =  addDummyVertex(g);
  if(originalV == -1){
    printf("Failed to add dummy vertex\n");
    return 0;
  }

  //step 2: Run BellmanFord from q = 5
  int *h = (int*)malloc(g->nVertex * sizeof(int));
  if(!h) return 0;
  
  if(!BellmanFord(g, originalV, h)){
    printf("Negative cycle detected! Cannot compute shortest paths.\n");
    free(h);
    return 0;
  }

  //step 3: Reweight all edges
  reweightEdges(g, h);
  
  //step 4: Run Diskstra on each vertex;
  int *distance = (int*)malloc(g->nVertex * sizeof(int));
  if(!distance) {
    free(h);
    return 0;
  }
  
  for(int s = 0; s < originalV; s++){
    Dijkstra(g, s, distance);
    
    //step 5: Convert distance back (undo the reweighting):
    for(int v = 0; v < originalV; v++){
      if(distance[v] >= 0x3f3f3f3f){
	result[s * V + v] = 0x3f3f3f3f;
      }else{
	long long val = (long long)distance[v] - (long long)h[s] + (long long)h[v];
	if(val > 0x7fffffff)
	  result[s * V + v] = 0x3f3f3f3f;
	else
	  result[s * V + v] = (int)val;
      }
    }
  }
  
  free(distance);
  free(h);
  return 1;
}

int main(){
  printf("Johnson's Algorithm session:\n");

  Graph *g = createGraph(5);

  addEdgeInGraph(g, 0, 1, 5);
  addEdgeInGraph(g, 0, 2, 3);
  addEdgeInGraph(g, 1, 3, 6);
  addEdgeInGraph(g, 2, 3, 7);
  addEdgeInGraph(g, 3, 4, -2);

  printGraph(g);

  int *allPairs = (int*)malloc(5 * 5 * sizeof(int));
  if(!JohnsonAlgo(g, allPairs, 5)){
    printf("Negative cycle detected; cannot comput all-pairs shortest paths.\n");
    free(allPairs);
    freeGraph(g);
    return 1;
  }

  printf("All-pairs shortest paths (original weights):\n");
  for (int s = 0; s < 5; ++s) {
    for (int v = 0; v < 5; ++v) {
      if (allPairs[s * 5 + v] >= 0x3f3f3f3f)
	printf(" INF ");
      else
	printf("%4d ", allPairs[s * 5 + v]);
    }
    printf("\n");
  }

  free(allPairs); 
  freeGraph(g);
  
  return 0;
}
