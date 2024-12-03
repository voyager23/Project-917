/* 
filename.ccp
*/
#include <map>
#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <array>

using namespace std;
typedef uint64_t ULL;
typedef std::pair<ULL,ULL> Coords;
typedef std::pair<ULL,ULL> SubValues;

class Node;	// fwd declaration
typedef vector<vector<Node>> Matrix;
typedef std::pair<Node,Node> NodePair;

// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	ULL local_value;
	
	ULL sum_path = 999999999;
	static const ULL M = 2;	// matrix dimension
	static const ULL mod = 998388889;	// modulus

	// Functions
	Node();
	pair<Node,Node> neighbours()const;
	void prt_node() const;	// const tells compiler nothing will change inside this function
	ULL move_sn_2places(ULL sn)const;

};

// ---------------------class definitions---------------------

Node::Node(){
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
}

pair<Node,Node> Node::neighbours()const{
	// M is matrix size
	// Do sanity checks based on current coords
	// Use current coords and values for min_path, ai and bj.
	// pair.first  has possible down move
	// pair.second has possible right move
	pair<Node,Node> dnrt;
	if((coords.first+1)<M){	// down possible
		Node d;
		d.coords = {coords.first+1, coords.second};
		d.aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)
		// set local_value
		d.local_value = d.aibj.first + d.aibj.second;
		// sum_path = local_value + parent.sum_path
		d.sum_path = d.local_value + sum_path;
		dnrt.first = d;
	}
	if((coords.second+1)<M){	// right possible
		Node r;
		r.coords = {coords.first, coords.second+1};
		r.aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
		// set local_value
		r.local_value = r.aibj.first + r.aibj.second;
		// sum_path = local_value + parent.sum_path
		r.sum_path = r.local_value + sum_path;
		dnrt.second = r;
	}
	return dnrt;
}

ULL Node::move_sn_2places(ULL sn)const{
	// move sn 2 places along the Sn sequence
	ULL sm = (sn*sn)%mod;
	return ((sm*sm)%mod);
}

void Node::prt_node() const { // const tells compiler nothing will change inside this function
	cout << "Node {" << coords.first << "," << coords.second << "}" << endl;
	cout << "ai:" << aibj.first << " bj:" << aibj.second << endl;
	cout << "node_value:" << local_value << endl;
	cout << "sum_path:" << sum_path;
	cout << endl;
}

//=============================================================================

int main(int argc, char **argv)
{
	// For this test use a sqr. grid of 4 Nodes
	// The answer A(2) = 2388327490
	struct PathCmp
	{
	    bool operator()(const ULL lhs, const ULL rhs) const
	    {
	        return lhs < rhs;
	    }
	};

	std::multimap<ULL&, Node&, PathCmp> node_map;

	Node start;	// Requires coords, aibj and local value
	start.coords = {0,0};
	start.aibj = {102022661, 864751430};
	start.local_value = start.aibj.first + start.aibj.second;
	start.sum_path = start.local_value;	// Unique to start node
	node_map.insert({start.sum_path, start});

	
	// Comment, gdb reports that pair uses a default constructor for both values.
	// If the value is not updated than the local_value = 0;

	// Using start node as reference, find 2 neighbours and add to node_map
	pair<Node,Node> node_list = start.neighbours();
	node_map.insert({node_list.first.sum_path, node_list.first});
	node_map.insert({node_list.second.sum_path, node_list.second});

	// debug
	for(auto i = node_map.begin(); i != node_map.end(); ++i){
		pair<ULL,Node> j = *i;
		j.second.prt_node();
	}
	return 0;
}
