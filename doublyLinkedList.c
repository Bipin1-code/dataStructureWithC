
/* Doubly Linked List */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
  int data;
  struct Node *prev;
  struct Node *next;
} Node;

void deleteNode(Node **head, int value){
  if((*head)->data == value){
    Node *temp = *head;
    *head = (*head)->next;
    if(*head != NULL){
      (*head)->prev = NULL;
    }
    free(temp);
    return;
  }
  Node *current = *head;
  while(current->next != NULL){
    if(current->next->data == value){
      Node *temp = current->next;
      current->next = current->next->next;
      if(current->next != NULL){
        current->next->prev = current;
      }  
      free(temp);
      return;
    }
    current = current->next;
  }
  printf("%d value is not present in Doubly Linked List.\n", value);
  
}

//This is responsible for to insert new node after a value of node which is present in D-linked List
void insertAfterNode(Node **head, int afterValue, int valueOfNewNode){
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = valueOfNewNode;
  newNode->prev = NULL;
  newNode->next = NULL;
  if(*head == NULL){
    printf("The Doubly Linked List which provide is empty.\n");
    free(newNode);
    return;
  }
  Node *current = *head;
  while( current != NULL){
    if(current->data == afterValue){
      newNode->prev = current; //this makes bond of new Node  with current node with the help of previous pointer of new node
      newNode->next = current->next; //this makes bond of new Node with current next node with the help of next pointer also
      if(current->next != NULL){
         current->next->prev = newNode;  //this makes bond of new Node with current next node with the help of previous pointer
         //this line also break direct connection of current and current next in term of previous pointer
      }                              
      current->next = newNode; //this makes bond of new Node with the current with the help of next pointer of current node
      //this line of code also breaks the direction connection of current node with current next node with the help of next pointer
      return;
    }
    current = current->next;
  }
  // If afterValue is not found in the list
  printf("Value %d not found in the Doubly Linked List.\n", afterValue);
  free(newNode);
}

void insertNodeAtBeginning(Node **head, int value){
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = value;
  newNode->prev = NULL;
  newNode->next = NULL;
 
  if(*head != NULL){
    newNode->next = *head;
    (*head)->prev = newNode;
  }
  *head = newNode;
}

void insertNodeAtEnd(Node **head, int value){
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = value;
  newNode->next = NULL;
  newNode->prev = NULL;
  if(*head == NULL){
   *head = newNode;
   return;
  }
  Node *current = *head;
  while(current->next != NULL){
    current = current->next;
  }
  current->next = newNode;
  newNode->prev = current;
}

void printListInForwardDir(Node *head){
  Node *current = head;
  while(current != NULL){
    printf("%d->", current->data);
    current = current->next;
  }
  printf("NULL\n");
}

void printListInBackwardDir(Node *head){
  if(head == NULL) return;
  Node *current = head;
  //First we need to be on last node
  while(current->next != NULL){
    current = current->next;
  }
  //Now we can do backward directional traversing which is unconventional
  //but we can do that because of previous pointer which state in the doubly linked list definition 
  while(current != NULL){
    printf("%d<-", current->data);
    current = current->prev;
  }
  printf("NULL\n");
}

int main(){
  Node *head = NULL;
  insertNodeAtBeginning(&head, 20);
  insertNodeAtBeginning(&head, 30);
  insertNodeAtBeginning(&head, 40);
  printListInForwardDir(head);
  printListInBackwardDir(head);
  insertNodeAtEnd(&head, 50);
  insertAfterNode(&head, 30, 60);
  //deleteNode(&head, 20);
  printListInForwardDir(head);
  printListInBackwardDir(head);
  return 0;
}
