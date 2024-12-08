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
	pair<Node,Node> neighbours()const;
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

bool Node::goal() const{
	return ((coords.first == M-1) and (coords.second == M-1));
}



//=============================================================================

int main(int argc, char **argv)
{
	/* This version uses 2 maps.

	1) map<coords, node> to map a single position to a node.
	2) multimap<minimum_cost, coords> may be several nodes with equal minimal path costs

	get first minimum_cost_node from multimap. Sort function std::less<ULL>
	get associated working node from map<coords,node> Sort function compare std::pair<ULL,ULL>

	*/


	struct SumPathCmp
	{
	    bool operator()(const ULL lhs, const ULL rhs) const
	    {
	        return lhs < rhs;
	    }
	};

	// Redefine multimap using a Coords pair as the key and associated Node as value
	// Requires new compare functions.

	std::multimap<ULL, Node, SumPathCmp> min_cost_map;	// map multiple minimum cost path values to a node 
	std::multimap<ULL, Node, SumPathCmp>::iterator mincost_i;

	std::map<Coords, Node> id_node_map;	// map unique key coordinates to a Node
	std::map<Coords, Node>::iterator idnode_i;

	pair<Node,Node> node_list;

	ULL minimum_path = 0;

	Node start;	// Requires coords, aibj and local value
	start.coords = {0,0};
	start.aibj = {102022661, 864751430};
	start.local_value = start.aibj.first + start.aibj.second;
	start.sum_path = start.local_value;	// Unique to start node
	node_map.insert({start.coords, start});

	// Comment, gdb reports that pair uses a default constructor for both values.
	// If the value is not updated than the local_value = 0;

	// The first entry in multimap has the minimum cost path value.
	// Using this as reference, find 2 neighbours and add/sort to node_map.

	// -----LOOP START-----
	bool run_flag = true;
	do{
		// For reasons unknown, this test is required!
		if(!node_map.empty())	
			node_list = (node_map.begin())->second.neighbours();

		// // Check for empty first value
		if(node_list.first.local_value > 0){// empty test
			auto imap = id_node_map.find(node_list.first.coords);
			if(imap == id_node_map.end()){ // New code - insert coords and node into id_node_map
				id_node_map.insert({node_list.first.coords, node_list.first});
			} else { // Node already exists in id-node_map
				// get a reference to the corresponding node
				if( (imap->second).sum_path > node_list.first.sum_path ) // new path is lower cost
					(imap->second).sum_path = node_list.first.sum_path;  // reduce path cost
			}
		}

		// Check for empty second value
		if(node_list.second.local_value > 0){// empty test
			auto imap = node_map.find(node_list.second.coords);
			if(imap == node_map.end()){ // New code - insert into node_map
				node_map.insert({node_list.second.coords, node_list.second});
			} else { // Node already exists in node_map 
				if( (imap->second).sum_path > node_list.second.sum_path ) // new path is lower cost
					(imap->second).sum_path = node_list.second.sum_path;  // reduce path cost
			}
		}


		// now remove the first entry in node_map
		node_map.erase(node_map.begin());

		// debug printout
		cout << endl;
		for(auto i = node_map.begin(); i != node_map.end(); ++i){
			pair<Coords,Node> j = *i;
			j.second.prt_node();
		} // end debug printout

		// Test for end of loop
		nmi_0 = node_map.begin();
		nmi_1 = ++nmi_0;
		if ((*nmi_0).second.goal() and (*(nmi_1)).second.goal()){		
			minimum_path = min((*nmi_0).second.sum_path,(*nmi_0).second.sum_path);
			cout << "\nGoal position found minimum_path = " << minimum_path << endl;
			run_flag = false;
		}
	} while (run_flag);
		
	return 0;
}
