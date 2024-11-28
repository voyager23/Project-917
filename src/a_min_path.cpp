/*
 * a_min_path.cpp
 * 
 * Copyright 2024 Mike <mike@Fedora40>
 * 
 */


#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <set>

using namespace std;
typedef uint64_t ULL;


// --------------------class declaration----------------------
class Node {
public:
	// Data
	ULL i, j;	// matrix indices
	ULL ai, bj;	// components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	ULL local_value;
	ULL min_path = 999999999;
	bool visited = false;

	// Functions
	Node();
	void prt_node() const;	// const tells compiler nothing will change inside this function

};


// ---------------------class definitions---------------------

Node::Node(){
	i = 0;	j = 0;
	ai = 0; bj = 0;
	local_value = 0;
	min_path = 999999999;
	visited = false;
}

void Node::prt_node() const { // const tells compiler nothing will change inside this function
	cout << "Node {" << i << "," << j << "}" << endl;
	cout << "ai:" << ai << " bj:" << bj << endl;
	cout << "node_value:" << local_value;
	cout << endl;
}


//-----------------------------------------------------------------------------

typedef vector<vector<Node>> Matrix;

// Helper functions
ULL an(int n, vector<ULL>& S);
ULL an(int n, vector<ULL>& S){
	return S[(2*n - 1)];
}
ULL bn(int n, vector<ULL>& S);
ULL bn(int n, vector<ULL>& S){
	return S[(2*n)];
}

void prt_Svector(vector<ULL>& S);
void prt_Svector(vector<ULL>& S){
	for(int i = 1; i != S.size(); ++i)
		cout << i << ") " << S[i] << endl;
}

//=============================================================================

int main(int argc, char **argv)
{
	const ULL mod = 998388889;
	const ULL s1 = 102022661;	// a1
	const ULL s2 = 864751430;	// b1
	const ULL N = 10;	//matrix dimension

	// Initialise the matrix
	Matrix matrix(N, vector<Node>(N));

	/*
	Initialse the first node {1,1} with the given values
	Then starting at {1,2} update the values for ai,bj and node_value
	*/

	for(auto a = 0; a < N; ++a){
		if(a==0){ // top row
			ULL ai = s1;
			ULL bj = s2;
			for(auto b = 0; b < N; ++b){
				matrix[a][b].i = a;
				matrix[a][b].j = b;
				matrix[a][b].ai = ai;
				matrix[a][b].bj = bj;
				matrix[a][b].local_value = ai + bj;
				// ai is const
				// bj is updated to next column
				bj = (bj*bj) % mod;
				bj = (bj*bj) % mod;
			}
		} else {
			ULL ai = matrix[a-1][0].ai;	// from prev row
			ai = (ai*ai) % mod;
			ai = (ai*ai) % mod;		// update to new value for this row
			ULL bj = matrix[a-1][0].bj;
			for(auto b = 0; b < N; ++b){
				matrix[a][b].i = a;
				matrix[a][b].j = b;
				matrix[a][b].ai = ai;	// const value
				matrix[a][b].bj = matrix[a-1][b].bj;	// pull bj from neighbour above
				matrix[a][b].local_value = ai + bj;
			}
		}
	}

	// prt matrix for debug
	for(auto a : matrix){
		for(auto b : a){
			b.prt_node();
		}
	}

/*	----------Outline------------
	Complete setup of Node type and Matrix
	Matrix is 10 x 10, start(1,1) goal(10,10) Answer: 13389278727
	Load priority queue pq with Node(1,1) all relevant fields set
	Start_Loop
		pq.top() gets ref to node with smallest path cost
		Goal test - if node is(10,10), return path_cost, STOP
		make a short vector<Node> of next-nodes
		update each next node with ai, bj and path_cost
		pq.pop() to remove current node
		pq.push() next-nodes. This pushes and sorts nodes
		goto Start_Loop
*/

	cout << "\nAnswer: 13389278727 complete\n" << endl;
	return 0;
}
