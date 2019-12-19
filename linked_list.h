//2019 Levi D. Smith - levidsmith.com
struct Node {
  void *data;
  struct Node *next;
};

struct Node *add_node(struct Node **head, void *value);
void remove_first(struct Node **head);
void remove_node(struct Node **head, struct Node *node);
void print_list(struct Node *head);
void clear_list(struct Node **head);
int count_list(struct Node *head);
