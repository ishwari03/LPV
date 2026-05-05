#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n); //Creates dynamic array of size n

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int sum = 0;
    int minVal = arr[0];
    int maxVal = arr[0];

    #pragma omp parallel for reduction(+:sum) reduction(min:minVal) reduction(max:maxVal)
    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] < minVal)
            minVal = arr[i];

        if (arr[i] > maxVal)
            maxVal = arr[i];
    }

    float avg = (float)sum / n; //Avoids integer division

    cout << "\nSum = " << sum << endl;
    cout << "Min = " << minVal << endl;
    cout << "Max = " << maxVal << endl;
    cout << "Average = " << avg << endl;

    return 0;
}

// Enter number of elements: 4
// Enter elements:
// 8 1 4 5
// Sum = 18
// Min = 1
// Max = 8
// Average = 4.5

// #include <iostream>
// #include <vector>
// #include <omp.h>
// #include <climits>
// #include <cstdlib>
// using namespace std;

// /* 🔹 Sequential Functions */
// void min_seq(vector<int>& arr) {
//     int min_value = INT_MAX;
//     for (int i = 0; i < arr.size(); i++) {
//         if (arr[i] < min_value) {
//             min_value = arr[i];
//         }
//     }
//     cout << "Minimum value: " << min_value << endl;
// }

// void max_seq(vector<int>& arr) {
//     int max_value = INT_MIN;
//     for (int i = 0; i < arr.size(); i++) {
//         if (arr[i] > max_value) {
//             max_value = arr[i];
//         }
//     }
//     cout << "Maximum value: " << max_value << endl;
// }

// void sum_seq(vector<int>& arr) {
//     int sum = 0;
//     for (int i = 0; i < arr.size(); i++) {
//         sum += arr[i];
//     }
//     cout << "Sum: " << sum << endl;
// }

// void average_seq(vector<int>& arr) {
//     int sum = 0;
//     for (int i = 0; i < arr.size(); i++) {
//         sum += arr[i];
//     }
//     cout << "Average: " << (double)sum / arr.size() << endl;
// }

// /* 🔹 Parallel Functions */
// void min_reduction(vector<int>& arr) {
//     int min_value = INT_MAX;
//     #pragma omp parallel for reduction(min: min_value)
//     for (int i = 0; i < arr.size(); i++) {
//         if (arr[i] < min_value) {
//             min_value = arr[i];
//         }
//     }
//     cout << "Minimum value: " << min_value << endl;
// }

// void max_reduction(vector<int>& arr) {
//     int max_value = INT_MIN;
//     #pragma omp parallel for reduction(max: max_value)
//     for (int i = 0; i < arr.size(); i++) {
//         if (arr[i] > max_value) {
//             max_value = arr[i];
//         }
//     }
//     cout << "Maximum value: " << max_value << endl;
// }

// void sum_reduction(vector<int>& arr) {
//     int sum = 0;
//     #pragma omp parallel for reduction(+: sum)
//     for (int i = 0; i < arr.size(); i++) {
//         sum += arr[i];
//     }
//     cout << "Sum: " << sum << endl;
// }

// void average_reduction(vector<int>& arr) {
//     int sum = 0;
//     #pragma omp parallel for reduction(+: sum)
//     for (int i = 0; i < arr.size(); i++) {
//         sum += arr[i];
//     }
//     cout << "Average: " << (double)sum / arr.size() << endl;
// }

// int main() {
//     cout << "Generating random array...\n";

//     int n = 10000000;   // 🔥 very large input

//     vector<int> arr(n);

//     // 🔥 generate random data
//     for (int i = 0; i < n; i++) {
//         arr[i] = rand();
//     }

//     double start, end;

//     /* 🔹 Sequential */
//     cout << "\nSequential:\n";
//     start = omp_get_wtime();
//     min_seq(arr);
//     max_seq(arr);
//     sum_seq(arr);
//     average_seq(arr);
//     end = omp_get_wtime();
//     cout << "Sequential Time: " << end - start << " seconds\n\n";

//     /* 🔹 Parallel */
//     cout << "Parallel:\n";
//     start = omp_get_wtime();
//     min_reduction(arr);
//     max_reduction(arr);
//     sum_reduction(arr);
//     average_reduction(arr);
//     end = omp_get_wtime();
//     cout << "Parallel Time: " << end - start << " seconds\n";

//     return 0;
// }

// 1. What is OpenMP?

// 👉

// OpenMP is an API for parallel programming in C/C++ that uses multiple threads to execute code simultaneously on multi-core processors.

// 🔹 2. What does #pragma omp parallel for do?

// 👉

// It divides loop iterations among multiple threads so they run in parallel.

// 🔹 3. What is reduction in OpenMP?

// 👉

// Reduction is a mechanism that combines results from multiple threads into a single variable safely.

// 🔹 4. Why do we use reduction here?

// 👉

// To avoid race conditions when multiple threads update shared variables like sum, min, and max.

// 🔹 5. What is a race condition?

// 👉

// A situation where multiple threads try to modify the same variable simultaneously, causing incorrect results.

// 🔹 6. What happens if we remove reduction?

// 👉

// The output will be incorrect because threads will overwrite each other’s results.

// 🔹 7. How does reduction work internally?

// 👉

// Each thread gets a private copy of the variable, computes its part, and then all values are combined at the end.

// 🔹 8. Why can we use multiple reductions in one loop?

// 👉

// OpenMP allows multiple reduction clauses, so we can compute sum, min, and max in a single pass efficiently.

// 🔹 9. Why initialize minVal and maxVal with arr[0]?

// 👉

// To ensure comparisons start with a valid element from the array.

// 🔹 10. Why typecast (float)sum?

// 👉

// To avoid integer division and get a correct decimal average.

// 🔹 11. What is the advantage of parallel reduction?

// 👉

// It improves performance by using multiple cores and ensures thread safety.

// 🔹 12. What is the time complexity?

// 👉

// Sequential: O(n)
// Parallel: Approximately O(n / number_of_threads)

// 🔹 13. What is the difference between reduction and critical?

// 👉

// Reduction	Critical
// Faster	Slower
// No locking needed	Uses lock
// Automatic combining	Manual

// 👉 Answer:

// Reduction is more efficient than critical because it avoids locking overhead.

// 🔹 14. What is the difference between reduction and atomic?

// 👉

// Atomic is for single operations, while reduction is for combining results across threads.

// 🔹 15. Can reduction be used for all operations?

// 👉

// No, only for associative operations like +, min, max, *, etc.

// 🔹 16. What happens if array size is very small?

// 👉

// Parallel overhead may be higher than benefit, so sequential may be faster.

// 🔹 17. What is thread?

// 👉

// A thread is a lightweight process that runs a part of a program independently.

// 🔹 18. How many threads will run this code?

// 👉

// Depends on system (usually equal to number of CPU cores).

// 🔹 19. How to control number of threads?

// 👉

// omp_set_num_threads(4);
// 🔹 20. Why use vector instead of array?

// 👉

// Vector is dynamic, safer, and standard in C++.