#include <stdio.h>

void printBinary(unsigned int num) {
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    unsigned int number; 
    unsigned int new_byte; 

    printf("Enter a number: ");
    scanf("%u", &number); 

    printf("Enter a value for the third byte (0-255): ");
    scanf("%u", &new_byte); 

    number &= 0xFFFF00FF; 
    number |= (new_byte << 16); 

    printf("New number: %u\n", number);
    
    printf("New byte in binary: ");
    printBinary(new_byte);

    printf("Original number in binary: ");
    printBinary(number);
    
    
    return 0;
}
