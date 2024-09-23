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

// Helper function to return the maximum of two values
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to find the maximum sum of a subsequence such that:
// 1. No two elements are adjacent.
// 2. The sum of the subsequence is divisible by d.
int maxNonAdjacentSumDivisibleByD(int arr[], int n, int d, int kthLargest) {
    int include = 0, exclude = 0;

    for (int i = 0; i < n; i++) {
        // Check if we can include the current element
        int new_include;
        if (arr[i] == kthLargest) {
            // Make sure to include the k-th largest element
            new_include = exclude + arr[i];
        } else {
            // General case, choose the max between including or excluding
            new_include = exclude + arr[i];
        }

        // Update exclude to be the maximum of previous include or exclude
        exclude = max(include, exclude);
        include = new_include;
    }

    // At this point, we need to check which of include or exclude is divisible by d
    int maxSum = max(include, exclude);
    if (maxSum % d == 0) {
        return maxSum;
    } else {
        // Try to include the k-th largest element forcefully
        int result = max(kthLargest, (maxSum + kthLargest) % d == 0 ? maxSum + kthLargest : kthLargest);
        return result;
    }
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    int arr[] = {5, 5, 10, 100, 10, 5};  // Example array
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 2;  // k-th largest element
    int d = 10; // Given divisor

    // Step 1: Find the k-th largest element
    int kthLargest = findKthLargest(arr, n, k);
    printf("The %d-th largest element is: %d\n", k, kthLargest);

    // Step 2: Find the maximum sum of subsequence including the k-th largest element
    // and divisible by d
    int maxSum = maxNonAdjacentSumDivisibleByD(arr, n, d, kthLargest);
    printf("Maximum sum of subsequence divisible by %d: %d\n", d, maxSum);

    return 0;
}
