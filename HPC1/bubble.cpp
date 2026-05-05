#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// 🔹 Print
void printArray(const vector<int>& arr) { //Takes array as input
    for (int x : arr) cout << x << " "; //Range-based loop → prints each element
    cout << endl;
}

// 🔹 Sequential Bubble Sort
void bubbleSort(vector<int>& arr) { //Pass array by reference
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) { //Outer loop → number of passes
        for (int j = 0; j < n - i - 1; j++) { //Inner loop → compares adjacent elements
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]); //Largest element moves to end each pass
            }
        }
    }
}

// 🔹 Parallel Bubble Sort (Odd-Even)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool isSorted = false; //isSorted → checks if sorting is complete

    while (!isSorted) {
        isSorted = true; //assume sorted initially
        bool sorted_local = true; //sorted_local → used in parallel safely

        // Even phase
        #pragma omp parallel for reduction(&&: sorted_local)
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]); //swap if needed
                sorted_local = false; //Mark array as not sorted
            } 
        }
// reduction(&&: sorted_local)? Each thread has its own copy
// Final result = AND of all threads
// Prevents race condition
        // Odd phase
        #pragma omp parallel for reduction(&&: sorted_local)
        for (int i = 1; i < n - 1; i += 2) { //Compares:(1,2), (3,4), (5,6)...
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted_local = false;
            }
        }

        isSorted = sorted_local; //If no swaps happened → stop loop
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    vector<int> seq = arr; //copy array: seq=sequential
    vector<int> par = arr;

    double start, end;

    // 🔹 Sequential
    start = omp_get_wtime();
    bubbleSort(seq);
    end = omp_get_wtime();
    cout << "\nSequential Bubble Time: " << end - start << endl;

    // 🔹 Parallel
    start = omp_get_wtime();
    parallelBubbleSort(par);
    end = omp_get_wtime();
    cout << "Parallel Bubble Time:   " << end - start << endl;

    cout << "\nSorted Array: ";
    printArray(par);

    return 0;
}

// Enter number of elements: 4
// Enter elements:
// 8 4 2 1

// Sequential Bubble Time: 0
// Parallel Bubble Time:   0.00200009

// Sorted Array: 1 2 4 8 

// 1. What is Bubble Sort?
// A simple comparison-based sorting algorithm that repeatedly swaps adjacent elements if they are in the wrong order.

// 2. Time complexity of Bubble Sort?
// Worst and average: O(n²), Best: O(n) (if already sorted with optimization).

// 3. Why is Bubble Sort inefficient?
// Because it compares each pair repeatedly, leading to O(n²) complexity.

// 🔹 Parallel Bubble Sort

// 4. How did you parallelize Bubble Sort?
// Using the odd-even transposition method, where independent pairs are compared in parallel.

// 5. What is Odd-Even Transposition Sort?
// It alternates between:

// Even phase: compare (0,1), (2,3)...
// Odd phase: compare (1,2), (3,4)...

// 6. Why is odd-even method used?
// Because adjacent swaps in normal bubble sort are dependent, so we separate them into independent pairs for parallel execution.

// 🔹 OpenMP Questions

// 7. What is #pragma omp parallel for?
// It divides loop iterations among multiple threads to execute in parallel.

// 8. What is reduction in OpenMP?
// It combines results from all threads safely using an operation (like AND, sum, etc.).

// 9. Why did you use reduction(&&: sorted_local)?
// To safely check if the array is sorted across all threads without race conditions.

// 10. What happens without reduction?
// Race condition may occur, leading to incorrect sorted status.

// 🔹 Code Logic Questions

// 11. Why use while (!isSorted) loop?
// To stop early when no swaps occur, improving efficiency.

// 12. What is sorted_local?
// A thread-safe variable to track if any swap happened during parallel execution.

// 13. Why are there two loops (even and odd)?
// To ensure all adjacent pairs are compared without conflicts.

// 🔹 Performance Questions

// 14. Is parallel Bubble Sort always faster?
// No. For small inputs, overhead of thread creation may make it slower.

// 15. When does parallel Bubble Sort perform better?
// For large input sizes where parallel work outweighs overhead.

// 16. What is overhead?
// Extra cost of thread creation, synchronization, and communication.

// 🔹 Comparison Questions

// 17. Difference between sequential and parallel Bubble Sort?
// Sequential processes one pair at a time; parallel processes multiple independent pairs simultaneously.

// 18. Is Bubble Sort suitable for parallelization?
// Not very efficient compared to other algorithms, but can be parallelized using odd-even method.