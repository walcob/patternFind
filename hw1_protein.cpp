/*NAME: Benjamin David Walcott
Language: C++
Compiler: g++

These programs are intended to search a FASTA sequence of a 
protein or dna for a specific pattern specified in either
prosite or IUPAC format respectively.  They take in a few command
line arguments as follows:

./hw1.exe [sequence file] [search file] [output file]

[sequence file]: The file containing the FASTA formatted sequence to be analyzed 
[search file]: The file containing either the IUPAC or Prosite format search pattern
[output file]: The file to which to save the results
*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cctype>

//take the FASTA sequence and convert it into a string with no blank spaces
std::string read_sequence(std::istream &seq){
	std::string sequence;
	std::vector <std::string> lines;
	std::string line;
	while(!seq.eof()){
		std::getline(seq, line);
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
	std::string::iterator pos = sequence.begin();
	while(pos != sequence.end()){
		if(std::isspace(*pos)) sequence.erase(pos);
		else{
			//convert lowercase characters to capital letters
			*pos = toupper(*pos);
			pos++;
		}
	}
	return sequence;
}

std::vector <std::string> prosite_parse(std::istream &search){
	
	//create master string of all amino acids.
	//ambiguous tokens will be a copy of this string
	//that will be modified according to the token
	std::string aa = "RHKDESTNQCGPAVILMFYW";
	std::vector <std::string> tokens;
	char tmp;
	while(!search.eof()){
		search>>tmp;
		if(tmp != 'x' && tmp != '[' && tmp != '{')
			tokens.push_back(std::string(1,tmp));
		else if(tmp=='x')
			tokens.push_back(aa);
			//expand this to accept x()
		else if(tmp=='['){
			std::string token;
			while(tmp != ']'){
				search>>tmp;
				if(tmp!=']') token+=tmp;
			}
			tokens.push_back(token);
		}
		else if(tmp=='{'){
			std::string token(aa);
			while(tmp != '}'){
				search>>tmp;
				size_t loc;
				loc = token.find(tmp);
				if(loc != std::string::npos){
					std::string::iterator pos = token.begin()+(int)loc;
					token.erase(pos);
				}
			}
			tokens.push_back(token);
		}
		else
			std::cerr<<"Invalid search input.\n";
		
		
	}
	tokens.pop_back();
	return tokens;
}

std::vector <std::string> find_pattern(const std::string &sequence, const std::vector <std::string> &search){
	
	std::vector <std::string> matches;
	int size = search.size();
	for(int i = 0; i<(sequence.size()-size); i++){
		std::string tmp;
		for(int k = i; k<(i+size); k++)
			tmp+=sequence[k];
		int j = 0;
		bool is_hit = true;
		while(is_hit && j<size){
			if(search[j].find(tmp[j]) == std::string::npos) 
				is_hit = false;
			else j++;
		}
		if(is_hit){
			std::stringstream itos;
			itos<<i+1<<"-"<<tmp<<"-"<<i+size;
			std::string match = itos.str();
			matches.push_back(match);
		}
	}
	return matches;
}


	
	

int main(int argc, char* argv[]){ //arguments should be ./hw1.exe sequence search protein or dna
	
	//ensure proper command arguments
	if(argc != 4)
		std::cerr<<"Missing arguments\n";
	
	//test read_sequence()
	std::ifstream seq;
	seq.open(argv[1]);
	if(!seq.is_open()){
		std::cerr<<"Sequence input file "<<argv[1]<<" cannot be read.\n";
		return -1;
	}
	std::string sequence = read_sequence(seq);
	
	//test prosite_parse()
	std::ifstream search;
	search.open(argv[2]);
	if(!search.is_open()){
		std::cerr<<"Search input file "<<argv[2]<<" cannot be read.\n";
		return -1;
	}
	std::vector <std::string> tokens = prosite_parse(search);

	std::ofstream output;
	output.open(argv[3]);
	if(!output.is_open()){
		std::cerr<<"File "<<argv[3]<<" cannot be opened\n";
		return -1;
	}

	
	//test actual search function
	std::vector <std::string> matches = find_pattern(sequence, tokens);
	
	for(int i = 0; i<matches.size(); i++)
		output<<matches[i]<<std::endl;
		
	
	return 0;
}