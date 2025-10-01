

/* Basic Of file System */

#include <stdio.h>
#include <string.h>

int main(){
  FILE *file;
  file = fopen("newFile.txt", "r");   // r is mode we have other modes also w, a, r+, w+, a+
  if(file == NULL){
    printf("Error occur while opening file!\n");
    return 1;
  }
   printf("File opened successfully.\n"); 
  /* fputs("Adding new content int the file.\n", file); */
  /*
    fseek(file, 0, SEEK_SET);
    // We need fseek() for to move the pointer to the initial position or at the beginning after writing process is done
    // while write in file make sure u have w or a mode,
    // After writing we need to read what we write so we move pointer at the beginning.
  */

  //these are for to storage
  char buffer[100]; 
  char name[20];
  
  while(fgets(buffer, sizeof(buffer), file) != NULL){
    if(sscanf(buffer, "Name: %s", name) == 1){
      printf("Read Name: %s\n", name);
    }else{
      printf("Read: %s", buffer);
    }
  }
  /* why we need fgets?
     fgets function reads line and store it to the buffer(buffer have limit).
     fgets reads until buffer not filled when it fill at the end '\0' add and print it out
     why we use sscanf?
     sscanf is used for to read the input formated from the string
     Here, sscanf(buffer, "Name: %s", name) == 1 , we parsed the name,  we are parsed 1 input formated so if it successfull it should be equal to 1;
           if( scanf(buffer, "fName: %s lName %s", fname, lname) == 2) we write 2 for comapred result, because we want two parsed input result;
           else if it's not formated string then print as it is;
           if we don't handle the formated input while reading we might get unexpected behaviour as output;
  */
  fseek(file, 15, SEEK_SET);
  long position = ftell(file); //ftell function tell the current position of the pointer, hence it name ftell 
  printf("The current position of the pointer is %ld\n", position);
  fgets(buffer, sizeof(buffer), file);
  printf("After position reset: %s", buffer);
  fclose(file);
  printf("File closed successfully.\n");
  return 0;
}
