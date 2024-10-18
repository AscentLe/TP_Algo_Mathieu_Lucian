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

void arbre_couvrant(int n, Arete edges[], int m) {
    UnionFind uf;
    init_unionfind(&uf, n);

    Arete arbre[MAX];  
    int count = 0;     

    for (int i = 0; i < m; i++) {
        int u = edges[i].u;
        int v = edges[i].v;

        if (find(&uf, u) != find(&uf, v)) {
            arbre[count++] = edges[i];
            union_set(&uf, u, v);
        }
    }

    writeOutFile("OUTARBGRAPH.txt", count, arbre);
}

int main() {
    int n, m;  
    Arete edges[MAX]; 

    readInpFile("INPARBGRAPH.txt", &n, &m, edges);

    arbre_couvrant(n, edges, m);

    return 0;
}
