
#include <enet/enet.h>

class Node {
	public:
		bool init();
		bool update();
		void deinit();

	private:
		ENetHost* host;
		ENetPeer* master;
		ENetAddress masteraddr;
};
