#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    int N;
    printf("Enter the number of strings: ");
    scanf("%d", &N);

    char **arr = (char **)calloc(N, sizeof(char *));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        arr[i] = (char *)calloc(10, sizeof(char)); 

        if (arr[i] == NULL) {
            printf("Memory allocation failed for string %d.\n", i);
            return 1;
        }

        printf("Enter string %d (up to 9 characters): ", i + 1);
        scanf("%9s", arr[i]);  
    }

    printf("\nEntered strings:\n");
    for (int i = 0; i < N; i++) {
        printf("%d: %s\n", i + 1, arr[i]);
    }
    printf("\n");
        
    if (N > 0) {
        arr = (char **)realloc(arr, (N + 5) * sizeof(char *));  

        for (int i = N; i < N + 5; i++) {
            arr[i] = (char *)calloc(10, sizeof(char));
            if (arr[i] == NULL) {
                printf("Memory allocation failed for new string %d.\n", i);
                return 1;
            }
            printf("Enter string %d (up to 9 characters): ", i + 1);
            scanf("%9s", arr[i]);  
        }
    }

    printf("\nEntered strings:\n");
    for (int i = 0; i < N + 5; i++) {
        printf("%d: %s\n", i + 1, arr[i]);
    }

    
    for (int i = 0; i < N + 5; i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}
