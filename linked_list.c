//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


struct Node *add_node(struct Node **head, void *value) {
//  printf("Address of list head is %x\n", head);
  struct Node *newNode = malloc(sizeof(struct Node));

  newNode->data = value;
  newNode->next = *head;
  *head = newNode;

  return newNode;
}

void remove_first(struct Node **head) {
  struct Node *current = *head;  

    *head = current->next;
    free(current);
}

void remove_node(struct Node **head, struct Node *node) {
  struct Node *current;
  struct Node *previous;

  current = *head;
  previous = NULL;

  while(current != NULL) {
//    printf("  Checking value %d, address %x\n", *((int *) current->data), current);
    if (current == node) {
//      printf("  Found node to remove: %d, address %x\n", *((int *) current->data), current);

      if (previous == NULL) {
        *head = current->next;
        free(current);
        current = *head;
      } else {
        previous->next = current->next;
        free(current);
        current = previous->next;
        
      }  
    } else {
      previous = current;
      current = current->next;
    }
  }

}

void print_list(struct Node *head) {
  struct Node *current = head;

  printf("start print_list\n");
//  printf("  head address: %x\n", head);
  while(current != NULL) {
//    printf("  value: %d, address: %x\n", *((int *) current->data), current);
    current = current->next;
  }
//  printf("end print_list\n");
}

void clear_list(struct Node **head) {
  struct Node *currentNode;
//  struct Node *nextNode;
  
  currentNode = *head;
//  nextNode = NULL;

  while(currentNode != NULL) {
        *head = currentNode->next;
        free(currentNode);
        currentNode = *head;
	
  }
}


int count_list(struct Node *head) {
	int iCount;
	struct Node *current = head;

	iCount = 0;
	while(current != NULL) {
		iCount++;
		current = current->next;
	}
	
	return iCount;

}


/*
int main(void) {
  int *p1;
  int *p2;
  int *p3;
  int *p4;
  int *p5;

  printf("allocating pointers\n");
  p1 = malloc(sizeof(int));
  p2 = malloc(sizeof(int));
  p3 = malloc(sizeof(int));
  p4 = malloc(sizeof(int));
  p5 = malloc(sizeof(int));

  printf("assigning pointers\n");
  *p1 = 10;
  *p2 = 20;
  *p3 = 30;
  *p4 = 42;
  *p5 = 42;

  struct Node *listNumbers;
  listNumbers = NULL;
  printf("address of list: %x\n", &listNumbers);
  printf("list points at address (NULL): %x\n", listNumbers);

  struct Node *n1 = add_node(&listNumbers, p1);
  struct Node *n2 = add_node(&listNumbers, p2);
  struct Node *n3 = add_node(&listNumbers, p3);
  printf("added three nodes\n");
  print_list(listNumbers);

  struct Node *n4 = add_node(&listNumbers, p4); 
  printf("added fourth node\n");
  print_list(listNumbers);

  printf("Remove node n2, address %x\n", n2);
  remove_node(&listNumbers, n2);
  print_list(listNumbers);


  printf("Remove node n2, address %x\n", n2);
  remove_node(&listNumbers, n2);
  print_list(listNumbers);

  printf("Head is %d, address %x\n", * ((int *)listNumbers->data), listNumbers);

  printf("removing node - n3, address %x\n", n3);
  remove_node(&listNumbers, n3);
  printf("removed item - n3\n");
  print_list(listNumbers);

  struct Node *n5 = add_node(&listNumbers, p5); 
  printf("Added another node with value 42\n");
  print_list(listNumbers);

  printf("remove only n4 node: %x (p5 node: %x with value 42 should remain)\n", n4, n5);
  remove_node(&listNumbers, n4);
  print_list(listNumbers);

  printf("remove first node\n");
  remove_first(&listNumbers);
  print_list(listNumbers);


  return 0;
}
*/
