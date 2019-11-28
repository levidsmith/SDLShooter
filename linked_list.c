#include <stdio.h>
#include <stdlib.h>

struct Node {
  void *data;
  struct Node *next;
};



void add_node(struct Node *head, void *value) {
  printf("add_node\n");
  if (head == NULL) {
    printf("head is NULL\n");
    head = malloc(sizeof(struct Node));
    head->data = value;
    head->next = NULL;
  } else {
    printf("head is NOT NULL, move to next\n");
    add_node(head->next, value); 
  }
}


void print_list(struct Node *head) {
  printf("printing list\n");
  if (head != NULL) {
    printf("Node: %d\n", *((int *) head->data));
    print_list(head->next);
  }
  printf("finished printing\n");
}

int main(void) {
  int *p1;
  int *p2;
  int *p3;

  printf("allocating pointers\n");

  p1 = malloc(sizeof(int));
  p2 = malloc(sizeof(int));
  p3 = malloc(sizeof(int));

  printf("assigning pointers\n");
  *p1 = 10;
  *p2 = 20;
  *p3 = 30;

  printf("making list\n");
  struct Node *listNumbers;
  listNumbers = NULL;

  add_node(listNumbers, p1);
  add_node(listNumbers, p2);
  add_node(listNumbers, p3);

  printf("print list\n");
  print_list(listNumbers);

  printf("finished\n");

  return 0;
}
