//We are going to make counting sort by using cumulative frequency
//By using cumulative frequency we make it more stability while working with large data
//cumulative frequency also help to find the end position(or index) of each value in the sorted array

#include <stdio.h>
#include <stdlib.h>

void printArray(int *arrPar, size_t len){
  printf("Here is your array:\n");
  for(size_t i = 0; i < len; i++){
    printf("%d ", arrPar[i]);
  }
  printf("\n");
}

void insertDataInArray(int *arrPar, size_t len){
  printf("Enter the data of the array:\n");
  fflush(stdout);
  for(size_t i = 0; i < len; i++){
    scanf("%d", &arrPar[i]);
  }
}

void countingSort(int *arrPar, size_t len){
  //step 1: Find the max value present in the array
  int max = *arrPar;
  int min = *arrPar;
  for(size_t i = 0; i < len; i++){
    if(max < arrPar[i]) max = arrPar[i];
    if(min > arrPar[i]) min = arrPar[i];
  }
  size_t range = max - min + 1;
  //step 2: create the array for to count frequency of each distinct number present in the input array
  int *cArr = (int *)malloc(range * sizeof(int));
  if(cArr == NULL){
    printf("Memory allocation failed for cArr!\n");
    return;
  }
  //step 3: fill the count arr with 0
  for(size_t i = 0; i < range; i++){
    cArr[i] = 0;
  }
  //step 4: Now we need to fill count array with frequency of each distinct number present in the input array
  for(size_t i = 0; i < len; i++){
    cArr[arrPar[i] - min]++;  //"arrPar[i] - min" is to make indices equivalent to range numbers min to max 
  }
  //step 5: Now we need to update count array with cumulative frequency
  for(size_t i = 1; i < range; i++){
    cArr[i] += cArr[i-1];
  }
  printArray(cArr, range);
  //step 6: create a temp arr for to store the sorted array
  int *sortedArr = (int *)malloc(len * sizeof(int));
  if(sortedArr == NULL){
    printf("Memory allocation failed for sortedArr!\n");
    return;
  }
  //step 7: final one for to put the actual value to its ending index in the sorted array
  for(size_t i = len; i-- > 0; ){
    int cIdx = (arrPar[i] - min) % range;
    sortedArr[--cArr[cIdx]] = arrPar[i];
  }
  printArray(sortedArr, len);
  free(sortedArr);
  free(cArr); //don't forget to free the memory
}

int main(){
  printf("Hello, welcome! to counting sort program.\nBut for to do counting sort you need make an array.\n");
  int size;
  printf("Enter the size of the array:\n");
  fflush(stdout);
  scanf("%d", &size);
  int *arr = (int *)malloc(size * sizeof(int));
  if(arr == NULL){
    printf("Memory allocation failed for arr!\n");
    return 1;
  }
  insertDataInArray(arr, size);
  printArray(arr, size);
  countingSort(arr, size);
  free(arr);
  return 0;
}
