
///In this program we are trying to learn about stack.

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

typedef struct Stack{
  int arr[MAX]; //this is an array of int type which size is 10 defined with macro
  int top; //top is a tracker which track the top index having item.
} Stack;

void initializeStack(Stack *var ){ 
   var->top = -1;
} 

 void push(Stack *var, int data){
   if(var->top == MAX - 1){
     printf("Stack is full!\n");
     return;
   }
   var->top++;
   var->arr[var->top] = data; 
}

void pull(Stack *var){
  if(var->top == -1){
    printf("Stack is empty!\n");
  }else{
    printf("%d index of data and data is %d.\n", var->top, var->arr[var->top]);
    var->top--; //because we are taking out the data present on top shell of the stack
  }
}

void peek(Stack *var){
  if(var->top == -1){
    printf("Stack is empty!\n");
  }else{
    //Here we are only seeing the data present on top shell of the stack
    printf("%d index of the data and data is %d.\n", var->top, var->arr[var->top]);
  }
}

//this function is bad because we updated top tracker position
//we may lose data and get data insert in wrong position because of updated top 
void isValuePresent(Stack *var, int data){
  if(var->top == -1){
    printf("Stack is empty!\n");
  }else{
    //solution: int tempTop = var->top //copy the top which make sure that we don't change the top position
    while(var->top > -1){
      if(var->arr[var->top] == data){
        printf("Yes! %d is present on %d index of the stack.\n", var->arr[var->top], var->top);
        //we can write data instead of "var->arr[var->top] == data"
        return;
      }
      var->top--;
    }
    printf("No! %d is not present in the stack.\n", data);
  }
}

void printStackData(Stack *var){
  if(var->top == -1){
    printf("Stack is empty!\n");
  }
  //As like isValuePresent function we can do same thing here
  //because we just want to display the stack data only
  int tempTop = var->top;
  /* while(var->top > -1){ */
  /*   printf("%d index data is %d\n", var->top, var->arr[var->top]); */
  /*   var->top--; */
  /* } */ //instead do this:
  while(tempTop > -1){
    printf("%d index data is %d\n", tempTop, var->arr[tempTop]);
    tempTop--;
  }
}

int main(){
  Stack num;
  initializeStack(&num);
  push(&num, 7);
  push(&num, 10);
  push(&num, 20);
  push(&num, 4);
  push(&num, 12);
  peek(&num);
  // printStackData(&num);
  //pull(&num);
  printStackData(&num);
  // isValuePresent(&num, 7);
  return 0;
}
