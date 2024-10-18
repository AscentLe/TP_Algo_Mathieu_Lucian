#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Structure pour représenter une arête
typedef struct {
    int u, v;
} Arete;

// Structure Union-Find pour détecter les cycles
typedef struct {
    int parent[MAX + 1];  
    int rank[MAX + 1];
} UnionFind;

// Initialisation de la structure Union-Find
void init_unionfind(UnionFind *uf, int n) {
    for (int i = 1; i <= n; i++) {
        uf->parent[i] = i;  // Initialiser chaque sommet pour qu'il soit son propre parent
        uf->rank[i] = 0;
    }
}

// Trouver la racine d'un élément dans Union-Find
int find(UnionFind *uf, int u) {
    if (uf->parent[u] != u) {
        uf->parent[u] = find(uf, uf->parent[u]);  // Compression de chemin
    }
    return uf->parent[u];
}

// Union de deux ensembles
void union_set(UnionFind *uf, int u, int v) {
    int root_u = find(uf, u);
    int root_v = find(uf, v);
    
    if (root_u != root_v) {
        if (uf->rank[root_u] > uf->rank[root_v]) {
            uf->parent[root_v] = root_u;
        } else if (uf->rank[root_u] < uf->rank[root_v]) {
            uf->parent[root_u] = root_v;
        } else {
            uf->parent[root_v] = root_u;
            uf->rank[root_u]++;
        }
    }
}

void readInpFile(const char *fileName, int *n, int *m, Arete edges[]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }

    fscanf(file, "%d %d", n, m);  
    for (int i = 0; i < *m; i++) {
        fscanf(file, "%d %d", &edges[i].u, &edges[i].v);  
    }

    fclose(file);
}

void readInpFileMatrix(const char *fileName, int *n, int *m, int adjMatrix[MAX][MAX]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }

    fscanf(file, "%d %d", n, m);
    for (int i = 1; i <= *n; i++) {
        for (int j = 1; j <= *n; j++) {
            adjMatrix[i][j] = 0; // Initialiser la matrice
        }
    }

    for (int i = 0; i < *m; i++) {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        adjMatrix[u][v] = 1; // Ajouter l'arête (u, v)
        adjMatrix[v][u] = 1; // Ajouter l'arête (v, u) pour le graphe non orienté
    }

    fclose(file);
}

void writeOutFile(const char *fileName, int m, Arete edges[]) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }

    for (int i = 0; i < m; i++) {
        fprintf(file, "%d %d\n", edges[i].u, edges[i].v);  
    }

    fclose(file);
}

void arbre_couvrant_matrix(int n, int adjMatrix[MAX][MAX]) {
    UnionFind uf;
    init_unionfind(&uf, n);
    
    Arete arbre[MAX];  
    int count = 0;     

    for (int u = 1; u <= n; u++) {
        for (int v = u + 1; v <= n; v++) {
            if (adjMatrix[u][v] == 1) { // Vérifier s'il y a une arête entre u et v
                if (find(&uf, u) != find(&uf, v)) {
                    arbre[count++] = (Arete){u, v}; // Ajouter l'arête à l'arbre
                    union_set(&uf, u, v);
                }
            }
        }
    }

    writeOutFile("OUTARBGRAPHMAT.txt", count, arbre);
}

int main() {
    int n, m;  
    int adjMatrix[MAX][MAX];

    readInpFileMatrix("INPARBGRAPH.txt", &n, &m, adjMatrix);
    arbre_couvrant_matrix(n, adjMatrix);

    return 0;
}
