#include "Character.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

string winner = ""; //states the winner community or draw.
string lastKilledin1 = ""; //stores the last killed character's name in community-1.
string lastKilledin2 = ""; //stores the last killed character's name in community-2.
bool isGoing = true; //checks whether the war should continue.
vector<Character*> com1; //contains pointers to characters in community-1.
vector<Character*> com2; //contains pointers to charactes in community-2.

/*
used for sorting according to alphabetical order.
a and b are the pointers to characters to be sorted.
*/
bool comp(Character* a, Character* b){return a->name < b->name;} 

/*
returns a pointer to the last killed character in the community.
no indicates if the round number is even or odd.
dead is the name of the character to be returned.
*/
Character* detectLastKilled(int no, string dead){
	if(no==1){ //if round number is odd, search among community-1.
		for(int i=0; i<5; i++){
			if(com1[i]->name == dead) return com1[i];
		}
	}
	else{ //if round number is even, search among community-2.
		for(int i=0; i<5; i++){
			if(com2[i]->name == dead) return com2[i];
		}
	}
}

/*
returns a pointer to the defending character.
c is the name of the defending character.
no indicates if the round number is even or odd.
*/
Character* detectDef(string c, int no){
	if(no==0){ //if round number is even, search among community-1.
		for(int i=0;i<5; i++){
			if(com1[i]->name == c) return com1[i];
		}
	}
	else{ //if round number is odd, search among community-2.
		for(int i=0; i<5; i++){
			if(com2[i]->name == c) return com2[i];
		}
	}
}

/*
returns a pointer to the attacking character.
c is the name of the attacking character.
no indicates if the round number is even or odd.
*/
Character* detectAtt(string c, int no){
	if(no==1){ //if round number is odd, search among community-1.
		for(int i=0; i<5; i++){
			if(com1[i]->name == c) return com1[i];
		}
	}
	else{ //if round number is even, search among community-2.
		for(int i=0; i<5; i++){
			if(com2[i]->name == c) return com2[i];
		}
	}
}

/*
returns a pointer to the next alive character.
c is the pointer to the current dead character.
no indicates if the round number is even or odd.
move states if the character to be returned is attacking or defending.
*/
Character* detectNext(Character* c, int no, string move){
	if((no==1 && move=="attack") || (no==0 && move=="defense")){ //if round number is odd and character is attacking, search among community-1.
		int j;												  //or if round number is even and character is defending, search among community-1.	
		int k;
		for(int i=0; i<5; i++){
			if(com1[i]->name == c->name){ //when dead character's index is found, store the index.
				j=i;
				k=i;
			}
		}
		while(c->isAlive == false){ //change the character since it is alive.
			if(j<4){ //traverse through end of the community-1.
				c = com1[j+1];
				j++;
			}
			else if(k>0){ //traverse through beginning of the community-1.
				c = com1[k-1];
				k--;
			}
		}
	}
	else{ 														//if round number is even and character is attacking, search among community-2.
		int j;												 //or if round number is odd and character is defending, search among community-2.
		int k;
		for(int i=0; i<5; i++){
			if(com2[i]->name == c->name){ //when dead character's index is found, store the index.
				j=i;
				k=i;
			}
		}
		while(c->isAlive == false){ //change the character since it is alive.
			if(j<4){ //traverse through end of the community-2.
				c = com2[j+1];
				j++;
			}
			else if(k>0){ //traverse through beginning of the community-2.
				c = com2[k-1];
				k--;
			}
		}

	}
	return c; //return next alive character.
}

/*
returns a pointer to the Hobbit.
att is a pointer to the attacking character, actually att is an elf.
no indicates if the round number is even or odd.
*/
Character* detectHobbit(const Character* att, int no){
	if(no==1){ //if round number is odd, search among community-1.
		for(int i=0; i<5; i++){
			if(com1[i]->type == "Hobbit") return com1[i];
		}
	}
	else{ //if round number is even, search among community-2.
		for(int i=0; i<5; i++){
			if(com2[i]->type == "Hobbit") return com2[i];
		}
	}

}

