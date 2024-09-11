#include <stdio.h>

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
        buffer[i] = c;
        i++;
    }
}

