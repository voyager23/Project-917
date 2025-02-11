/*
 * class_file.cpp
 * 
 * Copyright 2024 Mike <mike@Fedora40>
 * 
 */


#include <iostream>
#include <cstdint>
#include <vector>
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
}

void Node::prt_node(){
	cout << "Node {" << this->i << "," << this->j << "}" << endl;
	cout << "ai:" << this->ai << " bj:" << this->bj << endl;
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
	const ULL s1 = 102022661;
	const ULL N = 3;	//matrix dimension
	// For convenience - construct a short vector of Sn values
	ULL sn = s1;
	vector<ULL> Sn = {0};
	for(int i = 1; i <= (N*N); ++i){
		//cout << "s(" << i << ") = " << sn << endl;
		Sn.push_back(sn);
		sn *= sn;
		sn %= mod;
	}
	prt_Svector(Sn);

	// Initialise the matrix
	Matrix matrix(N, vector<Node>(N));

	/*
	Initialse the first node {1,1} with the given values
	Then starting at {1,2} update the values for ai,bj and node_value
	*/


	for(auto a = 0; a != N; ++a){
		for(auto b = 0; b != N; ++b){
			matrix[a][b].i = a;
			matrix[a][b].j = b;
			matrix[a][b].ai = Sn[2*(a+1) - 1];
			matrix[a][b].bj = Sn[2*(b+1)];
			matrix[a][b].local_value = matrix[a][b].ai + matrix[a][b].bj;
		}
	}

	for(auto a : matrix){
		for(auto b : a){
			b.prt_node();
		}
	}

	cout << "\ncomplete\n" << endl;
	return 0;
}

