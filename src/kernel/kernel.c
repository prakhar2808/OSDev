void main() {
	char* video_memory = (char*)0xb8000;
	//Adding 161 bytes takes it to next line (80 characters of 2 bytes in a row)
	*(video_memory + 0xA1) = 'X';
}
