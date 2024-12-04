/* 
	mappairs.ccp
*/
#include <map>

#include <iostream>
#include <vector>
#include <cstdint>
#include <utility>
#include <string>

using namespace std;
typedef uint64_t ULL;
typedef std::pair<ULL,ULL> Coords;

int main(int argc, char **argv)
{
	std::multimap<Coords,std::string> pmap;
	std::multimap<Coords,std::string>::iterator imap;
	Coords c1{1,2};
	Coords c2{2,1};
	Coords c3{2,1};

	imap = pmap.insert({c1,"node 1"});
	cout << (imap->first).first<< "," << (imap->first).second << " " << (imap->second) << endl;

	imap = pmap.insert({c2,"node 2"});
	cout << (imap->first).first << "," << (imap->first).second << " " << (imap->second) << endl;

	cout << "Attempting search" << endl;
	imap = pmap.find(c2);
	if(imap != pmap.end())
			cout << (imap->first).first << "," << (imap->first).second << " " << (imap->second) << endl;


	return 0;
}
