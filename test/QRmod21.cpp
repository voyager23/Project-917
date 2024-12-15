		
/* 
	QRmod21.ccp
*/
#include <map>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <set>

using namespace std;
set<int> seq;

void prt_set(set<int> seq){
	for(auto n : seq) cout << n << " ";
	cout << endl;
}	

int main(int argc, char **argv)
{

	const int mod = (29*41);
	const int mid = (mod / 2);	//integer division
	int seed, Sa, Sb;

	int max_cycle = 0;

	for(seed = 36; seed != 37; ++seed){
		Sa = seed;
		seq.clear();
		auto result = seq.insert(Sa);
		if((result.second)==false){
			cout << "insert seed error" << endl;
			exit(1);
		}
		while(1){
			Sb = (Sa*Sa) % mod;
			auto result = seq.insert(Sb);
			if((result.second)==false){
				cout << "seed " << seed << " cycle " << seq.size() << endl;
				if(seq.size() > max_cycle) max_cycle = seq.size();
				prt_set(seq);
				break;
			} //if
			Sa = Sb;
		} //whilw
	} //for	
	cout << "\nComplete. max_cycle = " << max_cycle << endl;
	return 0;
}


	// // cout << min(1,2) << endl;
	// // exit(0);

	// cout << "Quadratic residues modulo " << mod << endl << endl;
	// //cout << n << ")\t" << m << "\t" << endl;

	// for(auto n = 0; n != 2*mod+1; ++n){
		
	// 	m = (n*n)%mod;
	// 	cout << n << ")\t";
	// 	if((n%2)==0) cout << "\t";
	// 	cout << m << endl;
	// 	if ((n%mid)==0) cout << "....................\n";
	// }