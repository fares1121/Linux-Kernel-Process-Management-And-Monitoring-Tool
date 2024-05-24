#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure to represent a memory block
typedef struct MemoryBlock {
    int size; // Size of the memory block
    int start_address; // Start address of the memory block
    int end_address; // End address of the memory block
    bool is_allocated; // Memory block is allocated or not
    char process_name[10]; // Process name allocated to memory block
    struct MemoryBlock *next; // Next memory block
} MemoryBlock;

MemoryBlock* memory = NULL; // Memory blocks linked list
int num_blocks = 0; // Number of memory blocks
int total_memory = 0; // Total memory available

// Function prototypes
void initializeMemory(int size);
void allocateMemory(int size, char strategy, char process[10]);
void releaseMemory(char process[10]);
void compactMemory();
void printMemoryStatus();

// Initialize Memory Function
void initializeMemory(int size) {
    total_memory = size; // Total memory
    memory = (MemoryBlock*)malloc(sizeof(MemoryBlock)); // Allocate memory for the memory block
    memory->start_address = 0; // Start address of the memory block from 0
    memory->size = size;
    memory->end_address = size; 
    memory->is_allocated = false; // Memory block is not allocated
    strcpy(memory->process_name, "Unused"); // Process name as Unused
    num_blocks = 1; // Number of current memory blocks is 1
    memory->next = NULL;
}

// Allocate Memory Function
void allocateMemory(int size, char strategy, char process[10]) {
    
    // Check if the memory size is valid
    if (size <= 0) {
        printf("Error: Invalid memory size.\n");
        return;
    }
    
    // Check if the memory size is too large
    if (size > total_memory) {
        printf("Error: Memory size is too large.\n");
        return;
    }

    MemoryBlock* current = memory; // Current memory block
    int best_fit_memory_size; // Best fit memory size
    int worst_fit_memory_size; // Worst fit memory size

    while (current != NULL) { // Loop through the memory blocks

        if ((current->is_allocated == false) && (current->size >= size)) { // Check if the memory block is not allocated and the size is greater than or equal to the required size

            if (strategy == 'F') { // First fit strategy
                current->is_allocated = true;
                strcpy(current->process_name, process);
                int remaining_size = current->size - size; // Left memory size of allocated memory block
                
                if (remaining_size > 0) { // If some memory left after allocating the memory block
                    // Adjust memory block size and end address of the block allocated to the process
                    current->size = size;
                    current->end_address = current->start_address + size;

                    // Allocate a new memory block for the unused memory
                    MemoryBlock* left_memory = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                    left_memory->size = remaining_size;
                    left_memory->start_address = current->end_address + 1;
                    left_memory->end_address = left_memory->start_address + remaining_size;
                    left_memory->is_allocated = false;
                    strcpy(left_memory->process_name, "Unused");

                    // Add the new memory block to the memory blocks linked list
                    left_memory->next = current->next;
                    current->next = left_memory;
                    num_blocks++;
                }

                printf("Memory allocated for process %s\n", process);
                break;
            }
            else if (strategy == 'B') { // Best fit strategy
                best_fit_memory_size = current->size;

                if (best_fit_memory_size == size) { // If memory block size is equal to the memory request size, allocate memory block to the process
                    current->is_allocated = true;
                    strcpy(current->process_name, process);
                } 
                else { // Otherwise, find the smallest memory block that fits the request size
                    MemoryBlock* best_fit_current = current;

                    while (current->next != NULL) {
                        current = current->next;

                        if ((current->is_allocated == false) && (current->size >= size) && (current->size < best_fit_memory_size)) {
                            best_fit_current = current;
                            best_fit_memory_size = current->size;

                            if (best_fit_memory_size == size) { // If memory block size is equal to the memory request size
                                current->is_allocated = true;
                                strcpy(current->process_name, process);
                                break;
                            }

                        }

                    }

                    int remaining_size = best_fit_current->size - size; // Left memory size of allocated memory block
                    best_fit_current->is_allocated = true;
                    strcpy(best_fit_current->process_name, process);

                    if (remaining_size > 0) { // If some memory left from allocated the memory block to the process
                        // Adjust the memory size and end address of the allocated memory block
                        best_fit_current->size = size;
                        best_fit_current->end_address = best_fit_current->start_address + size;

                        // Allocate a new memory block for the unused memory
                        MemoryBlock* left_memory = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                        left_memory->size = remaining_size;
                        left_memory->start_address = best_fit_current->end_address + 1;
                        left_memory->end_address = left_memory->start_address + remaining_size;
                        left_memory->is_allocated = false;
                        strcpy(left_memory->process_name, "Unused");

                        // Add the new memory block to the memory blocks linked list
                        left_memory->next = best_fit_current->next;
                        best_fit_current->next = left_memory;
                        num_blocks++;
                    }

                }

                printf("Memory allocated for process %s\n", process);
                break;
            } 
            else if (strategy == 'W') { // Worst fit strategy
                worst_fit_memory_size = current->size;
                MemoryBlock* worst_fit_current = current;

                // Find the largest memory block
                while (current->next != NULL) {
                    current = current->next;

                    if ((current->is_allocated == false) && (current->size > worst_fit_memory_size)) {
                        worst_fit_current = current;
                        worst_fit_memory_size = current->size;
                    }

                }

                int remaining_size = worst_fit_current->size - size; // Left memory after allocating the memory block
                worst_fit_current->is_allocated = true;
                strcpy(worst_fit_current->process_name, process);

                if (remaining_size > 0) { // If some memory is left after allocating the block to the process
                    // Adjust the memory size and end address of the block allocated to the process
                    worst_fit_current->size = size;
                    worst_fit_current->end_address = worst_fit_current->start_address + size;

                    // Allocate a new memory block for the unused memory
                    MemoryBlock* left_memory = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                    left_memory->size = remaining_size;
                    left_memory->start_address = worst_fit_current->end_address + 1;
                    left_memory->end_address = left_memory->start_address + remaining_size;
                    left_memory->is_allocated = false;
                    strcpy(left_memory->process_name, "Unused");

                    // Add the new memory block to the memory blocks linked list
                    left_memory->next = worst_fit_current->next;
                    worst_fit_current->next = left_memory;
                    num_blocks++;
                }

                printf("Memory allocated for process %s\n", process);
                break;
            } 
            else { // Display error message for invalid allocation strategy
                printf("Error: Invalid allocation strategy.\n");
                break;
            }

        }

        current = current->next;
    }

    if (current == NULL) { // Display error message if unable to allocate a memory block
        printf("Error: Memory allocation failed.\n");
    }

}

