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
typedef std::pair<int64_t,int64_t > Coords;
typedef std::pair<int64_t,int64_t > SubValues;
class Node;	//Forward Declaration

//Top Level Parameter Block
const int64_t dimension = 5;
const int64_t Modulus = 1189;
const int64_t initial_ai = 1183;
const int64_t initial_bj = 36;

// const int64_t dimension = 10;
// const int64_t Modulus = 998388889;
// const int64_t initial_ai = 102022661;
// const int64_t initial_bj = 864751430;
// End TLPB

// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// pair of components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	int64_t local_value;
	static const int64_t M = dimension;	// matrix dimension
	static const int64_t mod = Modulus;	// modulus
	bool decided = false;		// True if both fromN/fromW set OR (edge_node AND minimum_path > 0)
	int64_t fromN = LLONG_MAX;	// default values
	int64_t fromW = LLONG_MAX;
	int64_t minimum_path = 0;	// minimum of fromN and fromW

	// Public Functions
	Node();
	void prt_node() const;	// const tells compiler nothing will change inside this function
	int64_t  move_sn_2places(int64_t sn);
	bool goal() const;
	void set_decided();
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

int64_t  Node::move_sn_2places(int64_t sn){
	// move sn 2 places along the Sn sequence
	int64_t  sm = (sn*sn)%mod;
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

void Node::set_decided(){	
	// Edge nodes need 1 inbound path and min_path == inbound_path;
	// Inside nodes need 2 inbound paths and min_path = min(fromN, fromW);
	// Test Inner
	if ((fromW != LLONG_MAX)and(fromN != LLONG_MAX)and(minimum_path == min(fromW,fromN)))
		decided = true;
	// Test Edge
	else if ((fromN != LLONG_MAX and minimum_path == fromN) or (fromW != LLONG_MAX and minimum_path == fromW)) 
		decided = true;
	// Default
	else
		decided = false;
}

// -----------------------------------------------------



//========================Main================================================

int main(int argc, char **argv)
{

	// Define the maps to use

	std::map<Coords, Node*> x_ref;	// map unique key coordinates to a Node pointeer

	std::multimap<int64_t, Node*> node_map;	// map minimum_cost path to Node*

	Node* working  = new Node;	// Set coords, aibj and local value

	working ->coords = {0,0};	// Zero based indexing
	working ->aibj = {initial_ai, initial_bj};
	working ->local_value = working ->aibj.first + working ->aibj.second;
	working->fromN = LLONG_MAX;
	working->fromW = LLONG_MAX;
	// Unique to start node
	working ->minimum_path = working ->local_value;
	working->decided = true;

	x_ref.insert({working->coords, working});
	node_map.insert({working->minimum_path, working});

	// Node selector
	for(auto i = node_map.begin(); (i != node_map.end() and i->second->decided == false); ++i) {}

	return 0;
}
