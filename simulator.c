/**
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8       /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct
{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    int num_executed = 0;

    // if (argc != 2)
    // {
    //     printf("error: usage: %s <machine-code file>\n", argv[0]);
    //     exit(1);
    // }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL)
    {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    state.pc = 0;
    for (size_t i = 0; i < state.numMemory; i++)
    {
        state.mem[i] = 0;
    }
    for (size_t i = 0; i < NUMREGS; i++)
    {
        state.reg[i] = 0;
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
         state.numMemory++)
    {

        if (sscanf(line, "%d", state.mem + state.numMemory) != 1)
        {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    while (state.pc < state.numMemory)
    {
        printState(&state);

        int instruction = state.mem[state.pc] >> 22;

        num_executed++;
        // add
        if (instruction == 0)
        {
            int src1 = (state.mem[state.pc] & 0b1111111111111111111111) >> 19;
            int src2 = (state.mem[state.pc] & 0b1111111111111111111) >> 16;
            int dst = (state.mem[state.pc] & 0xFFFF);

            state.reg[dst] = state.reg[src1] + state.reg[src2];
            state.pc++;
        }
        // nor
        else if (instruction == 1)
        {
            int src1 = (state.mem[state.pc] & 0b1111111111111111111111) >> 19;
            int src2 = (state.mem[state.pc] & 0b1111111111111111111) >> 16;
            int dst = (state.mem[state.pc] & 0xFFFF);

            state.reg[dst] = ~(state.reg[src1] | state.reg[src2]);
            state.pc++;
        }
        //lw
        else if (instruction == 2)
        {
            int src = (state.mem[state.pc] & 0b1111111111111111111111) >> 19;
            int dst = (state.mem[state.pc] & 0b1111111111111111111) >> 16;
            int offset = convertNum(state.mem[state.pc] & 0xFFFF);

            state.reg[dst] = state.mem[state.reg[src] + offset];
            state.pc++;
        }
        // sw
        else if (instruction == 3)
        {
            int src = (state.mem[state.pc] & 0b1111111111111111111111) >> 19;
            int dst = (state.mem[state.pc] & 0b1111111111111111111) >> 16;
            int offset = convertNum(state.mem[state.pc] & 0xFFFF);

            state.mem[state.reg[src] + offset] = state.reg[dst];
            state.pc++;
        }
        // beq
        else if (instruction == 4)
        {
            int reg1 = (state.mem[state.pc] & 0b1111111111111111111111) >> 19;
            int reg2 = (state.mem[state.pc] & 0b1111111111111111111) >> 16;
            int offset = convertNum(state.mem[state.pc] & 0xFFFF);

            if (state.reg[reg1] == state.reg[reg2])
            {
                state.pc += (1 + offset);
            }
            else
            {
                state.pc++;
            }
        }
        // jalr
        else if (instruction == 5)
        {
            int reg1 = (state.mem[state.pc] & 0b1111111111111111111111) >> 19;
            int reg2 = (state.mem[state.pc] & 0b1111111111111111111) >> 16;

            state.reg[reg2] = state.pc + 1;

            state.pc = state.reg[reg1];
        }
        // halt
        else if (instruction == 6)
        {
            state.pc++;
            break;
        }
        // noop
        else if (instruction == 7)
        {
            state.pc++;
        }
    }

    printf("machine halted\ntotal of %d instructions executed\nfinal state of machine:\n", num_executed);
    printState(&state);

    return (0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1 << 15))
    {
        num -= (1 << 16);
    }
    return (num);
}
