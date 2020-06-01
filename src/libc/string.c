#include "string.h"

void int_to_ascii(int n, char str[]) {
    int i, sign;
		// Set to 1 if the number is negative
		sign = (n < 0);
    if (n < 0) { 
			n = -n;
		}
    i = 0;
    do {
        str[i++] = n % 10 + '0';
				n /= 10;
    } while (n > 0);
    if (sign) str[i++] = '-';
    str[i] = '\0';
		reverse(str);
}

int strlen(char str[]) {
	int len = 0;
	while(str[len] != '\0') {
		len++;
	}
	return len;
}

void reverse(char str[]) {
	int len = strlen(str);
  // Now reversing the string
	int j; char temp;
	for(j = 0; j < len/2; j++ ) {
		temp = str[j];
		str[j] = str[len - 1 - j];
		str[len - 1 - j] = temp;
	}
}

void append(char str[], char to_append[]) {
	int len = strlen(str);
	int i = 0;
	while(to_append[i] != '\0') {
		str[len++] = to_append[i++];
	}
	str[len] = '\0';
}

void backspace(char str[]) {
	int len = strlen(str);
	str[len-1] = '\0';
}

void clear(char str[]) {
	str[0] = '\0';
}

int strcmp(char str1[], char str2[]) {
	int len = strlen(str1);
	if(strlen(str2) != len) {
		return 0;
	}
	int i;
	for(i = 0; i < len; i++) {
		if(str1[i] != str2[i]) {
			return 0;
		}
	}
	return 1;
}
