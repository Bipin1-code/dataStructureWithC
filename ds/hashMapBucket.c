 /* This program is mainly focus on making bucket for the Hash Map to overcome the collision. */
/* Collision meant with, "when two or more key-value pairs map to the same bucket." */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Table_Size 10
//Node to store a key-value pair
typedef struct Node{
  char *key;
  int value;
  struct Node *next;
} Node;

//Bucket represents the head of the linked list 
typedef struct Bucket{
  Node *head; //Points to the first Node in the bucket
} Bucket;

//HashMap represents the entire hash map
typedef struct HashMap{
  Bucket *table[Table_Size]; //table array which type is bucket (linked list)
} HashMap;

//A function to create a new key-value pair node
Node* createNode(const char *key, int value){
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->key = strdup(key);
  newNode->value = value;
  newNode->next = NULL;
  return newNode;
}

//Function adds new key-value pair to a bucket, using chaining to handle collisions.
void addToBucket(Bucket *bucket, const char *key, int value){
  Node *current = bucket->head;
  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      current->value = value;
      return;
    }
    current = current->next;  
  }
  Node *newNode = createNode(key, value);
  newNode->next = bucket->head;
  bucket->head = newNode;
}

//This function retrieves the value for a given key
int searchInBucket(Bucket *bucket, const char *key){
  Node *current = bucket->head;
  while(current != NULL){
    if (strcmp(current->key, key) == 0){
      return current->value;
    }
    current = current->next;
  }
  printf("Key '%s' not found.\n", key);
  return -1;
}
//A hash function: this one is simple but you can make it with your own style as your goal needs
size_t hash(const char *key){
  size_t hashValue = 0;
  for(size_t i = 0; key[i] != '\0'; i++){
    hashValue += key[i];
  }
  return hashValue % Table_Size;
}

//Initialize the hash map and allocates memory for the buckets
void initializeHashMap(HashMap *map){
  for(size_t i = 0; i < Table_Size; i++){
    map->table[i] = (Bucket*)malloc(sizeof(Bucket));
    map->table[i]->head= NULL;
  }
}

//this function to insert key-value pairs into the hashMap
void insert(HashMap *map, const char *key, int value){
  size_t index = hash(key);
  Node *current = map->table[index]->head;

  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      current->value = value;
      return;
    }
    current = current->next;
  }

  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->key = strdup(key);
  newNode->value = value;
  newNode->next = map->table[index]->head;
  map->table[index]->head = newNode;
}

//searches for a key in the hashmap and return the associated value
int search(HashMap *map, const char *key){
  size_t index = hash(key);
  Node *current = map->table[index]->head;

  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      return current->value;
    }
    current = current->next;
  }
  printf("Key \"%s\" not found.\n", key);
  return -1;
}

int main(){
  /* Bucket bucket;  */
  /* bucket.head = NULL; */

  /* addToBucket(&bucket, "apple", 100);  */
  /* addToBucket(&bucket, "ae", 10);  */
  /* addToBucket(&bucket, "aple", 1); */

  /* printf("Value of 'apple': %d \n", searchInBucket(&bucket, "apple")); */
  /* printf("Value of 'ae': %d \n", searchInBucket(&bucket, "ae")); */
  /* printf("Value of 'aple': %d \n", searchInBucket(&bucket, "aple")); */

  HashMap map;
  initializeHashMap(&map);
  insert(&map, "apple", 100); 
  insert(&map, "ae", 10); 
  insert(&map, "aple", 1);

  printf("Value of 'apple': %d \n", search(&map, "apple"));
  printf("Value of 'ae': %d \n", search(&map, "ae"));
  printf("Value of 'aple': %d \n", search(&map, "aple"));
  return 0;
}
