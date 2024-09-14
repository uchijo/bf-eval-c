#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocess.h"

int *cacheJumpDest(virtual_instr *src, int length) {
  int *jumpDest = calloc(length, sizeof(int));
  for (int i = 0; i < length; i++) {
    if (src[i].instr == JUMP_FORWARD) {
      int start = i;
      int nest = 1;
      for (;;) {
        start++;
        if (src[start].instr == JUMP_FORWARD) {
          nest++;
        } else if (src[start].instr == JUMP_BACKWARD) {
          nest--;
          if (nest == 0) {
            break;
          }
        } else {
          continue;
        }
      }
      jumpDest[i] = start;
    } else if (src[i].instr == JUMP_BACKWARD) {
      int start = i;
      int nest = 1;
      for (;;) {
        start--;
        if (src[start].instr == JUMP_BACKWARD) {
          nest++;
        } else if (src[start].instr == JUMP_FORWARD) {
          nest--;
          if (nest == 0) {
            break;
          }
        } else {
          continue;
        }
      }
      jumpDest[i] = start;
    }
  }
  return jumpDest;
}

int main() {
  // read file named m.bf
  FILE *file = fopen("m.bf", "r");
  if (file == NULL) {
    printf("File not found\n");
    return 1;
  }

  // save the file content in a buffer
  char buffer[30000];
  int i = 0;
  while (1) {
    char c = fgetc(file);
    if (c == EOF) {
      break;
    }
    if (c != '<' && c != '>' && c != '+' && c != '-' && c != '.' && c != ',' &&
        c != '[' && c != ']') {
      continue;
    }
    buffer[i] = c;
    i++;
  }
  buffer[i] = '\0';

  virtual_instr_node *parsed = parse(buffer);
  pack_instr(parsed);
  virtual_instr *instr_array = to_array(parsed);

  int pc = 0;
  int pointer = 15000;
  int mem[30000];
  int length = len(parsed);
  int *jump_dest = cacheJumpDest(instr_array, length);
  static void *labels[] = {
      &&l_inc_ptr, &&l_dec_ptr, &&l_inc_val,      &&l_dec_val,
      &&l_output,  &&l_input,   &&l_jump_forward, &&l_jump_backward,
  };

  goto *labels[instr_array[pc].instr];

l_inc_ptr:
  pointer += instr_array[pc].data;
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_dec_ptr:
  pointer -= instr_array[pc].data;
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_inc_val:
  mem[pointer] += instr_array[pc].data;
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_dec_val:
  mem[pointer] -= instr_array[pc].data;
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_output:
  printf("%c", mem[pointer]);
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_jump_forward:
  if (mem[pointer] == 0) {
    pc = jump_dest[pc];
  }
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_jump_backward:
  if (mem[pointer] != 0) {
    pc = jump_dest[pc];
  }
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];
l_input:
  mem[pointer] = getchar();
  pc++;
  if (pc >= length) {
    goto exit;
  }
  goto *labels[instr_array[pc].instr];

exit:
  return 0;
}
