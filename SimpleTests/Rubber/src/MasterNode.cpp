
#include <iostream>
using namespace std;

#include "MasterNode.hpp"

bool MasterNode::init() {
	if(enet_initialize() != 0) {
		cout << "Could not initialize ENet, aborting." << endl;
		exit(1);
	}

	//enet_address_set_host(&address, "127.0.0.1");
	address.host = ENET_HOST_ANY;
	address.port = 25252;
	host = enet_host_create(&address, 64, 2, 0, 0);
	if(host == NULL) {
		cout << "Could not create an ENet host, aborting." << endl;
		enet_deinitialize();
		exit(1);
	}

	return true;
}

bool MasterNode::update() {
	ENetEvent event;
	if(enet_host_check_events(host, &event) > 0) {
		switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				cout << "New Node connection: " << event.peer->address.host
				<< ":" << event.peer -> address.port << endl;
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				cout << "Node disconnect: " << event.peer->address.host << endl;
				cout << "Nodes remaining: " << host->connectedPeers << endl;
				if(host->connectedPeers == 0) {
					cout << "All Nodes have disconnected, shutting down." << endl;
					return false;
				}
				break;
		}
	}

	return true;
}

void MasterNode::deinit() {
	enet_host_destroy(host);
	enet_deinitialize();
}
