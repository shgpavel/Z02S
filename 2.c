#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char str[] = "Дата рождения: 01.02.2003";
    char dob[11];
    int i, j = 0;
    
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            dob[j] = str[i];
            j++;
        }
    }
    
    dob[j] = '\0';
    
    printf("Дата рождения: %c%c.%c%c.%c%c%c%c\n", dob[0], dob[1], dob[2], dob[3], dob[4], dob[5], dob[6], dob[7]);
    
    return 0;
}
