//Doing type-detection without RTTI (at least that's what I thought at the time), i.e. it can be compiled with -fno-rtti. Written March 2013.
//Based on Scott Meyers' double-dispatch idea from More Efficient C++

#include <iostream>

class BaseListener;

class BaseMessage {
	public:
		virtual void send(BaseListener* listener) = 0;
		virtual ~BaseMessage() { }
};

class MessageA;
class MessageB;

class BaseListener {
	public:
		void detectMessage(BaseMessage* msg) { msg->send(this); }
		virtual void sendMessage(MessageA* msg);
		virtual void sendMessage(MessageB* msg);
};

class MessageA: public BaseMessage {
	public:
		virtual void send(BaseListener* listener) { listener->sendMessage(this); }
		virtual int AFunc() { return 1; }
};

class MessageB: public BaseMessage {
	public:
		virtual void send(BaseListener* listener) { listener->sendMessage(this); }
		virtual int BFunc() { return 2; }
};

class BListener: public BaseListener {
	public:
		void sendMessage(MessageB* msg) {
			std::cout << msg->BFunc()
			<< std::endl;;
		}
};

void BaseListener::sendMessage(MessageA* msg) { delete msg; }
void BaseListener::sendMessage(MessageB* msg) { delete msg; }

int main() {
	BListener b;
	b.detectMessage(new MessageA);
	b.detectMessage(new MessageB);
	b.sendMessage(new MessageB);

	return 0;
}
