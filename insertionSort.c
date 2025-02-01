
//let's do insertion sort

#include <stdio.h>

void printArray(int *arr, size_t size){
  printf("Here is your array:\n");
  for(size_t i = 0; i < size; i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void insertSort(int *arr, size_t s){
  for(size_t i = 1; i < s; i++){
    size_t t = i;
    for(size_t j = t; j-- > 0;){
      if(arr[t] < arr[j]){
        arr[t] ^= arr[j];
        arr[j] ^= arr[t];
        arr[t] ^= arr[j];
        t--;
      }else{
        break;
      }  
    }
  }
}

int main(){
  int num[] = {5, 6, 4, 2, 9, 1, 0};
  size_t size = sizeof(num)/sizeof(num[1]);
  printf("Size of the array is %zu.\n", size);
  insertSort(num, size);
  printArray(num, size);
  return 0;
}
