#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct abonent {
    char name[10];
    int tel;
} abonent;

int main() {
    int N;
    printf("Amount of abonents: ");
    scanf("%d", &N);

    abonent *ptr = (abonent *)calloc(N, sizeof(abonent)); 

    if (ptr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        if (ptr[i].name[0] == '\0' && ptr[i].tel == 0) {
            printf("Enter name for abonent %d: ", i + 1);
            scanf("%9s", ptr[i].name);  

            printf("Enter phone number for abonent %d: ", i + 1);
            scanf("%d", &ptr[i].tel);  

            printf("Abonent added successfully at index %d.\n", i);
        }
    }

    printf("\nAbonents List:\n");
    for (int i = 0; i < N; i++) {
        printf("Abonent %d: Name = %s, Phone = %d\n", i + 1, ptr[i].name, ptr[i].tel);
    }

    free(ptr);

    return 0;
}
