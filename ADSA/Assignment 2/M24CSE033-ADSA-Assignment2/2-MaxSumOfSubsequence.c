#include <stdio.h>
#include <stdlib.h>

// Function to compare two integers for qsort
int compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);  // Sort in descending order
}

// Function to find the k-th largest element in the array
int findKthLargest(int arr[], int n, int k) {
    // Create a copy of the array and sort it
    int *copy = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        copy[i] = arr[i];
    }
    qsort(copy, n, sizeof(int), compare);  // Sort array in descending order
    int kthLargest = copy[k - 1];          // k-th largest element
    free(copy);
    return kthLargest;
}

// Function to find the maximum sum of a subsequence such that no two elements are adjacent
int maxNonAdjacentSum(int arr[], int n, int kthLargest) {
    int include = 0, exclude = 0;
    
    for (int i = 0; i < n; i++) {
        // If the current element is the k-th largest, include it in the sum
        if (arr[i] == kthLargest) {
            include = exclude + arr[i];
        } else {
            // Current max sum excluding the current element
            int new_exclude = (include > exclude) ? include : exclude;
            // Update include to the previous exclude + current element
            include = exclude + arr[i];
            // Update exclude to the new exclude value
            exclude = new_exclude;
        }
    }
    // Return the maximum of include and exclude
    return (include > exclude) ? include : exclude;
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    int arr[] = {5, 5, 10, 100, 10, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 2;
    
    // Step 1: Find the k-th largest element
    int kthLargest = findKthLargest(arr, n, k);
    printf("The %d-th largest element is: %d\n", k, kthLargest);
    
    // Step 2: Find the maximum sum of a subsequence including the k-th largest element
    int maxSum = maxNonAdjacentSum(arr, n, kthLargest);
    printf("Maximum sum of subsequence including %d: %d\n", kthLargest, maxSum);
    
    return 0;
}
