/*
  We are going to solve the merge sort with bottom-up approach,
  In bottom up we start solving from bottom means with small one and
  adding the other small half solution with it. We go layer by layer
  with resultant solution so we don't have to backtrack for to get the
  whole problem solution at end.
  */

#include <stdio.h>
#include <stdlib.h>

void displayArray(int *arrvar, size_t l){
  printf("Here is your array:\n");
  for(size_t i = 0; i < l; i++){
    printf("%d ", arrvar[i]);
  }
  printf("\n");
}

void insertDataInArray(int *arr, size_t len){
  printf("Enter the data of your array:\n");
  fflush(stdout);
  for(size_t i = 0; i < len; i++){
    scanf("%d", &arr[i]);
  }
}

void merge(int *arr, int *temp, size_t left, size_t mid, size_t right){
  size_t i = left, j = mid + 1, k = left;
  //Merge the two halves into the temp array
  while( i <= mid && j <= right){
    if(arr[i] <= arr[j]){
      temp[k++] = arr[i++]; /*see we don't need swapping expicilty just compare and
                              do swapping at once while copy it to original array */
    }else{
      temp[k++] = arr[j++];
    }
  }
  //copy the remaining elements of the left half, if any
  while( i <= mid){
    temp[k++] = arr[i++];
  }
  while(j <= right){
    temp[k++] = arr[j++];
  }
  //copy the merge subarray back to original array
  for(i = left; i <= right; i++){
    arr[i]  = temp[i];
  }
}

void mergeSortArray(int *arr, size_t len) {
  //We are making temp array which going to help us to store the sort array
  int *temp = (int *)malloc(len * sizeof(int)); 
  if(temp == NULL){
    printf("Memory allocation failed!\n");
    return;
  }

  /*
    What is size?
    Size is the width of the subarray,
    each iteration we are updating it with multiply of two because we are working with pairs.
    When size => len (width of original array) it means our array sorting is done.
   */
  
  size_t size = 1;
  while( size < len){
    /*
      left is the starting index of first subarray,
       mid is the ending index of the first subarray,
       right is that ending index of second subarray.
       All key things of bottom-up depends on these above three factors
    */
    for (size_t left = 0; left < len - size; left += 2 * size) {
            size_t mid = left + size - 1;
            size_t right = (left + 2 * size - 1 < len) ? (left + 2 * size - 1) : (len - 1);
            
            // Merge the two halves
            merge(arr, temp, left, mid, right);
            //these lines are just to see the each  size ouput step by step
            printf("After: %zu \n", size); 
            displayArray(arr, len);
            printf("\n");
        }
    size *= 2;
  }
  free(temp);//don't forget to free up the memory
}


int main(){
  size_t size;
  printf("Enter the size of the array:\n");
  fflush(stdout);
  scanf("%zu", &size);
  int *num = (int *)malloc(size * sizeof(int));
  if(num == NULL){
    printf("Memory allocation failed!\n");
    return 1;
  }
  insertDataInArray(num, size);
  displayArray(num, size);
  mergeSortArray(num, size);
  displayArray(num, size);
  free(num);
  return 0;
}

 /* output:
   Enter the size of the array:
   7
   Enter the data of your array:
   8 1 4 2 5 6 0
   Here is your array:
      8 1 4 2 5 6 0 
   After: 1 
   Here is your array:
   1 8 4 2 5 6 0 
   
   After: 1 
   Here is your array:
   1 8 2 4 5 6 0 
   
   After: 1 
   Here is your array:
   1 8 2 4 5 6 0 
   
   After: 2 
   Here is your array:
   1 2 4 8 5 6 0 
   
   After: 2 
   Here is your array:
   1 2 4 8 0 5 6 
   
   After: 4 
   Here is your array:
   0 1 2 4 5 6 8 
   
   Here is your array:
   0 1 2 4 5 6 8
  */
