#include <limits.h>
#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

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

Vertex* dijkstra(int rank, int size, int *graph, int n, int source) {
  Vertex *vertices = (Vertex *)malloc(n * sizeof(Vertex));

  MPI_Datatype MPI_VERTEX;
  int counts[3] = {1, 1, 1};
  MPI_Aint displacements[3];
  displacements[0] = offsetof(Vertex, weight);
  displacements[1] = offsetof(Vertex, visited);
  displacements[2] = offsetof(Vertex, previous);

  MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
  MPI_Type_create_struct(3, counts, displacements, types, &MPI_VERTEX);
  MPI_Type_commit(&MPI_VERTEX);
  //
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
      if (!vertices[j].visited && graph[minIndex * n + j] != 0 &&
          vertices[minIndex].weight != INFINITY &&
          vertices[minIndex].weight + graph[minIndex * n + j] <
              vertices[j].weight) {
        vertices[j].weight =
            vertices[minIndex].weight + graph[minIndex * n + j];
        vertices[j].previous = minIndex;
      }
    }
  }
  Vertex *allVertices = NULL;
  allVertices = (Vertex *)malloc(n * sizeof(Vertex) * size);

  MPI_Gather(vertices, n, MPI_VERTEX, allVertices, n, MPI_VERTEX, 0,
             MPI_COMM_WORLD);
  // Uncomment to display shortest paths length and path
  // Print the shortest paths
  // printShortestPaths(vertices, n, source);
  // printf("%lf", timeTaken);
  // printf("\n");
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
Vertex *serialDijkstra(int **graph, int n, int source) {
  double timeStart = MPI_Wtime();
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
void correctnessAssertion(Vertex *inputVertices, int **originalGraph,
                          long size) {
  Vertex *originalVertices = serialDijkstra(originalGraph, size, 0);
  int result = compareArrays(inputVertices, originalVertices, size);
  if (result == 1) {
    printf("Results are correct:\n");
  } else {
    printf("Results are incorrect:\n");
  }
}
int main(int argc, char *argv[]) {
  int rank, numOfProcesses;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n, m;

  int *flattenedGraph = NULL;
  int **original = NULL;
  if (rank == 0) {

    if (argc != 2) {
      printf("Usage: %s <input_file>\n", argv[0]);
      return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
      printf("Error opening the input file.\n");
      return 1;
    }

    fscanf(input_file, "%d %d", &n, &m);

    int **graph = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
      graph[i] = (int *)calloc(n, sizeof(int));
    }

    original = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
      original[i] = (int *)calloc(n, sizeof(int));
    }

    for (int i = 0; i < m; i++) {
      int u, v;
      int weight;
      fscanf(input_file, "%d %d %d", &u, &v, &weight);
      graph[u][v] = weight;
      graph[v][u] = weight; // For undirected graphs
      original[u][v] = weight;
      original[v][u] = weight;
    }
    fclose(input_file);

    flattenedGraph = (int *)malloc(n * n * sizeof(int));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        flattenedGraph[i * n + j] = graph[i][j];
      }
    }
  }
  int timeStart = MPI_Wtime();
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  int *local_graph = (int *)malloc(n * n / numOfProcesses * sizeof(int));
  MPI_Scatter(flattenedGraph, (n * n) / numOfProcesses, MPI_INT, local_graph,
              (n * n) / numOfProcesses, MPI_INT, 0, MPI_COMM_WORLD);
  // Uncomment to give user input source vertex
  //  int source;
  //  printf("Enter the source vertex: ");
  //  scanf("%d", &source);
  Vertex *input = dijkstra(rank, numOfProcesses, local_graph, n - 1, 0);

  double timeEnd = MPI_Wtime();
  double timeTaken = timeEnd - timeStart;
  
  if (rank == 0) {
    correctnessAssertion(input, original, n);
    
    printf("%f", timeTaken);
  }
  MPI_Finalize();
  free(local_graph);
  return 0;
}
