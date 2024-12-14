		
/* 
	QRmod21.ccp
*/
#include <map>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{

	const int mod = (21);
	const int mid = (mod / 2);	//integer division
	int n=0, m=0;
	
	// cout << min(1,2) << endl;
	// exit(0);

	cout << "Quadratic residues modulo " << mod << endl << endl;
	//cout << n << ")\t" << m << "\t" << endl;

	for(auto n = 0; n != 2*mod+1; ++n){
		
		m = (n*n)%mod;
		cout << n << ")\t";
		if((n%2)==0) cout << "\t";
		cout << m << endl;
		if ((n%mid)==0) cout << "....................\n";
	}
	
	return 0;
}
