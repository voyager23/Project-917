/* 
pointermap.cpp
*/
#include <map>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <climits>

using namespace std;
typedef std::pair<long long,long long > Coords;
typedef std::pair<long long,long long > SubValues;
class Node;	//Forward Declaration

//Top Level Parameter Block
const long long dimension = 5;
const long long Modulus = 1189;
const long long initial_ai = 1183;
const long long initial_bj = 36;

// const long long dimension = 10;
// const long long Modulus = 998388889;
// const long long initial_ai = 102022661;
// const long long initial_bj = 864751430;

// End TLPB

// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// pair of components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	long long local_value;
	static const long long M = dimension;	// matrix dimension
	static const long long mod = Modulus;	// modulus

	long long fromN = mod * 2;	// default values
	long long fromW = mod * 2;
	long long minimum_path = 0;	// minimum of fromN and fromW

	// Public Functions
	Node();
	void prt_node() const;	// const tells compiler nothing will change inside this function
	long long  move_sn_2places(long long sn);
	bool goal() const;
};

// ---------------------class definitions---------------------
Node::Node(){	// constructor
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
	fromN = LLONG_MAX;
	fromW = LLONG_MAX;
	minimum_path = 0;
}

long long  Node::move_sn_2places(long long sn){
	// move sn 2 places along the Sn sequence
	long long  sm = (sn*sn)%mod;
	return ((sm*sm)%mod);
}

void Node::prt_node() const { // const tells compiler nothing will change inside this function
	cout << "Node {" << coords.first << "," << coords.second << "}" << endl;
	cout << "ai:" << aibj.first << " bj:" << aibj.second << endl;
	cout << "node_value:" << local_value << endl;
	cout << "minimum_path:" << minimum_path;
	cout << endl << endl;;
}

bool Node::goal() const{
	return ((coords.first == M-1) and (coords.second == M-1));
}


// ---------------------end class definitions--------------------------------



//========================Main================================================

int main(int argc, char **argv)
{

	// Version 211224
	// Define the maps to use

	std::map<Coords, Node*> x_ref;	// map unique key coordinates to a Node pointeer
	typedef std::map<Coords, Node*>::iterator iter;

	std::multimap<long long, iter> determined;		// Iterator to Node with minimum_path fully defined

	std::multimap<long long, iter> not_determined;	// minimum_path only partially defined

	Node* working  = new Node;	// Set coords, aibj and local value

	working ->coords = {0,0};	// Zero based indexing
	working ->aibj = {initial_ai, initial_bj};
	working ->local_value = working ->aibj.first + working ->aibj.second;
	working ->minimum_path = working ->local_value;	// Unique to start node
	working->fromN = LLONG_MAX;
	working->fromW = LLONG_MAX;

	x_ref.insert({working->coords, working});
	determined.insert({working->minimum_path, x_ref.begin()});	//????????????????????????????

	return 0;
}
