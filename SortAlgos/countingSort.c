/* In this program we are doing Counting sort */
/* What is Counting Sort?
   Counting sort is a non-comparison-based sorting algorithm that sorts integers within a specific range.
   It works by counting the occurrences of each element in the input array and using that information
   to place elements directly in the sorted order.
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 10

//For to find the range(Max Value and Min Value) of the input array
void findRangeOfArray(int *arr, size_t size, int *setOfR){
  int max = arr[0];
  int min = arr[0];
  for(size_t i = 0; i < size; i++){
    if(max < arr[i]){
      max = arr[i];
    }
    else{
      if(min > arr[i])
        min = arr[i];  
    }
  }
  setOfR[0] = min;
  setOfR[1] = max;
}

//this function count the occurrence of each input value in input array and update countArray
void insertDataInCountArray(int *arr, size_t sOfArr, int *ctArr, size_t sOfC, int min){
  for(size_t i = 0; i < sOfC; i++){
    ctArr[i] = 0;
  }
  for(size_t i = 0; i < sOfArr; i++){
    ctArr[arr[i] - min]++;
  }
}

//For to insert the data into the array
void insertDataInArray(int *arr, size_t size ){
  printf("Enter the values of the array");
  fflush(stdout);
  for(size_t i = 0; i < size; i++){
    scanf("%d", &arr[i]);
  }
}

//For to print or display the array
void printArray(int *arr, size_t size){
  for(size_t i = 0; i < size; i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void sortArray(int *arr, size_t size){
  int minMax[2]; //this array contain min and max value of input array which is arr in our case
  findRangeOfArray(arr, size, minMax);
  int min = minMax[0], max = minMax[1];
  size_t rng = min - max + 1; //add 1 for to include the min value place also
  //creating auxiliary array for to store the occurrence of each value in the input array
  int *countArr = (int *)malloc(rng * sizeof(int));
  if(countArr == NULL){
    printf("Memory allocation failed for countArr!\n");
    return;
  }
  insertDataInCountArray(arr, size, countArr, rng, min);
  size_t e = 0; //this is the index of sorted array
  //We are using countArr
  //travesing countArr which contains frequency of distinct input value of the input array (arr)
  for(size_t i  = 0; i < rng; i++){
    size_t freq  = countArr[i]; //frequency of num
    int num = i + min; //as you knew i and min relation(i is just differences of min-max's range)
    if(num < 1){
      //if num is less then 1 meant that this number not recieved as input so skip it
      continue;
    }else{
      for(size_t k = freq; k > 0; k--){
        arr[e++] = num;
        //here why we are not applying bound for e,
        //because countArr data sum is alway equal to the size of input array
        //no need to worry about limit-out of bound exception error
       }
     }
  }
  free(countArr);
}
int main(){
  int arr[MAX_LEN];
  insertDataInArray(arr, MAX_LEN);
  printArray(arr, MAX_LEN);
  sortArray(arr, MAX_LEN);
  printArray(arr, MAX_LEN);
  return 0;
}
