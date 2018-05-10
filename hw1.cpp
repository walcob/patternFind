#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
using namespace std;

//take the FASTA sequence and convert it into a string with no blank spaces
string read_sequence(istream &seq){
	string sequence;
	vector <string> lines;
	string line;
	while(!seq.eof()){
		getline(seq, line);
		lines.push_back(line);
	}
	
	//append each line into the sequence ignoring lines that begin with ">"
	int i = 0;
	while(i<lines.size()){
		if(lines[i][0]=='>') i++;
		else{
			sequence += lines[i];
			i++;
		}
	}
	
	//eliminate blank space characters from sequence
	string::iterator pos = sequence.begin();
	while(pos != sequence.end()){
		if(isspace(*pos)) sequence.erase(pos);
		else pos++;
	}
	return sequence;
}

vector <int> find_protein(string sequence, string search){
	vector <int> hit_indexes;
	int sea = 0;
	vector <int> itr;
	itr.push_back(0);
	//generate initial vector of hit indexes
		
	//[]
	if (search[sea]=='['){
		string tmp;
		sea++;
		while (search[sea] != ']'){
			tmp += search[sea];
			sea++;
		}
		for(int i = 0; i<sequence.size(); i++){
			for(int j = 0; j<tmp.size(); j++){
				if(sequence[i]==tmp[j]) hit_indexes.push_back(i);
			}
		}
		itr[0]++;
	}
	
	//{}
	else if (search[sea] == '{'){
		string tmp;
		sea++;
		while (search[sea] != '}'){
			tmp += search[sea];
			sea++;
		}
		for(int i = 0; i<sequence.size(); i++){
			for(int j = 0; j<tmp.size(); j++){
				if(sequence[i]!=tmp[j]) hit_indexes.push_back(i);
			}
		}
		itr[0]++;
	}
	
	//x
	else if (search[sea] == 'x'){
		sea++;
		if(search[sea] == '('){
			cout<<"(\n";
			sea++;
			vector <int> nums;
			string num;
			while(search[sea]!= ')'){
				cout<<")\n";
				while(search[sea] != ',' && search[sea] !=')'){
					num += search[sea];
					sea++;
				}
				cout<<",\n";
				nums.push_back(atoi(num.c_str()));
				num.clear();
				if(search[sea]==',') sea++;
			}
			/*
			for(int i = 0; i<sequence.size(); i++){
				for(int j = 0; j<nums.size(); j++){
					if(i<=sequence.size()-nums[j])
						hit_indexes.push_back(i);
				}
			}
			sea++;
		*/
			
		for(int i = 0; i<sequence.size(); i++){
				for(int j = 0; j<nums.size(); j++){
					if(i<=sequence.size()-nums[j] ){
						bool already_there = false;
						int k = 0;
						while(k < hit_indexes.size()&&!already_there){
							if(i==hit_indexes[k]) already_there = true;
							else k++;
						}
						
						if(!already_there) hit_indexes.push_back(i);
					}
				}
			}
			sea++;
			
		}
		else{
			for(int i = 0; i<sequence.size(); i++)
				hit_indexes.push_back(i);
			itr[0]++;
		}
				
	}
	
	else{
		for(int i = 0; i<sequence.size(); i++){
			if(sequence[i]==search[sea]) hit_indexes.push_back(i);
		}
		sea++;
	}
	return hit_indexes;
}

	
	//now test using vector hit_indexes
	//while(sea < search.size()){

		
	

int main(int argc, char* argv[]){ //arguments should be ./hw1.exe protein or dna output
	
	//test read_sequence()
	ifstream seq;
	seq.open(argv[1]);
	string sequence = read_sequence(seq);
	cout<<sequence<<endl;
	
	//test initial part of find protein
	string search;
	cout<<"Enter search criteria for protein: ";
	cin>>search;
	vector <int> hits = find_protein(sequence, search);
	for(int i=0; i<hits.size(); i++)
		cout<<endl<<hits[i]<<endl;
	
	
	return 0;
}

