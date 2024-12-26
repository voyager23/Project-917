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
void foobar(Node* p, NodeMap& node_map,  XRef& x_ref, bool right = true);

void foobar(Node* p, NodeMap& node_map,  XRef& x_ref, bool right){ 
		// p is pointer to parent node, un-decided and non-goal - default direction is -Right-

		Node *xNode = NULL;	// working pointer
		auto search = x_ref.find(p->coords);
		if(search != x_ref.end()) { // existing un-decided node;
			xNode = search->second;	// get pointer to part node
			if(right){
				xNode->fromW = p->minimum_path + xNode->local_value;
			} else {
				xNode->fromN = p->minimum_path + xNode->local_value;
			}
			xNode->minimum_path = min(xNode->fromN, xNode->fromW);
			// Since we have altered the node minimum_path, it must be removed/reinserted into node_map
			// find xNode in node_map {iterator}

			auto result = node_map.equal_range(xNode->minimum_path);
			NodeMap::iterator iter;
			for (iter = result.first; iter != result.second; ++iter) {
				if (iter->second == xNode) { 
					node_map.erase(iter);
					node_map.insert({xNode->minimum_path, xNode});
					break;
				}				
			}
			// Check success
			if (iter == result.second){
				cout << "Error, foobar find/remove/insert failed" << endl; 
				exit(1);
			}

		} else { // New node
			xNode = new Node;
			if(right) { // move right
				xNode->coords = {p->coords.first, p->coords.second+1};
				xNode->aibj = {p->aibj.first, xNode->move_sn_2places(p->aibj.second)};
				xNode->local_value = xNode->aibj.first + xNode->aibj.second;
				xNode->fromW = xNode->local_value + p->minimum_path;
			} else { // move down
				xNode->coords = {p->coords.first+1, p->coords.second};
				xNode->aibj = {xNode->move_sn_2places(p->aibj.first), p->aibj.first};
				xNode->local_value = xNode->aibj.first + xNode->aibj.second;
				xNode->fromN = xNode->local_value + p->minimum_path;
			}
			xNode->minimum_path = min(xNode->fromN, xNode->fromW);

			// check North/West from this new node
			auto up = x_ref.find({xNode->coords.first - 1, xNode->coords.second});
			if ((up != x_ref.end()) and (up->second->decided)) {
				xNode->fromN = up->second->minimum_path;
				xNode->minimum_path = min(xNode->fromN, xNode->fromW);
			}
			xNode->set_decided();
			// Insert minimum_path, xNode into node_map
			node_map.insert({xNode->minimum_path, xNode});
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
	for(auto i = node_map.begin(); i != node_map.end(); ++i) {
		if(i->second->goal()){
			cout << endl << "Goal Node Selected. Minimum path: " << i->second->minimum_path << endl;
			exit(0);
		}
		if(!i->second->decided) continue;	// Don't use an incomplete node to extend

		foobar(i->second, node_map, x_ref, true);	// move right

		// Remove node_map entry referenced by i
		// node_map.erase(i);

	}

	return 0;
}
