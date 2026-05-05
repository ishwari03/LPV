#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// ---------------- Sequential DFS ----------------
void sequentialDFSUtil(int node, vector<vector<int>> &tree, vector<bool> &visited) {
    cout << node << " ";
    visited[node] = true;

    for (int i = 0; i < tree[node].size(); i++) { //Loop through neighbors
        int child = tree[node][i]; //Get neighbor

        if (!visited[child]) { //If not visited → go deeper
            sequentialDFSUtil(child, tree, visited); //Recursive call (this is DFS core!)
        }
    }
}

void sequentialDFS(vector<vector<int>> &tree, int root, int n) {
    vector<bool> visited(n, false); //Initially all nodes unvisited

    cout << "\nSequential DFS: ";
    sequentialDFSUtil(root, tree, visited); //Start DFS from root
    cout << endl;
}

// ---------------- Parallel DFS (Task-based) ----------------
void parallelDFSUtil(int node, vector<vector<int>> &tree, vector<bool> &visited) {

    bool doWork = false;

    #pragma omp critical
    {
        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";
            doWork = true;
        }
    }

    if (!doWork) return; //if node already visisted skip node

    for (int i = 0; i < tree[node].size(); i++) { //Explore all children
        int child = tree[node][i];

        if (!visited[child]) {
            #pragma omp task //Create a new thread/task for this child So:Multiple branches run in parallel
            parallelDFSUtil(child, tree, visited);
        }
    }
}

void parallelDFS(vector<vector<int>> &tree, int root, int n) {
    vector<bool> visited(n, false);

    cout << "\nParallel DFS: ";

    #pragma omp parallel //Start multiple threads
    {
        #pragma omp single //Only ONE thread starts recursion,others execute tasks
        parallelDFSUtil(root, tree, visited);
    }

    cout << endl;
}

// ---------------- MAIN ----------------
int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    vector<vector<int>> tree(n);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    int root;
    cout << "Enter root: ";
    cin >> root;

    double start, end;

    // -------- Sequential DFS --------
    start = omp_get_wtime();
    sequentialDFS(tree, root, n);
    end = omp_get_wtime();
    cout << "Sequential DFS Time: " << (end - start) << " seconds\n";

    // -------- Parallel DFS --------
    start = omp_get_wtime();
    parallelDFS(tree, root, n);
    end = omp_get_wtime();
    cout << "Parallel DFS Time: " << (end - start) << " seconds\n";

    return 0;
}


// Enter number of nodes: 7
// Enter edges (u v):
// 0 1 
// 0 2
// 1 3
// 1 4
// 2 5
// 2 6
// Enter root: 0

// Sequential DFS: 0 1 3 4 2 5 6 
// Sequential DFS Time: 0.00300002 seconds

// Parallel DFS: 0 1 2 3 4 5 6 
// Parallel DFS Time: 0.00399995 seconds
//“Parallel DFS is slower here because the input size is small and overhead of thread creation, synchronization, and critical sections dominates execution time. Parallelism benefits appear only for large datasets.”

//1. What is DFS?
// DFS (Depth First Search) is a graph traversal method that explores as deep as possible along a branch before backtracking.

// 2. Which data structures are used in DFS?
// Recursion (implicit stack) or an explicit stack.

// 3. Time complexity of DFS?
// O(V + E), where V = vertices and E = edges.

// 4. Why do we use a visited array?
// To avoid revisiting nodes and prevent infinite loops in cyclic graphs.

// 5. Difference between BFS and DFS?
// BFS is level-order using a queue; DFS is depth-first using recursion/stack.

// 6. Is DFS unique?
// No. Traversal order can vary based on adjacency order.

// 7. What is backtracking in DFS?
// Returning to the previous node after exploring all children.

// 8. What type of graph did you use?
// Undirected graph (tree), stored as an adjacency list.

// 🔹 Code-Specific DFS Questions

// 9. Why is DFS implemented recursively?
// Recursion naturally follows depth-first exploration.

// 10. What happens if visited[] is not used?
// Nodes may be revisited, causing infinite loops.

// 11. Why does parallel DFS output order differ from sequential?
// Because multiple threads execute tasks concurrently; order is non-deterministic.

// 12. Is the parallel DFS output still correct?
// Yes, all nodes are visited once; only the order differs.

// 13. What is the role of the doWork flag?
// Ensures a node is processed only once by one thread.

// 14. Why check visited[node] inside a critical section?
// To prevent race conditions when multiple threads access/update it.

// 🔹 Parallel DFS (OpenMP Tasks)

// 15. Why use tasks instead of parallel for in DFS?
// DFS is recursive; tasks allow parallel execution of recursive calls.

// 16. What does #pragma omp task do?
// Creates a new task that can run concurrently with others.

// 17. What is #pragma omp parallel used for?
// Creates a team of threads.

// 18. What is #pragma omp single?
// Ensures only one thread starts the initial task; others help execute spawned tasks.

// 19. Why is critical needed in parallel DFS?
// To protect shared resources (visited array, output).

// 20. What happens without critical?
// Race conditions: duplicate visits or corrupted output.

// 🔹 OpenMP – Core Concepts

// 21. What is OpenMP?
// An API for parallel programming in C/C++ using shared memory and compiler directives.

// 22. What is a thread?
// A lightweight unit of execution within a process.

// 23. What is parallelism?
// Executing multiple tasks simultaneously.

// 24. What is a race condition?
// When multiple threads access/modify shared data concurrently, leading to unpredictable results.

// 25. What is a critical section?
// A block where only one thread can execute at a time.

// 26. What is synchronization?
// Coordinating threads to ensure correct execution order and data consistency.

// 27. What is task parallelism?
// Different tasks (functions/recursions) run in parallel.

// 28. What is data parallelism?
// Same operation applied to different data elements in parallel.

// 29. Difference between parallel for and task?
// parallel for splits loop iterations; task handles independent work units (useful for recursion).

// 30. What is overhead in parallel programming?
// Extra cost of thread creation, synchronization, and communication.

// 🔹 Performance Questions

// 31. Why is parallel DFS slower for small inputs?
// Thread creation and synchronization overhead dominate computation time.

// 32. When does parallel DFS become beneficial?
// For large graphs with many independent branches.

// 33. Is DFS easily parallelizable?
// No. It has dependencies; only some branches can be parallelized.

// 34. What limits speedup in your code?
// Critical sections and shared data structures.