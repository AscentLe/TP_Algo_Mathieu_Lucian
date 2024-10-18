#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* head[MAX]; 
} Graph;

Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph* graph, int u, int v) {
    Node* newNode = createNode(v);
    newNode->next = graph->head[u];
    graph->head[u] = newNode;

    newNode = createNode(u);
    newNode->next = graph->head[v];
    graph->head[v] = newNode; 
}

void readInpFileList(const char *fileName, int *n, int *m, Graph* graph) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }

    fscanf(file, "%d %d", n, m);
    for (int i = 1; i <= *n; i++) {
        graph->head[i] = NULL; 
    }

    for (int i = 0; i < *m; i++) {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        addEdge(graph, u, v); 
    }

    fclose(file);
}

// Fonction de parcours en profondeur pour trouver les composantes connexes
void dfs(int u, int visited[], Graph* graph, int component[], int* count) {
    visited[u] = 1; // Marquer le sommet comme visité
    component[(*count)++] = u; // Ajouter le sommet à la composante

    for (Node* node = graph->head[u]; node != NULL; node = node->next) {
        int v = node->vertex;
        if (!visited[v]) {
            dfs(v, visited, graph, component, count);
        }
    }
}

// Fonction pour déterminer les composantes connexes
void findComponentsList(int n, Graph* graph) {
    int visited[MAX] = {0}; // Tableau pour suivre les sommets visités
    int component[MAX];
    int numComponents = 0;

    FILE *output = fopen("OUTCONGRAPHLIS.txt", "w");
    if (output == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie\n");
        exit(1);
    }

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            int count = 0;
            dfs(i, visited, graph, component, &count);
            numComponents++;

            fprintf(output, "composante connexe %d\n", numComponents);
            for (int j = 0; j < count; j++) {
                fprintf(output, "%d ", component[j]);
            }
            fprintf(output, "\n");
        }
    }

    fprintf(output, "nb de composantes connexes %d\n", numComponents); // Écrire le nombre de composantes connexes
    fclose(output);
}

int main() {
    int n, m;  
    Graph graph; 

    readInpFileList("INPCONGRAPH.txt", &n, &m, &graph);
    findComponentsList(n, &graph);

    return 0;
}
