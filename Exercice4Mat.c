#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void readInpFileMatrix(const char *fileName, int *n, int *m, int adjMatrix[MAX][MAX]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }

    fscanf(file, "%d %d", n, m);
    for (int i = 1; i <= *n; i++) {
        for (int j = 1; j <= *n; j++) {
            adjMatrix[i][j] = 0; 
        }
    }

    for (int i = 0; i < *m; i++) {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        adjMatrix[u][v] = 1; 
        adjMatrix[v][u] = 1;
    }

    fclose(file);
}

// Fonction de parcours en profondeur pour trouver les composantes connexes
void dfs(int u, int visited[], int adjMatrix[MAX][MAX], int n, int component[], int *count) {
    visited[u] = 1; // Marquer le sommet comme visité
    component[(*count)++] = u; // Ajouter le sommet à la composante

    for (int v = 1; v <= n; v++) {
        if (adjMatrix[u][v] == 1 && !visited[v]) {
            dfs(v, visited, adjMatrix, n, component, count);
        }
    }
}

// Fonction pour déterminer les composantes connexes
void findComponents(int n, int adjMatrix[MAX][MAX]) {
    int visited[MAX] = {0}; // Tableau pour suivre les sommets visités
    int component[MAX];
    int numComponents = 0;

    FILE *output = fopen("OUTCONGRAPHMAT.txt", "w");
    if (output == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie\n");
        exit(1);
    }

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            int count = 0;
            dfs(i, visited, adjMatrix, n, component, &count);
            numComponents++;

            fprintf(output, "composante connexe %d\n", numComponents);
            for (int j = 0; j < count; j++) {
                fprintf(output, "%d ", component[j]);
            }
            fprintf(output, "\n");
        }
    }
    fprintf(output, "nb de composantes connexes %d\n", numComponents); 
    fclose(output);
}

int main() {
    int n, m;  
    int adjMatrix[MAX][MAX]; 

    readInpFileMatrix("INPCONGRAPH.txt", &n, &m, adjMatrix);
    findComponents(n, adjMatrix);

    return 0;
}
