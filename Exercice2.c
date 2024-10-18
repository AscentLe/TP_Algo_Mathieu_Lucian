#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#define MAX_N 100
void read_input_ex2(const char* filename, int* sequence, int* n, int* k) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);  
    fscanf(file, "%d", k); 
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%d", &sequence[i]);  
    }

    fclose(file);
}

void write_output_ex2(const char* filename, int* lis, int lis_length) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d\n", lis_length);  
    for (int i = 0; i < lis_length; i++) {
        if (i > 0) fprintf(file, "\n");
        fprintf(file, "res[%d] = %d", i, lis[i]);  
    }
    fprintf(file, "\n");

    int sum = 0;
    for (int i = 0; i < lis_length; i++){
        sum += lis[i];
    }
    fprintf(file, "Sum = %d", sum);
    fclose(file);
}




void question2();
void findBestSubset(int* A, int aSize, int k, int* bestRemovedIndexList, int* bestDepth);
bool isValid(int* removedIndexList, int depth, int index);

void question2(){
    int k;
    int A[MAX_N];
    int aSize;

    read_input_ex2("INPDIVSEQ.txt", A, &aSize, &k);

    int bestRemovedIndexList[MAX_N];
    int bestDepth = 0; 
    findBestSubset(A, aSize, k, bestRemovedIndexList, &bestDepth);

    int resSize = aSize - bestDepth; 
    int res[resSize];

    int count = 0;
    for (int i = 0; i < aSize; i++){
        if (!isValid(bestRemovedIndexList, bestDepth, i)){
            res[count++] = A[i];
        }
    }

    write_output_ex2("OUTDIVSEQ.txt", res, resSize);
}

void findBestSubset(int* A, int aSize, int k, int* bestRemovedIndexList, int* bestDepth) {
    // Utilisation d'un tableau pour mémoriser les états
    bool dp[MAX_N][MAX_N]; // dp[i][j] = true si la somme j peut être obtenue en retirant les indices dans {0, ..., i}
    memset(dp, false, sizeof(dp)); 

    dp[0][0] = true; 

    for (int i = 0; i < aSize; i++) {
        for (int j = 0; j <= *bestDepth; j++) {
            // Cas où l'on ne retire pas l'élément A[i]
            if (j == 0 || dp[i][j - 1]) {
                dp[i + 1][j] = true; // On peut obtenir la même somme sans l'élément
            }
            // Cas où l'on retire l'élément A[i]
            if (j < aSize) {
                dp[i + 1][j + 1] = dp[i][j]; // On peut obtenir la somme j + 1 en retirant A[i]
            }
        }
    }

    // Recherche du meilleur sous-ensemble
    for (int j = 0; j < aSize; j++) {
        if (dp[aSize][j] && (aSize - j) % k == 0) {
            if (j > *bestDepth) {
                *bestDepth = j; // Mettre à jour la meilleure profondeur
                // Sauvegarder les indices retirés
                int idx = 0;
                for (int i = 0; i < aSize; i++) {
                    if (!isValid(bestRemovedIndexList, j, i)) {
                        bestRemovedIndexList[idx++] = i; // Ajouter l'indice retiré
                    }
                }
            }
        }
    }
}

bool isValid(int* removedIndexList, int depth, int index) {
    for (int i = 0; i < depth; i++) {
        if (removedIndexList[i] == index) {
            return true; 
        }
    }
    return false; 
}


int main(){
    question2();
    return 0;
}