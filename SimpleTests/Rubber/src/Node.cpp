
#include <iostream>
using namespace std;

#include "Node.hpp"

bool Node::init() {
	if(enet_initialize() != 0) {
		cout << "Could not initialize ENet, aborting." << endl;
		exit(1);
	}

	host = enet_host_create(NULL, 1, 2, 0, 0);
	if(host == NULL) {
		cout << "Could not create an ENet client, aborting." << endl;
		enet_deinitialize();
		exit(1);
	}

	cout << "Attempting to connect to MasterNode..." << endl;
	enet_address_set_host(&masteraddr, "192.168.1.3");
	//masteraddr.host = ENET_HOST_ANY; //ENET_HOST_BROADCAST;
	masteraddr.port = 25252;
	master = enet_host_connect(host, &masteraddr, 2, 0);
	if(master == NULL) {
		cout << "No available ENet peers for initiating a connection, aborting." << endl;
		deinit();
		exit(1);
	}

	ENetEvent event;
	if(enet_host_service(host, &event, 3000) > 0
	&& event.type == ENET_EVENT_TYPE_CONNECT) {
		cout << "Connected to MasterNode." << endl;
	} else {
		enet_peer_reset(master);
		cout << "Could not connect to MasterNode." << endl;
		return false;
	}

	enet_peer_disconnect(master, 0);
	return true;
}

bool Node::update() {
	ENetEvent event;
	if(enet_host_check_events(host, &event) > 0) {
		switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				cout << "New peer connection: " << event.peer->address.host
				<< ":" << event.peer -> address.port << endl;
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				cout << "Peer disconnect: " << event.peer->address.host << endl;
				if(event.peer->address.host == master->address.host) {
					cout << "Peer was MasterNode, shutting down." << endl;
					return false;
				}
				cout << "Peers remaining: " << host->connectedPeers << endl;
				//TODO: Figure out new peer number; master counts as peer
				if(host->connectedPeers == 0) {
					cout << "All peers have disconnected, shutting down." << endl;
					return false;
				}
				break;
		}
	}

	return true;
}

void Node::deinit() {
	enet_host_destroy(host);
	enet_deinitialize();
}
