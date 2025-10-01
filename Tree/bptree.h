
#ifndef BPTREE_H
#define BPTREE_H

#include <stdio.h>
#include <stdlib.h>

#ifndef T
#define T 2
#endif

#define MaxKey (2 * T - 1)
#define MinKey (T - 1)
#define MaxCP (2*T)

typedef enum{LEAF, INTERNAL} NodeType;

typedef struct Node{
  NodeType type;
  int keys[MaxKey];
  int n;
  union{
    struct{
      struct Node *cPs[MaxCP];
    };
    struct{
      void* records[MaxKey];
      struct Node *next;
    };
  }; 
} Node;

Node* createNode(NodeType type);

void displayBPlusTree(Node *root, int level);

Node* findFirstLeaf(Node *root);

void displayLeafChain(Node *root);

void freeTree(Node *root);

void splitNode(Node *parent, int idx);

void insertInNonFull(Node *node, int key);

void insertKey(Node **node, int key);

#endif
