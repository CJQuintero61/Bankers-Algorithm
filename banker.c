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

void print_alloc_matrix(int num_processes, int num_resource_types, int** alloc);
void print_max_matrix(int num_processes, int num_resource_types, int** max);
void print_need_matrix(int num_processes, int num_resource_types, int** need);
void print_available_vector(int num_resource_types, int* available);
void free_matrix(int num_resource_types, int** matrix);
int** create_matrix(int num_processes, int num_resource_types);

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

    // create the matricies and vectors
    int** alloc = create_matrix(num_processes, num_resource_types);
    int** max = create_matrix(num_processes, num_resource_types);
    int** need = create_matrix(num_processes, num_resource_types);
    int* available = malloc(num_resource_types * sizeof(int));

    // read the allocation matrix 
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resource_types; j++) {
            fscanf(file_pointer, "%d", &alloc[i][j]);
        }
    }

    // read the max matrix
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resource_types; j++) {
            fscanf(file_pointer, "%d", &max[i][j]);
        }
    }

    // calculate the need matrix
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resource_types; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // read the available vector
    for(int i = 0; i < num_resource_types; i++) {
        fscanf(file_pointer, "%d", &available[i]);
    }

    printf("Number of Processes: %d\nNumber of Resources: %d\n", num_processes, num_resource_types);
    print_alloc_matrix(num_processes, num_resource_types, alloc);
    print_max_matrix(num_processes, num_resource_types, max);
    print_need_matrix(num_processes, num_resource_types, need);
    print_available_vector(num_resource_types, available);

    // free the matricies and vector
    free_matrix(num_processes, alloc);                  // alloc matrix
    free_matrix(num_processes, max);                    // max matrix
    free_matrix(num_processes, need);                   // need matrix
    free(available);                                    // available vector

    // close the file and return 0
    fclose(file_pointer);
    return 0;
}


void print_alloc_matrix(int num_processes, int num_resource_types, int** alloc) {
    /*
        prints the allocation matrix formatted
    */
    printf("Allocation Matrix:\n");
    printf("   ");

    // dynamically list the resources 
    // ex) for 4 resoures -> "A B C D"
    // ex) for 5 resources -> "A B C D E"
    for(int i = 0; i < num_resource_types; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    for(int i = 0; i < num_processes; i++) {
        printf("%d: ", i);

        for(int j = 0; j < num_resource_types; j++) {
            printf("%d ", alloc[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void print_max_matrix(int num_processes, int num_resource_types, int** max) {
    /*
        prints the max matrix formatted
    */
    printf("Max Matrix:\n");
    printf("   ");

    // dynamically print the resource types
    for(int i = 0; i < num_resource_types; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    for(int i = 0; i < num_processes; i++) {
        printf("%d: ", i);

        for(int j = 0; j < num_resource_types; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void print_need_matrix(int num_processes, int num_resource_types, int** need) {
    /*
        prints the need matrix formatted
    */
    printf("Need Matrix:\n");
    printf("   ");

    // dynamically print the resource types
    for(int i = 0; i < num_resource_types; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    for(int i = 0; i < num_processes; i++) {
        printf("%d: ", i);

        for(int j = 0; j < num_resource_types; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void print_available_vector(int num_resource_types, int* available) {
    /*
        prints the available vector
    */
    printf("Available Vector:\n");
    printf("   ");

    // dynamically print the resource types
    for(int i = 0; i < num_resource_types; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n   ");

    for(int i = 0; i < num_resource_types; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");
}


void free_matrix(int num_processes, int** matrix) {
    /*
        frees a matrix using the number of rows (aka num_processes)
    */
    
    // free the inner arrays
    for(int i = 0; i < num_processes; i++) {
        free(matrix[i]);
    }
    // free the array itself
    free(matrix);
}

int** create_matrix(int num_processes, int num_resource_types) {
    /*
        allocates and returns a 2D matrix
    */
    int** matrix;
    matrix = malloc(num_processes * sizeof(int*));
    for(int i = 0; i < num_processes; i++) {
        matrix[i] = malloc(num_resource_types * sizeof(int));
    }

    return matrix;
}