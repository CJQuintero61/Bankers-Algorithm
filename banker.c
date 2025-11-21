/*
    banker.c
    For COSC 3346 Operating Systems proj4
    Created 11/21/2025 by Christian Quintero
    Last updated 11/21/2025 by Christian Quintero

    This program implements the banker's algorithm for
    resource allocation.

    To run:
    1. cc banker.c
    2. ./a.out <file_name>.txt
    3. rm a.out 
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    /*
        params:
            argc: int - the command line argument count
            argv: char * - the argument vector 

            argv[0] - the program name
            argv[1] - the input file to read from

        returns:
            int - 0 on success. Else, an error code
    */

    FILE* file_pointer = NULL;
    int num_processes = 0;
    int num_resource_types = 0;

    // open the file in read only mode
    file_pointer = fopen(argv[1], "r");

    if (file_pointer == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    // read the number of processes and resource types
    fscanf(file_pointer, "%d %d", &num_processes, &num_resource_types);
    printf("Number of Processes: %d\nNumber of Resources: %d\n", num_processes, num_resource_types);

    // create the allocation matrix
    int** alloc = malloc(num_processes * sizeof(int*));
    for (int i = 0; i < num_processes; i++) {
        alloc[i] = malloc(num_resource_types * sizeof(int));
    }


    // free allocation matrix
    for (int i = 0; i < num_processes; i++) {
        free(alloc[i]);
    }
    free(alloc);

    fclose(file_pointer);
    return 0;
}