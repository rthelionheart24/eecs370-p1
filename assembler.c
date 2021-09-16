/**
 * Project 1 
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

struct labelInfo
{
    char label[7];
    int addr;
};

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
        arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3)
    {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
               argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL)
    {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL)
    {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        /* reached end of file */
        return 0;
    }

    rewind(inFilePtr);

    int trace = 0;
    int num_label = 0;

    struct labelInfo idx[MAXLINELENGTH];

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        if (strcmp(label, "") != 0)
        {
            int repeat = 0;
            for (int i = 0; i < num_label; i++)
            {
                if (strcmp(idx[i].label, label) == 0)
                {
                    repeat = 1;
                    break;
                }
            }

            if (repeat == 1)
                exit(1);

            strcpy(idx[num_label].label, label);
            idx[num_label].addr = trace;

            num_label++;
        }

        trace++;
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    int instruction;
    int PC = 0;

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        /* after doing a readAndParse, you may want to do the following to test the
        opcode */
        if (!strcmp(opcode, "add"))
        {
            /* do whatever you need to do for opcode "add" */
            instruction = 0b000 << 3;
            instruction += atoi(arg0);
            instruction <<= 3;
            instruction += atoi(arg1);
            instruction <<= 16;

            if (isNumber(arg2))
            {
                instruction += atoi(arg2);
            }
            else
            {
                int found = 0;
                for (int i = 0; i < num_label; i++)
                {
                    if (strcmp(idx[i].label, arg2) == 0)
                    {
                        instruction += idx[i].addr;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    exit(1);
            }
        }
        else if (!strcmp(opcode, "nor"))
        {
            instruction = 0b001 << 3;
            instruction += atoi(arg0);
            instruction <<= 3;
            instruction += atoi(arg1);
            instruction <<= 16;

            if (isNumber(arg2))
            {
                instruction += atoi(arg2);
            }
            else
            {
                int found = 0;
                for (int i = 0; i < num_label; i++)
                {
                    if (strcmp(idx[i].label, arg2) == 0)
                    {
                        instruction += idx[i].addr;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    exit(1);
            }
        }
        else if (!strcmp(opcode, "lw"))
        {
            instruction = 0b010 << 3;
            instruction += atoi(arg0);
            instruction <<= 3;
            instruction += atoi(arg1);
            instruction <<= 16;

            if (isNumber(arg2))
            {
                if (atoi(arg2) <= 32767 && atoi(arg2) >= -32768)
                    instruction += atoi(arg2);
                else
                    exit(1);
            }
            else
            {
                int found = 0;
                for (int i = 0; i < num_label; i++)
                {
                    if (strcmp(idx[i].label, arg2) == 0)
                    {
                        instruction += idx[i].addr;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    exit(1);
            }
        }
        else if (!strcmp(opcode, "sw"))
        {
            instruction = 0b011 << 3;
            instruction += atoi(arg0);
            instruction <<= 3;
            instruction += atoi(arg1);
            instruction <<= 16;

            if (isNumber(arg2))
            {
                if (atoi(arg2) <= 32767 && atoi(arg2) >= -32768)
                    instruction += atoi(arg2);
                else
                    exit(1);
            }
            else
            {
                int found = 0;
                for (int i = 0; i < num_label; i++)
                {
                    if (strcmp(idx[i].label, arg2) == 0)
                    {
                        instruction += idx[i].addr;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    exit(1);
            }
        }
        else if (!strcmp(opcode, "beq"))
        {
            instruction = 0b100 << 3;
            instruction += atoi(arg0);
            instruction <<= 3;
            instruction += atoi(arg1);
            instruction <<= 16;

            if (isNumber(arg2))
            {
                if (atoi(arg2) <= 32767 && atoi(arg2) >= -32768)
                    instruction += atoi(arg2);
                else
                    exit(1);
            }
            else
            {
                for (int i = 0; i < num_label; i++)
                {
                    int found = 0;
                    if (strcmp(idx[i].label, arg2) == 0)
                    {
                        int trimmed = (idx[i].addr - PC - 1) & 0b1111111111111111;
                        instruction += trimmed;
                        found = 1;
                        break;
                    }
                    if (!found)
                        exit(1);
                }
            }
        }
        else if (!strcmp(opcode, "jalr"))
        {
            instruction = 0b101 << 3;
            instruction += atoi(arg0);
            instruction <<= 3;
            instruction += atoi(arg1);
            instruction <<= 16;
        }
        else if (!strcmp(opcode, "halt"))
        {
            instruction = 0b110;
            instruction <<= 22;
        }
        else if (!strcmp(opcode, "noop"))
        {
            instruction = 0b111;
            instruction <<= 22;
        }
        else if (!strcmp(opcode, ".fill"))
        {

            if (isNumber(arg0))
                instruction = atoi(arg0);

            else
            {
                int found = 0;
                for (int i = 0; i < num_label; i++)
                {
                    if (strcmp(idx[i].label, arg0) == 0)
                    {
                        instruction += idx[i].addr;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    exit(1);
            }
        }
        else
        {
            exit(1);
        }

        fprintf(outFilePtr, "%d\n", instruction);

        PC++;

        instruction = 0;
    }

    return (0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
                 char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
    {
        /* reached end of file */
        return (0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL)
    {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    char *ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label))
    {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
           opcode, arg0, arg1, arg2);
    return (1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return ((sscanf(string, "%d", &i)) == 1);
}
