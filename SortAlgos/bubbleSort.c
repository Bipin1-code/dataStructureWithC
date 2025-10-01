
///let try to do bubble sort
#include <stdio.h>

void bubbleSort(int *n, size_t s){
  size_t c = 1;
  while( c > 0){
    c = 0;
    for(size_t i = 0, j = 1; j < s; i++, j++){
      if(n[i] > n[j]){
        //swap
        n[i] ^= n[j];
        n[j] ^= n[i];
        n[i] ^= n[j];
        c++;
      }
    }
  }
}

void printArray(int *n, size_t z){
  for(size_t i = 0; i < z; i++){
    printf("%d ", n[i]);
  }
  printf("\n");
}

int main(){
  int num[] = {9, 1, 4, 8, 2, 7};
  size_t len = sizeof(num)/sizeof(num[0]);
  bubbleSort(num, len);
  printArray(num, len);
  return 0;
}
