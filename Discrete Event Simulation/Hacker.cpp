#include "Hacker.h"
#include "Event.h"
#include <iostream>

using namespace std;

/*
creates hacker with given id. sets default fields.
*/
Hacker::Hacker(int _id) {
    this->id = _id;
    this->nofLinesChanged=0;
    this->totalLines=0;
    this->ownedHoodies=0;
    this->ownedStickers=0;
    this->nofCommits=0;
    this->nofValidCommits=0;
    this->didArrive=false;
    this->arriveTimeToHoodie=0;
    this->arriveTimeToSticker=0;
    this->canEnter=false;
    this->canCommit=false;
    this->waitingTimeSticker=0;
    this->waitingTimeHoodie=0;
    this->turnaround=0;
    this->invalidAttempts=0;
    this->excessGifts=0;
}