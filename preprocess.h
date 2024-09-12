#ifndef PREPROCESS_H
#define PREPROCESS_H

// enum for the instructions
enum instr {
  INC_PTR,
  DEC_PTR,
  INC_VAL,
  DEC_VAL,
  OUTPUT,
  INPUT,
  JUMP_FORWARD,
  JUMP_BACKWARD
};

typedef struct virtual_instr_node {
  enum instr instr;
  int data;
  struct virtual_instr_node *next;
} virtual_instr_node;

typedef struct {
    enum instr instr;
    int data;
} virtual_instr;

virtual_instr_node *parse(char *input);
void print_virtual_instr(virtual_instr_node *head);
void pack_instr(virtual_instr_node *head);
virtual_instr *to_array(virtual_instr_node *head);
int len(virtual_instr_node *head);

#endif
