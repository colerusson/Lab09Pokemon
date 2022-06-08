#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "HashMap.h"
#include "Set.h"
using namespace std;

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif		

int main(int argc, char* argv[]) {
	VS_MEM_CHECK	//implementing memory check

		if (argc < 3) {
			cerr << "Please provide name of input and output files";
			return 1;
		}
	cout << "Input file: " << argv[1] << endl;
	ifstream in(argv[1]);
	if (!in) {
		cerr << "Unable to open " << argv[1] << " for input";
		return 2;
	}
	cout << "Output file: " << argv[2] << endl;
	ofstream out(argv[2]);
	if (!out) {
		in.close();
		cerr << "Unable to open " << argv[2] << " for output";
		return 3;
	}

	Set<string> mySet;			//set instantiation for any basic set
	Set<string> effectiveSet;	//set instantiation for the effectives 
	Set<string> ineffectiveSet;	//set instatiation for the ineffectives
	HashMap<string, string> myPokemon;	//hash map instantiation for pokemon
	HashMap<string, string> myMoves;	//hash map instantiation for moves
	HashMap<string, Set<string>> effectivities;	 //hash map instantiation for effectivities
	HashMap<string, Set<string>> ineffectivities;	//hash map instantiation for ineffectivities
	
	for (string line; getline(in, line);) {		//parse through the file until the end
		string item1, item2, item3, item4, item5;
		if (line.size() == 0) continue;
		out << endl << line;
		istringstream iss(line);
		iss >> item1;
		if (item1 == "Set:") {			//check for call to Set:
			while (!iss.eof()) {
				iss >> item2;
				mySet.insert(item2);
			}
			Set<string>::Iterator iter = mySet.begin();		//use an iterator of the bst to output infix order of the set
			out << endl << "  [";
			while (!iter.isEnd()) {		//iterate through our string stack outputting each item
				out << iter.curr()->data_;
				if (iter.curr()->right_ != NULL || iter.iterSize()	> 1) {		//check for if the item is not the last so to ouptut a comma
					out << ",";
				}
				iter.next();
			}
			out << "]";
			mySet.clear();
		}
		else if (item1 == "Pokemon:") {			//check for call to Pokemon:
			iss >> item2;
			iss >> item3;
			myPokemon[item2] = item3;			//take in the pokemon name and type and add to the hash map
		}
		else if (item1 == "Move:") {			//check for call to Move:
			iss >> item2;
			iss >> item3;
			myMoves[item2] = item3;				//take in the move name and type and add to the hash map
		}
		else if (item1 == "Pokemon") {			//check for call to Pokemon
			out << ": " << myPokemon.size() << "/" << myPokemon.max_size();		//output the current load size and capacity of the hash map
			out << endl;
			out << myPokemon;														//print out the hash map of pokemon
			out << endl;
		}
		else if (item1 == "Moves") {		//check for call to Moves
			out << ": " << myMoves.size() << "/" << myMoves.max_size();		//output the current load size and capacity of the hash map
			out << myMoves;													//print out the hash map of Moves
			out << endl;
		}
		else if (item1 == "Effective:") {		//check for call to Effective:
			iss >> item2;
			while (iss >> item3) {
				effectivities[item2].insert(item3);			//add the items of the set into a set and add that to the hash map
			}
		}
		else if (item1 == "Ineffective:") {			//check for call to Ineffective:
			iss >> item2;
			while (iss >> item3) {
				ineffectivities[item2].insert(item3);		//add the items of the set into a set and add that to the hash map
			}
		}
		else if (item1 == "Effectivities") {		//check for call to Effectivities
			out << ": " << effectivities.size() << "/" << effectivities.max_size();		//output the current load size and capacity of the hash map
			out << effectivities;															//print out the effectivities 
			out << endl;
		}
		else if (item1 == "Ineffectivities") {			//check for call to Ineffectivities
			out << ": " << ineffectivities.size() << "/" << ineffectivities.max_size();		//output the current load size and capacity of the hash map
			out << ineffectivities;															//print out the ineffectivities
			out << endl;
		}
		else if (item1 == "Battle:") {			//check for call to Battle:
			iss >> item2;					//take in all the items into different strings
			iss >> item3;
			iss >> item4;
			iss >> item5;
			out << endl << "  " << item2 << " (" << item3 << ") vs " << item4 << " (" << item5 << ")" << endl;
			out << "  " << item2 << " is a " << myPokemon[item2] << " type Pokemon." << endl;						//output here all of the extra sentences
			out << "  " << item4 << " is a " << myPokemon[item4] << " type Pokemon." << endl;						//about the pokemon and their information
			out << "  " << item3 << " is a " << myMoves[item3] << " type Move." << endl;
			out << "  " << item5 << " is a " << myMoves[item5] << " type Move." << endl;
			out << "  " << item3 << " is super effective against [" << effectivities[myMoves[item3]] << "] type Pokemon." << endl;
			out << "  " << item3 << " is ineffective against [" << ineffectivities[myMoves[item3]] << "] type Pokemon." << endl;
			int damageAToB = effectivities[myMoves[item3]].count(myPokemon[item4]) - ineffectivities[myMoves[item3]].count(myPokemon[item4]);	//calculate damageAtoB
			int damageBToA = effectivities[myMoves[item5]].count(myPokemon[item2]) - ineffectivities[myMoves[item5]].count(myPokemon[item2]);	//calculate damageBtoA
			if (damageAToB == 0) {
				out << "  " << item2 << "'s " << item3 << " is effective against " << item4 << endl;		//output the correct effectiveness based on damage calculation
			}
			else if (damageAToB  > 0) {
				out << "  " << item2 << "'s " << item3 << " is super effective against " << item4 << endl;
			}
			else {
				out << "  " << item2 << "'s " << item3 << " is ineffective against " << item4 << endl;
			}

			out << "  " << item5 << " is super effective against [" << effectivities[myMoves[item5]] << "] type Pokemon." << endl;
			out << "  " << item5 << " is ineffective against " << ineffectivities[myMoves[item5]] << " type Pokemon." << endl;

			if (damageBToA == 0) {
				out << "  " << item4 << "'s " << item5 << " is effective against " << item2 << endl;		//output the correct effectiveness based on damage calculation
			}
			else if (damageBToA > 0) {
				out << "  " << item4 << "'s " << item5 << " is super effective against " << item2 << endl;
			}
			else {
				out << "  " << item4 << "'s " << item5 << " is ineffective against " << item2 << endl;
			}

			if (damageAToB - damageBToA == 0) {
				out << "  " << "The battle between " << item2 << " and " << item4 << " is a tie." << endl;		//output who the winner is, or if a tie occurred 
			}																									//based on subtracting the calculated damage
			else if (damageAToB - damageBToA > 0) {
				out << "  " << "In the battle between " << item2 << " and " << item4 << ", " << item2 << " wins!" << endl;
			}
			else {
				out << "  " << "In the battle between " << item2 << " and " << item4 << ", " << item4 << " wins!" << endl;
			}
		}
	}

	in.close();
	return 0;
}
