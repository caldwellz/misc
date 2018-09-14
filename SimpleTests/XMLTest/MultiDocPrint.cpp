
#include <fstream>

#include "tinyxml2.h"

int main() {
	tinyxml2::XMLDocument doc1, doc2;
	tinyxml2::XMLPrinter prnt, prntcompact(0, true);
	std::ofstream out, outcompact;

	doc1.LoadFile("dream.xml");
	doc2.LoadFile("fireworks.xml");

	prnt.OpenElement("root");
	doc1.Print(&prnt);
	doc2.Print(&prnt);
	prnt.CloseElement();

	prntcompact.OpenElement("root");
	doc1.Print(&prntcompact);
	doc2.Print(&prntcompact);
	prntcompact.CloseElement();

	out.open("MultiDoc.xml", std::ios_base::trunc);
	out.write(prnt.CStr(), prnt.CStrSize()-1);						//-1 is to leave off the Null at the end

	outcompact.open("MultiDocCompact.xml", std::ios_base::trunc);
	outcompact.write(prntcompact.CStr(), prntcompact.CStrSize()-1);

	return 0;
}
