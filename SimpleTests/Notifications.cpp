// Simple test of the POCO Notification system. Written Feb 2012.

#include <iostream>
#include <Poco/NObserver.h>
#include <Poco/Notification.h>
#include <Poco/NotificationCenter.h>
using namespace Poco;

NotificationCenter NCenter;

class Notify1: public Notification {};
class Notify2: public Notification {};
class Notify3: public Notification {};

class Observe1 {
	public:
		Observe1():
			obs2(*this, &Observe1::handleNotify2),
			obs1(*this, &Observe1::handleNotify1)
		{ NCenter.addObserver(obs2); NCenter.addObserver(obs1); }
		void handleNotify2(const AutoPtr<Notify2>& notification) { std::cout << notification->name() << std::endl; }
		NObserver<Observe1, Notify2> obs2;
		void handleNotify1(const AutoPtr<Notify1>& notification) { std::cout << notification->name() << std::endl; }
		NObserver<Observe1, Notify1> obs1;

	protected:
} Observer1;

/*
class Observe1 {
	public:
		Observe1():
			obs2(*this, &Observe1::handleNotify),
			obs1(*this, &Observe1::handleNotify)
		{ NCenter.addObserver(obs2); NCenter.addObserver(obs1); }
		void handleNotify(const AutoPtr<Notification>& notification) { std::cout << notification->name() << std::endl; }
		NObserver<Observe1, Notify2> obs2;
		NObserver<Observe1, Notify1> obs1;
} Observer1;
*/
class Observe2 {
	public:
		Observe2():
			obs2(*this, &Observe2::handleNotify2),
			obs3(*this, &Observe2::handleNotify3)
		{ NCenter.addObserver(obs2); NCenter.addObserver(obs3); }
		void handleNotify2(const AutoPtr<Notify2>& notification) { std::cout << notification->name() << std::endl; }
		NObserver<Observe2, Notify2> obs2;
		void handleNotify3(const AutoPtr<Notify3>& notification) { std::cout << notification->name() << std::endl; }
		NObserver<Observe2, Notify3> obs3;
} Observer2;

int main() {
	NCenter.postNotification(new Notify1);
	NCenter.postNotification(new Notify2);
	NCenter.postNotification(new Notify3);
}
