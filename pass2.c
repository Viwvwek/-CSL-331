#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to swap two characters
void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

// Function to reverse a portion of a character array
char *reverse(char *buffer, int i, int j)
{
    while (i < j)
    {
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}

// Manual implementation of itoa (integer to ASCII conversion)
char *itoa(int value, char *buffer, int base)
{
    // Validate the base
    if (base < 2 || base > 32)
    {
        return buffer;
    }

    // Consider absolute value for conversion
    int n = abs(value);
    int i = 0;

    // Convert the number to the specified base
    while (n)
    {
        int r = n % base;
        buffer[i++] = (r >= 10) ? 65 + (r - 10) : 48 + r;
        n = n / base;
    }

    // Handle case when the number is 0
    if (i == 0)
    {
        buffer[i++] = '0';
    }

    // Add minus sign if the base is 10 and the number is negative
    if (value < 0 && base == 10)
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // Null terminate the string

    // Reverse the buffer to get the correct order
    return reverse(buffer, 0, i - 1);
}

int main()
{
    // Variable declarations for intermediate files, symbol tables, and other details
    char a[10], ad[10], label[10], opcode[10], operand[10], symbol[10];
    int start, diff, i, address, add, len, actual_len, finaddr, prevaddr, j = 0;

    // Mnemonic opcode and corresponding object code arrays
    char mnemonic[15][15] = {"LDA", "STA", "LDCH", "STCH"};
    char code[15][15] = {"33", "44", "53", "57"};

    // File pointers for input and output files
    FILE *fp1, *fp2, *fp3, *fp4;
    fp1 = fopen("output.txt", "w"); // File to write output with line details

    fp2 = fopen("symtab.txt", "r"); // Symbol table file for operand address lookup

    fp3 = fopen("intermediate.txt", "r"); // Intermediate file with assembly code

    fp4 = fopen("objcode.txt", "w"); // Object code output file

    // Read the first line of the intermediate file
    fscanf(fp3, "%s\t%s\t%s", label, opcode, operand);

    // Loop to determine the final address
    while (strcmp(opcode, "END") != 0)
    {
        prevaddr = address;
        fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
    }
    finaddr = address; // Final address

    fclose(fp3);

    fp3 = fopen("intermediate.txt", "r");

    // Read the first line again and handle the START directive
    fscanf(fp3, "\t%s\t%s\t%s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);

        fprintf(fp4, "H^%s^00%s^00%d\n", label, operand, finaddr); // Header record

        fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);

        start = address;
        diff = prevaddr - start;
        fprintf(fp4, "T^00%d^%d", address, diff); // Text record header
    }

    // Process each line in the intermediate file
    while (strcmp(opcode, "END") != 0)
    {
        // Handle BYTE directive
        if (strcmp(opcode, "BYTE") == 0)
        {
            fprintf(fp1, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3;
            fprintf(fp4, "^");

            for (i = 2; i < (actual_len + 2); i++)
            {
                itoa(operand[i], ad, 16); // Convert character to hexadecimal
                fprintf(fp1, "%s", ad);
                fprintf(fp4, "%s", ad);
            }
            fprintf(fp1, "\n");
        }
        // Handle WORD directive
        else if (strcmp(opcode, "WORD") == 0)
        {
            len = strlen(operand);
            itoa(atoi(operand), a, 10); // Convert operand to string
            fprintf(fp1, "%d\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
            fprintf(fp4, "^00000%s", a);
        }
        // Handle RESB and RESW directives
        else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0))
        {
            fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
        }
        // Handle other opcodes
        else
        {
            while (strcmp(opcode, mnemonic[j]) != 0)
                j++;
            if (strcmp(operand, "COPY") == 0)
                fprintf(fp1, "%d\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code[j]);
            else
            {
                rewind(fp2); // Reset symbol table file pointer
                fscanf(fp2, "%s%d", symbol, &add);
                while (strcmp(operand, symbol) != 0)
                    fscanf(fp2, "%s%d", symbol, &add);
                fprintf(fp1, "%d\t%s\t%s\t%s\t%s%d\n", address, label, opcode, operand, code[j], add);
                fprintf(fp4, "^%s%d", code[j], add);
            }
        }

        // Read next line from intermediate file
        fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
    }

    // Handle END directive
    fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(fp4, "\nE^00%d", start); // End record

    // Close all files
    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);

    printf("Execution done successfully now open OBJ AND OUTPUT file\n");

    return 0;
}
