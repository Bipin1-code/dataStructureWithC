
//Let's do selection sort

#include <stdio.h>

void printArray(int *arr, size_t s){
  printf("Here is your array:\n");
  for(size_t i = 0; i < s; i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}
void selectionSort(int *arr, size_t s){
  for(size_t i = 0; i < s-1; i++){
    size_t j = i + 1, t = i; // t for to track the index of min value
    int min = arr[i];
    while(j < s){
      if(min > arr[j]){
        min = arr[j];
        t = j;
      }
      j++;
    }
    //swap
    if( t != i){
      arr[t] = arr[i];
      arr[i] = min;
    }  
  }
}

int main(){
  int num[] = {12, 1, 7, 8, 5, 1, 7, 8, 7, 7, 8, 4};
  size_t size = sizeof(num)/sizeof(num[0]);
  printf("The size of array is %zu.\n", size);
  selectionSort(num, size);
  printArray(num, size);
  return 0;
}
