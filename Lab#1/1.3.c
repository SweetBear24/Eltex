#include <stdio.h>

int main() {
    char number;
	int count = 0;
    int point = 0;
	printf("Enter a number: ");
    scanf("%hhd", &number);  

    int num_bits = sizeof(number) * 8;
	
    for (int i = num_bits - 1; i >= 0; i--) {
        point = (number >> i) & 1;
		printf("%d", point);

        if (i % 4 == 0) {
            printf(" ");
        }
        
        if (point == 1) {
        	count++;
        }
    }
    
    printf("\n");
    printf("Amount of 1 = %d", count);

    return 0;
}
