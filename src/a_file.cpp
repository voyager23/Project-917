/*
 * a_file.cpp
 * 
 * Copyright 2024 Mike <mike@Fedora40>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <cstdint>
#include <vector>

typedef uint64_t ULL;

using namespace std;

ULL an(int n, vector<ULL>& S);
ULL an(int n, vector<ULL>& S){
	return S[(2*n - 1)];
}
ULL bn(int n, vector<ULL>& S);
ULL bn(int n, vector<ULL>& S){
	return S[(2*n)];
}

int main(int argc, char **argv)
{
	const ULL mod = 998388889;
	const ULL s1 = 102022661;
	ULL sn = s1;
	vector<ULL> Sn = {0};
	for(int i = 1; i <= 20; ++i){
		//cout << "s(" << i << ") = " << sn << endl;
		Sn.push_back(sn);
		sn *= sn;
		sn %= mod;
	}
	for(auto&& z : Sn) cout << z << endl;
	
	ULL pathA = (an(1,Sn) + bn(1,Sn)) % mod;
	pathA += (an(1,Sn) + bn(2,Sn)) % mod;
	pathA += (an(2,Sn) + bn(2,Sn)) % mod;
	
	ULL pathB = (an(1,Sn) + bn(1,Sn)) % mod;
	pathB += (an(2,Sn) + bn(1,Sn)) % mod;
	pathB += (an(2,Sn) + bn(2,Sn)) % mod;
	
	cout << "PathA = " << pathA << endl;
	cout << "PathB = " << pathB << endl;
	
	return 0;
}

