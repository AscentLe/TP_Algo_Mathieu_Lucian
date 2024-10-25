#include <stdio.h>
#include <limits.h>

#define MAX 100
#define INF INT_MAX

// Fonction pour lire le fichier d'entrée et remplir la matrice d'adjacence
void readInpFileMatrix(const char *fileName, int *n, int *m, int *start, int *end, int adjMatrix[MAX][MAX]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }
    fscanf(file, "%d %d %d %d", n, m, start, end);
    for (int i = 1; i <= *n; i++) {
        for (int j = 1; j <= *n; j++) {
            if (i == j) {
                adjMatrix[i][j] = 0;  // Distance à soi-même est 0
            } else {
                adjMatrix[i][j] = INF;  // Distance infinie pour les autres
            }
        }
    }
    for (int i = 0; i < *m; i++) {
        int u, v, w;
        fscanf(file, "%d %d %d", &u, &v, &w);
        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;  // Non orienté
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

// Fonction de Dijkstra pour trouver le plus court chemin
void dijkstra(int n, int adjMatrix[MAX][MAX], int start, int end) {
    int dist[MAX];      // Distance minimale depuis le sommet de départ
    int visited[MAX];   // Marquer les sommets visités
    int parent[MAX];    // Pour stocker le chemin
    // Initialisation
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[start] = 0;
    // Boucle principale de Dijkstra
    for (int count = 1; count <= n - 1; count++) {
        int u = minDistance(dist, visited, n);
        visited[u] = 1;
        for (int v = 1; v <= n; v++) {
            if (!visited[v] && adjMatrix[u][v] != INF && dist[u] != INF && dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }
    // Écriture des résultats dans le fichier de sortie
    FILE *output = fopen("OUTDIJGRAPH.txt", "w");
    if (output == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie\n");
        exit(1);
    }
    // Ligne 1 : La distance la plus courte
    fprintf(output, "%d\n", dist[end]);

    // Ligne 2 : Le chemin le plus court
    fprintf(output, "%d", start);
    printPath(parent, end, output);
    fprintf(output, "\n");
    fclose(output);
}

int main() {
    int n, m, start, end;
    int adjMatrix[MAX][MAX];

    readInpFileMatrix("INPDIJGRAPH.txt", &n, &m, &start, &end, adjMatrix);
    dijkstra(n, adjMatrix, start, end);
    return 0;
}