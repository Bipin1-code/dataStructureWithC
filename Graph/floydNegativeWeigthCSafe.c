/*
  What is Floyd Warshall Algorithm?
  -It's an all-pairs shortest path algorithm.
  -Meaning: It finds the shortest paths between every pair of nodes in a weighted graph.
  -It works on direacted and weighted graphs
  (with positive or negative edge weights)
  -Now neagtive cycle is detect 
  
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int nVt;
  int *adjM;
  int *next; //next-hop matrix 
} Graph;

void initialGraph(Graph *g){
  for(int i = 0; i < g->nVt; i++){
    for(int j = 0; j < g->nVt; j++){
      if(i == j){
	g->adjM[i * (g->nVt) + j] = 0;
	g->next[i * g->nVt + j] = j;
      }else{
	g->adjM[i * (g->nVt) + j] = 0x3f3f3f3f;
	g->next[i * g->nVt + j] = -1;
      }
    }
  }
}

Graph* createGraph(int nV){
  Graph *g = (Graph*)malloc(sizeof(Graph));
  if(!g){
    perror("Memory allocation Failed for Graph Matrix.\n");
    return NULL;
  }
  g->nVt = nV;
  g->adjM = (int*)calloc(nV * nV, sizeof(int));
  g->next = (int*)calloc(nV * nV, sizeof(int));
  if(!(g->adjM) || !(g->next)){
    perror("Memory allocation Failed for Graph Matrix.\n");
    free(g->adjM);
    free(g->next);
    free(g);
    return NULL;
  }
  
  initialGraph(g);
  return g;
}

void updateGraph(Graph *g, int row, int col, int cost){
  if (row >= 0 && row < g->nVt && col >= 0 && col < g->nVt) {
    //we have negative cycle detection logic so we can omit this below if condition true case
    /* if (row == col) { */
    /*   printf("Warning: Cannot update diagonal element (%d,%d). It must remain 0.\n", row, col); */
    /*   return; */
    /* } */
    g->adjM[row * (g->nVt) + col] = cost;
    g->next[row * (g->nVt) + col] = col; //path begins with direct edge
  } else {
    printf("Error: Index out of bounds.\n");
  }
}

void displayGraph(Graph *g){
  for(int i = 0; i < g->nVt; i++){
    for(int j = 0; j < g->nVt; j++){
      printf("%10d ", g->adjM[i * (g->nVt) + j]);
    }
    printf("\n");
  }
}

void freeGraph(Graph *g){
  free(g->adjM);
  free(g->next);
  free(g);
}

void floydWarshallAlgo(Graph *g){
  int n = g->nVt;

  //Floyd-Warshall
  for(int k = 0; k < n; k++){
    for(int  i = 0; i < n ; i++){
      for(int j = 0; j < n; j++){

	int ik = g->adjM[i*n + k];
        int kj = g->adjM[k*n + j];
        int ij = g->adjM[i*n + j];

	
        if (ik == 0x3f3f3f3f || kj == 0x3f3f3f3f) continue;

        if (ij > ik + kj){
          g->adjM[i*n + j] = ik + kj;
          g->next[i*n + j] = g->next[i*n + k];
        }
      }
    } 
  }

  //Neagtive Cycle Detection
  for(int k = 0; k < n; k++){
    if(g->adjM[k * n + k] < 0){
      printf("Negative weight cycle detected at node %d\n", k);
      
      // For a negative cycle at node k:
      // Any path i -> ... -> k -> ... -> j is invalid because shortest paths
      // passing through k are undefined (can decrease infinitely).
      // Therefore, for every (i, j) pair where i can reach k and k can reach j,
      // mark next[i][j] = -1 to disable path reconstruction.

      for(int i = 0; i < n; i++){
	for(int j = 0; j < n; j++){
	  if(g->adjM[i * n + k] != 0x3f3f3f3f && g->adjM[k*n + j] != 0x3f3f3f3f)
	    g->next[i * n + j] = -1;
	}
      }
    }
  }
  
}

