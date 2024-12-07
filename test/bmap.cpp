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
	static const ULL M = 3;	// matrix dimension
	static const ULL mod = 998388889;	// modulus

	// Functions
	Node();
	// pair<Node,Node> neighbours()const;
	vector<Node> adjacent()const;
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

// pair<Node,Node> Node::neighbours()const{
// 	// M is matrix size
// 	// Do sanity checks based on current coords
// 	// Use current coords and values for min_path, ai and bj.
// 	// pair.first  has possible down move
// 	// pair.second has possible right move
// 	pair<Node,Node> dnrt;
// 	if((coords.first+1)<M){	// down possible
// 		Node d;
// 		d.coords = {coords.first+1, coords.second};
// 		d.aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)
// 		// set local_value
// 		d.local_value = d.aibj.first + d.aibj.second;
// 		// sum_path = local_value + parent.sum_path
// 		d.sum_path = d.local_value + sum_path;
// 		dnrt.first = d;
// 	}
// 	if((coords.second+1)<M){	// right possible
// 		Node r;
// 		r.coords = {coords.first, coords.second+1};
// 		r.aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
// 		// set local_value
// 		r.local_value = r.aibj.first + r.aibj.second;
// 		// sum_path = local_value + parent.sum_path
// 		r.sum_path = r.local_value + sum_path;
// 		dnrt.second = r;
// 	}
// 	return dnrt;
// }

vector<Node> Node::adjacent()const{
	// Return a, possibly empty, vector of Nodes to the right and down.
	// The Nodes have initial values only
	vector<Node> rtdn;
	if((coords.first+1)<M){	// down possible
		Node d;
		d.coords = {coords.first+1, coords.second};
		d.aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)
		// set local_value
		d.local_value = d.aibj.first + d.aibj.second;
		// sum_path = local_value + parent.sum_path
		d.sum_path = d.local_value + sum_path;
		rtdn.push_back(d);
	}
	if((coords.second+1)<M){	// right possible
		Node r;
		r.coords = {coords.first, coords.second+1};
		r.aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
		// set local_value
		r.local_value = r.aibj.first + r.aibj.second;
		// sum_path = local_value + parent.sum_path
		r.sum_path = r.local_value + sum_path;
		rtdn.push_back(r);
	}
	// Vector of initialised Nodes
	return rtdn;
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

//----------------------Helper functions______________________________________
struct SumCmp
{
    bool operator()(const ULL lhs, const ULL rhs) const
    {
        return lhs < rhs;
    }
};

struct CoordCmp
{
    bool operator()(const Node lhs, const Node rhs) const
    {
        return lhs.coords < rhs.coords;
    }
};



//=============================================================================

int main(int argc, char **argv)
{

	// map the node sum_path to Node - multiple keys possible
	std::multimap<ULL, Node&, SumCmp> node_map;
	std::multimap<ULL, Node&, SumCmp>::iterator nmi_0, nmi_1;
	// Vector of up to two adjacent nodes
	vector<Node> adj_nodes;	// used by call to Node::adjacent()

	// Check if a Node found by adjacent is already in the node map 
	std::map<Coords, Node&> x_ref;	// Unique key-value pair



	Node start;	// Requires coords, aibj and local value
	start.coords = {0,0};
	start.aibj = {102022661, 864751430};
	start.local_value = start.aibj.first + start.aibj.second;
	start.sum_path = start.local_value;	// Unique to start node
	node_map.insert({start.local_value, start});
	x_ref.insert({start.coords, start});


	bool run_flag = true;
	adj_nodes = start.adjacent(); // Vector of nodes adjacent to start node.

	do{	// LOOP START
		if(adj_nodes.empty()){
			cout << "empty vector of nodes. stopping" << endl;
			exit(1);
		}
		// process the vector of adjacent nodes, the coords, local_value and sum_path are assumed valid.
		for(auto adj : adj_nodes){
			// x_ref to see if this adjacent node is already in node_map
			// if (found in x_ref){ // iterator points to matching element pair
			//		if(existing path_sum > adjacent path_sum)
			//			copy existing node using iterator
			//			update copy path_sum to new value
			//			delete existing node using iterator
			//			insert copy - this re-sorts the node_map
			//	} else { //not found new data
			//		node_map.insert(adjacent path_sum, adjacent node)
			//	}



		}


		// if(node_list.first.local_value > 0){// empty test
		// 	auto imap = node_map.find(node_list.first);
		// 	if(imap == node_map.end()){ // New code - insert into node_map
		// 		node_map.insert({node_list.first.coords, node_list.first});
		// 	} else { // Node already exists in node_map 
		// 		if( (imap->second).sum_path > node_list.first.sum_path ) // new path is lower cost
		// 			(imap->second).sum_path = node_list.first.sum_path;  // reduce path cost
		// 	}
		// }

		// // Check for empty second value
		// if(node_list.second.local_value > 0){// empty test
		// 	auto imap = node_map.find(node_list.second);
		// 	if(imap == node_map.end()){ // New code - insert into node_map
		// 		node_map.insert({node_list.second.coords, node_list.second});
		// 	} else { // Node already exists in node_map 
		// 		if( (imap->second).sum_path > node_list.second.sum_path ) // new path is lower cost
		// 			(imap->second).sum_path = node_list.second.sum_path;  // reduce path cost
		// 	}
		// }


		// now remove the first entry in node_map
		node_map.erase(node_map.begin());

		// debug printout
		cout << endl;
		for(auto i = node_map.begin(); i != node_map.end(); ++i){
			pair<ULL,Node> j = *i;
			j.second.prt_node();
		} // end debug printout

		// Test for end of loop
		nmi_0 = node_map.begin();
		nmi_1 = ++nmi_0;
		if ((*nmi_0).second.goal() and (*(nmi_1)).second.goal()){		
			ULL minimum_path = min((*nmi_0).second.sum_path,(*nmi_0).second.sum_path);
			cout << "\nGoal position found minimum_path = " << minimum_path << endl;
			run_flag = false;
		} else {
			// Using top of node_map update the vector of adjacent nodes
			adj_nodes = (node_map.begin()->second).adjacent();
			// update x_ref and node maps
			x_ref.erase((node_map.begin()->second).coords);
			node_map.erase(node_map.begin());
		}

	} while (run_flag);
		
	return 0;
}
