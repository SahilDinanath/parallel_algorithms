#include <limits.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INFINITY INT_MAX

typedef struct {
  int weight;
  int visited;
  int previous;
} Vertex;

void printShortestPaths(Vertex vertices[], int n, int source) {
  printf("The distance from %d to each vertex is:\n", source);
  printf("  v    dist %d->v\n", source);
  printf("----   ---------\n");

  for (int i = 1; i < n; i++) {
    printf("  %d         ", i);
    if (vertices[i].weight == INFINITY) {
      printf("inf\n");
    } else {
      printf("%d\n", vertices[i].weight);
    }
  }

  printf("\nThe shortest path from %d to each vertex is:\n", source);
  printf("  v     Path %d->v\n", source);
  printf("----    ---------\n");

  for (int i = 1; i < n; i++) {
    printf("  %d:    %d ", i, source);

    int current = i;
    int *path = (int *)malloc(n * sizeof(int));
    int pathLength = 0;

    while (current != -1) {
      path[pathLength++] = current;
      current = vertices[current].previous;
    }

    for (int j = pathLength - 2; j >= 0; j--) {
      printf("%d ", path[j]);
    }
    printf("\n");

    free(path); // Free the dynamically allocated memory
  }
}

Vertex *dijkstra(int **graph, int n, int source) {
  Vertex *vertices = (Vertex *)malloc(n * sizeof(Vertex));

  // Initialize vertices
  for (int i = 0; i < n; i++) {
    vertices[i].weight = INFINITY;
    vertices[i].visited = 0;
    vertices[i].previous = -1;
  }

  // Set source vertex weight to 0
  vertices[source].weight = 0;

  // Find shortest path for all vertices
  for (int i = 0; i < n - 1; i++) {
    // Find the vertex with the minimum weight
    int minWeight = INFINITY;
    int minIndex = -1;
    for (int j = 0; j < n; j++) {
      if (!vertices[j].visited && vertices[j].weight < minWeight) {
        minWeight = vertices[j].weight;
        minIndex = j;
      }
    }

    // Mark the selected vertex as visited
    vertices[minIndex].visited = 1;

    // Update the weights of the adjacent vertices
    for (int j = 0; j < n; j++) {
      if (!vertices[j].visited && graph[minIndex][j] != 0 &&
          vertices[minIndex].weight != INFINITY &&
          vertices[minIndex].weight + graph[minIndex][j] < vertices[j].weight) {
        vertices[j].weight = vertices[minIndex].weight + graph[minIndex][j];
        vertices[j].previous = minIndex;
      }
    }
  }
  // Uncomment to display shortest paths length and path
  // Print the shortest paths
  // printShortestPaths(vertices, n, source);
  return vertices;
}
int compareArrays(Vertex *inputVertices, Vertex *originalVertices, int size) {
  for (int i = 0; i < size; i++) {
    if (inputVertices[i].weight != originalVertices[i].weight &&
        inputVertices[i].visited != originalVertices[i].visited &&
        inputVertices[i].previous != originalVertices[i].previous) {
      return 0;
    }
  }
  return 1;
}

void correctnessAssertion(Vertex *inputVertices) {
  Vertex originalVertices[6];
  originalVertices[0].weight = 0;
  originalVertices[0].visited = 1;
  originalVertices[0].previous = 0;
  originalVertices[1].weight = 1;
  originalVertices[1].visited = 1;
  originalVertices[1].previous = 0;
  originalVertices[2].weight = 3;
  originalVertices[2].visited = 1;
  originalVertices[2].previous = 3;
  originalVertices[3].weight = 1;
  originalVertices[3].visited = 1;
  originalVertices[3].previous = 0;
  originalVertices[4].weight = 4;
  originalVertices[4].visited = 1;
  originalVertices[4].previous = 2;
  originalVertices[5].weight = 4;
  originalVertices[5].visited = 1;
  originalVertices[5].previous = 1;

  int result = compareArrays(inputVertices, originalVertices, 6);
  if (result == 1) {
    printf("Results are correct:\n");
  } else {
    printf("Results are incorrect:\n");
  }
}
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    printf("Error opening the input file.\n");
    return 1;
  }

  int n, m;
  fscanf(input_file, "%d %d", &n, &m);

  int **graph = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) {
    graph[i] = (int *)calloc(n, sizeof(int));
  }

  for (int i = 0; i < m; i++) {
    int u, v;
    int weight;
    fscanf(input_file, "%d %d %d", &u, &v, &weight);
    graph[u][v] = weight;
    graph[v][u] = weight; // For undirected graphs
  }

  fclose(input_file);

  // Uncomment to give user input source vertex
  //  int source;
  //  printf("Enter the source vertex: ");
  //  scanf("%d", &source);

  double timeStart = omp_get_wtime();
  Vertex *input = dijkstra(graph, n, 0);
  double timeStop = omp_get_wtime();

  double timeTaken = timeStop - timeStart;
  if (strcmp(argv[1], "graph_0.txt")==0) {
    correctnessAssertion(input);
  }
  printf("%lf", timeTaken);
  printf("\n");

  free(graph);

  return 0;
}
