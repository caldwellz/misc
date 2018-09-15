#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <cstring>
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::set;

static const int max_word_size = 24;

int main() {
	PropertyTree_target tree;
	string fn("KJVBible.txt"); //fn("LoremIpsum.txt");
	std::ifstream inFile;
	set<string> wordlist;
	string wordstr;
	char word[max_word_size];
	char c;
	UInt i, n, x, y;
	UInt64 startts, endts;


	startts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	cout << "[main] Initializing tree..." << endl;
	if(!tree.initialize())
		return 1;

	cout << "[main] Opening file '" << fn << "'..." << endl;
	inFile.open(fn.c_str());

	if(inFile.good()) {
		cout << "[main] Parsing file..." << endl;
	} else {
		cout << "[main] File not good. Exiting." << endl;
		return 1;
	}

	//Fill database
	memset(word, 0, max_word_size);
	i = 0;
	while(inFile.get(c)) { //inFile.good()) {
		if((c < 65)						//Below 'A'
			|| ((c > 90) && (c < 97))	//Between 'Z' and 'a'
			|| (c > 122)) {				//Above 'z'
			if(word[0] != '\0') {
				//Flush word
				wordstr = word;
				wordlist.insert(wordstr);
				tree.setUInt(wordstr, tree.getUInt(wordstr, 0) + 1);
				memset(word, 0, max_word_size);
				i = 0;
			} else continue;
		} else {
			//Character is a letter; lowercase it if necessary then add it to the word if possible
			if(i < max_word_size) {
				if(c < 97) c += 32;
				word[i] = c;
				i += 1;
			}
		}
	}
/* *SUPER SLOW ALGORITHM*
	//Sort by highest number
	string newtable[wordlist.size()];
	x = wordlist.size();
	for(i = 0; i < x; ++i) {
		y = 0;
		for(set<string>::iterator it = wordlist.begin(); it != wordlist.end(); ++it) {
			n = tree.getUInt(*it, 0);
			if(n > y) {
				y = n;
				newtable[i] = *it;
			}
		}
		wordlist.erase(newtable[i]);
	}
*/

	//Sort by highest number redoux -- num-to-string map in DB ;)
	

	//Print results
	cout << endl << "[main] Results:" << endl;
	for(set<string>::iterator it = wordlist.begin(); it != wordlist.end(); ++it) {
		cout << "    " << *it << ": " << tree.getUInt(*it, 0) << endl;
	}
/*
	for(i = 0; i < x; ++i) {
		cout << "    " << newtable[i] << ": " << tree.getUInt(newtable[i], 0) << endl;
	}
*/
	//Clean up
	cerr << "[main] Pausing before cleanup for state analysis; press Enter..." << endl;
	cin >> c;
	cout << "[main] Cleaning up..." << endl;
	tree.destroy();
	inFile.close();
	wordlist.clear();
	cerr << "[main] Pausing after cleanup for state analysis; press Enter..." << endl;
	cin >> c;

	endts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	cout << "Total ms: " << endts - startts << endl;

	return 0;
}
