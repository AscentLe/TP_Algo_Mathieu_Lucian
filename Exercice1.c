#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define NMAX 5000

void readInpFile(const char *fileName, int *n, int A[]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", fileName);
        exit(1);
    }
    fscanf(file, "%d", n);
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%d", &A[i]);
    }
    fclose(file);
}

void writeOutFile(const char *fileName, int maxLength, int subsequence[], int indices[], int size) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        exit(1);
    }
    fprintf(file, "%d\n", maxLength);

    for (int i = 0; i < size; i++) {
        fprintf(file, "a[%d] = %d\n", indices[i] + 1, subsequence[i]);  
    }

    fclose(file);
}
void findSubSeq(int SEQ[],int n, int *maxLength, int subsequence[], int indices[]){
    int SEQlength[n];
    int prevInd[n];
    for (int i=0; i<n; i++){
        SEQlength[i]=1; 
        prevInd[i]=-1;
    }
    for (int i=n-2; i>=0; i--){
        for (int j=i+1; j<n; j++){
            if (SEQ[j]>SEQ[i] && SEQlength[i] < SEQlength[j] + 1){
                SEQlength[i] = SEQlength[j] + 1;
                prevInd[i]=j;
            }
        }
    }

    int maxInd = 0;
    *maxLength = SEQlength[0];
    for (int i = 1; i < n; i++) {
        if (SEQlength[i] > *maxLength) {
            *maxLength = SEQlength[i];
            maxInd = i;
        }
    }
    int k=0;
    while (maxInd!=-1){
        subsequence[k] = SEQ[maxInd];
        indices[k] = maxInd;
        k++;
        maxInd = prevInd[maxInd];
    }

}

int main(){
    int SEQ[NMAX];
    int n;
    int maxLength;
    int subsequence[NMAX];  
    int indices[NMAX];

    readInpFile("INPMONOSEQ.txt", &n, SEQ);
    printf("Input Size : %d\n", n);
    printf("Input Sequence : ");
    for (int i = 0; i < n; i++) {
        printf("%d ", SEQ[i]);
    }
    printf("\n");

    findSubSeq(SEQ, n, &maxLength, subsequence, indices);

    writeOutFile("OUTMONOSEQ.txt", maxLength, subsequence, indices, maxLength);
}