#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int start;
    int end;
    int cost;
} Edge;

typedef struct{
    int nNode;       
    int cEdge;
    int edgeCapacity;
    Edge *listEdge;
} Graph;

Graph* createGraph(int n, int initialEdgeCapacity){
    Graph *g = (Graph*)malloc(sizeof(Graph));
    if (!g) {
        perror("Failed to allocate graph");
        return NULL;
    }
    g->nNode = n;
    g->cEdge = 0;
    g->edgeCapacity = initialEdgeCapacity > 0 ? initialEdgeCapacity : 4;
    g->listEdge = (Edge*)malloc(g->edgeCapacity * sizeof(Edge));
    if (!g->listEdge) {
        perror("Failed to allocate edges");
        free(g);
        return NULL;
    }
    return g;
}

void addEdge(Graph *g, int start, int end, int cost){
    if (g->cEdge == g->edgeCapacity) {
        int newCapacity = g->edgeCapacity * 2;
        Edge *tmp = (Edge*)realloc(g->listEdge, newCapacity * sizeof(Edge));
        if (!tmp) {
            perror("Failed to reallocate edges");
            return;
        }
        g->listEdge = tmp;
        g->edgeCapacity = newCapacity;
    }
    g->listEdge[g->cEdge++] = (Edge){start, end, cost};
}

// Free graph memory
void freeGraph(Graph *g){
    if (!g) return;
    free(g->listEdge);
    free(g);
}

int compareEdges(const void *a, const void *b) {
    const Edge *ea = (const Edge*)a;
    const Edge *eb = (const Edge*)b;

    if (ea->cost < eb->cost) return -1;
    if (ea->cost > eb->cost) return 1;
    return 0;
}

void displayGraph(Graph *g){
    printf("Graph with %d nodes and %d edges:\n", g->nNode,
	   g->cEdge);
    for (int i = 0; i < g->cEdge; i++) {
        printf("Edge %d: %d --(%d)--> %d\n",
	       i,
	       g->listEdge[i].start,
	       g->listEdge[i].cost,
	       g->listEdge[i].end);
    }
}

void makeSet(Graph *g, int parent[], int rank[]){
  for(int i = 0; i < g->nNode; i++){
    parent[i] = i;
    rank[i] = 0;
  }
}

int find(int parent[], int x){
  if(parent[x] != x)
    parent[x] = find(parent, parent[x]);
  return parent[x];
}

void unionSet(int parent[], int rank[], int x, int y){
  int rootX = find(parent, x);
  int rootY = find(parent, y);
  if(rootX == rootY) return;

  if(rank[rootX] < rank[rootY]){
    parent[rootX] = rootY;
  }else if(rank[rootX] > rank[rootY]){
    parent[rootY] = rootX;
  }else{
    parent[rootY] = rootX;
    rank[rootX]++;
  }
}

void kruskalAlgo(Graph *g, int parent[], int rank[]){
  int totalMSTCost = 0;
  for(int i = 0; i < g->cEdge; i++){
    Edge cur = g->listEdge[i];
    int u = cur.start;
    int v = cur.end;
    if(find(parent, u) != find(parent, v)){
      printf("MST Edge: %d -- %d (cost %d)\n", cur.start, cur.end, cur.cost);
      totalMSTCost += cur.cost;
      unionSet(parent, rank, u, v);
    }
    else
      continue;
  }
  printf("Total MST cost = %d\n", totalMSTCost);
}

int main(){
  printf("Kruskal ALgorithm session:\n");

  Graph *g = createGraph(5, 0);
  addEdge(g, 0, 1, 4);
  addEdge(g, 1, 2, 3);
  addEdge(g, 2, 4, 2);
  addEdge(g, 2, 3, 1);

  displayGraph(g);

  qsort(g->listEdge, g->cEdge, sizeof(Edge), compareEdges);

  int parent[g->nNode];
  int rank[g->nNode];
  makeSet(g, parent, rank);
  
  puts("\nEdges sorted by cost:");
  displayGraph(g);
  
  puts("\n Parent Array:");
  for(int i = 0; i < g->nNode; i++){
    printf("%2d ", parent[i]);
  }
  
  puts("\n Kruskal Algorithm:");
  kruskalAlgo(g, parent, rank);

  puts("\n After Kruskal Implement parent array:");
  for(int i = 0; i < g->nNode; i++){
    printf("%2d ", parent[i]);
  }
  printf("\n");
    
  freeGraph(g);

  return 0;
}
