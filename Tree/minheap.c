#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int size;
  int capacity;
  int *arr;
} MinHeap;

MinHeap* createHeap(int capacity){
  MinHeap *mh = (MinHeap*)malloc(sizeof(MinHeap));
  if(!mh){
    perror("ERROR: Memory allocation for min heap failed.\n");
    return NULL;
  }
  mh->size = 0;
  mh->capacity = capacity;
  mh->arr = (int*)calloc(capacity, sizeof(int));
  if(!(mh->arr)){
    perror("ERROR: minheap's dynamic array memory allocation failed.\n");
    free(mh);
    return NULL;
  }
  return mh;
}

void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

void heapInsert(MinHeap *h, int val){
  int i = h->size;
  if(h->size == h->capacity){
    puts("Heap is overflow");
    return;
  }
  h->arr[i] = val;
  h->size++;

  //this make sure we create heap by following min-heap rule:
  //Parent node <= child nodes
  while(i != 0 && h->arr[(i-1)/2] > h->arr[i]){
    swap(&(h->arr[(i-1)/2]), &(h->arr[i]));
    i = (i-1) / 2;
  }
}

int extractMin(MinHeap *h){
  if(h->size <= 0) return -1;
  int root = h->arr[0];
  h->arr[0] = h->arr[h->size - 1];
  h->size--;

  //Heapify: Percolate means slowly develop the idea (heap structure again) 
  int i = 0;
  while(1){
    int left = 2 * i + 1;
    int right = 2 * i + 2;;
    int smallest = i;

    if(left < h->size && h->arr[left] < h->arr[smallest])
      smallest = left;

    if(right < h->size && h->arr[right] < h->arr[smallest])
      smallest = right;

    if(smallest == i) break;

    swap(&(h->arr[i]), &h->arr[smallest]);
    i = smallest;
  }

  return root;
}

void decreaseKey(MinHeap *h, int index, int newVal){
  h->arr[index] = newVal;

  //Restoring heap
  int i = index;
  while(i != 0 && h->arr[(i-1) / 2] > h->arr[i]){
    swap(&(h->arr[(i-1) / 2]), &(h->arr[i]));
    i = (i - 1) / 2;
  }
}

void displayHeap(MinHeap *h){
  for(int i = 0; i < h->size; i++){
    printf("%d ", h->arr[i]);
  }
  printf("\n");
}

int main(){
  printf("Prim's algorithm session.\n");
  MinHeap *heap = createHeap(5);
  if(heap){
    puts("Heap canvas create Successfully.");
  }

  heapInsert(heap, 10);
  heapInsert(heap, 1);
  heapInsert(heap, 8);
  heapInsert(heap, 11);
  heapInsert(heap, 9);

  displayHeap(heap);

  decreaseKey(heap, 3, 4);
  int minVal = extractMin(heap);
  printf("MinValue =  %d\n", minVal);

  puts("After decreaseKey used:");
  displayHeap(heap);
  
  free(heap->arr);
  free(heap);
  return 0;
}
