
#include <enet/enet.h>

class MasterNode {
	public:
		bool init();
		bool update();
		void deinit();

	private:
		ENetHost* host;
		ENetAddress address;
};
