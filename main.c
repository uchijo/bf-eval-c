#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocess.h"

int *cacheJumpDest(virtual_instr *src, int length) {
	int *jumpDest = calloc(length, sizeof(int));
	for (int i=0; i<length; i++) {
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
        if (c != '<' && c!='>' && c!='+' && c!='-' && c!='.' && c!=',' && c!='[' && c!=']') {
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
    for (;;) {
        if (pc >= length) {
            break;
        }
        switch (instr_array[pc].instr) {
            case INC_PTR:
                pointer += instr_array[pc].data;
                break;
            case DEC_PTR:
                pointer -= instr_array[pc].data;
                break;
            case INC_VAL:
                mem[pointer] += instr_array[pc].data;
                break;
            case DEC_VAL:
                mem[pointer] -= instr_array[pc].data;
                break;
            case OUTPUT:
                printf("%c", mem[pointer]);
                break;
            case JUMP_FORWARD:
                if (mem[pointer] == 0) {
                    pc = jump_dest[pc];
                }
                break;
            case JUMP_BACKWARD:
                if (mem[pointer] != 0) {
                    pc = jump_dest[pc];
                }
                break;
            case INPUT:
                mem[pointer] = getchar();
                break;
        }
        pc++;
    }
}

