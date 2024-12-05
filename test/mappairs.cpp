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
	std::multimap<Coords,std::vector<ULL>> pmap;
	std::multimap<Coords,std::vector<ULL>>::iterator imap;
	Coords c1{1,2};
	Coords c2{10,20};
	Coords c3{10,20};

	imap = pmap.insert({c1,{1,2}});
	cout << "Coords: " << (imap->first).first<< "," << (imap->first).second << endl;
	for(auto d : (imap->second)) cout << d << " ";
	cout << endl;

	imap = pmap.insert({c2,{10,20}});
	cout << "Coords: " << (imap->first).first << "," << (imap->first).second << endl;
	for(auto d : (imap->second)) cout << d << " ";
	cout << endl;

	cout << "Attempting search" << endl;
	imap = pmap.find(c3);
	if(imap != pmap.end()) {
			cout << "Coords: " << (imap->first).first << "," << (imap->first).second << endl;
			for(auto d : (imap->second)) cout << d << " ";
	} else {
		cout << "Search failed." << endl;
	}
	cout << endl;

	cout << "modifying vector" << endl;
	imap = pmap.find(c3);
	if(imap != pmap.end()) {
			// modify the vector
			for(auto i = (imap->second).begin(); i !=(imap->second).end(); ++i ) *i *= 13;
			cout << "Coords: " << (imap->first).first << "," << (imap->first).second << endl;
			for(auto d : (imap->second)) cout << d << " ";
	} else {
		cout << "Search failed." << endl;
	}
	cout << endl;
	
	cout << "Attempting confirmation" << endl;
	imap = pmap.find(c3);
	if(imap != pmap.end()) {
			cout << "Coords: " << (imap->first).first << "," << (imap->first).second << endl;
			for(auto d : (imap->second)) cout << d << " ";
	} else {
		cout << "Search failed." << endl;
	}
	cout << endl;

	return 0;
}
