//Simple inherited object-id system. Written March 2013.

#include <vector>
#include <iostream>

//class UUID (Tiger-16)
//class UUID (uint)

class IDVectorCounted {
	public:
		IDVectorCounted() {
			if(recycledIdList.size()) {
				ID = recycledIdList.back();
				recycledIdList.pop_back();
			} else {
				ID = currentID;
				currentID++;
			}
		}

		~IDVectorCounted() {
			recycledIdList.push_back(ID);
		}

		int getID() const { return ID; }

	private:
		//static mutex poolLock;
		static int currentID;
		static std::vector<int> recycledIdList;
		int ID;
};
int IDVectorCounted::currentID;
std::vector<int> IDVectorCounted::recycledIdList;

class IDCounted {
	public:
		IDCounted() {
			//mutex.lock();
			if(IdList[0] == 0) {
				ID = currentID;
				currentID++;
			} else {
				ID = IdList[IdList[0]];
				IdList[0]--;
			}
			//mutex.unlock();
		}

		~IDCounted() {
			//mutex.lock();
			if(IdList[0] < 1023) {
				IdList[0]++;
				IdList[IdList[0]] = ID;
			}
			//mutex.unlock();
		}

		unsigned int getID() const { return ID; }

	private:
		//static mutex poolLock;
		static unsigned int currentID;
		static unsigned int IdList[1024];
		unsigned int ID;
};
unsigned int IDCounted::currentID;
unsigned int IDCounted::IdList[1024];

class Object: public IDCounted {
};

//class CountedObject: public IDCounted, public RefCounted {

int main() {
	Object* A = new Object;
	Object* B = new Object;
	Object C, D;
	std::cout << A->getID() << " " << B->getID() << " " << C.getID() << " " << D.getID() << " ";
	delete A;
	Object E;
	delete B;
	Object F, G;
	std::cout << E.getID() << " " << F.getID() << " " << G.getID();
	//while(1) {}

	return 0;
}
