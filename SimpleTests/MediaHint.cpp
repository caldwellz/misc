// 2018 GitHub Upload Notes: I am not sure when I originally wrote this (or even if it was originally my code; please yell at me if you are the real author) or why the file is named this way, but I would guess 2013/2014-ish. It apparently converts a straight integer into an IPv4 address, and I suspect it had something to do with Tor proxy routing.

#include <cmath>
#include <iostream>
using namespace std;

void PrintIP(int n) {
	int da, db, dc, dd = n % 256;
	n = floor(n / 256);
	dc = n % 256;
	n = floor(n / 256);
	db = n % 256;
	n = floor(n / 256);
	da = n % 256;
	cout << da << "." << db << "." << dc << "." << dd << endl;
}

int main() {
	PrintIP(3223469900);
	PrintIP(3223469902);
	PrintIP(3226589467);
	PrintIP(628805681);		//UK
	PrintIP(3231406921);
	PrintIP(2733852387);	//US
	PrintIP(2733852102);	//US
}
