/*
  What is Radix Sort?
   Radix Sort is a non-comparative sorting algorithm that works by processing the individual digits or characters of the data,
   one place value at a time. It is particularly effective for sorting integers and fixed-length strings.
   Instead of comparing values directly, it groups the data by digits or characters.

 Steps in Radix sort(LSD Approach):
  1-> Find the Maximum value
  2-> Sort by each digit
     Main problem, "How to sort lSD and put each number in correct position?"
     Solution: Counting frequency and cumulative frequency are there for you.
  3->Combine buckets,
  here:
  Buckets refers to the place of digit in a number:
   for instance; 1234
   Thousand place = 1, hunderd = 2, Ten = 3, Unit = 4
*/

#include <stdio.h>
#include <stdlib.h>

#define LENGTH 6

int findMaxVal(int *arr, size_t size){
  int max = *arr;
  for(size_t i = 0; i < size; i++){
    if( max < arr[i]) max = arr[i];
  }
  return max;
}

void printArray(int *arrvar, size_t size){
  for(size_t i = 0; i < size; i++){
    printf("%d ", arrvar[i]);
  }
  printf("\n");
}

void updateCountArr(int *arrvar, size_t len, int *cArr, size_t clen, int position){
  for(size_t i = 0; i < clen; i++){
    cArr[i] = 0;
  }
  //count the frequency of each distinct number
  for(size_t i = 0; i < len; i++){
    int pDigit = (arrvar[i]/position) % 10;
    /* position is digit place (for e.g, unit, tenth, hundredth)
      arrvar[i]/position remove the position's digits which we already compute
      then its result gets divided with 10 but why
      because we are playing with digits 0-9
    */
    cArr[pDigit]++;
  }
  //Now update countArr for to fill cumulative frequency
  for(size_t i = 1; i < clen; i++){
    cArr[i] += cArr[i - 1];
  }
}

void radixSort(int *arrvar, size_t size, int max){
  int *countArr = (int *)malloc(10 * sizeof(int));
  if(countArr == NULL){
    printf("Memo allocation failed for countArr!\n");
    return;
  } 
  int pozson = 1;
  while(max/pozson > 0){
    updateCountArr(arrvar, size, countArr, 10, pozson);
    int *sortedArr = (int *)malloc(size * sizeof(int));
    if(sortedArr == NULL){
      printf("Memo allocation failed for sortedArr!\n");
      free(countArr);
      return;
    }
    //Iterate in reverse order to maintain stable sort
    for(size_t i = size; i-- > 0;){
      int pDigit = (arrvar[i]/pozson) % 10;
      sortedArr[--countArr[pDigit]] = arrvar[i];
      /* first: countArr[pDigit] compute
         suppose countArr[pDigit] = 3
         second: it decrease with 1 because we are doing prefx
       */
    }
    /*after this copy the sorted value to original array
     because we need for it to sort the other positions digit */
    for(size_t i = 0; i < size; i++){
      arrvar[i] = sortedArr[i];
    }
    free(sortedArr);
    pozson *= 10;
  }
  free(countArr);
}


void insertDataInArray(int *arrvar, size_t size){
  printf("Enter the data in array variable:\n");
  fflush(stdout);
  for(size_t i = 0; i < size; i++){
    scanf("%d", &arrvar[i]);
  }
}

int main(){
  int arr[LENGTH];
  insertDataInArray(arr, LENGTH);
  printArray(arr, LENGTH);
  int maxVal = findMaxVal(arr, LENGTH);
  printf("Max value is: %d\n", maxVal);
  fflush(stdout); //all correct 
  radixSort(arr, LENGTH, maxVal);
  printArray(arr, LENGTH);
  return 0;
}

/* syntax:  for(size_t i = size; i-- > 0;){ body}
   why we did this way?
   size_t in unsigned data type
   when i become less than zero size_t makes it SIZE_MAX which is very large value and
   i will never be less than 0
   for(size_t i = size; i >= 0; i--) is wrong due to i will always true case for condition
   but
   in this  syntax: for(size_t i = size; i-- > 0;)
   first: i value assigned which size
   second: i value checked
   third: i value decrease by 1
   fourth: i value used in body of the loop
    while
    in this syntax: for(size_t i = size; i > 0; i--)
    third and fourth switch its place
 */
