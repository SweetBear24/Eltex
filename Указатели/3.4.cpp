#include <stdio.h>
#include <string.h>

int main(void) {
    char text[6];     
    char substring[3];
    char *substring_ptr;

    
    printf("Enter a 5-character text: ");
    scanf("%5s", text); 

    
    printf("Enter a 2-character substring: ");
    scanf("%2s", substring);

    
    substring_ptr = strstr(text, substring);

    if (substring_ptr) {
        long position = substring_ptr - text;
        printf("Substring index: %ld\n", position); 
    } else {
        printf("Substring not found\n");
    }

    return 0;
}
