

/*
  What is cocktail sort?
  Cocktail sort is a upgraded version of "Bubble Sort",
  Cocktail sort, sort in both directions.
  Instead of going through the array in just one pass (left to right),
  it moves back and forth, which helps reduce the number of swaps in certain cases.
 */

#include <stdio.h>


void printArray(int *num, size_t len){
  for(size_t i = 0; i < len; i++){
    printf("%d ", num[i]);
  }
  printf("\n");
}

void swap(int *n1, int *n2){
  // ^ is XOR in which same bit result is 0 and different bit result is 1
  /*
    For instance: a = 1 in binary 001 and b = 2 in binary 010
                  As we knew, a = a + b = 1 + 2 = 3 similar with binary 001 ^ 010 = 011 //011 is 3
                        Now,  b = a - b = 3 - 2 = 1  binary 011 ^ 010 = 001
                              a = a - b = 3 - 2 = 1  binary 011 ^ 001 = 010
   */
   *n1 ^= *n2;
   *n2 ^= *n1;
   *n1 ^= *n2;
}

void cocktailSort(int *num, size_t l){
  size_t count = 1, start = 0, end = l;
  while(count > 0){
    count = 0;
    //every iteration one Max element is sort
    for(size_t i = start; i < end; i++){
      size_t j = i+1;
      if(num[i] > num[j]){
        swap(&num[i], &num[j]);
        /*
          why we are using address?
          Because swapping work but it doesn't reflect on original array.
          This is basic but subtle concept of reference with pointer
        */ 
        count++;
      }
    }
    //every iteration one min element is sort
    if(count > 0){
      for(size_t i = end; i-- > 1;){
        size_t j = i - 1;
        if(num[i] < num[j]){
          swap(&num[i], &num[j]);
          /* num[i] ^= num[j]; */
          /* num[j] ^= num[i]; */
          /* num[i] ^= num[j]; */
          count++;
        }
      }
    }
    else{
      break;
    }
    //These loop completion sorts max value and min value to end and start position respectively
    //So, we should not iterate already sorted position for that we are updating end and start index:
    end--;
    start++;
  }
}

int main(){
  int num[] = {5, 3, 8, 4, 2};
  size_t size = sizeof(num)/sizeof(num[0]);
  cocktailSort(num, size);
  printArray(num, size);
  return 0;
}
