/*
Medium
Problem:
Read a text file and count how many unique words it contains (ignore punctuation, case-insensitive).
Skills: file I/O, tokenization, string comparison, arrays / linked list / hash logic (your choice).
*/

//Create a file.txt
//getlengthOfStr
//HashValueOfStr
//compareStr

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Buckets{
  size_t stC;
  char word[64];
  struct Buckets *next;
} Buckets;

typedef struct{
  Buckets buckArr[1031];
} HashMap;


size_t hashValue(const char *str){
  int primeBase = 31;
  size_t hashV = 0;
  int i = 0;
  while(str[i] != '\0'){
    hashV = ( hashV + (unsigned char)str[i] * primeBase ) % 1031;
    i++;
  }
  return hashV;
}

void initHashMap(HashMap *bucks){
  for(int i= 0; i < 1031; i++){
    bucks->buckArr[i].stC = 0;
    memset(bucks->buckArr[i].word, 0, 64);
    bucks->buckArr[i].next = NULL;
  }
}

void tokenStr(HashMap *hM){

  FILE *f = fopen("testFile.txt", "rb+");
  if(!f){
    perror("Failed to open file.\n");
    return;
  }
  char wordTk[64];
  char flineBuff[200];
  while(fgets(flineBuff, sizeof(flineBuff), f) != NULL){
     const char *tP = flineBuff;
     while(*tP){
       if(sscanf(tP, "%63[^ %,';.:?\\/><0-9~`!*()[]+-]", wordTk) != 1){
	 tP++;
	 continue;
       }
       printf("Word : %s\n", wordTk);
       size_t hashIdx = hashValue(wordTk);

       //Handle the hashCollision
       //first the hashValue index if free or not
       
       if(hM->buckArr[hashIdx].stC == 0){
	 hM->buckArr[hashIdx].stC += 1;
	 strncpy(hM->buckArr[hashIdx].word, wordTk, 63);
	 hM->buckArr[hashIdx].next = NULL;
       }else{
	 //it means there is some living(I meant with word lamo)
	 //let's compare that word is same as wordTk
	 //First check length of words true then increase appearrence count
	 //else create new Bucket and store word there and update next
	 Buckets *exist = &hM->buckArr[hashIdx];
	 size_t existStrlen = strlen(exist->word), wordTklen = strlen(wordTk);
	 if(existStrlen == wordTklen){
	   //true compare each char of words
	   if(strncmp(exist->word, wordTk, existStrlen) == 0){
	     exist->stC += 1;
	   }
	 }else if(exist->next){
	   //check that index's list each word and compare
	   Buckets *curr = exist->next;
	   while(curr != NULL){
	     existStrlen = strlen(curr->word);
	     if(existStrlen == wordTklen){
	       if(strncmp(curr->word, wordTk, existStrlen) == 0){
		 curr->stC += 1;
	       }
	     }
	     curr = curr->next;
	   }
	 }else{
	   //this means not word match we need to add this word in the list of that index
	   Buckets *newBuck = (Buckets*)malloc(sizeof(Buckets));
	   if(!newBuck){
	     perror("Memory allocation failed.\n");
	     fclose(f);
	     return;
	   }
	   newBuck->stC = 1;
	   strncpy(newBuck->word, wordTk, 63);
	   newBuck->word[wordTklen+1] = '\0';
           newBuck->next = NULL;
           if(exist->next == NULL){
	     exist->next = newBuck;
	   }else{
	     Buckets *curr = exist->next;
	     while(curr->next != NULL){
	       curr = curr->next;
	     }
	     curr->next = newBuck;
	   }
	 }
       }
       tP += strlen(wordTk);
       while(*tP && strchr(" %,';.:?><0-9~`!*()[]+-", *tP)) tP++;
     }
  }
  fclose(f);
}

void displayHashV(HashMap *hM){
  for(int i= 0; i < 1031; i++){
    printf("Index: %d -> Word = %s Appearred = %zu\n",
	   i, hM->buckArr[i].word, hM->buckArr[i].stC);
    Buckets *curr = &hM->buckArr[i];
    while(curr->next != NULL){ 
      curr = curr->next;
      printf("=====Index: %d -> Word = %s Appearred = %zu\n",
	     i, curr->word, curr->stC);
    }
  }
}


int main(){

  HashMap hp;
  initHashMap(&hp);

  tokenStr(&hp);

  displayHashV(&hp);
  
  return 0;
}