// Release Memory Function
void releaseMemory(char process[10]) {
    MemoryBlock* current = memory;
    MemoryBlock* prev = NULL;

    while (current != NULL) { // Loop through the memory blocks

        if (strcmp(current->process_name, process) == 0) { // If process found
            current->is_allocated = false;
            strcpy(current->process_name, "Unused");

            if ((prev != NULL) && (prev->is_allocated == false)) { // If previous memory block is unused
                prev->size += current->size; // Add current block memory to the previous block memory
                prev->end_address = current->end_address;
                prev->next = current->next; // Remove current block from memory blocks linked list
                free(current); // Free memory of the current block
                current = prev;
                num_blocks--;
            }

            if ((current->next != NULL) && (current->next->is_allocated == false)) { // If next memory block is unused
                current->size += current->next->size; // Add next block memory to current block memory
                current->end_address = current->next->end_address;
                MemoryBlock* temp = current->next;
                current->next = current->next->next; // Remove next block from memory blocks linked list
                free(temp); // Free memory of the next block
                num_blocks--;
            }

            printf("Memory relased for process %s\n", process);
            break;
        }

        prev = current;
        current = current->next;
    }

    if (current == NULL) { // Display an error message if memory release is failed
        printf("Memory release failed!\n");
    }

}

// Compact Memory Function
void compactMemory() {
    MemoryBlock* current = memory;
    MemoryBlock* prev = NULL;
    MemoryBlock* first_hole = NULL; // First unused memory block
    MemoryBlock* first_hole_prev = NULL;
    int holes_added_size = 0; // Total holes sizes that will be added to the first unused hole

    while (current != NULL) { // Loop through the memory blocks

        // Find first unused memory block
        if (current->is_allocated == false) {
            first_hole = current;
            prev = current;
            current = current->next;
            break;
        }

        prev = current;
        current = current->next;
    }

    if (first_hole != NULL) { // If an unused memory block was found

        while (current != NULL) { // Loop through the remaining memory blocks

            // Find other unused memory blocks
            if (current->is_allocated == false) {
                holes_added_size += current->size;
                prev->next = current->next; // Remove unused hole from memory blocks linked list
                free(current); // Free memory of the unused memory block
                current = prev->next;
                num_blocks--;
            }
            else {
                prev = current;
                current = current->next;
            }

        }

        if (holes_added_size > 0) { // If found other unused memory blocks
            // Add total unused memory to the first hole
            first_hole->size += holes_added_size;
            first_hole->end_address += holes_added_size;

            first_hole_prev = first_hole;
            first_hole = first_hole->next;

            // Update memory blocks allocated to processess addresses 
            while (first_hole != NULL) {
                first_hole->start_address = first_hole_prev->end_address + 1;
                first_hole->end_address = first_hole->start_address + first_hole->size;
                first_hole_prev = first_hole;
                first_hole = first_hole->next;
            }

        }

    }

    printf("Succesful compaction!\n");
}

// Print Memory Status Function
void printMemoryStatus() {
    MemoryBlock* block = memory;

    while (block != NULL) {
        printf("Addresses [%d:%d] Process %s\n", block->start_address, block->end_address, block->process_name);
        block = block->next;
    }

}

int main(int argc, char *argv[]) {
    
    // Check if the user entered the initial memory size
    if (argc != 2) {
        printf("Usage: ./allocator <initial_memory_size>\n");
        return 1;
    }

    // Initialize memory
    int initial_memory_size = atoi(argv[1]);
    initializeMemory(initial_memory_size);

    char command[10]; // Command entered by the user

    while (1) {
        printf("allocator> ");
        scanf("%s", command); // Get user command

        if (strcmp(command, "X") == 0) { // Exit program
            printf("Program exited. Good bye!\n");
            break;
        } else if (strcmp(command, "RQ") == 0) { // Request memory for process
            char process_name[10];
            int process_size;
            char strategy;
            scanf("%s %d %c", process_name, &process_size, &strategy);
            allocateMemory(process_size, strategy, process_name);
        } else if (strcmp(command, "RL") == 0) { // Release memory from process
            char process_name[10];
            scanf("%s", process_name);
            releaseMemory(process_name);
        } else if (strcmp(command, "C") == 0) { // Compact unused memory blocks
            compactMemory();
        } else if (strcmp(command, "STAT") == 0) { // Report memory blocks status
            printMemoryStatus();
        }
        else { // Invalid command
            printf("Invalid command\n");
        }

    }

    return 0;
}
