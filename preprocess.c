#include "preprocess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append(virtual_instr_node **head, enum instr instr, int data) {
  virtual_instr_node *new_instr = malloc(sizeof(virtual_instr_node));
  new_instr->instr = instr;
  new_instr->data = data;
  new_instr->next = NULL;

  if (*head == NULL) {
    *head = new_instr;
    return;
  }

  virtual_instr_node *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_instr;
}

virtual_instr_node *parse(char *input) {
  virtual_instr_node *current = NULL;

  for (int i = 0; i < strlen(input); i++) {
    // printf("current: %p\n", current);
    // printf("%d: %c\n", i, input[i]);
    switch (input[i]) {
    case '>':
      append(&current, INC_PTR, 1);
      break;
    case '<':
      append(&current, DEC_PTR, 1);
      break;
    case '+':
      append(&current, INC_VAL, 1);
      break;
    case '-':
      append(&current, DEC_VAL, 1);
      break;
    case '.':
      append(&current, OUTPUT, 0);
      break;
    case ',':
      append(&current, INPUT, 0);
      break;
    case '[':
      append(&current, JUMP_FORWARD, 0);
      break;
    case ']':
      append(&current, JUMP_BACKWARD, 0);
      break;
    default:
      continue;
    }
  }

  return current;
}

void pack_instr(virtual_instr_node *head) {
  virtual_instr_node *current = head;
  while (current != NULL) {
    switch (current->instr) {
    case INC_PTR:
    case DEC_PTR:
    case INC_VAL:
    case DEC_VAL:
      while (current->next != NULL && current->next->instr == current->instr) {
        current->data++;
        virtual_instr_node *prev_next = current->next;
        current->next = current->next->next;
        free(prev_next);
      }
    default:
      current = current->next;
      break;
    }
  }
}

void apply_zero_reset(virtual_instr_node *head) {
  virtual_instr_node *current = head;
  while (current != NULL) {
    if (current->instr == JUMP_FORWARD && current->next->instr == DEC_VAL &&
        current->next->next->instr == JUMP_BACKWARD) {
            current->instr = ZERO_RESET;
            virtual_instr_node *prev_next = current->next;
            virtual_instr_node *prev_next_next = current->next->next;
            current->next = current->next->next->next;
            free(prev_next);
            free(prev_next_next);
    }
    current = current->next;
  }
}

int len(virtual_instr_node *head) {
  int count = 0;
  while (head != NULL) {
    count++;
    head = head->next;
  }
  return count;
}

virtual_instr *to_array(virtual_instr_node *head) {
  int length = len(head);
  virtual_instr *instr_array = malloc(sizeof(virtual_instr) * length);
  for (int i = 0; i < length; i++) {
    instr_array[i].instr = head->instr;
    instr_array[i].data = head->data;
    head = head->next;
  }
  return instr_array;
}

void print_virtual_instr(virtual_instr_node *head) {
  while (head != NULL) {
    switch (head->instr) {
    case INC_PTR:
      printf("INC_PTR %d\n", head->data);
      break;
    case DEC_PTR:
      printf("DEC_PTR %d\n", head->data);
      break;
    case INC_VAL:
      printf("INC_VAL %d\n", head->data);
      break;
    case DEC_VAL:
      printf("DEC_VAL %d\n", head->data);
      break;
    case OUTPUT:
      printf("OUTPUT\n");
      break;
    case INPUT:
      printf("INPUT\n");
      break;
    case JUMP_FORWARD:
      printf("JUMP_FORWARD\n");
      break;
    case JUMP_BACKWARD:
      printf("JUMP_BACKWARD\n");
      break;
    case ZERO_RESET:
      printf("ZERO_RESET\n");
    }
    head = head->next;
  }
}
