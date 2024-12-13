#include <stdio.h>
#include <string.h>

int main(void) {
    char text[6];     
    char substring[3];
    char *substring_ptr = NULL; 

    printf("Enter a 5-character text: ");
    scanf("%5s", text); 

    printf("Enter a 2-character substring: ");
    scanf("%2s", substring);

    for (int i = 0; i <= strlen(text) - strlen(substring); i++) {
        int found = 1;
        for (int j = 0; j < strlen(substring); j++) {
            if (text[i + j] != substring[j]) {
                found = 0;
                break;
            }
        }
        if (found) {
            substring_ptr = &text[i];
            break;
        }
    }

    if (substring_ptr) {
        long position = substring_ptr - text;
        printf("Substring index: %ld\n", position); 
    } else {
        printf("Substring not found\n");
    }

    return 0;
}
