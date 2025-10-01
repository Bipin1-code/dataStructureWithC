/*
  What is merge sort?
  Merge Sort is a divide and conquer sort algorithm that works by
  repeatedly splitting an array into smaller subarrays, sorting them,
  and then merging them back together in order.
*/

#include <stdio.h>

void printArray( int *arr, size_t len){
  printf("Here is your array:\n");
  for(size_t i = 0; i < len; i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void mergeSortArray(int *a, size_t b, size_t e, size_t mid){
  size_t sL = mid - b; //t = 7, 0 + 7/ 2  = 3 => mid - b = 3 - 0 = 3 =>sL
  size_t sR = e - mid; //7 - 3 = 4
  //Creating temp arrays for to store the item of left and right separatly 
  int tL[sL], tR[sR]; //tL for temp Left and tR for temp Right

  //Copying the array's item to temp array
  /* Note: Each time array gets divide and simultanenously it get store until the recursive doesn't end.*/
  for(size_t i = 0; i < sL; i++) tL[i] = a[b + i];
  for(size_t i = 0; i < sR; i++) tR[i] = a[mid + i];

  //Now begin to merge the copied temp array's items to the orginal array by comparing and sorting
  size_t i = 0;
  size_t j = 0;
  size_t k = b;
  while(i < sL && j < sR){
    if(tL[i] <= tR[j]){
      a[k] = tL[i];
      i++;
    }else{
      a[k] = tR[j];
      j++;
    }
    k++;
  }
  //if i have more element then j then that remain needs to be add in original array
  while( i < sL){
    a[k++] = tL[i++];
  }
  //similary for j case
  while(j < sR){
    a[k++] = tR[j++];
  }
  printf("\nMerge sort array:\n");
  printArray(a, k);
  printf("\n");
}

void divideArray(int *ar, size_t begin, size_t end){
  if(end - begin <= 1) return;
  size_t mid = (begin + end) / 2; // we used begin for to get accurate mid, when beginning index is not 0
    divideArray(ar, mid, end); //for right subarray
    divideArray(ar, begin, mid);//for left subarray
    printf("left:");
    printArray(ar + begin, mid - begin); // for to see the left side array
    printf("Right:");
    printArray(ar + mid, end - mid); // for to see the right side array
    mergeSortArray(ar, begin, end, mid); //calling to merge the sort array
}


void mergeSort(int *ar, size_t l){
  divideArray(ar, 0, l); //for to do merge sort we need to divide each item of the array
}

int main(){
  int num[] = {7, 5, 2, 9, 1};
  size_t size = sizeof(num)/sizeof(num[0]);
  printArray(num, size);//original array
  mergeSort(num, size); //callinf merge sort
  printArray(num, size); //0, 1, 2, 3, 4, 11, 12
  return 0;
}

/*Check-out the this for to now how it work with top-down approach:
  output:
   Here is your array:
 4 12 3 11 1 2 0 
 left:Here is your array:
 2 
 Right:Here is your array:
 0 
 
 Merge sort array:
 Here is your array:
 4 12 3 11 1 0 2 
 
 left:Here is your array:
 11 
 Right:Here is your array:
 1 
 
 Merge sort array:
 Here is your array:
 4 12 3 1 11 
 
 left:Here is your array:
 1 11 
 Right:Here is your array:
 0 2 
 
 Merge sort array:
 Here is your array:
 4 12 3 0 1 2 11 
 
 left:Here is your array:
 12 
 Right:Here is your array:
 3 
 
 Merge sort array:
 Here is your array:
 4 3 12 
 
 left:Here is your array:
 4 
 Right:Here is your array:
 3 12 
 
 Merge sort array:
 Here is your array:
 3 4 12 
 
 left:Here is your array:
 3 4 12 
 Right:Here is your array:
 0 1 2 11 
 
 Merge sort array:
 Here is your array:
 0 1 2 3 4 11 12 
 
 Here is your array:
 0 1 2 3 4 11 12 
 */
