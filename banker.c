/*
    banker.c
    For COSC 3346 Operating Systems proj4
    Created 11/21/2025 by Christian Quintero
    Last updated 11/24/2025 by Christian Quintero

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
void print_request_vector(int num_resource_types, int* request, int process_request);
void free_matrix(int num_resource_types, int** matrix);
int** create_matrix(int num_processes, int num_resource_types);
void check_state(int num_processes, int num_resource_types, int** alloc, int** need, int* available);
int check_request(int num_processes, int num_resource_types, int process_request, int* request, int** alloc, int* available);

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
    int process_request = 0;
    int can_grant_request = 0;

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
    int* request = malloc(num_resource_types * sizeof(int));

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

    // print stats before doing requests
    printf("Number of Processes: %d\nNumber of Resources: %d\n", num_processes, num_resource_types);
    print_alloc_matrix(num_processes, num_resource_types, alloc);
    print_max_matrix(num_processes, num_resource_types, max);
    print_need_matrix(num_processes, num_resource_types, need);
    print_available_vector(num_resource_types, available);

    // check state
    check_state(num_processes, num_resource_types, alloc, need, available);

    // read the requests
    while(fscanf(file_pointer, "%d:", &process_request) == 1) {

        // read the rest of the request
        for(int i = 0; i < num_resource_types; i++) {
            fscanf(file_pointer, "%d", &request[i]);
        }

        // print the request
        print_request_vector(num_resource_types, request, process_request);

        // check if the request can be granted
        can_grant_request = check_request(num_processes, num_resource_types, process_request, request, alloc, available);

        // update the available vector if we could grant it
        if(can_grant_request) {
            print_available_vector(num_resource_types, available);
        }

    }

    // free the matricies and vector
    free_matrix(num_processes, alloc);                  // alloc matrix
    free_matrix(num_processes, max);                    // max matrix
    free_matrix(num_processes, need);                   // need matrix
    free(available);                                    // available vector
    free(request);                                      // request vector

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
    printf("\n\n");
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


void check_state(int num_processes, int num_resource_types, int** alloc, int** need, int* available) {
    /*
        computes if the system is in a safe state
    */

    int work[num_resource_types];
    int finish[num_processes];
    int can_satify = 1;
    int found = 1;
    int safe = 1;

    // copy the available vector
    for(int i = 0; i < num_resource_types; i++) {
        work[i] = available[i];
    }
    
    // set all processes to unfinished
    for(int i = 0; i < num_processes; i++) {
        finish[i] = 0;
    }

    while(found) {
        // reset the flag
        found = 0;

        // check every process
        for(int i = 0; i < num_processes; i++) {

            // find an unfinished process
            if(finish[i] == 0) {

                // reset the check
                can_satify = 1;

                // check every resource type need
                // if a need is > work, we cannot satisfy it
                for(int j = 0; j < num_resource_types; j++) {
                    if(need[i][j] > work[j]) {
                        can_satify = 0;
                        break;
                    }
                }

                if(can_satify == 1) {
                    // if it can be satisfied, add its 
                    // resources to the pool
                    for(int j = 0; j < num_resource_types; j++) {
                        work[j] = work[j] + alloc[i][j];
                    }

                    // set the process to finished
                    finish[i] = 1;
                    // set the found flag to true
                    found = 1;
                }
            }
        }
    }

    for(int i = 0; i < num_processes; i++) {
        // check that all processes finished
        // if not, set the state to 0 and break
        if(finish[i] == 0) {
            safe = 0;
            break;
        }
    }

    
    if(safe) {
        printf("The system is in a safe state.\n\n");
    }
    else {
        printf("The system is NOT in a safe state.\n\n");
    }
}


int check_request(int num_processes, int num_resource_types, int process_request, int* request, int** alloc, int* available) {
    /*
        checks if a request can be granted 

        params:
            num_processes: int - the number of different processes. This is the ROW count in the matricies
            num_resource_type: int - the number of different resource types. This is the COL count in the matricies
            process_request: int - the process number making the request.
            request: int* - the requested amounts for each resource type for process <process_request number> 
            alloc: int** - the allocation array for what each process already has
            available: int* - the available resources currently

        returns:
            int - 1 if the request can be granted. 0 otherwise.
    */

    // iterate through each resource request
    for(int i = 0; i < num_resource_types; i++) {

        // for each resource type, check if the sum of what has already been
        // alocated + the request for resource i is > what is available for resource i, 
        // then we cannot grant the request.
        if(alloc[process_request][i] + request[i] > available[i]) {
            printf("The request for process [%d] cannot be granted.\n\n", process_request);
            return 0;
        }
    }

    // if all the resources can be granted for each resource type, then we can grant
    printf("The request for process [%d] can be granted!\n\n", process_request);

    // update the available vector
    for(int i = 0; i < num_resource_types; i++) {
        available[i] = available[i] - request[i];
    }

    return 1;
}


void print_request_vector(int num_resource_types, int* request, int process_request) {
    /*
        prints the request vector
    */
    printf("Request Vector for process [%d]:\n", process_request);
    printf("   ");

    // dynamically print the resource types
    for(int i = 0; i < num_resource_types; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n   ");

    for(int i = 0; i < num_resource_types; i++) {
        printf("%d ", request[i]);
    }
    printf("\n\n");
}