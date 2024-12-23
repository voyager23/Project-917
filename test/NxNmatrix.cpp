/* 
	NxNmatrix.cpp
*/
#include <map>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <climits>
#include <iomanip>
#include <string>

using namespace std;
typedef std::pair<long long,long long > Coords;
typedef std::pair<long long,long long > SubValues;
class Node;	//Forward Declaration

// Top Level Parameter Block
// const long long dimension  =        10;
// const long long Modulus =    998388889;
// const long long initial_ai = 102022661;
// const long long initial_bj = 864751430;

const long long dimension = 5;
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
	static const long long M = dimension;	// matrix dimension
	static const long long mod = Modulus;	// modulus

	long long local_value;
	long long fromN = mod * 2;	// default values
	long long fromW = mod * 2;
	long long minimum_path = 0;	// minimum of fromN and fromW
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
	minimum_path = 0;

	fromN = LLONG_MAX;
	fromW = LLONG_MAX;
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
	if(fromW != LLONG_MAX)
		cout << "fromW:" << fromW << endl;
	if(fromN != LLONG_MAX)
		cout << "fromN:" << fromN << endl;
	cout << "minimum_path:" << minimum_path;
	cout << endl << endl;;

}

bool Node::goal() const{
	return ((coords.first == M-1) and (coords.second == M-1));
}

pair<Node*,Node*> Node::neighbours(std::map<Coords, Node*>& id_node_map){

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
		auto foo = id_node_map.find({coords.first, coords.second});
		if(foo != id_node_map.end()){
			d->fromN = foo->second->minimum_path + d->local_value;
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
	// fromN, fromW, *Nparent, *Wparent have default values of LLONG_MAX and NULL respectively.

	// Construct a complete matrix using TLPB elements
	// set matrix[0][0] to working
	// set top row {1->n-1}
	// set left col {1->n-1}
	// fill in remaining rows

	vector<vector<Node>> matrix;
	// Resize the matrix to allow direct access
	for(auto row = 0; row != dimension; ++row) {
		vector<Node> temp;
		temp.reserve(dimension);
		temp.clear();
		matrix.push_back(temp);
	}

	matrix[0].push_back(*working);	// set start node
	// Note: *Nparent and *Wparent are not used in this case since we have direct access via coords

	// Set top row:
	cout << "Top row." << endl;
	for(auto col = 1; col != dimension; ++col){
		Node tNode = matrix[0][col-1]; // copy west parent
		tNode.coords = {0,col};
		// Update tnode
		tNode.aibj.second = tNode.move_sn_2places(tNode.aibj.second); //update column value
		tNode.local_value = tNode.aibj.first + tNode.aibj.second; //update sum
		tNode.fromW = tNode.minimum_path; // update using west parent minimum_path
		tNode.minimum_path += tNode.local_value; // West parent min_path + local_value
		matrix[0].push_back(tNode);
	}
	// Set Left column
	for(auto row = 1; row != dimension; ++row){
		Node tNode = matrix[row-1][0]; // copy north parent
		tNode.coords = {row,0};
		// Update tNode
		tNode.aibj.first = tNode.move_sn_2places(tNode.aibj.first); //update row value
		tNode.local_value = tNode.aibj.first + tNode.aibj.second; //update sum
		tNode.fromN = tNode.minimum_path; // update using north parent minimum_path
		tNode.minimum_path += tNode.local_value; // North parent min_path + local_value
		matrix[row].push_back(tNode);		
	}
	// Set remaining rows starting at row 1 column 1
	// Note that all nodes now have N & W parents
	for(auto row = 1; row != dimension; ++row){
		for(auto col = 1; col != dimension; ++col){
			Node tNode = matrix[row][col-1]; // copy west parent
			tNode.coords = {row,col};
			// Update tnode
			tNode.aibj.second = tNode.move_sn_2places(tNode.aibj.second); //update column value
			tNode.local_value = tNode.aibj.first + tNode.aibj.second; //update sum

			tNode.fromW = matrix[row][col-1].minimum_path; // update using west parent minimum_path
			tNode.fromN = matrix[row-1][col].minimum_path; // update using north parent minimum_path
			tNode.minimum_path = min(tNode.fromW, tNode.fromN) + tNode.local_value; // North&West parent min_path + local_value
			matrix[row].push_back(tNode);			
		}
	}

	// // Debug matrix printout
	// for(auto r = matrix.begin(); r != matrix.end(); ++r){
	// 	cout << "====Row====" << endl;
	// 	for(auto i = r->begin(); i != r->end(); ++i) i->prt_node();
	// }
	// // end debug

	// print N 'dimension' Nodes across the screen
	const int columns = 180;
	const int offset = columns / dimension - 16;
	for(auto r = matrix.begin(); r != matrix.end(); ++r){
		// cout << "====Row====" << endl;

		for(auto i = r->begin(); i != r->end(); ++i){
			string line = "Node {" + to_string(i->coords.first) + "," + to_string(i->coords.second) + "}";
			cout << setw(offset) << left << line;
		}
		cout << endl;

		for(auto i = r->begin(); i != r->end(); ++i){
			string line = "node_value:" + to_string(i->local_value);
			cout << setw(offset) << left << line;
		}
		cout << endl;

		for(auto i = r->begin(); i != r->end(); ++i){
			string line = "fromW:";
			if((i->fromW)!= LLONG_MAX){
				line += to_string(i->fromW);
			} else {
				line += " - ";
			}
			cout << setw(offset) << left << line;
		}
		cout << endl;

		for(auto i = r->begin(); i != r->end(); ++i){
			string line = "fromN:";
			if((i->fromN)!= LLONG_MAX){
				line += to_string(i->fromN);
			} else {
				line += " - ";
			}
			cout << setw(offset) << left << line;
		}
		cout << endl;

		for(auto i = r->begin(); i != r->end(); ++i){
			string line = "minimum_path:" + to_string(i->minimum_path);
			cout << setw(offset) << left << line;
		}
		cout << endl << endl;

	}	

	return 0;
}

	// cout << "ai:" << aibj.first << " bj:" << aibj.second << endl;
	// cout << "node_value:" << local_value << endl;
	// if(fromW != LLONG_MAX)
	// 	cout << "fromW:" << fromW << endl;
	// if(fromN != LLONG_MAX)
	// 	cout << "fromN:" << fromN << endl;
	// cout << "minimum_path:" << minimum_path;