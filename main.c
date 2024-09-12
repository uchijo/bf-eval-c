#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// enum for the instructions
enum Instruction {
    INC_PTR,
    DEC_PTR,
    INC_VAL,
    DEC_VAL,
    OUTPUT,
    INPUT,
    JUMP_FORWARD,
    JUMP_BACKWARD
};

// struct for virtual instruction
struct VirtualInstruction {
    enum Instruction instruction;
    int data;
};

int *cacheJumpDest(char *src) {
    int src_length = strlen(src);
	int *jumpDest = calloc(src_length, sizeof(int));
	// for pc, c := range src {
	for (int i=0; i<src_length; i++) {
		if (src[i] == '[') {
			int start = i;
			int nest = 1;
			for (;;) {
				start++;
				if (src[start] == '[') {
					nest++;
				} else if (src[start]== ']') {
					nest--;
					if (nest == 0) {
						break;
					}
				} else {
					continue;
				}
			}
			jumpDest[i] = start;
		} else if (src[i] == ']') {
			int start = i;
			int nest = 1;
			for (;;) {
				start--;
				if (src[start] == ']') {
					nest++;
				} else if (src[start] == '[') {
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

    // printf("%s\n", buffer);

    int pc = 0;
    int pointer = 15000;
    int mem[30000];
    int instructions = strlen(buffer);
    int *jump_dest = cacheJumpDest(buffer);
    // for (int i=0; i<30000; i++) {
    //     printf("mem %d: %d\n", i, mem[i]);
    // }
    // printf("mem %d: %d\n", pointer, mem[pointer]);
    // printf("jump_dest for %d: %d\n", pc, jump_dest[pc]);
    // return 0;
    for (;;) {
        // printf("current instr: %d, %c\n", pc, buffer[pc]);
        // printf("pointer: %d, mem: %d\n", pointer, mem[pointer]);
        if (pc >= instructions) {
            break;
        }
        switch (buffer[pc]) {
            case '>':
                pointer++;
                break;
            case '<':
                pointer--;
                break;
            case '+':
                mem[pointer]++;
                break;
            case '-':
                mem[pointer]--;
                break;
            case '.':
                printf("%c", mem[pointer]);
                break;
            case '[':
                if (mem[pointer] == 0) {
                    pc = jump_dest[pc];
                }
                break;
            case ']':
                if (mem[pointer] != 0) {
                    pc = jump_dest[pc];
                }
                break;
        }
        pc++;
    }
}

