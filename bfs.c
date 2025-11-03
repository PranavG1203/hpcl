#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 6

int main() {
    // Example graph (Adjacency Matrix)
    int graph[N][N] = {
        {0,1,1,0,0,0},
        {1,0,1,1,0,0},
        {1,1,0,0,1,0},
        {0,1,0,0,1,1},
        {0,0,1,1,0,1},
        {0,0,0,1,1,0}
    };

    int visited[N] = {0};
    int queue[N];
    int front = 0, rear = 0;
    int start_node = 0;

    queue[rear++] = start_node;
    visited[start_node] = 1;

    printf("Parallel BFS Traversal: ");

    double start = omp_get_wtime();

    while (front < rear) {
        int level_size = rear - front;

        #pragma omp parallel for
        for (int i = 0; i < level_size; i++) {
            int node = queue[front + i];
            printf("%d ", node);

            for (int j = 0; j < N; j++) {
                if (graph[node][j] && !visited[j]) {
                    #pragma omp critical
                    {
                        if (!visited[j]) {
                            visited[j] = 1;
                            queue[rear++] = j;
                        }
                    }
                }
            }
        }

        front += level_size;
    }

    double end = omp_get_wtime();

    printf("\nTime Taken: %f seconds\n", end - start);
    return 0;
}
