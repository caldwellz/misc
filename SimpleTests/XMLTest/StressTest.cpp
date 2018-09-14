
#include <iostream>
#include <fstream>
#include <sstream>

#include "tinyxml2.h"
using namespace tinyxml2;

int main() {
	XMLDocument doc;
	XMLPrinter prnt(0, true);
	XMLElement *root, *ecur;
	std::ofstream out;

	doc.Parse("<?xml version=\"1.0\"?><root/>");
	root = doc.RootElement();

	int e, i;
	char* estr = new char[5];
	std::stringstream str;

	std::cout << "---Setting test---" << std::endl;
	for(e = 1; e <= 1000; ++e) {
		for(i = 1; i <= 25; i++) {
			str.str("");
			str << "Attribute" << i;
			XMLUtil::ToStr(e, estr, 4);
			ecur = doc.NewElement(estr);
			ecur->SetAttribute(str.str().c_str(), (str.str() + "-String").c_str());
			ecur->SetAttribute(str.str().c_str(), e + i);
			ecur->SetAttribute(str.str().c_str(), true);
			ecur->SetAttribute(str.str().c_str(), (float) (e + i) / 3);
			root->InsertEndChild(ecur);
		}
		if((e % 25) == 0) std::cout << "Setting: " << e << " entities processed..." << std::endl;
	}
	std::cout
	<< "Attributes: "
	<< root->FirstChildElement("1")->Attribute("Attribute10") << " "
	<< root->FirstChildElement("100")->IntAttribute("Attribute25") << " "
	<< root->FirstChildElement("10")->BoolAttribute("Attribute17") << " "
	<< root->FirstChildElement("50")->FloatAttribute("Attribute10") << std::endl;
	std::cin.get();

	std::cout << "---Fetching test---" << std::endl;
	for(e = 1; e <= 1000; ++e) {
		for(i = 1; i <= 25; i++) {
			str.str("");
			str << "Attribute" << i;
			XMLUtil::ToStr(e, estr, 4);
			ecur = root->FirstChildElement(estr);
			ecur->Attribute(str.str().c_str());
			ecur->IntAttribute(str.str().c_str());
			ecur->BoolAttribute(str.str().c_str());
			ecur->FloatAttribute(str.str().c_str());
		}
		if((e % 25) == 0) std::cout << "Fetching: " << e << " entities processed..." << std::endl;
	}
	std::cin.get();

	prnt.OpenElement("element");
	doc.Print(&prnt);
	prnt.CloseElement();
	out.open("StressTest.xml", std::ios_base::trunc);
	out.write(prnt.CStr(), prnt.CStrSize()-1);

	std::cout << "---Clearing test---" << std::endl;
	for(e = 1; e <= 1000; ++e) {
		XMLUtil::ToStr(e, estr, 4);
		root->DeleteChild(root->FirstChildElement(estr));
		if((e % 25) == 0) std::cout << "Clearing: " << e << " entities processed..." << std::endl;
	}
	std::cin.get();

	return 0;
}
