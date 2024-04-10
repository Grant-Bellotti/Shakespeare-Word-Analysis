// Filename: shakespeare.c
// 
// Appends most frequent word or i'th most frequent word in shakespeare's transcripts
// 
// Grant Bellotti, April 14th, 2024
#include "shakespeare.h"

// Print error
void print_usage_error() {
  fprintf(stderr, "Usage: ./bard [input file] [output file]\n");
  exit(1);
}

// Main Program
int main(int argc, char *argv[]) {
  // Prints error if incorrect number of arguements are given
  if (argc != 3) {
    print_usage_error();
  }

  int wordLength;
  int rank;

  FILE *fileInput;
  FILE *file;
  FILE *fileOutput;

  // Open or create output file
  fileOutput = fopen(argv[2], "w");
  if (fileOutput == NULL) {
    printf("Error creating output file.\n");
    return 1;
  }

  // Open input file
  fileInput = fopen(argv[1], "r");
  if (fileInput == NULL) {
    fprintf(stderr, "Error opening input file\n");
    exit(1);
  }
  // Iterate through each line in the input file
  while (fscanf(fileInput, "%d %d", &wordLength, &rank) != EOF) {
    if (wordLength <= 0 || rank < 0) {
      fprintf(fileOutput, "-\n");
      continue;
    }
    node *head = NULL;
    
    // Open text file
    file = fopen("shakespeare-cleaned5.txt", "r");
    if (file == NULL) {
      fprintf(stderr, "Error opening file\n");
      exit(1);
    }

    // Each line in text file 
    char line[MAX_LINE_LENGTH]; 
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      if ((strlen(line) - 1) == wordLength) {
        if (findNode(head, line) == NULL) {
          // Add or increase value of word in linked list
          head = addNode(head, line);
        }
      }
    }
    fclose(file);

    // Put required word in output file
    printWord(head, rank, fileOutput);

    // Free memory allocated for the linked list nodes, pasted from previous project from CSE 13s
    node *currentNode = head;
    while (currentNode != NULL) {
      node *temp = currentNode;
      currentNode = currentNode->next;
      free(temp);
    }
  }
  
  fclose(fileInput);
  fclose(fileOutput);
  
  return 0;
}

// Create new linked list node
node* createNode(char *word) {
  // Allocate memory for new node
  node* newNode = (node*)malloc(sizeof(node) + strlen(word) + 1);

  // Assign data to node
  strcpy(newNode->word, word);
  newNode->value = 1;
  newNode->next = NULL;

  return newNode;
}

// Add a node to an list
node* addNode(node *head, char *word) {
  if (head == NULL) {
    return createNode(word);
  }

  node *previousNode = head;
  while (previousNode->next != NULL) {
    previousNode = previousNode->next;
  }

  previousNode->next = createNode(word);

  return head;
}

// Find a node in list, if it is there, increment it's value
node* findNode(node *head, char *findWord) {
  node *currentNode = head;
  while (currentNode != NULL) {
    if (strcmp(currentNode->word, findWord) == 0) {
      // If the node is already in list, increment it's value
      currentNode->value += 1;
      return currentNode;
    }

    currentNode = currentNode->next;
  }

  return NULL;
}

// Print the whole list (only used in testing)
void printList(node *head) {
  node *currentNode = head;
  while (currentNode != NULL) {
    printf("%d %s", currentNode->value, currentNode->word);
    currentNode = currentNode->next;
  }
}

// Add required word to file 
void printWord(node *head, int rank, FILE *fileOutput) {
  // Loop until you get to i'th value away from 1st
  for (int i = 0; i < rank+1; i++) {
    node *findSameValueNodes = head;
    node *currentNode = head;
    node *highestValueNode = NULL;
    node *highestSortedNode = NULL;

    int highestValue = 0;

    int amount = 0; // How many times the the while loop looped until currentNode = NULL
    while (currentNode != NULL) {
      // Get highest value node
      if (currentNode->value > highestValue) {
        highestValue = currentNode->value;
        highestValueNode = currentNode;
      }

      currentNode = currentNode->next;
      amount ++;
    }
    // If the while loop ended before the i'th element or there is no word
    if (amount <= rank || highestValueNode == NULL) {
      fprintf(fileOutput, "-\n");
      return;
    }

    // Find all words of the same value and make new linked list 
    while (findSameValueNodes != NULL) {
      if (findSameValueNodes->value == highestValue) {
        highestSortedNode = addNode(highestSortedNode, findSameValueNodes->word);
      }

      findSameValueNodes = findSameValueNodes->next;
    }

    if (highestSortedNode != NULL) {
      // If theres only one word with the frequency
      if (highestSortedNode->next == NULL && highestValueNode != NULL) {
        highestValueNode->value = 0;
      }
      // If there are more than one words with the same frequency
      else if (highestSortedNode->next != NULL) {
        char currentWord[MAX_LINE_LENGTH];
        strcpy(currentWord, highestSortedNode->word);
        // Check all the words to find the first alphabetical one
        while (highestSortedNode != NULL) {
          if (strcmp(currentWord, highestSortedNode->word) > 0) {
            strcpy(currentWord, highestSortedNode->word);
            highestValueNode = highestSortedNode;
          }

          highestSortedNode = highestSortedNode->next;
        }

        // If there is a node, create a new pointer back to original linked list to change the value
        if (highestValueNode != NULL) {
          node *temp = head;
          while (temp != NULL) {
            if (strcmp(temp->word, highestValueNode->word) == 0) {
              temp->value = 0;
              break;
            }
            temp = temp->next;
          }
        }
      }
    }

    // When the loop ends, append the correct word to file
    if (i == rank) {
      if (highestValueNode != NULL) {
        fprintf(fileOutput, "%s", highestValueNode->word);
      }
      else {
        fprintf(fileOutput, "-\n");
      }
    }

    // Free memory allocated for the highestSortedNode list
    currentNode = highestSortedNode;
    while (currentNode != NULL) {
      node *temp = currentNode;
      currentNode = currentNode->next;
      free(temp);
    }
  }
}