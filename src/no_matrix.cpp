/*
 * priority_q.cpp
 * 
 * Copyright 2024 Mike <mike@Fedora40>
 * 01Dec2024_09:30
 * Try to remove the matrix completely and rely on the info
 * in the priority queue to advance the nodes. 
 * Keep coord pair
 */


#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <utility>

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
	
	ULL min_path = 999999999;
	const ULL M = 10;	// matrix dimension
	const ULL mod = 998388889;	// modulus

	// Functions
	Node();
	vector<vector<Node>> neighbours();
	void prt_node() const;	// const tells compiler nothing will change inside this function
	ULL move_sn_2places(ULL sn);

};

// ---------------------class definitions---------------------

Node::Node(){
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
}

//~ Node::Node(Coords c, SubValues ab, ULL _M){
	//~ coords = {get<0>(c), get<1>(c)};
	//~ aibj = {get<0>(ab), get<1>(ab)};
	//~ M = _M;
//~ }

vector<vector<Node>> Node::neighbours(){
	// M is matrix size
	// Do sanity checks based on current coords
	// Use current coords and values for min_path, ai and bj.
	// vector[0] has possible down move
	// vector[1] has possible right move
	vector<vector<Node>> dnrt;
	if((coords.first+1)<M){	// down possible
		Node d;
		d.coords = {coords.first+1, coords.second};
		d.aibj = {move_sn_2places(aibj.first), aibj.second};	// new ai (row)
		//np.first = d;
		dnrt[0].push_back(d);
	}
	if((coords.second+1)<M){	// right possible
		Node r;
		r.coords = {coords.first, coords.second+1};
		r.aibj = {aibj.first, move_sn_2places(aibj.second)};	// new bj (col)
		dnrt[1].push_back(r);
	}
	return dnrt;
}

ULL Node::move_sn_2places(ULL sn){
	// move sn 2 places along the Sn sequence
	ULL sm = (sn*sn)%mod;
	return ((sm*sm)%mod);
}

void Node::prt_node() const { // const tells compiler nothing will change inside this function
	cout << "Node {" << coords.first << "," << coords.second << "}" << endl;
	cout << "ai:" << aibj.first << " bj:" << aibj.second << endl;
	cout << "node_value:" << local_value;
	cout << endl;
}

//-----------------------------------------------------------------------------

// Helper functions
// Assume n is a member of recursive sequence S
// Sn = (S(n-1))^2 % 998388889
ULL an(int n, vector<ULL>& S);
ULL bn(int n, vector<ULL>& S);

ULL an(int n, vector<ULL>& S){
    // Single iteration
	return S[(2*n - 1)];
}
ULL bn(int n, vector<ULL>& S){
    // Single iteration
	return S[(2*n)];
}

//---------------------------------
// Output recursive sequence
void prt_Svector(vector<ULL>& S);
void prt_Svector(vector<ULL>& S){
	for(int i = 1; i != S.size(); ++i)
		cout << i << ") " << S[i] << endl;
}

//=============================================================================

int main(int argc, char **argv)
{
	const ULL N = 3;	//matrix dimension
	const ULL mod = 998388889;
	const ULL s1 = 102022661;
	Node& rfn;	// Reference to reference node
	priority_queue
	
	// For convenience - construct a short vector of Sn values
	ULL sn = s1;
	vector<ULL> Sn = {0};
	for(int i = 1; i <= (N*N); ++i){
		//cout << "s(" << i << ") = " << sn << endl;
		Sn.push_back(sn);
		sn *= sn;
		sn %= mod;
	}
	// prt_Svector(Sn);
	
	// Initialise the matrix
	// Matrix matrix(N, vector<Node>(N));

	/*
	Initialse the first node {1,1} with the given values
	Then starting at {1,2} update the values for ai,bj and node_value
	*/
	Node start;
	start.coords = {0,0};	// Zero based indexing
	start.aibj = {102022661, 864751430};
	start.local_value = 966774091;


	for(auto a = 0; a <= N; ++a){
		for(auto b = 0; b <= N; ++b){

			vector<vector<Node>> nb = nref.neighbours();
			if(!nb[0].empty()){
				//down neighbour
				cout << "Down a:" << a << " b:" << b << endl;
				Node& d = matrix[a+1][b];
				// set coords, aibj and local value
				d.coords = {a+1,b};
				d.aibj = {nref.move_sn_2places(nref.aibj.first),(nref.aibj.second)};
				d.local_value = d.aibj.first + d.aibj.second;
			}
			if(!nb[1].empty()){
				//right neighbour
				Node& r = matrix[a][b+1];
				// set coords, aibj and local value
				r.coords = {a,b+1};
				r.aibj = {nref.aibj.first, nref.move_sn_2places(nref.aibj.second)};
				r.local_value = r.aibj.first + r.aibj.second;
			}
				
		}
	}

/*	for(auto a : matrix){
		for(auto b : a){
			b.prt_node();
		}
	}*/

/*	// Construct a priority queue using local_value as key, prioritise minimum local_value
	// Using a custom function object to compare elements.
    struct compare
    {
        bool operator()(const Node& l, const Node& r) const { return l.local_value > r.local_value; }
    };

	priority_queue <Node, vector<Node>, compare> pq;

	cout << "\nFilling priority queue" << endl;
	for(auto a : matrix){
		for(auto b : a){
			pq.push(b);	// insert and sort
		}
	}

	// print the priority queue
	cout << "\n Contents of minimum pq." << endl;
	while(!pq.empty()){
		Node const& t = pq.top();
		t.prt_node();
		pq.pop();
	}*/

	cout << "\ncomplete\n" << endl;
	return 0;
}

