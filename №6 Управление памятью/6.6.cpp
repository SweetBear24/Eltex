#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int N = 3;  
    char *staticArr[N]; 
    
    for (int i = 0; i < N; i++) {
        staticArr[i] = (char *)malloc(10 * sizeof(char));
        if (staticArr[i] == NULL) {
            printf("Memory allocation failed for string %d.\n", i);
            return 1;
        }
        printf("Enter string %d (up to 9 characters): ", i + 1);
        scanf("%9s", staticArr[i]);
    }


    printf("Strings from dynamic memory:\n");
    for (int i = 0; i < N; i++) {
        printf("%d: %s\n", i + 1, staticArr[i]);
    }
    
    printf("Free of memory!\n");
    for (int i = 0; i < N; i++) {
        free(staticArr[i]);
    }

    return 0;
}
