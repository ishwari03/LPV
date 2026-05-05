#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// 🔹 Print
void printArray(const vector<int>& arr) { //Takes array as input
    for (int x : arr) cout << x << " "; //rints each element
    cout << endl;
}

// 🔹 Merge
void merge(vector<int>& arr, int s, int m, int e) { //Combine two sorted halves
    vector<int> L(arr.begin() + s, arr.begin() + m + 1);
    vector<int> R(arr.begin() + m + 1, arr.begin() + e + 1);

    int i = 0, j = 0, k = s;
// i → index for L
// j → index for R
// k → index for original array
    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j]) arr[k++] = L[i++]; //Put smaller element into original array
        else arr[k++] = R[j++];
    }

    while (i < L.size()) arr[k++] = L[i++]; //Copy leftovers elem
    while (j < R.size()) arr[k++] = R[j++];
}

// 🔹 Sequential Merge Sort
void mergeSort(vector<int>& arr, int s, int e) {
    if (s >= e) return; //1 element → already sorted

    int m = (s + e) / 2; //find middle

    mergeSort(arr, s, m); //recursovley sort leeft half and right half
    mergeSort(arr, m + 1, e);

    merge(arr, s, m, e); //merge sorted halves
}

// 🔹 Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int s, int e, int depth) {
    if (s >= e) return;

    int m = (s + e) / 2;

    if (depth <= 0) { //If depth limit reached:switch to seq
        mergeSort(arr, s, e);
    } else {
        #pragma omp parallel sections //Create parallel region with multiple sections
        {
            #pragma omp section
            parallelMergeSort(arr, s, m, depth - 1); //One thread handles left half

            #pragma omp section
            parallelMergeSort(arr, m + 1, e, depth - 1);
        }
    }
 
    merge(arr, s, m, e); //Combine results after both threads finish
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    vector<int> seq = arr;
    vector<int> par = arr;

    double start, end;

    // 🔹 Sequential
    start = omp_get_wtime();
    mergeSort(seq, 0, n - 1);
    end = omp_get_wtime();
    cout << "\nSequential Merge Time: " << end - start << endl;

    // 🔹 Parallel
    start = omp_get_wtime();
    parallelMergeSort(par, 0, n - 1, 3);  //depth = 3 → limits recursion parallelism
    end = omp_get_wtime();
    cout << "Parallel Merge Time:   " << end - start << endl;

    cout << "\nSorted Array: ";
    printArray(par);

    return 0;
}

// Enter number of elements: 4
// Enter elements:
// 8 4 2 1

// Sequential Merge Time: 0
// Parallel Merge Time:   0.0140002

// Sorted Array: 1 2 4 8 

//1. What is Merge Sort?
// Merge Sort is a divide-and-conquer sorting algorithm that divides an array into halves, sorts them recursively, and then merges the sorted halves.

// 2. What is the time complexity of Merge Sort?
// Best, average, and worst case: O(n log n)

// 3. Why is Merge Sort efficient?
// Because it consistently divides the array into halves and performs linear-time merging.

// 4. What is the space complexity of Merge Sort?
// O(n), due to temporary arrays used during merging.

// 5. Is Merge Sort stable?
// Yes, it is stable because equal elements maintain their relative order during merging.

// 🔹 Algorithm Understanding

// 6. What is divide and conquer?
// A technique where a problem is divided into smaller subproblems, solved independently, and combined.

// 7. What are the three steps of Merge Sort?

// Divide the array
// Conquer (recursively sort halves)
// Merge sorted halves

// 8. Why do we use the merge function?
// To combine two sorted subarrays into one sorted array.

// 9. Why is recursion used in Merge Sort?
// Because each half is repeatedly divided until base case (single element).

// 10. What is the base condition in Merge Sort?
// When the subarray size becomes 1 (already sorted).

// 🔹 Parallel Merge Sort (OpenMP)

// 11. How is Merge Sort parallelized?
// By executing recursive sorting of left and right halves concurrently using OpenMP sections.

// 12. What is #pragma omp parallel sections?
// It divides code into independent sections that run in parallel threads.

// 13. Why is parallelism applied in Merge Sort?
// Because left and right halves are independent and can be processed simultaneously.

// 14. What is the role of depth parameter in your code?
// It limits recursion depth to avoid excessive thread creation and overhead.

// 15. Why not parallelize entire recursion?
// Too many threads would increase overhead and reduce performance.

// 🔹 Performance Questions

// 16. Is parallel Merge Sort always faster?
// No. For small inputs, overhead of thread creation makes it slower.

// 17. When does parallel Merge Sort perform better?
// For large datasets where computation outweighs threading overhead.

// 18. What is overhead in parallel Merge Sort?
// Cost of thread creation, synchronization, and context switching.

// 19. Why does merge step remain sequential?
// Because merging requires combining results in order; it has data dependency.

// 🔹 Code-Specific Questions

// 20. Why do we create L and R arrays in merge?
// To temporarily store left and right halves for comparison.

// 21. Why is merge function important?
// It combines two sorted arrays into a single sorted array.

// 22. What happens if merge is not used?
// The array will remain partially sorted and incorrect.

// 23. Why do we use vector instead of array?
// Vectors are dynamic and easier to manage in recursive algorithms.

// 🔹 Concept Comparison

// 24. Merge Sort vs Bubble Sort?
// Merge Sort: O(n log n), efficient, divide & conquer
// Bubble Sort: O(n²), simple but slow

// 25. Why is Merge Sort preferred over Bubble Sort?
// Because it is significantly faster for large datasets.