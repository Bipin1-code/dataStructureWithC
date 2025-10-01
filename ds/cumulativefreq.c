
///Things need to be clear before doing Counting sort and Radix sort
//below program: first we make array and fill it at run time
//and use counting frequency algo for to find out the frequency of distinct value of the input array
//after that we update the counting frequency array(countArr) with cumulative frequency data
/*
  Note: You can use count frequency and cumulative frequency algo directly in any array
        I did fellowing way, because these are very important steps of count sort and Radix Sort
        for to make them stable and find out the ending position of each value in the sorted array.
        You will find out while doing Radix and count sort.
       */

#include <stdio.h>
#include <stdlib.h>
#define MAX 6

void fillcountArr(int *arr, size_t len, int *cArr, size_t len1, int minV){
  //fill the array with 0
  for(size_t i = 0; i < len1; i++){
    cArr[i] = 0;
  }
  //filling countArr with the frequency of distinct number which is present in the input array(arr)
  for(size_t i = 0; i < len; i++){
    cArr[arr[i] - minV]++;
  }
}

void insertDataInArray(int *arrvar, size_t length){
  printf("Enter the data in array:\n");
  fflush(stdout);
  for(size_t i = 0; i < length; i++){
    scanf("%d", &arrvar[i]);
  }
}

void printArray(int *arrvar, size_t length){
  printf("Here is your array:\n");
  for(size_t i = 0; i < length; i++){
    printf("%d ", arrvar[i]);
  }
  printf("\n");
}

void cumulativeFreqArr(int *arrvar, size_t length){
  int max = arrvar[0], min = arrvar[0];
  for(size_t i = 0; i < length; i++){
    if(max < arrvar[i]) max = arrvar[i];
    if(min > arrvar[i]) min = arrvar[i];
  }
  printf("Min : %d \n Max : %d\n", min, max);
  size_t range = max - min + 1;
  //countArr for to count the freq of the distinct value present in input array(numbers)
  int *countArr = (int *)malloc(range * sizeof(int));
  if(countArr == NULL){
    printf("Memeory allocation failed!\n");
    return;
  }
  fillcountArr(arrvar, length, countArr, range, min);
  //printing count frequency
  printArray(countArr, range);
  
  //converting count frequency into cumulative frequency inside CountArr
  for(size_t i = 1; i < range; i++){
    //why i begin with 1; because cumulative is incorporating all data up to the present
   //i = 0 is initial state there is nothing to incorporate as pervious data
    countArr[i] = countArr[i] + countArr[i - 1];
  }
  //printing cumulative frequency
  printArray(countArr, range);

  free(countArr); //don't forget to free the memory
}

int main(){
  int numbers[MAX];
  insertDataInArray(numbers, MAX);
  printArray(numbers, MAX);
  cumulativeFreqArr(numbers, MAX);
  return 0;
}
