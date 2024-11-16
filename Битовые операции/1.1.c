#include <stdio.h>

int main() {
    char number;

    printf("Enter a number: ");
    scanf("%hhd", &number);  

    int num_bits = sizeof(number) * 8;

    for (int i = num_bits - 1; i >= 0; i--) {
        
		printf("%d", (number >> i) & 1);

        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
