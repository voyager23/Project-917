/* 
filename.ccp
*/
#include <map>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>

using namespace std;
typedef uint64_t ULL;
typedef std::pair<ULL,ULL> Coords;
typedef std::pair<ULL,ULL> SubValues;
class Node;	//Forward Declaration


// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	ULL local_value;
	
	ULL sum_path = 999999999;
	static const ULL M = 10;	// matrix dimension
	static const ULL mod = 998388889;	// modulus

	// Functions
	Node();
	pair<Node*,Node*> neighbours()const;
	void prt_node() const;	// const tells compiler nothing will change inside this function
	ULL move_sn_2places(ULL sn)const;
	bool goal() const;
};

// ---------------------class definitions---------------------
Node::Node(){
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
}

pair<Node*,Node*> Node::neighbours()const{
	// M is matrix size
	// Do sanity checks based on current coords
	// Use current coords and values for min_path, ai and bj.
	// pair.first  has possible down move
	// pair.second has possible right move
	pair<Node*,Node*> dnrt{NULL,NULL};
	if((coords.first+1)<M){	// down possible
		Node* d = new Node;
		// new coords
		d->coords = {coords.first+1, coords.second};
		// new aibj
		d->aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)
		// set new local_value
		d->local_value = d->aibj.first + d->aibj.second;
		// sum_path = local_value + parent.sum_path
		d->sum_path = d->local_value + sum_path;
		// save pointer
		dnrt.first = d;
	}
	if((coords.second+1)<M){	// right possible
		Node* r = new Node;
		// new coords
		r->coords = {coords.first, coords.second+1};
		// new aibj
		r->aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
		// set new local_value
		r->local_value = r->aibj.first + r->aibj.second;
		// sum_path = local_value + parent.sum_path
		r->sum_path = r->local_value + sum_path;
		// save pointer
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

bool Node::goal() const{
	return ((coords.first == M-1) and (coords.second == M-1));
}


//========================Main================================================

int main(int argc, char **argv)
{

	std::multimap<long long unsigned, Node*> min_cost_map;	// map multiple minimum cost path values to a node pointer
	std::multimap<long long unsigned, Node*>::iterator mincost_i;

	std::map<Coords, Node*> id_node_map;	// map unique key coordinates to a Node pointeer
	std::map<Coords, Node*>::iterator idnode_i;

	Node* pstart = new Node;	// Requires coords, aibj and local value
	pstart->coords = {0,0};
	pstart->aibj = {102022661, 864751430};
	pstart->local_value = pstart->aibj.first + pstart->aibj.second;
	pstart->sum_path = pstart->local_value;	// Unique to start node

	min_cost_map.insert({pstart->sum_path, pstart});

	id_node_map.insert({pstart->coords, pstart});

	// -----LOOP START-----
	bool run_flag = true;
	pair<Node*,Node*> adjacent = pstart->neighbours();
	
	min_cost_map.insert({(adjacent.first)->sum_path, adjacent.first});
	id_node_map.insert({(adjacent.first)->coords, adjacent.first});

	min_cost_map.insert({(adjacent.second)->sum_path, adjacent.second});
	id_node_map.insert({(adjacent.second)->coords, adjacent.second});

	for(auto m : min_cost_map) (m.second)->prt_node();

	return 0;
}
