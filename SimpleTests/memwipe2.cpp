// Another user-space MemWipe implementation, written July 2013.
int main() {
	int* mem;
	int i;

	while(1) {
		mem = new int[100];
		for(i = 0; i < 100; i++) {
			mem[i] = 1111;
		}
	}

	return -1;
}
