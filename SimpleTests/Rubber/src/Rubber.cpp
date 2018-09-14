
#include <iostream>
using namespace std;

#include "MasterNode.hpp"
#include "Node.hpp"

int main() {
	MasterNode master;
	Node node;

	if(!node.init()) {
		node.deinit();
		cout << "Initializing as MasterNode." << endl;

		if(!master.init()) {
			cout << "Could not initialize as MasterNode, aborting." << endl;
			master.deinit();
			return 1;
		} else {
			
			while(master.update());
			master.deinit();
		}
	} else {
		while(node.update());
		node.deinit();
	}
	
	return 0;
}
