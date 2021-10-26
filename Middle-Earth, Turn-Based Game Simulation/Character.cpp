#include "Character.h"

/*
constructs a character with given attributes.
*/
Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
	name = _name;
	type = _type;
	attack = _attack;
	defense = _defense;
	remainingHealth = _remainingHealth;
	nMaxRounds = _nMaxRounds;
	nRoundsSinceSpecial = 0; 					//special skill loader.
	isAlive = true;								//makes character alive.
	healthHistory = new int[_nMaxRounds+1];		//stores character's health throughout the war.
	healthHistory[0] = remainingHealth;			
}

/*
constructs a character by copying one to another.
*/
Character::Character(const Character& character) {
	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;
	this->isAlive = character.isAlive;
	this->healthHistory = new int[character.nMaxRounds+1];
	for(int i=0; i<nMaxRounds+1; i++){
		this->healthHistory[i] = character.healthHistory[i];
	}
}

/*
constructs a character by assigning one to another.
*/
Character& Character::operator=(const Character& character) {

	if(this == &character) return *this; //if both characters are same, do nothing.

	if(this->healthHistory != NULL) delete[] this->healthHistory; //delete this character's health history to prevent confusion.

	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;
	this->isAlive = character.isAlive;
	this->healthHistory = new int[character.nMaxRounds+1];
	for(int i=0; i<nMaxRounds+1; i++){
		this->healthHistory[i] = character.healthHistory[i];
	}

}

/*
compares two characters according to alphabetical order of thier name.
*/
bool Character::operator<(const Character& other) {

	if(this->name < other.name) return true; 
	else return false;

}

/*
destructs a character by deleting the healthHistory array.
*/
Character::~Character() {

	delete[] this->healthHistory;

}