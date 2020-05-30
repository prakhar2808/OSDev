/**
 * K&R implementation
 */
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

    // Now reversing the string
		int j; char temp;
		for(j = 0; j < i/2; j++ ) {
			temp = str[j];
			str[j] = str[i - 1 - j];
			str[i - 1 - j] = temp;
		}
}

// Copy no_bytes from source to destination
void memory_copy(char* source, char* destination, int no_bytes) {
	int i;
	for(i = 0; i < no_bytes; i++) {
		*(destination + i) = *(source + i);
	}
}
