#include <stdio.h>

#include <string.h>

#include <stdlib.h>

void main()
{
    FILE *f1, *f2, *f3, *f4;

    int cur_loc = 0, start = 0, operand = 0, opcode = 0, len = 0;
    char label[20], instruction[20], op[20], symbol[20];

    // Open input file, symbol table file, and output file
    f1 = fopen("input.txt", "r");

    f3 = fopen("symtab.txt", "w");

    f4 = fopen("output.txt", "w");

    if (f1 == NULL || f3 == NULL || f4 == NULL)
        {
        printf("Error opening file\n");
            exit(1);
        }

    printf("\n\n### Pass 1 Assembler Program ###\n");

    // Read the first line from the input file
    fscanf(f1, "%s %s %d", label, instruction, &operand);

    // Check if the first instruction is START
    if (strcmp(instruction, "START") == 0)
    {
        start = operand; // Set the start address
        cur_loc = start; // Initialize current location counter

        fprintf(f4, "\t%s\t%s\t%d\n", label, instruction, operand); // Write to output file

        fscanf(f1, "%s %s", label, instruction); // Read the next line
    }
    else
    {
        cur_loc = 0; // If no START, initialize current location counter to 0
    }

    // Process each line until the end of the file
    while (!feof(f1))
    {
        fscanf(f1, "%s", op); // Read the operand

        fprintf(f4, "\n%d\t%s\t%s\t%s\n", cur_loc, label, instruction, op); // Write to output file

        // If there is a label, write it to the symbol table
        if (strcmp(label, "-") != 0)
        {
            fprintf(f3, "\n%d\t%s\n", cur_loc, label);
        }

        // Open the operation table file
        f2 = fopen("optab.txt", "r");
        if (f2 == NULL)
        {
            printf("Error opening optab.txt\n");
            exit(1);
        }

        int found = 0;
        // Search for the instruction in the operation table
        while (fscanf(f2, "%s %d", symbol, &opcode) != EOF)
        {
            if (strcmp(instruction, symbol) == 0)
            {
                cur_loc += 3; // Increment location counter by 3 for each instruction
                found = 1;
                break;
            }
        }
        fclose(f2);

        // Handle different types of instructions if not found in optab
        if (!found) {
            if (strcmp(instruction, "WORD") == 0)
            {
                cur_loc += 3; // Increment location counter by 3 for WORD
            }
            else if (strcmp(instruction, "RESW") == 0)
            {
                operand = atoi(op); // Convert operand to integer
                cur_loc += (3 * operand); // Increment location counter by 3 * operand for RESW
            }
            else if (strcmp(instruction, "BYTE") == 0)
            {
                if (op[0] == 'X') //BYTE X'1F'

                    cur_loc += 1; // Increment location counter by 1 for BYTE with hex constant
                else
                {
                    len = strlen(op) - 3; // Calculate length of the constant
                    cur_loc += len; // Increment location counter by length for BYTE with char constant
                }
            }
            else if (strcmp(instruction, "RESB") == 0)
            {
                operand = atoi(op); // Convert operand to integer
                cur_loc += operand; // Increment location counter by operand for RESB
            }
        }

        // Read the next line from the input file
        if (fscanf(f1, "%s %s", label, instruction) == EOF)
            break; // Exit loop if end of file
    }

    // Check if the last instruction is END
    if (strcmp(instruction, "END") == 0)
    {
        printf("\n\n Length of the Program = %d\n", cur_loc - start); // Print the length of the program
    }

    // Close all files
    fclose(f1);
    fclose(f3);
    fclose(f4);
}
