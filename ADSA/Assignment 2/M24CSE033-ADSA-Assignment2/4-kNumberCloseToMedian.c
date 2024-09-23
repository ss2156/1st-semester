#include <stdio.h>
#include <stdlib.h>

// Swap function
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function similar to quicksort partition
int partition(int arr[], int left, int right, int pivotIndex) {
    int pivotValue = arr[pivotIndex];
    swap(&arr[pivotIndex], &arr[right]);  // Move pivot to end
    int storeIndex = left;

    for (int i = left; i < right; i++) {
        if (arr[i] < pivotValue) {
            swap(&arr[storeIndex], &arr[i]);
            storeIndex++;
        }
    }
    swap(&arr[right], &arr[storeIndex]);  // Move pivot to its final place
    return storeIndex;
}

// Quickselect algorithm to find the k-th smallest element in an unsorted array
int quickselect(int arr[], int left, int right, int k) {
    if (left == right)  // If the list contains only one element
        return arr[left];

    // Select a pivotIndex between left and right
    int pivotIndex = left + rand() % (right - left + 1);
    pivotIndex = partition(arr, left, right, pivotIndex);

    // The pivot is in its final sorted position
    if (k == pivotIndex)
        return arr[k];
    else if (k < pivotIndex)
        return quickselect(arr, left, pivotIndex - 1, k);
    else
        return quickselect(arr, pivotIndex + 1, right, k);
}

// Function to find the median using the quickselect method
int findMedian(int arr[], int n) {
    return quickselect(arr, 0, n - 1, n / 2);
}

// Function to find the k numbers closest to the median
void findKClosestToMedian(int S[], int n, int k) {
    // Step 1: Find the median of the array S
    int* copy = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        copy[i] = S[i];
    }
    int median = findMedian(copy, n);
    printf("Median: %d\n", median);
    
    // Step 2: Create an array of absolute differences from the median
    int* absDiffs = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        absDiffs[i] = abs(S[i] - median);
    }
    
    // Step 3: Find the k-th smallest absolute difference using quickselect
    int kthSmallestDiff = quickselect(absDiffs, 0, n - 1, k - 1);
    
    // Step 4: Collect the k elements closest to the median
    printf("The %d numbers closest to the median are:\n", k);
    for (int i = 0; i < n && k > 0; i++) {
        if (abs(S[i] - median) <= kthSmallestDiff) {
            printf("%d ", S[i]);
            k--;
        }
    }
    printf("\n");
    
    free(copy);
    free(absDiffs);
}

int main() {

    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    
    int S[] = {12, 3, 17, 13, 26, 9, 5, 8, 15, 10, 7};
    int n = sizeof(S) / sizeof(S[0]);
    int k = 4;  // Find 4 numbers closest to the median
    
    findKClosestToMedian(S, n, k);
    
    return 0;
}