void displayNext(Graph *g){
  for(int i = 0; i < g->nVt; i++){
    for(int j = 0; j < g->nVt; j++){
      printf("%4d ", g->next[i * (g->nVt) + j]);
    }
    printf("\n");
  }
}

void reconstructPath(Graph *g, int u, int v){

  if(g->next[u * g->nVt + v] == -1){
    printf("No path from %d to %d\n", u, v);
    return;
  }

  printf("Path %d -> %d: ", u, v);
  printf("%d", u);

  while(u != v){
    u = g->next[u * g->nVt + v];
    printf(" -> %d", u);
  }
  printf("\n");
}

int main(){
  printf("Floyd-Warshall Algorithm session:\n");

  // test case 1: Passed
  Graph *g = createGraph(5);
  updateGraph(g, 0, 1, 10);
  updateGraph(g, 0, 4, 5);
  updateGraph(g, 1, 2, 1);
  updateGraph(g, 2, 3, 4);
  updateGraph(g, 3, 0, 7);
  updateGraph(g, 4, 3, 2);
  updateGraph(g, 4, 1, 3);

  //test case 2: (passed)
  /* Graph *g = createGraph(4); */
  /* updateGraph(g, 0, 1, 4); */
  /* updateGraph(g, 1, 2, 1); */
  /* updateGraph(g, 2, 3, -2); */
  /* updateGraph(g, 3, 1, -2); */
  /* updateGraph(g, 0, 3, 10); */

  //test case 3: (passed)
  /* Graph *g = createGraph(4); */
  /* updateGraph(g, 0, 1, 3); */
  /* updateGraph(g, 1, 2, -4); */
  /* updateGraph(g, 2, 3, 7); */

  //test case 4: (passed)
  /* Graph *g = createGraph(3); */
  /* updateGraph(g, 0, 1, 4); */
  /* updateGraph(g, 1, 2, 5); */
  /* updateGraph(g, 2, 0, -12);  */  // negative cycle
  

  //test case 5: (passed)
  /* Graph *g = createGraph(6); */

  /* updateGraph(g, 0, 1, 4); */
  /* updateGraph(g, 0, 2, 1); */
  /* updateGraph(g, 2, 1, -2); */
  /* updateGraph(g, 1, 3, 2); */
  /* updateGraph(g, 2, 3, 5); */

  /* updateGraph(g, 3, 4, 3); */
  /* updateGraph(g, 4, 5, -1); */
  /* updateGraph(g, 5, 3, -2);  */

  /* updateGraph(g, 0, 5, 20); */

  /* //test case 6: (passed) */
  /* Graph *g = createGraph(6); */

  /* // Region 1: Normal paths */
  /* updateGraph(g, 0, 1, 5); */
  /* updateGraph(g, 1, 5, 7); */
  /* updateGraph(g, 0, 5, 30); */

  /* updateGraph(g, 0, 2, 10);  // connects to cycle entry */
  /* updateGraph(g, 5, 4, 2);   // connects to cycle exit */

  /* // Region 2: Negative cycle (2 -> 3 -> 4 ->2) */
  /* updateGraph(g, 2, 3, 4);     // edge +4 */
  /* updateGraph(g, 3, 4, -8);    // edge -8 */
  /* updateGraph(g, 4, 2, 3);     // edge +3  (cycle total = -1) */

  /* // Region 3: Additional complexity */
  /* updateGraph(g, 1, 3, 20); */
  /* updateGraph(g, 3, 5, 1); */
  /* updateGraph(g, 4, 5, 6); */
  /* updateGraph(g, 5, 2, 5);   // creates complex loops */

  
  puts("\nAfter Update:");
  displayGraph(g);
  
  floydWarshallAlgo(g);
  puts("\nFloyd Warshall Algo implement:");
  displayGraph(g);

  puts("\nDisplay Next:");
  displayNext(g);

  printf("\nReconstructed Paths:\n");
  reconstructPath(g, 3, 2);
  reconstructPath(g, 0, 3);
  reconstructPath(g, 4, 2);
  
  freeGraph(g);
  
  return 0;
}
