/* 
delta_sn.ccp
*/
#include <map>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>

using namespace std;
typedef uint64_t ULL;
typedef std::pair<ULL,ULL> Coords;
typedef std::pair<ULL,ULL> SubValues;
class Node;	//Forward Declaration


// --------------------class declaration----------------------
class Node {
public:
	// Data
	Coords coords;
	SubValues aibj;	// components of local value - derived from Sn = (Sn-1)^2 mod 998388889
	ULL local_value;
	
	ULL sum_path = 999999999;
	static const ULL M = 10;	// matrix dimension
	static const ULL mod = 998388889;	// modulus

	// Functions
	Node();
	pair<Node*,Node*> neighbours()const;
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

pair<Node*,Node*> Node::neighbours()const{
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
		// set new local_value
		d->local_value = d->aibj.first + d->aibj.second;
		// sum_path = local_value + parent.sum_path
		d->sum_path = d->local_value + sum_path;
		// save pointer
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
		// sum_path = local_value + parent.sum_path
		r->sum_path = r->local_value + sum_path;
		// save pointer
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

//----------------------------------------------------------------------------

ULL next_sn_2places(ULL sn){
	// move sn 2 places along the Sn sequence
	static const ULL mod = 998388889;	// modulus
	ULL sm = (sn*sn)%mod;
	return ((sm*sm)%mod);
}


//========================Main================================================

int main(int argc, char **argv)
{
	// Outline
	// Output table in format ai an delta_a bj bn delta_b	10 Lines
	long long int an, ai = 102022661; 
	long long int bn, bj = 864751430;
	int lines = 10;
	long long int delta_ai = 0;
	long long int delta_bj = 0;

	do{
		an = next_sn_2places(ai);
		cout << ai << "\t" << an << "\t" << (ai-an) << "\t";
		delta_ai += (ai-an);
		ai = an;
		bn = next_sn_2places(bj);
		cout << bj << "\t" << bn << "\t" << (bj-bn) << endl;
		delta_bj += (bj-bn);
		bj = bn;
		lines -= 1;
	} while (lines>1);

	cout << "delta_ai: " << delta_ai << endl;
	cout << "delta_bj: " << delta_bj << endl;

	long long int Ai = 102022661 + delta_ai;
	long long int Bj = 864751430 + delta_bj;
	
	cout << "Final ai: " << Ai << endl;
	cout << "Final bj: " << Bj << endl;

	cout << "Path cost: " << (Ai+Bj) << endl;

	return 0;
}
