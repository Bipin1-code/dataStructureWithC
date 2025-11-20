/*
  Prim's algorithm is a way to build MST (Minimum spanning tree)
  -Prim's algorithm is used in dense graph whereas Kruskal's algo use for sparse graph
  -Prim use node to create MST by adding one node at a time.
  -Prim use visited array for to detect or prevent cycle because MST should have only N-1 egde and N nodes.
  -Prim use Priority Queue (min heap)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int vertex;
  int key;
} HeapNode;

HeapNode* createHeapNode(int v, int k){
  HeapNode *hN = (HeapNode*)malloc(sizeof(HeapNode));
  hN->vertex = v;
  hN->key = k;

  return hN;
}

//Step 1: Create heap
typedef struct{
  int size;
  int capacity;
  HeapNode **arr;
  int *pos;
} MinHeap;

MinHeap* createMinHeap(int capacity){
  MinHeap *mh = (MinHeap*)malloc(sizeof(MinHeap));
  if(!mh){
    perror("ERROR: Memory allocation for min heap failed.\n");
    return NULL;
  }
  
  mh->size = 0;
  mh->capacity = capacity;
  mh->arr = (HeapNode**)malloc(capacity * sizeof(HeapNode*));
  if(!(mh->arr)){
    perror("ERROR: minheap's dynamic array memory allocation failed.\n");
    free(mh);
    return NULL;
  }
  mh->pos = (int*)malloc(capacity * sizeof(int));
  if(!mh->pos){
    perror("ERROR: minheap pos allocation failed.\n");
    free(mh->arr);
    free(mh);
    return NULL;
  }
  for(int i = 0; i < capacity; ++i){
      mh->arr[i] = NULL;
      mh->pos[i] = -1;
  }
  return mh;
}

void swap(HeapNode **a, HeapNode **b){
  HeapNode *t = *a;
  *a = *b;
  *b = t;
}

int isEmpty(MinHeap *mh){
  return mh->size == 0;
}

void minHeapify(MinHeap *mh, int idx){
  
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;;
  int smallest = idx;

  if(left < mh->size && mh->arr[left]->key < mh->arr[smallest]->key)
    smallest = left;

  if(right < mh->size && mh->arr[right]->key < mh->arr[smallest]->key)
    smallest = right;

  if(smallest != idx){
    HeapNode *smallNode = mh->arr[smallest];
    HeapNode *idxNode = mh->arr[idx];

    mh->pos[smallNode->vertex] = idx;
    mh->pos[idxNode->vertex] = smallest;
    
    swap(&(mh->arr[smallest]), &(mh->arr[idx]));
    minHeapify(mh, smallest);
  } 
}

HeapNode* extractMin(MinHeap *mh){
  if(mh->size <= 0) return NULL;
  
  HeapNode *root = mh->arr[0];
  HeapNode *lastNode = mh->arr[mh->size - 1];
  
  mh->arr[0] = lastNode;

  mh->pos[lastNode->vertex] = 0;
  mh->pos[root->vertex] = -1;
 

  mh->size--;
  
  //Heapify: Percolate means slowly develop the idea (heap structure again) 
  minHeapify(mh, 0);

  return root;
}

void decreaseKey(MinHeap *mh, int vertex, int newKey){
  int index = mh->pos[vertex]; 
  mh->arr[index]->key = newKey;

  //Restoring heap
  int i = index;
  while(i != 0 && mh->arr[(i - 1) / 2]->key > mh->arr[i]->key){

    mh->pos[mh->arr[i]->vertex] = (i - 1) / 2;
    mh->pos[mh->arr[(i - 1) / 2]->vertex] = i;
    
    swap(&(mh->arr[i]), &(mh->arr[(i - 1) / 2]));
    
    i = (i - 1) / 2;
  }
}

int inMinHeap(MinHeap *mh, int v){
  if(v < 0 || v >= mh->capacity) return 0;
  return (mh->pos[v] >= 0 && mh->pos[v] < mh->size);
}

//Step 2: Create Graph
typedef struct Node{
  int value;
  int cost;
  struct Node *next; 
} Node;

typedef struct{
  int nNode;
  int *visited;
  Node **adjList;
} Graph;

Node* createNode(int value, int cost){
  Node *n = (Node*)malloc(sizeof(Node));
  if(!n){
    perror("Memory allocation failed for Node.\n");
    return NULL;
  }
  n->value = value;
  n->cost = cost;
  n->next = NULL;

  return n;
}

Graph* createGraph(int numOfNode){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("Memory allocation failed for Graph.\n");
    return NULL;
  }
  g->nNode = numOfNode;
  g->visited = (int*)calloc(numOfNode, sizeof(int));
  g->adjList = (Node**)calloc(numOfNode, sizeof(Node*));
 
  return g;
}

void addEdge(Graph *g, int src, int dest, int cost){
  Node *newN = createNode(dest, cost);
  newN->next = g->adjList[src];
  g->adjList[src] = newN;

  Node *newN2 = createNode(src, cost);
  newN2->next = g->adjList[dest];
  g->adjList[dest] = newN2;
}

void displayGraph(Graph *g){
  for(int i = 0; i < g->nNode; i++){
    Node *n = g->adjList[i];
    printf("[%d]:{ %d-", i, i);
    while(n){
      printf("(%d)->%d-", n->cost, n->value);
      n = n->next;
    }
    printf("NULL }\n");
  }
}

void freeGraph(Graph *g){
  for(int i = 0; i < g->nNode; i++){
     Node *n = g->adjList[i];
     Node *t;
     while(n){
       t = n;
       n = n->next;
       free(t);
     }
  }
  free(g->adjList);
  free(g->visited);
  free(g);
}

typedef struct{
  int start;
  int end;
  int cost;
} MST;

void primAlgo(Graph *g){

  int parent[g->nNode];
  int key[g->nNode];

  MinHeap *mh = createMinHeap(g->nNode);
 
  for(int i = 0; i < g->nNode; i++){
    parent[i] = -1;
    key[i] = 0x3f3f3f3f;
    mh->arr[i] = createHeapNode(i, key[i]);
    mh->pos[i] = i;
  }
  
  key[0] = 0;
  mh->arr[0]->key = 0;
  mh->size = g->nNode;

  while(!isEmpty(mh)){
    HeapNode *minNode = extractMin(mh);
    int u = minNode->vertex;

    Node *neighbour = g->adjList[u];
    while(neighbour != NULL){
      int v = neighbour->value;

      if(inMinHeap(mh, v) && neighbour->cost < key[v]){
	key[v] = neighbour->cost;
	parent[v] = u;
	decreaseKey(mh, v, key[v]);
      }
      neighbour = neighbour->next;
    }
  }

  puts("MST edges (Prim):\n");
  int total = 0;
  for(int i = 1; i < g->nNode; i++){
    printf("%d -- %d (cost %d)\n", parent[i], i, key[i]);
    total += key[i];
  }
  printf("Total MST Cost = %d\n", total);

  for(int i = 0; i < g->nNode; i++){
    free(mh->arr[i]);
  }
  free(mh->arr);
  free(mh->pos);
  free(mh);
}


int main(){
  printf("Prim's algorithm session.\n");
 
  Graph *g = createGraph(5);

  addEdge(g, 0, 1, 3);
  addEdge(g, 1, 2, 5);
  addEdge(g, 2, 3, 7);
  addEdge(g, 3, 4, 3);
  addEdge(g, 1, 3, 4);
  
  displayGraph(g);

  primAlgo(g);
  
  freeGraph(g);
  
  return 0;
}
