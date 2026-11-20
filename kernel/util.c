#include "util.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void int_to_str(int number, char* buf){
    char inv_buf[12];
    int i = 0;
    do {
        inv_buf[i] = (number%10)+'0';
        number /= 10;
        i++;
    } while (number > 0);
    int j = i;
    for(int c = 0; c < i; c++){
        buf[c] = inv_buf[j-1];
        j--;
    }

    buf[i] = '\0';
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

int starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return 0;
        }
    }
    return 1;
}
