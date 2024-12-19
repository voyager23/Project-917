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

// Top Level Parameter Block
const long long dimension = 3;
const long long Modulus = 1189;
const long long initial_ai = 1183;
const long long initial_bj = 36;
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
	Node* Nparent = NULL;
	Node* Wparent = NULL;

	// Public Functions
	Node();
	void prt_node() const;	// const tells compiler nothing will change inside this function
	long long  move_sn_2places(long long sn);
	bool goal() const;
	void find_update_adj_nodes(multimap<long long, Node*>& min_cost_map, 
									map<Coords,Node*>& id_node_map);
};

// ---------------------class definitions---------------------
Node::Node(){	// constructor
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
	fromN = LLONG_MAX;
	fromW = LLONG_MAX;
	minimum_path = 0;
	Nparent = NULL;
	Wparent = NULL;
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

void Node::find_update_adj_nodes(multimap<long long, Node*>& min_cost_map, map<Coords,Node*>& id_node_map){
	// Sanity chek
	if((min_cost_map.empty())or(id_node_map.empty())){
		cout << "?min_cost_map empty or id_node_map empty?" << endl;
		exit(1);
	}
	Node* working = min_cost_map.begin()->second;
	if(working->goal()){
		cout << "Goal Node reached." << endl;
		cout << working->minimum_path << endl;
		exit(0);
	}
	// Remove this working node from both maps;
	min_cost_map.erase(min_cost_map.begin());	// erase top of map
	id_node_map.erase(working->coords);			// erase by key

	// Consider a move right
	if ((working->coords.second + 1) < M){
		auto rNode = id_node_map.find({working->coords.first, working->coords.second + 1});
		if(rNode != id_node_map.end()){	// right neighbour exists
			// assume fromN and minimum_path have been set previously
			rNode->second->fromW = rNode->second->local_value + minimum_path;
			rNode->second->minimum_path = min(rNode->second->fromW, rNode->second->fromN);
		} else { // new Node

			//TODO NO NODE ALLOCATED
			
			coords = {working->coords.first, working->coords.second + 1};
			aibj = {working->aibj.first, move_sn_2places(working->aibj.second)};
			local_value = aibj.first + aibj.second;
			fromW = local_value + working->minimum_path;
			// check for possible N neighbour
			auto nNode = id_node_map.find(coords.first - 1, coords.second);
			if(nNode != id_node_map.end()){
				fromN = local_value + nNode->second->minimum_path;
				minimum_path = min(fromN, fromW);
			}else{
				fromN = LLONG_MAX;
				minimum_path = fromW;
			} // if_else
		} // if_else
	} // if down


	// Consider a move down
	if ((working->coords.first + 1) < M){
		auto dNode = id_node_map.find({working->coords.first, working->coords.second + 1});
		if(dnode != id_node_map.end()){	// down neighbour exists
			// assume fromW and minimum_path have been set previously
			dnode->second->fromN = dnode->second->local_value + minimum_path;
			dnode->second->minimum_path = min(dnode->second->fromW, dnode->second->fromN);
		} else { // new Node

			// NO NEW NODE ALLOCATED

			coords = {working->coords.first, working->coords.second + 1};
			aibj = {working->aibj.first, move_sn_2places(working->aibj.second)};
			local_value = aibj.first + aibj.second;
			fromW = local_value + working->minimum_path;
			// check for possible N neighbour
			auto nNode = id_node_map.find(coords.first - 1, coords.second);
			if(nNode != id_node_map.end()){
				fromN = local_value + nNode->second->minimum_path;
				minimum_path = min(fromN, fromW);
			}else{
				fromN = LLONG_MAX;
				minimum_path = fromW;
			} // if_else
		} // if_else
	} // if down

} // end



//========================Main================================================

int main(int argc, char **argv)
{

	std::multimap<long long, Node*> min_cost_map;	// map multiple minimum cost path values to a node pointer
	std::multimap<long long, Node*>::iterator mincost_i;

	std::map<Coords, Node*> id_node_map;	// map unique key coordinates to a Node pointeer
	std::map<Coords, Node*>::iterator idnode_i;

	Node* working  = new Node;	// Set coords, aibj and local value
	working ->coords = {0,0};	// Zero based indexing

	working ->aibj = {initial_ai, initial_bj};
	working ->local_value = working ->aibj.first + working ->aibj.second;
	working ->minimum_path = working ->local_value;	// Unique to start node

	min_cost_map.insert({working ->minimum_path, working });
	id_node_map.insert({working ->coords, working });

	working->find_update_adj_nodes(min_cost_map, id_node_map);	// updates maps

	return 0;
}
