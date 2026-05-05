#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;
double start, end; //Variables to measure execution time

// ---------------- BFS ----------------
void parallelBFS(vector<vector<int>> &tree, int root, int n) {
    vector<bool> visited(n, false);
    queue<int> q; //queue used for bfs

    visited[root] = true;
    q.push(root);

    cout << "\nParallel BFS: ";

    while (!q.empty()) {
        int size = q.size(); //Stores number of nodes in current level

        #pragma omp parallel for 
        for (int i = 0; i < size; i++) { //OpenMP splits loop among thread Each thread handles one node from current level
            int node;

            #pragma omp critical
            {
                node = q.front();
                q.pop();
                cout << node << " ";
            }

            // index-based loop
            for (int j = 0; j < tree[node].size(); j++) { //loop thru all neigh of node
                int child = tree[node][j];

                if (!visited[child]) {
                    #pragma omp critical
                    {
                        if (!visited[child]) {
                            visited[child] = true;
                            q.push(child);
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

void sequentialBFS(vector<vector<int>> &tree, int root, int n) {
    vector<bool> visited(n, false);
    queue<int> q;

    visited[root] = true;
    q.push(root);

    cout << "\nSequential BFS: ";

    while (!q.empty()) { //Only one thread → no need for critical
        int node = q.front();
        q.pop();

        cout << node << " ";

        for (int j = 0; j < tree[node].size(); j++) { //explore neighb one by one
            int child = tree[node][j]; //get neighbour

            if (!visited[child]) {
                visited[child] = true;
                q.push(child);
            }
        }
    }
    cout << endl;
}
// ---------------- MAIN ----------------
int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    vector<vector<int>> tree(n); //Create adjacency list

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        tree[u].push_back(v); //undirected graph
        tree[v].push_back(u);
    }

    int root;
    cout << "Enter root: ";
    cin >> root;

    double start, end;

    // -------- Sequential BFS --------
    start = omp_get_wtime();
    sequentialBFS(tree, root, n);
    end = omp_get_wtime();
    cout << "Sequential BFS Time: " << (end - start) << " seconds\n";

    // -------- Parallel BFS --------
    start = omp_get_wtime();
    parallelBFS(tree, root, n);
    end = omp_get_wtime();
    cout << "Parallel BFS Time: " << (end - start) << " seconds\n";

    return 0;
}

//“I implemented BFS using a queue. In the parallel version, I process nodes at the same level concurrently using OpenMP. Critical sections ensure safe access to shared data like queue and visited array.”
// Enter number of nodes: 7
// Enter edges (u v):
// 0 1
// 0 2
// 1 3
// 1 4
// 2 5
// 2 6
// Enter root: 0

// Sequential BFS: 0 1 2 3 4 5 6 
// Sequential BFS Time: 0.0139999 seconds

// Parallel BFS: 0 1 2 3 4 5 6 
// Parallel BFS Time: 0.00399995 seconds

// ❓1. What is BFS?

// 👉 Answer:

// BFS (Breadth First Search) is a graph traversal technique that visits nodes level by level using a queue data structure.

// ❓2. Why do we use a queue in BFS?

// 👉 Answer:

// Queue follows FIFO (First In First Out), which ensures nodes are processed in the same order they are discovered, maintaining level-wise traversal.

// ❓3. What is the difference between BFS and DFS?

// 👉 Answer:

// BFS explores level by level using a queue, while DFS explores depth-wise using recursion or stack.

// ❓4. What data structure did you use to represent the graph?

// 👉 Answer:

// I used an adjacency list implemented using a vector of vectors.

// ❓5. Why did you use a visited array?

// 👉 Answer:

// To avoid visiting the same node multiple times and to prevent infinite loops in cyclic graphs.

// 🔥 PARALLEL-SPECIFIC QUESTIONS
// ❓6. What is OpenMP?

// 👉 Answer:

// OpenMP is an API used for parallel programming in C/C++ that allows us to use multiple threads for faster execution.

// ❓7. What does #pragma omp parallel for do?

// 👉 Answer:

// It divides loop iterations among multiple threads so that they execute in parallel.

// ❓8. Why did you use parallelism in BFS?

// 👉 Answer:

// BFS processes nodes level by level, and nodes in the same level can be processed independently, making it suitable for parallel execution.

// ❓9. What is a critical section?

// 👉 Answer:

// A critical section ensures that only one thread accesses a shared resource at a time to avoid race conditions.

// ❓10. Why did you use #pragma omp critical in your code?

// 👉 Answer:

// To protect shared resources like queue and visited array from concurrent access by multiple threads.

// ⚠️ IMPORTANT CONCEPT QUESTIONS
// ❓11. What is a race condition?

// 👉 Answer:

// A race condition occurs when multiple threads try to access or modify shared data at the same time, leading to unpredictable results.

// ❓12. Can we remove critical sections?

// 👉 Answer:

// No, because without critical sections, multiple threads may corrupt the queue or visited array.

// ❓13. Why is BFS parallelization difficult?

// 👉 Answer:

// Because BFS uses a shared queue and sequential dependency, which limits efficient parallel execution.

// ❓14. Why do we store size = q.size() before the loop?

// 👉 Answer:

// To process only nodes of the current level, ensuring correct BFS order.

// ❓15. Why is parallel BFS faster than sequential BFS?

// 👉 Answer:

// Because multiple nodes at the same level are processed simultaneously using multiple threads.

// 🔥 CODE-BASED QUESTIONS
// ❓16. Why do you check if (!visited[child]) twice?

// 👉 Answer:

// First check avoids unnecessary entry into critical section, second check ensures correctness when multiple threads access it.

// ❓17. What happens if we don’t use visited array?

// 👉 Answer:

// Nodes may be revisited, causing infinite loops in cyclic graphs.

// ❓18. What is the time complexity of BFS?

// 👉 Answer:

// Time complexity is O(V + E) where V is vertices and E is edges.

// ❓19. Is the output order always same in parallel BFS?

// 👉 Answer:

// Not necessarily, because thread scheduling may vary, but level order is generally maintained.

// ❓20. What is the role of omp_get_wtime()?

// 👉 Answer:

// It is used to measure execution time of code segments in seconds.