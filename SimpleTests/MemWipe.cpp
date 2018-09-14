// Userspace memory wiper. Just keeps going until system runs out of memory and kills it. Written Aug 2013.

int main() {
	int* i;
	while(1) {
		i = new int;
		(*i) = 0xFFFFFFFF;
		(*i) = 0xABABABAB;
		(*i) = 0;
	}

	return 1;
}
