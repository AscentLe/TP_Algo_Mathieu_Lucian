#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100
#define INF INT_MAX

// Structure pour une arête dans la liste d'adjacence
typedef struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
} AdjListNode;

// Structure pour le graphe
typedef struct Graph {
    int numVertices;
    AdjListNode* adjList[MAX];
} Graph;
// Crée un nouveau noeud dans la liste d'adjacence
AdjListNode* createNode(int dest, int weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

//Fonction pour lire le fichier d'entrée et remplir la liste d'adjacence
void readInpFileList(const char *fileName, int *n, int *m, int *start, int *end, Graph* graph) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }
    fscanf(file, "%d %d %d %d", n, m, start, end);
    graph->numVertices = *n;
    for (int i = 1; i <= *n; i++) {
        graph->adjList[i] = NULL; // Initialisation
    }
    for (int i = 0; i < *m; i++) {
        int u, v, w;
        fscanf(file, "%d %d %d", &u, &v, &w);
        AdjListNode* node1 = createNode(v, w);
        node1->next = graph->adjList[u];
        graph->adjList[u] = node1;
        AdjListNode* node2 = createNode(u, w);
        node2->next = graph->adjList[v];
        graph->adjList[v] = node2; // Non orienté
    }
    fclose(file);
}

// Fonction pour trouver le sommet avec la distance minimale non encore traité
int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index;
    for (int v = 1; v <= n; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Fonction pour imprimer le chemin trouvé
void printPath(int parent[], int j, FILE *file) {
    if (parent[j] == -1) {
        return;
    }
    printPath(parent, parent[j], file);
    fprintf(file, " → %d", j);
}

// Fonction de Dijkstra avec une liste d'adjacence
void dijkstraList(Graph* graph, int start, int end) {
    int dist[MAX];
    int visited[MAX];
    int parent[MAX];
    // Initialisation
    for (int i = 1; i <= graph->numVertices; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[start] = 0;
    for (int count = 1; count <= graph->numVertices - 1; count++) {
        int u = minDistance(dist, visited, graph->numVertices);
        visited[u] = 1;
        AdjListNode* current = graph->adjList[u];
        while (current != NULL) {
            int v = current->dest;
            int weight = current->weight;
            if (!visited[v] && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
            current = current->next;
        }
    }
    // Écrire le résultat dans un fichier
    FILE *output = fopen("OUTDIJGRAPHMAT.txt", "w");
    if (output == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie\n");
        exit(1);
    }
    fprintf(output, "%d\n", dist[end]);
    fprintf(output, "%d", start);
    printPath(parent, end, output);
    fprintf(output, "\n");
    fclose(output);
}

int main() {
    int n, m, start, end;
    Graph graph;
    readInpFileList("INPDIJGRAPH.txt", &n, &m, &start, &end, &graph);
    dijkstraList(&graph, start, end);
    return 0;
}