/*
attacks the defender. attacker gives damage if capable.
att is a pointer to attacking character.
def is a pointer to defending character.
no indicates if the round number is even or odd.
*/
void normalAttack(Character* att, Character* def, int no){

	if((att->attack) > (def->defense)){ //if attack of attacker is bigger than defense of defender, than attacker is capable of giving damage to defender.
		def->remainingHealth -= (att->attack) - (def->defense); //damage given is equal the difference between attacker's attack point and defender's defense point.
		if(def->remainingHealth <= 0){ //defenser is dead.
			def->remainingHealth = 0;
			if(no==0) lastKilledin1 = def->name; //if defender dies in even round, store the name of the defender for community-1.
			else lastKilledin2 = def->name; //if defender dies in odd round, store the name of the defender for community-2.
			def->isAlive = false;
			if(def->isAlive == false && def->type == "Hobbit"){ //if dead character is hobbit, end the war and set the opponent as winner.
				isGoing = false;
				if(no==1) winner = "Community-1";
				else winner = "Community-2";
			}
		}
	}
}

/*
calls the normalAttack method after performing special skills if the attacker is capable.
attacker is the name of the attacking character.
defender is the name of the defending character.
ifs indicates whether there will be special skill or not.
no indicates if the round number is even or odd.
*/
void attack(string attacker, string defender, string ifs, int no){

	Character* att = detectAtt(attacker, no); //attacker detected.
	if(att->isAlive == false) att = detectNext(att, no, "attack"); //if attacker is not alive, detect alphabetically next character in the community.

	Character* def = detectDef(defender, no); //defender detected.
	if(def->isAlive == false) def = detectNext(def, no, "defense"); //if defender is not alive, detect alphabetically next character in the community.

	if(ifs.at(0) == 'S'){ //allows to perform special skill.
		if(att->type == "Elves"){ 											//if an elf is performing a special skill,
			if(att->nRoundsSinceSpecial > 10){								// at least ten rounds must pass since last round which the elf used its special skill.
				int transfer = (att->remainingHealth)/2;					//elves can transfer half of their health to the hobbit of its community.
				att->remainingHealth = att->remainingHealth - transfer;
				Character* hobbit = detectHobbit(att, no);
				hobbit->remainingHealth += transfer;
				att->nRoundsSinceSpecial = 0; //set skill loader to zero.
			}
			normalAttack(att, def, no); //if skill is not loaded, perform normal attack.
		}
		else if(att->type == "Dwarfs"){										//if a dwarf is performing a special skill,
			if(att->nRoundsSinceSpecial > 20){								//at least twenty rounds must pass since last round which the dwarf used its special skill.
				att->attack = att->attack * 2;								//dwarfs can attack the defender with a friend whose stats are the same with itself.
				def->defense = def->defense * 2;
				normalAttack(att, def, no);
				att->attack = att->attack / 2;
				def->defense = def->defense / 2;
				att->nRoundsSinceSpecial = 0; //set skill loader to zero.
			}
			else normalAttack(att, def, no); //if skill is not loaded, perform normal attack.
		}
		else if(att->type == "Wizards"){										//if a wizard is performing a special skill,
			if(att->nRoundsSinceSpecial > 50){									//at least fifty rounds must pass since last round which the wizard used its special skill.
				if(no==1){														//wizards can bring the last killed character back to life of its community.
					if(lastKilledin1 != ""){									//after detecting the last killed character,
						Character* dead = detectLastKilled(no, lastKilledin1);	//make its current health to its initial health,
						if(dead->isAlive == false){
							dead->nRoundsSinceSpecial = 0; //set skill loader to zero.
						}
						dead->isAlive = true;									
						dead->remainingHealth = dead->healthHistory[0];
						att->nRoundsSinceSpecial = 0;                           //and set the skill loader to zero.
					}
				}
				else{
					if(lastKilledin2 != ""){
						Character* dead = detectLastKilled(no, lastKilledin2);
						if(dead->isAlive == false){
							dead->nRoundsSinceSpecial = 0; //set skill loader to zero.
						}
						dead->isAlive = true;
						dead->remainingHealth = dead->healthHistory[0];
						att->nRoundsSinceSpecial = 0;                           //and set the skill loader to zero.
					} 
				}
			}
			normalAttack(att, def, no); //if skill is not loaded or after performing special skill, perform normal attack.
		}
		else{
			normalAttack(att, def, no); //if character is type of men or hobbit just perform normal attack.
		}
	}
	else{
		normalAttack(att, def, no); //if special skill is not used just perform normal attack.
	}
}


