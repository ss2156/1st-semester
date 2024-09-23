#include <stdio.h>

/**
 * Swap two integers
 * @param a: Pointer to the first integer
 * @param b: Pointer to the second integer
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Partition the array based on the pivot
 * @param arr: The array to be partitioned
 * @param l: The starting index
 * @param r: The ending index
 * @param pivot: The pivot element
 * @return The index of the pivot after partitioning
 */
int partition(int arr[], int l, int r, int pivot) {
    for (int i = l; i < r; i++) {
        if (arr[i] == pivot) {
            swap(&arr[i], &arr[r]); // Move pivot to the end
            break;
        }
    }
    int x = arr[r], i = l;
    for (int j = l; j <= r - 1; j++) {
        if (arr[j] <= x) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}

/**
 * Find the median of a small array (up to 5 elements)
 * @param arr: The array
 * @param l: Starting index
 * @param n: Number of elements
 * @return The median of the array
 */
int findMedian(int arr[], int l, int n) {
    for (int i = l; i < l + n - 1; i++) {
        for (int j = i + 1; j < l + n; j++) {
            if (arr[i] > arr[j]) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
    return arr[l + n / 2]; // Return the middle element
}

/**
 * Recursively find the k-th smallest element using Median of Medians algorithm
 * @param arr: The array
 * @param l: Starting index
 * @param r: Ending index
 * @param k: The rank of the element to find (1-based)
 * @return The k-th smallest element
 */
int kthSmallest(int arr[], int l, int r, int k) {
    if (k > 0 && k <= r - l + 1) {
        int n = r - l + 1;

        // Divide the array into groups of 5 and find the medians
        int i, median[(n + 4) / 5];
        for (i = 0; i < n / 5; i++)
            median[i] = findMedian(arr, l + i * 5, 5);
        if (i * 5 < n) {
            median[i] = findMedian(arr, l + i * 5, n % 5);
            i++;
        }

        // Find the median of the medians
        int medOfMed = (i == 1) ? median[i - 1] : kthSmallest(median, 0, i - 1, i / 2);

        // Partition the array around the median of medians
        int pos = partition(arr, l, r, medOfMed);

        if (pos - l == k - 1)
            return arr[pos]; // The element is found
        if (pos - l > k - 1)
            return kthSmallest(arr, l, pos - 1, k); // Recur in the left part

        return kthSmallest(arr, pos + 1, r, k - pos + l - 1); // Recur in the right part
    }
    return -1; // If k is out of bounds
}

/**
 * Find the median of an array using the Median of Medians algorithm
 * @param arr: The array
 * @param n: Number of elements
 * @return The median of the array
 */
int findMedianOfArray(int arr[], int n) {
    return kthSmallest(arr, 0, n - 1, (n + 1) / 2);
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    int arr[] = {12, 3, 5, 7, 4, 19, 26};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Median of the array is %d\n", findMedianOfArray(arr, n));

    return 0;
}
