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
typedef int64_t LL;
typedef std::pair<LL,LL > Coords;
typedef std::pair<LL,LL > SubValues;
class Node;	//Forward Declaration

// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// pair of components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	LL local_value;
	// LL  sum_path = 999999999;
	static const LL M = 3;	// matrix dimension
	static const LL mod = 21;	// modulus

	LL fromN = mod * 2;	// default values
	LL fromW = mod * 2;
	LL minimum_path = 0;	// minimum of fromN and fromW

	// Functions
	Node();
	pair<Node*,Node*> neighbours(std::map<Coords, Node*>& id_node_map)const;
	void prt_node() const;	// const tells compiler nothing will change inside this function
	LL  move_sn_2places(LL sn)const;
	bool goal() const;
};

// ---------------------class definitions---------------------
Node::Node(){	// constructor
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
	fromN = LLONG_MAX;
	fromW = LLONG_MAX;
}

pair<Node*,Node*> Node::neighbours(std::map<Coords, Node*>& id_node_map)const{
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
		// set local valur
		d->local_value = aibj.first + aibj.second;
		// fromNorth = this local_value + parent local value
		d->fromN = d->aibj.first + d->aibj.second + local_value;
		// Does this node have a west neighbor?
		// if so get minimum_path from west neighbor and set fromW value;
		// west neighbor = x,y-1
		// fetch node pointer from id_node_map Node* westP
		auto bar = id_node_map.find({coords.first, coords.second-1});
		if(bar != id_node_map.end())
			d->fromW = (bar->second)->minimum_path;

		d->minimum_path = min(d->fromN, d->fromW);
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
		// fromW = new local_value + this local value
		r->fromW = r->aibj.first + r->aibj.second + local_value;
		// Does this node have a North neighbor?
		// if so get minimum_path from north neighbor and set fromN value;
		// north neighbor = x-1,y
		// fetch node pointer from id_node_map Node* northP
		auto bar = id_node_map.find({coords.first-1, coords.second});
		if(bar != id_node_map.end())
			r->fromN = (bar->second)->minimum_path;
		
		r->minimum_path = min(r->fromN, r->fromW);
		dnrt.second = r;
	}

	return dnrt;
}

LL  Node::move_sn_2places(LL  sn)const{
	// move sn 2 places along the Sn sequence
	LL  sm = (sn*sn)%mod;
	return ((sm*sm)%mod);
}

void Node::prt_node() const { // const tells compiler nothing will change inside this function
	cout << "Node {" << coords.first << "," << coords.second << "}" << endl;
	cout << "ai:" << aibj.first << " bj:" << aibj.second << endl;
	cout << "node_value:" << local_value << endl;
	cout << "minimum_path:" << minimum_path;
	cout << endl;
}

bool Node::goal() const{
	return ((coords.first == M-1) and (coords.second == M-1));
}


//========================Main================================================

int main(int argc, char **argv)
{

	std::multimap<LL, Node*> min_cost_map;	// map multiple minimum cost path values to a node pointer
	std::multimap<LL, Node*>::iterator mincost_i;

	std::map<Coords, Node*> id_node_map;	// map unique key coordinates to a Node pointeer
	std::map<Coords, Node*>::iterator idnode_i;

	Node* working  = new Node;	// Set coords, aibj and local value
	working ->coords = {0,0};	// Zero based indexing
	working ->aibj = {4,15};
	working ->local_value = working ->aibj.first + working ->aibj.second;
	working ->minimum_path = working ->local_value;	// Unique to start node

	min_cost_map.insert({working ->minimum_path, working });

	id_node_map.insert({working ->coords, working });

	// -----LOOP-----

	while(1){
		pair<Node*,Node*> adjacent = working ->neighbours(id_node_map);

		if(((adjacent.first)==NULL) and ((adjacent.second)==NULL)){
			cout << "\nGoal Node reached\n";
			cout << working->minimum_path << endl;
			return 0;
		}
		
		// -----
		if(adjacent.first != NULL){		
			min_cost_map.insert({(adjacent.first)->minimum_path, adjacent.first});
			id_node_map.insert({(adjacent.first)->coords, adjacent.first});
		}
		if(adjacent.second != NULL){	
			min_cost_map.insert({(adjacent.second)->minimum_path, adjacent.second});
			id_node_map.insert({(adjacent.second)->coords, adjacent.second});
		}
					// cout << "\nbefore:\n";
					// for(auto m : min_cost_map) (m.second)->prt_node(); // DEBUG only
					// cout << "complete.\n";

		// Remove working node from two maps
		auto foo = min_cost_map.find(working->minimum_path);
		if(foo != min_cost_map.end()){
			free(foo->second);
			min_cost_map.erase(foo);
		} else {
			cout << "min_cost_map.find() failed" << endl;
		}
		
		auto bar = id_node_map.find(working->coords);
		if(bar != id_node_map.end()){
			free(bar->second);
			id_node_map.erase(bar);
		} else {
			//cout << "id_node_map.find() failed" << endl;
		}

					// cout << "\nafter:\n";
					// for(auto m : min_cost_map) (m.second)->prt_node(); // DEBUG only
					// cout << "complete.\n";
		
		// update the working pointer to top of min_cost_map
		working = (min_cost_map.begin())->second;
	}


	return 0;
}
