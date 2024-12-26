#include <iostream>
#include <vector>
#include <map>
#include <utility>

using namespace std;
vector<int> ai = {1020,6616,2398,5045,7031};
vector<int> bj = {8647,3289,3870,7171,1930};
map<int, pair<int,int>> sums;
map<pair<int,int>, int> paths;

int main(int argc, char const *argv[])
{
	for(int i = 0; i != 5; ++i){
		for(int j = 0; j != 5; ++j){
			sums.insert({ai[i]+bj[j], {i,j}});
			paths.insert({{i,j}, ai[i]+bj[j]});
		}
	}
	for(auto i = paths.begin(); i != paths.end(); ++i){
		cout << i->first.first << "," << i->first.second << "  " << i->second << endl;
	}
	return 0;
}