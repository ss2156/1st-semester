#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS 100

typedef struct {
    int start;
    int end;
} MemoryBlock;

MemoryBlock freeBlocks[MAX_BLOCKS];
int freeCount = 0;

// Function to print the largest free block
void printLargestFreeBlock() {
    int max_size = 0;
    MemoryBlock largest = {-1, -1};
    
    for (int i = 0; i < freeCount; i++) {
        int size = freeBlocks[i].end - freeBlocks[i].start;
        if (size > max_size) {
            max_size = size;
            largest = freeBlocks[i];
        }
    }
    
    if (largest.start != -1) {
        printf("[%d %d]\n", largest.start, largest.end);
    }
}

// Function to merge adjacent free blocks
void mergeFreeBlocks() {
    for (int i = 0; i < freeCount - 1; i++) {
        if (freeBlocks[i].end == freeBlocks[i + 1].start) {
            freeBlocks[i].end = freeBlocks[i + 1].end;
            // Shift the remaining blocks
            for (int j = i + 1; j < freeCount - 1; j++) {
                freeBlocks[j] = freeBlocks[j + 1];
            }
            freeCount--;
            i--; // Recheck the merged block
        }
    }
}

// Function to allocate memory
void allocateMemory(int size) {
    for (int i = 0; i < freeCount; i++) {
        int blockSize = freeBlocks[i].end - freeBlocks[i].start;
        if (blockSize >= size) {
            printf("[%d %d]\n", freeBlocks[i].start, freeBlocks[i].start + size - 1);
            freeBlocks[i].start += size;
            if (freeBlocks[i].start == freeBlocks[i].end) {
                // If the block is fully allocated, remove it from the list
                for (int j = i; j < freeCount - 1; j++) {
                    freeBlocks[j] = freeBlocks[j + 1];
                }
                freeCount--;
            }
            mergeFreeBlocks(); // Merge if necessary
            printLargestFreeBlock();
            return;
        }
    }
}

// Function to add free memory block back and merge if necessary
void freeMemory(int start, int end) {
    freeBlocks[freeCount].start = start;
    freeBlocks[freeCount].end = end;
    freeCount++;
    mergeFreeBlocks(); // Merge adjacent blocks
    printLargestFreeBlock();
}

int main() {
    int n;
    scanf("%d", &n); // Input number of free blocks

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &freeBlocks[i].start, &freeBlocks[i].end);
        freeCount++;
    }

    int size;
    scanf("%d", &size); // Input the size to allocate

    allocateMemory(size); // Allocate the requested memory size

    return 0;
}