int main(int argc, char* argv[]) {

	string givenFile = argv[1]; //store the first argument.
	string outputted = argv[2]; //store the second argument.
	ifstream in(givenFile); //input file.
	
	int nofMaxRounds; //maximum number of rounds can occur throughout the war.
	in>>nofMaxRounds;
	
	vector<Character*> toBePrinted; //container of pointers to characters to print to the output file.
	
	for(int i=0; i<5; i++){ //construct community-1 characters after reading from the input file.
		string name, type;
		int att, def, hlt;
		in>>name>>type>>att>>def>>hlt;
		Character* c = new Character(name, type, att, def, hlt, nofMaxRounds);
		com1.push_back(c); 
		toBePrinted.push_back(c); 
	}
	
	for(int i=0; i<5; i++){ //construct community-2 characters after reading from the input file.
		string name, type;
		int att, def, hlt;
		in>>name>>type>>att>>def>>hlt;
		Character* c = new Character(name, type, att, def, hlt, nofMaxRounds);
		com2.push_back(c);
		toBePrinted.push_back(c);
	}

	sort(com1.begin(), com1.end(), comp); //sort community-1 characters in alphabetical order.
	sort(com2.begin(), com2.end(), comp); //sort community-2 characters in alphabetical order.

	int roundNo = 0; //current round number.
	
	while(isGoing){ //check if the war is over or not.

		/*
		attacker is the name of the attacker.
		defender is the name of the defender.
		ifs states whether attacker uses its special skill.
		*/
		string attacker, defender, ifs; 
		in>>attacker>>defender>>ifs;

		roundNo++; //increment round number by one before every round.
		attack(attacker, defender, ifs, roundNo%2); //attacker attacks to defender.
		
		for(int i=0; i<5; i++){ //after attack, arrange the health history of the characters and the skill loaders.

			com1[i]->healthHistory[roundNo] = com1[i]->remainingHealth;
			com1[i]->nRoundsSinceSpecial++;

			com2[i]->healthHistory[roundNo] = com2[i]->remainingHealth;
			com2[i]->nRoundsSinceSpecial++;
			
		}
		

		bool oneAlive1 = false; //checks if there is someone alive other than hobbit in the community-1.
		for(int i=0; i<5; i++){
			if(com1[i]->isAlive && com1[i]->type != "Hobbit"){
				oneAlive1 = true;
				break;
			}
		}

		bool oneAlive2 = false;
		for(int i=0; i<5; i++){ //checks if there is someone alive other than hobbit in the community-2.
			if(com2[i]->isAlive && com2[i]->type != "Hobbit"){
				oneAlive2 = true;
				break;
			}
		}

		if(!oneAlive1){ //if there is noone other than hobbit in the community-1, opponent wins.
			winner = "Community-2";
			isGoing = false; //war over.
		}
		if(!oneAlive2){ //if there is noone other than hobbit in the community-2, opponent wins.
			winner = "Community-1"; 
			isGoing = false; //war over.
		}

		if(roundNo == nofMaxRounds){ //if maximum number of rounds reached, the war ends in draw.
			winner = "Draw";
			isGoing = false; //war over.
		}

	}

	int nofCasulties = 0; //number of dead characters in the war.
	for(int i=0; i<10; i++){ //check for every character if they are dead or not at the end of the war.
		if(toBePrinted[i]->healthHistory[roundNo] == 0) nofCasulties++; //if there is dead, increment number of dead characters by one.
	}

	ofstream out(outputted); //output file.

	out<<winner<<endl<<roundNo<<endl<<nofCasulties<<endl; //print to the file winner, round number of the war and the number of dead bodies.
	
	/*
	print to the file every character and their health history.
	*/
	for(int i=0; i<10; i++){ 
		out<<toBePrinted[i]->name<<" ";
		for(int j=0; j<=roundNo; j++){
			out<<toBePrinted[i]->healthHistory[j]<<" ";
		}
		if(i<9) out<<endl;
	}

	
	return 0;
}