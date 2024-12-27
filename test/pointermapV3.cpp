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
typedef std::map<Coords, Node*> XRef;	// map type. unique key coordinates to a Node pointeer
typedef std::multimap<int64_t, Node*> NodeMap;	// map minimum_cost path to Node*

//Top Level Parameter Block
const int64_t dimension = 3;
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
	cout << "fromN:" << fromN << endl;
	cout << "fromW:" << fromW << endl;
	cout << "minimum_path:" << minimum_path << endl;
	cout << endl;
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
void foobar(Node* parent, NodeMap& node_map,  XRef& x_ref, bool right = true);

void foobar(Node* parent, NodeMap& node_map,  XRef& x_ref, bool right){ 
	// iter references a NodeMap entry, decided and non-goal

	// Limits check
	if((right)and((parent->coords.second+1)>=dimension)) return;
	if((!right)and((parent->coords.first+1)>=dimension)) return;

	Node *xNode = NULL;	// Node pointer
	//setup the coords to search for
	XRef::iterator search;
	if(right){
		search = x_ref.find({parent->coords.first, parent->coords.second+1});
	} else {
		search = x_ref.find({parent->coords.first+1, parent->coords.second});
	}
	if(search != x_ref.end()) { // existing node;
		cout << "found existing node" << endl;	//DEBUG
		xNode = search->second;	// get pointer to right/down node
		if(right){
			xNode->fromW = parent->minimum_path + xNode->local_value;
		} else {
			xNode->fromN = parent->minimum_path + xNode->local_value;
		}
		xNode->minimum_path = min(xNode->fromN, xNode->fromW);

		// Since we have altered the node minimum_path, it must be removed/reinserted into node_map
		// find xNode in node_map {iterator}

		auto erange = node_map.equal_range(xNode->minimum_path);
		NodeMap::iterator iter;
		for (iter = erange.first; iter != erange.second; ++iter) {
			if (iter->second == xNode) { 
				node_map.erase(iter);
				node_map.insert({xNode->minimum_path, xNode});
				break;
			}				
		}
		// Check success
		if (iter == erange.second){
			cout << "Error, foobar find/remove/insert failed" << endl; 
			exit(1);
		}

	} else { // New node
		cout << "Allocating new mode" << endl;
		xNode = new Node;
		if(right) { // move right
			xNode->coords = {parent->coords.first, parent->coords.second+1};
			xNode->aibj = {parent->aibj.first, xNode->move_sn_2places(parent->aibj.second)};
			xNode->local_value = xNode->aibj.first + xNode->aibj.second;
			xNode->fromW = xNode->local_value + parent->minimum_path;
		} else { // move down
			xNode->coords = {parent->coords.first+1, parent->coords.second};
			xNode->aibj = {xNode->move_sn_2places(parent->aibj.first), parent->aibj.second};
			xNode->local_value = xNode->aibj.first + xNode->aibj.second;
			xNode->fromN = xNode->local_value + parent->minimum_path;
		}
		// check North/West from this new node
		XRef::iterator wn;
		if(right){	// check NORTH	
			wn = x_ref.find({xNode->coords.first+1, xNode->coords.second});
			if ((wn != x_ref.end()) and (wn->second->decided)) {
				xNode->fromN = wn->second->minimum_path;
			}
		} else {	// check WEST
			wn = x_ref.find({xNode->coords.first, xNode->coords.second-1});
			if ((wn != x_ref.end()) and (wn->second->decided)) {
				xNode->fromW = wn->second->minimum_path;
			}
		}
		xNode->minimum_path = min(xNode->fromN, xNode->fromW);

		xNode->set_decided();
		// Insert minimum_path, xNode into node_map
		node_map.insert({xNode->minimum_path, xNode});
		// Also insert coords/Node* into x_ref map
	} // else new right/down node

}


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
	NodeMap::iterator i = node_map.begin();

	while(1){
		if(i->second->goal()){
			cout << endl << "Goal Node Selected. Minimum path: " << i->second->minimum_path << endl;
			exit(0);
		}

		if(i->second->decided) {
			Node* parent = i->second;	// copy pointer to parent node
			node_map.erase(i);			// erase from map

			foobar(parent, node_map, x_ref, true);	// true  == move right
			cout << "right	node_map.size():" << node_map.size() << endl;

			foobar(parent, node_map, x_ref, false);	// false == move down
			cout << "down	node_map.size():" << node_map.size() << endl;

			// remove entry in x_ref map
			x_ref.erase(parent->coords);

			// debug
			for(auto z = node_map.begin(); z != node_map.end(); ++z){
				z->second->prt_node();
			}
		} // if decided

	} while(++i != node_map.end());

	return 0;
}
