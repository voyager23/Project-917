/*
 * priority_q.cpp
 * 
 * Copyright 2024 Mike <mike@Fedora40>
 * 
 */


#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <set>
#include <utility>

using namespace std;
typedef uint64_t ULL;
typedef std::pair<ULL,ULL> Coords;
typedef std::pair<ULL,ULL> SubValues;

// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	ULL local_value;
	ULL min_path = 999999999;

	// Functions
	Node();
	Node(Coords c, SubValues ab );
	vector<Node> neighbours(ULL N);
	void prt_node() const;	// const tells compiler nothing will change inside this function
	ULL new_row_col(ULL sn);

};


// ---------------------class definitions---------------------

Node::Node(){
	coords = {0,0};
	aibj = {0,0};
	local_value = 0;
	min_path = 999999999;
}

Node::Node(Coords c, SubValues ab ){

}

vector<Node> Node::neighbours(ULL N){
	// N is matrix size
	// Do sanity checks based on current coords
	// Use current coords and values for min_path, ai and bj.
	// Generate a possibly empty vector of up to 2 new nodes
	vector<Node> new_nodes;
	if((coords.first+1)<N){	// down possible
		Node d;
		d.coords = {coords.first+1, coords.second};
		d.aibj = {new_row_col(aibj.first), aibj.second};	// new ai (row)
	}
	if((coords.second+1)<N){	// right possible
		Node r;
		r.coords = {coords.first, coords.second+1};
		r.aibj = {aibj.first, new_row_col(aibj.second)};	// new bj (col)
	}

	return new_nodes;
}

ULL Node::new_row_col(ULL sn, ULL mod){
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

typedef vector<vector<Node>> Matrix;

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
	const ULL mod = 998388889;
	const ULL s1 = 102022661;
	const ULL N = 10;	//matrix dimension
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
	Matrix matrix(N, vector<Node>(N));

	/*
	Initialse the first node {1,1} with the given values
	Then starting at {1,2} update the values for ai,bj and node_value
	*/

	for(auto a = 0; a != N; ++a){
		for(auto b = 0; b != N; ++b){
			Node& node = matrix[a][b];
			if((a==0)and(b==0)){ //start node
				node.coords = {a,b};
				node.aibj = {Sn[1],Sn[2]};
				node.local_value = Sn[1]+Sn[2];
			} else {
				vector<Node> vNodes = node.neighbours(N);
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
