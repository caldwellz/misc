// RTTI testing while learning about it in my early days of C++. Written Aug 2012.
#include <typeinfo>
#include <iostream>
#include <string>

class base {
	public:
		virtual std::string name() = 0;
};

class classA: public base {
	public:
		virtual std::string name() { return "ClassA"; }
};

class classB: public classA {
	public:
		virtual std::string name() { return "ClassB"; }
		virtual std::string classBFunc() { return "ClassB"; }
};

class classC: public classA {
	public:
		virtual std::string name() { return "ClassC"; }
		virtual std::string classCFunc() { return "ClassC"; }
};

class Main {
	public:
		void getName(classB& obj) { std::cout << obj.name(); }
		void getName(classC& obj) { std::cout << obj.name(); }
		void typeDetect(base& obj) {
			if(dynamic_cast<classB*>(&obj)) {
				getName(*(dynamic_cast<classB*>(&obj)));
				std::cout << " " << (*(dynamic_cast<classB*>(&obj))).classBFunc() << " " << (typeid(&obj) == typeid(dynamic_cast<classB*>(&obj)) ? "1" : "0") << std::endl;
				//return;
			}
			if(dynamic_cast<classC*>(&obj)) {
				getName(*(dynamic_cast<classC*>(&obj)));
				std::cout << " " << (*(dynamic_cast<classC*>(&obj))).classCFunc() << " " << (typeid(&obj) == typeid(dynamic_cast<classC*>(&obj)) ? "1" : "0") << std::endl;
				//return;
			}
		}
};

int main() {
	classA a;
	classB b;
	classC c;
	Main main;

	main.typeDetect(c);
	std::cout << '\n' << (typeid(b) == typeid(b) ? "1" : "0") << (typeid(a) == typeid(b) ? "1" : "0") << (typeid(b) == typeid(c) ? "1" : "0");

	return 0;
}
