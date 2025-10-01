
//Linked List data structure with c

#include <stdio.h>
#include <stdLib.h>

typedef struct Node{
  int data;
  struct Node *next;
} Node;

void insertAtBeginning( Node **head, int data){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = *head;
  *head = newNode;
}

void insertAtEnd(Node **head, int data){
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  Node *current = *head;
  if(current == NULL){
    current = newNode;
    newNode->next = NULL;
    return;
  }
  while(current->next != NULL){
    current = current->next;
  }
  current->next = newNode;
  newNode->next = NULL;
}

void deleteNode(Node **head, int deleteValue){
  
  if((*head)->data == deleteValue){
    Node *temp = *head;
    *head = (*head)->next;
    free(temp);
    return;
  }
  Node *current = *head;
  while(current->next != NULL){ 
    if(current->next->data == deleteValue){
      Node *temp = current->next;
      current->next = current->next->next;
      free(temp);
      return;
    }
    current = current->next;
  }
  printf("%d doesn't exist in linkedlist.\n", deleteValue);
}

void printLinkedList(Node *head){
  Node *current = head;
  while(current != NULL){
    printf("%d->", current->data);
    current = current->next;
  }
  printf("NULL\n");
}
int main(){
  Node *head = (Node*)malloc(sizeof(Node));
  Node *second = (Node*)malloc(sizeof(Node));

  head->data = 1;
  head->next = second;

  second->data = 2;
  second->next = NULL;
  insertAtBeginning(&head, 0);
  insertAtEnd(&head, 3);
  printLinkedList(head);
  deleteNode(&head, 5);
  printLinkedList(head);
  free(head);
  free(second);
  return 0;
}
