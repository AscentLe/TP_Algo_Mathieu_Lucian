
#include "stdio.h"
#include "stdlib.h"
#define MAX_N 100

int checksum(int* A, int* removedIndexList, int indexListSize, int aSum, int k);
void recFind(int* A, int aSize, int* removedIndexList, int depth, int aSum, int k, int* bestRemovedIndexList, int* bestDepth);

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

int checksum(int* A, int* removedIndexList, int removedIndexListSize, int aSum, int k){
    for (int i = 0; i < removedIndexListSize; i++){
        aSum -= A[removedIndexList[i]];
    }
    return (aSum % k == 0);
}

void recFind(int* A, int aSize, int* removedIndexList, int depth, int aSum, int k, int* bestRemovedIndexList, int* bestDepth){
    if (depth > *bestDepth){
        return; // Ne pas continuer si on a déjà trouvé une meilleure solution
    }
    else if (checksum(A, removedIndexList, depth, aSum, k)){
        *bestDepth = depth;
        for (int i = 0; i < depth; i++){
            bestRemovedIndexList[i] = removedIndexList[i];
        }
    }
    else{
        for (int j = 0; j < aSize; j++){
            // Vérifier si l'indice a déjà été retiré
            int alreadyRemoved = 0;
            for (int i = 0; i < depth; i++) {
                if (removedIndexList[i] == j) {
                    alreadyRemoved = 1;
                    break;
                }
            }
            if (!alreadyRemoved){
                removedIndexList[depth] = j;  // Utiliser depth comme niveau actuel
                recFind(A, aSize, removedIndexList, depth + 1, aSum, k, bestRemovedIndexList, bestDepth);
            }
        }        
    }
}


int main(){
    int k;
    int A[MAX_N];
    int aSize;
    read_input_ex2("INPDIVSEQ.txt", A, &aSize, &k);

    int removedIndexList[aSize];
    int bestRemovedIndexList[aSize];
    int aSum = 0;
    int bestDepth = aSize;
    for (int i = 0; i < aSize; i++){
        removedIndexList[i] = -1;  // -1 pour indiquer que cet indice n'est pas encore utilisé
        bestRemovedIndexList[i] = -1;  // Initialiser avec -1
        aSum += A[i];
    }

    if (k == 0){ // cas d'une division par 0
        printf("Vous essayez de diviser par 0 : pas de résultat.\n");
        return 0;
    }

    if (aSize == 0){ // cas d'une liste A vide
        printf("Tableau vide.\n");
        return 0;
    }
    // Appel de la fonction récursive pour chercher le meilleur sous-ensemble
    recFind(A, aSize, removedIndexList, 0, aSum, k, bestRemovedIndexList, &bestDepth);

    int resSize = aSize - bestDepth; 
    int res[resSize];

    int removedIndex; // Variable pour vérifier si l'indice est un élément retiré ou non
    int count = 0; // Indice pour écrire les valeurs dans res
    for (int i = 0; i < aSize; i++){
        removedIndex = 0;
        for (int j = 0; j < bestDepth; j++){
            if (i == bestRemovedIndexList[j]){
                removedIndex = 1;
                break;
            }
        }
        if (!removedIndex){
            res[count] = A[i];
            count++;
        }
    }
    write_output_ex2("OUTDIVSEQ.txt", res, resSize);

    return 0;
}