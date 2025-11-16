/*
  What is Floyd Warshall Algorithm?
  -It's an all-pairs shortest path algorithm.
  -Meaning: It finds the shortest paths between every pair of nodes in a weighted graph.
  -It works on direacted and weighted graphs
  (with positive or negative edge weights, but no negative cycles).  
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int nVt;
  //int **adjM; //non-contiguous memory
  int *adjM; //contiguous memory
} Graph;

void initialGraph(Graph *g){
   for(int i = 0; i < g->nVt; i++){
    for(int j = 0; j < g->nVt; j++){
      if(i == j){
	//g->adjM[i][j] = 0; //for non-contiguous memory
	g->adjM[i * (g->nVt) + j] = 0;
	continue;
      }
      // g->adjM[i][j] = 0x3f3f3f3f; // for non-contiguous memory
      g->adjM[i * (g->nVt) + j] = 0x3f3f3f3f;
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
  //for non-contiguous way
  /* g->adjM =(int**)malloc(nV * sizeof(int*)); */
  /* if(!g->adjM){ */
  /*   perror("Memory allocation Failed for Graph Matrix.\n"); */
  /*   free(g); */
  /*   return NULL; */
  /* } */
  /* for(int i = 0; i < nV; i++){ */
  /*   g->adjM[i] = (int*)calloc(nV, sizeof(int)); */
  /* } */

  //for contiguous way
  g->adjM = (int*)calloc(nV * nV, sizeof(int));
  if(!(g->adjM)){
    perror("Memory allocation Failed for Graph Matrix.\n");
    free(g);
    return NULL;
  }
  
  initialGraph(g);
  return g;
}

void updateGraph(Graph *g, int row, int col, int cost){
  if (row >= 0 && row < g->nVt && col >= 0 && col < g->nVt) {
    if (row == col) {
      printf("Warning: Cannot update diagonal element (%d,%d). It must remain 0.\n", row, col);
      return;
    }
    //g->adjM[row][col] = cost; //for non-contiguous memory
    g->adjM[row * (g->nVt) + col] = cost;
  } else {
    printf("Error: Index out of bounds.\n");
  }
}

void displayGraph(Graph *g){
  for(int i = 0; i < g->nVt; i++){
    for(int j = 0; j < g->nVt; j++){
      //printf("%d ", g->adjM[i][j]);//for non-contiguous memory
      printf("%10d ", g->adjM[i * (g->nVt) + j]);
    }
    printf("\n");
  }
}

void freeGraph(Graph *g){
  //loop needs for non-contiguous memory
  /* for(int i = 0; i< g->nVt; i++){ */
  /*   free(g->adjM[i]); */
  /* } */
  free(g->adjM);
  free(g);
}

void floydWarshallAlgo(Graph *g){
  for(int k = 0; k < g->nVt; k++){
    for(int  i = 0; i < g->nVt ; i++){
      for(int j = 0; j < g->nVt; j++){
	if(g->adjM[i * g->nVt + j] > g->adjM[i * g->nVt + k] + g->adjM[k *g->nVt + j]){
	  g->adjM[i * g->nVt + j] = g->adjM[i * g->nVt + k] + g->adjM[k * g->nVt + j];
	}
      }
    }
  } 
}

int main(){
  printf("Floyd-Warshall Algorithm session:\n");

  Graph *g = createGraph(5);
  
  updateGraph(g, 0, 1, 10);
  updateGraph(g, 0, 4, 5);
  updateGraph(g, 1, 2, 1);
  updateGraph(g, 2, 3, 4);
  updateGraph(g, 3, 0, 7);
  updateGraph(g, 4, 3, 2);
  updateGraph(g, 4, 1, 3);
  
  puts("\nAfter Update:");
  displayGraph(g);
  
  floydWarshallAlgo(g);
  puts("\nFloyd Warshall Algo implement:");
  displayGraph(g);
  
  freeGraph(g);
  
  return 0;
}
