
/* Library System: */
/* Define a struct Book with fields nameau and author. Write a program to: */
/* Add new books to the library. */
/* Search for a book by name or author. */
/* Display all available books. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max 100
#define File_Nam "libData.txt"
#define Table_size 10

typedef struct Book{
  char name[Max];
  char author[Max];  
} Book;

typedef struct Node{
  char *key;
  Book value;
  struct Node *next;
} Node;

typedef struct Bucket{
  Node *head;
} Bucket;

typedef struct HashMap{
  Bucket *table[Table_size];
} HashM;

void readString(char *n){
  size_t i = 0;
  char ch;
  while((ch = getchar()) != '\n' && ch != EOF){
    n[i++] = ch;
    if(i > Max - 1) break;
  }
  n[i] = '\0';
}

size_t hashKey(const char *str){
  size_t hashkey = 0, result = 0;
  for(size_t i = 0; str[i] != '\0'; i++){
    hashkey += str[i];
  }
   result = hashkey % Table_size;
   return result;
}

void initializeHashMap(HashM *map){
  for(size_t i= 0; i < Table_size; i++){
    map->table[i] = (Bucket *)malloc(sizeof(Bucket));
    map->table[i]->head = NULL;
  }
}

Node* createNode(const char *str,  Book *b){
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->key = strdup(str);
  newNode->value = *b;
  newNode->next = NULL;
  return newNode;
}

void insertInMap(HashM *map, const char *key, Book *b){
  size_t index = hashKey(key);
  Node *current = map->table[index]->head;
  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      current->value = *b;
      return;
    }
    current = current->next;
  }
  Node *newNode = createNode(key, b);
  newNode->next = map->table[index]->head;
  map->table[index]->head = newNode;
}

Book* searchInMap(HashM *map, const char *key){
  size_t index = hashKey(key);
  Node *current = map->table[index]->head;
  while(current != NULL){
    if(strcmp( current->key, key) == 0){
       return &current->value;       
    }
    current = current->next;
  }
  printf("Book '%s' is not found.\n", key);
  return NULL;
}

//CreateBook Function
Book* createBook( Book *b){
  printf("Enter the name of the book:\n");
  fflush(stdout);
  // fgets(b->name ,sizeof((b)->name), stdin);
  readString(b->name); //same operation as fgets function
  printf("Enter the author name of the book:\n");
  fflush(stdout);
  readString(b->author);
  return b;
}

void freeHashMap(HashM *map) {
    for (size_t i = 0; i < Table_size; i++) {
        Node *current = map->table[i]->head;
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
        free(map->table[i]);
    }
    free(map);
}

int main(){
  FILE *file = fopen(File_Nam, "r");
  if(file == NULL){
    printf("ERROR: File %s is not opening!\n", File_Nam);
    return 1;
  }
  HashM *map = (HashM *)malloc(sizeof(HashM));
  initializeHashMap(map);
  char buffer[256];
  while(fgets(buffer, sizeof(buffer), file) != NULL){
    char bookName[Max];
    char authorName[Max];
    if(sscanf(buffer, "Book Name : %[^\n]", bookName) == 1
       && fgets(buffer, sizeof(buffer), file) != NULL
       && sscanf(buffer, "Author: %[^\n]", authorName) == 1){
      Book book;
      strcpy(book.name, bookName);
      strcpy(book.author, authorName);
      insertInMap(map, book.name, &book);
      printf("Book Name: %s\n", book.name);
      printf("Author: %s\n", book.author);
     }
    else{
      printf("%s", buffer);
    }
  }
  fclose(file);
 
  char desire;
  printf("Do you want to add new book?\n Press: 'y' = yes or 'n' = no\n");
  fflush(stdout);
  scanf("%c", &desire);
  getchar();
  if(desire == 'y'){
    file = fopen(File_Nam, "a");
    if(file == NULL){
      printf("ERROR: File %s is not opening!\n", File_Nam);
      return 1;
    }
    Book *book = (Book *)malloc(sizeof(Book));
    if(book == NULL){
      printf("Memory allocation failed!\n");
      return 1;
    }
    createBook(book);  
    insertInMap(map, book->name, book);
    
    fprintf(file, "Book Name: %s\nAuthor: %s\n", book->name, book->author);
    printf("The new book you just created:\nBook's name: %s\nAuthor's name: %s\n", book->name, book->author);
    fclose(file);
    free(book);
    printf("File closed successfully.\n");
  }
  printf("Do you wanna search book?\n Press: 'y' = yes or 'n' = no\n");
  fflush(stdout);
  scanf("%c", &desire);
  getchar();
  if(desire == 'y'){
    char bookNam[Max];
    printf("Enter the book name:\n");
    fflush(stdout);
    fgets(bookNam, sizeof(bookNam), stdin);
    strtok(bookNam, "\n");
    Book *foundBook = searchInMap(map, bookNam);
    if(foundBook != NULL){
      printf("Book is found:\n");
      printf("Book Name: %s\n Author Name: %s\n", foundBook->name, foundBook->author);
    }
    else{
       printf("Book is not found.\n");
    }
  }
  freeHashMap(map);
  return 0;
}
