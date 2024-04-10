// Filename: shakespeare.h
// 
// Header file for the shakespeare c file
// 
// Grant Bellotti, April 14th, 2024
#ifndef SHAKESPEARE_H
#define SHAKESPEARE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100 // Max size of line in file

typedef struct node {
  struct node *next;
  int value;
  char word[];
} node;

// Print error
void print_usage_error();

// Create new linked list node
node* createNode(char *word);

// Add a node to an list
node* addNode(node *head, char *word);

// Find a node in list, if it is there, increment it's value
node* findNode(node *head, char *findWord);

// Print the whole list (only used in testing)
void printList(node *head);

// Add required word to file 
void printWord(node *head, int rank, FILE *fileInput);

#endif /* SHAKESPEARE_H */