/*
  What we have to accomplish in this code session?
  -> We have to print the paths of cycle if detected in a graph.

  //Roadmap:
  Create World-> create cities
  //Give Eagle view to world
  Create Road between Cities
  //Add visiter who want to travel and explore it
    But that visiter need a World map (hence we need stack) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int curCity;
  int totalCities;
  char **ExpC;
} Map;

Map* createMap(int tc){
  Map *m = (Map*)malloc(sizeof(Map));
  if(!m){
    perror("Failed to create map.\n");
    return NULL;
  }
  m->curCity = -1;
  m->totalCities = tc;
  m->ExpC = (char**)malloc(tc * sizeof(char*));
  for(int i = 0; i < tc; i++){
    m->ExpC[i] = (char*)calloc(20,  sizeof(char));
  }
  return m;
}

int isEmpty(Map *m){
  return m->curCity == -1;
}

int isFull(Map *m){
  return m->curCity == (m->totalCities - 1);
}

void markVC(Map *m, const char *cityName){
  if(isFull(m)){
    puts("Congrats! \n\
You have explored whole cities of this world.\n\
Someone, trying to scam you!\n\
Be aware of fake city and inform to EMPEROR.\n");
    return;
  }
  m->curCity++;
  strncpy(m->ExpC[m->curCity], cityName, 20);
}

char* checkCurCity(Map *m){
  if(isEmpty(m)){
    puts("You have not marked a single explore city.\n");
    return NULL;
  }
  return m->ExpC[m->curCity];
}

char* removeCurCity(Map *m){
  if(isEmpty(m)){
    puts("You have nothing to remove.\n");
    return NULL;
  }
  char *erase = m->ExpC[m->curCity];
  m->curCity--;
  return erase;
}

typedef struct City{
  char cNam[20];
  struct City *next;
} City;
//Let's make it simple world
typedef struct{
  int numfc; //number of cities
  City **ngbC; //negibours cities
  int *visitedC; //visited Cities tracker
  char **label; //this is gives name to city [Register your city name]
  int *recStack;
} World;

City* createCity(const char *cName){
  City *newCity = (City*)malloc(sizeof(City));
  if(!newCity){
    perror("Land allotment for newCity failed.\n");
    return NULL;
  }
  strncpy(newCity->cNam, cName, 20);
  newCity->next = NULL;

  return newCity;
}

World* createWorld(int nfC){
  World *world = (World*)malloc(sizeof(World));
  if(!world){
    perror("World Demision doesn't exist.\n");
    return NULL;
  }
  world->numfc = nfC;
  world->ngbC = (City**)malloc(nfC * sizeof(City*));
  world->visitedC = (int*)calloc(nfC, sizeof(int));
  world->recStack = (int*)calloc(nfC, sizeof(int));
  world->label = (char**)malloc(nfC * sizeof(char*));
  if(!(world->ngbC) || !(world->visitedC) || !(world->label)){
    perror("Memory allocation failed inside World creation.\n");
    free(world->ngbC);
    free(world->visitedC);
    free(world);
    return NULL;
  }
  for(int i = 0; i < nfC; i++){
    world->ngbC[i] = NULL;
    world->label[i] = (char*)calloc(20, sizeof(char));
  }
  return world;
}

int getCityIdx(World *w, const char *cityName){
  int index = -1;
  for(int i = 0; i < w->numfc; i++){
    if(w->label[i][0] == '\0'){
      strncpy(w->label[i], cityName, 20);
      index = i;
      break;
    }else if(strncmp(w->label[i], cityName, 20) == 0){
	index = i;
	break;
      }
  }
  return index;
}

void createRoad(World *w, char *oC, char *nC){
  int oCIdx = getCityIdx(w, oC);
  if(oCIdx == -1){
    perror("Index label is not avaiable for to allote!\n");
    return;
  }
  getCityIdx(w, nC); //no need of error check for this now
  
  City *c = createCity(nC);
  c->next = w->ngbC[oCIdx];
  w->ngbC[oCIdx] = c;
}

void displayWorld(World *w){
  printf("[Index]     [City]     [Paths]\n");
  for(int i = 0; i < w->numfc; i++){
    printf("[%3d] :  [%7s] ->", i, w->label[i][0] ? w->label[i] : "[EMPTY]");
    City *c = w->ngbC[i];
    while(c){
      printf(" [%7s] -> ", c->cNam);
      c = c->next;
    }
    printf(" NULL\n");
  }
}

void dfsOn(World *w, Map *m, int idx, int path[], int depth){
  w->visitedC[idx] = 1;
  w->recStack[idx] = 1; //this is recrusive stack for to trace back edge;
  path[depth] = idx; //this is stack trace of DFS calls
  
  City *tc = w->ngbC[idx];
  
  while(tc != NULL){
    int index = getCityIdx(w, tc->cNam);
    if(w->recStack[index] == 1){
      printf("Cycle Detect: %s\n", w->label[index]);
      for(int i = 0; i <= depth; i++){ 
	if(path[i] == index){
	  for(int j = i; j <= depth; j++){
	    printf("%s -> ", w->label[path[j]]);
	  }
	  printf("%s\n", w->label[index]);
	  break;
	}
      }
    }else if(w->visitedC[index] == 0){
      dfsOn(w, m, index, path, depth + 1);
    }
    tc = tc->next;
  }
  w->recStack[idx] = 0;
  markVC(m, w->label[idx]);
}

void topoSort(World *w, Map *m){
  int path[w->numfc];
  for(int i = 0; i < w->numfc; i++){
    if(w->label[i][0] != '\0' && w->visitedC[i] == 0){
      dfsOn(w, m, i, path, 0);
    }   
  }
}

void displayMap(Map *m){
  for(int i = 0; i <= m->curCity; i++){
    printf("%s ", m->ExpC[i]);
  }
}

void freeMap(Map *m){
  for(int i = 0; i < m->totalCities; i++){
    free(m->ExpC[i]);
  }
  free(m->ExpC);
  free(m);
}

void freeWorld(World *w){
  for(int i = 0; i < w->numfc; i++){
    City *temp = w->ngbC[i];
    while(temp){
      City *fC = temp->next;
      free(temp);
      temp = fC;
    }
    free(w->label[i]);
  }
  free(w->label);
  free(w->visitedC);
  free(recStack);
  free(w->ngbC);
  free(w);
}

int main(){

  World *w = createWorld(6);

  createRoad(w, "A", "B");
  createRoad(w, "B", "C");
  createRoad(w, "C", "D");
  createRoad(w, "D", "B");
  createRoad(w, "C", "E");
  createRoad(w, "E", "F");
  createRoad(w, "F", "A");
   
  displayWorld(w);

  puts("Toposort:\n");
  Map *m = createMap(w->numfc);

  topoSort(w, m);
  displayMap(m);

  freeMap(m);
  freeWorld(w);
  
  return 0;
}

