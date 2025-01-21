/* Date Comparison: */
/* Define a struct Date with fields day, month, and year. Write a program to: */
/* Input two dates from the user. */
/* Compare them to find which one is earlier. */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct Date{
  uint8_t day;
  uint8_t month;
  uint16_t year;
} Date;

void createDate( Date *d){
  printf("Date Formate must be like this:\n day-month-year \n for instance: 2-6-2000\n");
  printf("Enter the day in number (1-31):\n");
  fflush(stdout);
  scanf("%hhu", &d->day);
  printf("Enter the month in number (1-12):\n");
  fflush(stdout);
  scanf("%hhu", &d->month);
  printf("Enter the year in number (1000+):\n");
  fflush(stdout);
  scanf("%hu", &d->year);
}

int main(){
  size_t n = 2;
  Date *date =(Date *)malloc( n * sizeof(Date));
  if(date == NULL){
    printf("Memory allocation failed.\n");
    return 1;
  }
  for(size_t i = 0; i < n; i++ ){
    createDate(&date[i]);  
  }
  for(size_t i = 0; i < n; i++ ){
    printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 "\n", date[i].day, date[i].month, date[i].year);  
  }
  //We can make separate function for this which increase the modularity, inside logic will remain same
  //like: compareDate(&date[0], &date[i]);
  //Use int 1, 0, -1 as return type and according to result print the three case:
  // Earlier(1), after (-1) and same (0)
  for(size_t i = 0; i < n - 1 ;i++){
    size_t j = i + 1;
    if(date[i].year < date[j].year){
      printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 " is earlier\n", date[i].day, date[i].month, date[i].year);
    }
    else if(date[i].year > date[j].year){
      printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 " is earlier\n", date[j].day, date[j].month, date[j].year);
    }
    else{
      //one condition remains which one is same year
      if(date[i].month < date[j].month){
         printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 " is earlier\n", date[i].day, date[i].month, date[i].year);
       }else if(date[i].month > date[j].month){
         printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 " is earlier\n", date[j].day, date[j].month, date[j].year);
      }else{
        if(date[i].day < date[j].day){
          printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 " is earlier\n", date[i].day, date[i].month, date[i].year);
        }else if(date[i].day > date[j].day){
          printf("Date: %" PRIu8 "/%" PRIu8 "/%" PRIu16 " is earlier\n", date[j].day, date[j].month, date[j].year);
        }
        else{
          printf("Both dates are same.\n");
        }
      }
    }
  }
  free(date);
  return 0;
}

//In next we will try to get the region time without using date file or library.
