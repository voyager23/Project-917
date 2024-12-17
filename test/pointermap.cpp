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
const LL dimension = 3;
const LL Modulus = 1189;
const LL initial_ai = 1183;
const LL initial_bj = 36;
// End TLPB

// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// pair of components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	LL local_value;
	static const LL M = dimension;	// matrix dimension
	static const LL mod = Modulus;	// modulus

	LL fromN = mod * 2;	// default values
	LL fromW = mod * 2;
	LL minimum_path = 0;	// minimum of fromN and fromW
	Node* Nparent = NULL;
	Node* Wparent = NULL;


	// Public Functions
	Node();
	pair<Node*,Node*> neighbours(std::map<Coords, Node*>& id_node_map);
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
	Nparent = NULL;
	Wparent = NULL;
}

/* Commented out
pair<Node*,Node*> Node::neighbours(std::map<Coords, Node*>& id_node_map)const{


	// Use current coords and values for min_path, ai and bj.
	// pair{first, second}  has possible down, right node*

	pair<Node*,Node*> dnrt{NULL,NULL};

	// Check for Down move
	if((coords.first+1)<M){
		Node* d = new Node;
		// new coords
		d->coords = {coords.first+1, coords.second};

		// new aibj
		d->aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)

		// set local value
		d->local_value = d->aibj.first + d->aibj.second;

		// fromNorth = this local_value + parent local value
		d->fromN = d->local_value + minimum_path;

		// Does this node have a west neighbor?
		// if so get minimum_path from west neighbor and set fromW value;
		// west neighbor = x,y-1
		// fetch node pointer from id_node_map Node* Wptr

		auto Wptr = id_node_map.find({coords.first, coords.second-1});	// move left 1 column
		if(Wptr != id_node_map.end()){
			d->fromW = (Wptr->second)->minimum_path;
		}else{
			d->fromW = LLONG_MAX;
		}
		d->minimum_path = min(d->fromN, d->fromW);
		dnrt.first = d;
	}

	// Check for right move
	if((coords.second+1)<M){
		Node* r = new Node;
		// new coords
		r->coords = {coords.first, coords.second+1};

		// new aibj
		r->aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
		// set local value

		r->local_value = r->aibj.first + r->aibj.second;
		// fromW = new local_value + this local value
		r->fromW = r->local_value + minimum_path;

		// Does this node have a North neighbor?
		// if so get minimum_path from north neighbor and set fromN value;
		// north neighbor = x-1,y
		// fetch node pointer from id_node_map Node* Nptr

		auto Nptr = id_node_map.find({coords.first-1, coords.second});	// new ai (row)
		if(Nptr != id_node_map.end()){
			r->fromN = (Nptr->second)->minimum_path;
		}else{
			r->fromN = LLONG_MAX;
		}		
		r->minimum_path = min(r->fromN, r->fromW);
		dnrt.second = r;
	}
	return dnrt; // pair<Node*, Node*>
}
*/


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

pair<Node*,Node*> Node::neighbours(std::map<Coords, Node*>& id_node_map)const{

	// Use current coords and values for min_path, ai and bj.
	// pair{first, second}  has possible down, right node*

	pair<Node*,Node*> dnrt{NULL,NULL};

	// Check for Down move
	if((coords.first+1)<M){
		Node* d = new Node;
		// new coords
		d->coords = {coords.first+1, coords.second};
		// new aibj
		d->aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)
		// set local value
		d->local_value = d->aibj.first + d->aibj.second;
		// Path from North = this local_value + parent local value
		auto foo = id_node_map.find({coords.first, coords.second})
		if(foo != id_node_map.end()){
			d->fromN = foo->second.minimum_path + d->local_value;
		}else{
			cout << "Error North parent not found." << endl;
			exit(1);
		}

		dnrt.first = d;
	}

	// Check for right move
	if((coords.second+1)<M){
		Node* r = new Node;
		// new coords
		r->coords = {coords.first, coords.second+1};
		// new aibj
		r->aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
		// set local value
		r->local_value = r->aibj.first + r->aibj.second;
		// fromW = new local_value + this local value

		dnrt.second = r;
	}
	return dnrt; // pair<Node*, Node*>
}



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

	// -----LOOP-----

	while(1){
		// look for new neighbouring Nodes
		cout << "Working -> " << working->coords.first << "," << working->coords.second << endl;
		pair<Node*,Node*> adjacent = working ->neighbours(id_node_map);


		// test for goal state
		if(((adjacent.first)==NULL) and ((adjacent.second)==NULL)){
			cout << "\nGoal Node reached\n";
			//cout << working->minimum_path << endl;
			working->prt_node();
			return 0;
		}


		
		if(adjacent.first != NULL){		
			min_cost_map.insert({(adjacent.first)->minimum_path, adjacent.first});
			id_node_map.insert({(adjacent.first)->coords, adjacent.first});		// possible silent fail
		}
		if(adjacent.second != NULL){	
			min_cost_map.insert({(adjacent.second)->minimum_path, adjacent.second});
			id_node_map.insert({(adjacent.second)->coords, adjacent.second});	// possible silent fail
		}
					cout << "\nmin_cost after insertions:\n";
					for(auto m : min_cost_map) (m.second)->prt_node(); // DEBUG only
					cout << "\n\n===============\n";


//------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------


		
		// update the working pointer to top of min_cost_map
		working = (min_cost_map.begin())->second;

		break;
	}


	return 0;
}
